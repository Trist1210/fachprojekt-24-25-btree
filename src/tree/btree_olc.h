#pragma once

#include <atomic>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sched.h>
#include <builtin.h>
#include <emmintrin.h>
#include <bitset>

namespace btreeolc {

enum class PageType : uint8_t
{
    BTreeInner = 1,
    BTreeLeaf = 2
};

static const uint64_t pageSize = 256U;

struct OptLock
{
    std::atomic<uint64_t> typeVersionLockObsolete{0b100};

    bool isLocked(uint64_t version) { return ((version & 0b10) == 0b10); }

    uint64_t readLockOrRestart(bool &needRestart)
    {
        uint64_t version;
        version = typeVersionLockObsolete.load();
        if (isLocked(version) || isObsolete(version))
        {
            builtin::pause();
            needRestart = true;
        }
        return version;
    }

    void writeLockOrRestart(bool &needRestart)
    {
        uint64_t version;
        version = readLockOrRestart(needRestart);
        if (needRestart)
            return;

        upgradeToWriteLockOrRestart(version, needRestart);
        if (needRestart)
            return;
    }

    void upgradeToWriteLockOrRestart(uint64_t &version, bool &needRestart)
    {
        if (typeVersionLockObsolete.compare_exchange_strong(version, version + 0b10))
        {
            version = version + 0b10;
        }
        else
        {
            builtin::pause();
            needRestart = true;
        }
    }

    void writeUnlock() { typeVersionLockObsolete.fetch_add(0b10); }

    bool isObsolete(uint64_t version) { return (version & 1) == 1; }

    void checkOrRestart(uint64_t startRead, bool &needRestart) const { readUnlockOrRestart(startRead, needRestart); }

    void readUnlockOrRestart(uint64_t startRead, bool &needRestart) const
    {
        needRestart = (startRead != typeVersionLockObsolete.load());
    }

    void writeUnlockObsolete() { typeVersionLockObsolete.fetch_add(0b11); }
};

struct NodeBase : public OptLock
{
    PageType type;
    uint16_t count;

    virtual ~NodeBase() = default;
};

struct BTreeLeafBase : public NodeBase
{
    static const PageType typeMarker = PageType::BTreeLeaf;

    virtual ~BTreeLeafBase() = default;
};

template <class Key, class Payload> struct BTreeLeaf : public BTreeLeafBase
{
    struct Entry
    {
        Key k;
        Payload p;
    };

    static const uint16_t maxEntries = (pageSize - sizeof(NodeBase)) / (sizeof(Key) + sizeof(Payload));

    Key keys[maxEntries];
    Payload payloads[maxEntries];

    BTreeLeaf()
    {
        count = 0;
        type = typeMarker;
    }

    virtual ~BTreeLeaf() = default;

    bool isFull() { return count == maxEntries; };

    unsigned lowerBound(Key k)
    {
       /*
       // std::cout << maxEntries << std::endl;
        unsigned pos = 0;
        for(int i = 0; i < count; i+=8) {
            
        
        __m512i keysArray = _mm512_loadu_epi64((__m512 *)&(this->keys[i]));
        __m512i kArray = _mm512_set1_epi64(k);
        __mmask8 mask;
        mask = _mm512_cmp_epi64_mask(keysArray, kArray, _MM_CMPINT_LT);
        unsigned rest = 0xFFFFFFFF >> (32-(count-i));

        mask = mask & rest;
         pos+= __builtin_popcount (mask);
        }
        return pos;
 */
        
        unsigned lower = 0;
        unsigned upper = count;
        do
        {
            unsigned mid = ((upper - lower) / 2) + lower;
            if (k < keys[mid])
            {
                upper = mid;
            }
            else if (k > keys[mid])
            {
                lower = mid + 1;
            }
            else
            {
                return mid;
            }
        } while (lower < upper);
        return lower;
    }

    void insert(Key k, Payload p)
    {
        assert(count < maxEntries);
        if (count)
        {
            unsigned pos = lowerBound(k);
            if ((pos < count) && (keys[pos] == k))
            {
                // Upsert
                payloads[pos] = p;
                return;
            }
            memmove(keys + pos + 1, keys + pos, sizeof(Key) * (count - pos));
            memmove(payloads + pos + 1, payloads + pos, sizeof(Payload) * (count - pos));
            keys[pos] = k;
            payloads[pos] = p;
        }
        else
        {
            keys[0] = k;
            payloads[0] = p;
        }
        count++;
    }

    BTreeLeaf *split(Key &sep)
    {
        BTreeLeaf *newLeaf = new BTreeLeaf();
        newLeaf->count = count - (count / 2);
        count = count - newLeaf->count;
        memcpy(newLeaf->keys, keys + count, sizeof(Key) * newLeaf->count);
        memcpy(newLeaf->payloads, payloads + count, sizeof(Payload) * newLeaf->count);
        sep = keys[count - 1];
        return newLeaf;
    }
};

struct BTreeInnerBase : public NodeBase
{
    static const PageType typeMarker = PageType::BTreeInner;

    virtual ~BTreeInnerBase() = default;
};

template <class Key> struct BTreeInner : public BTreeInnerBase
{
    static const uint16_t maxEntries = (pageSize - sizeof(NodeBase)) / (sizeof(Key) + sizeof(NodeBase *));
    NodeBase *children[maxEntries];
    Key keys[maxEntries];
    

    BTreeInner()
    {
        count = 0;
        type = typeMarker;
    }

    virtual ~BTreeInner()
    {
        for (auto i = 0u; i < count; i++)
        {
            if (children[i] != nullptr)
            {
                delete children[i];
            }
        }
    }

    bool isFull() { return count == (maxEntries - 1); };


    unsigned lowerBound(Key k)
    {
        /*
        //return branchless_lower_bound(keys, &keys[count], k) - (keys);
       //std::cout << maxEntries << std::endl;
        unsigned pos = 0;
        for(int i = 0; i < count; i+=8) {
            
        
        __m512i keysArray = _mm512_loadu_epi64((__m512 *)&(this->keys[i]));
        __m512i kArray = _mm512_set1_epi64(k);
        __mmask8 mask;
        mask = _mm512_cmp_epi64_mask(keysArray, kArray, _MM_CMPINT_LT);
        unsigned rest = 0xFFFFFFFF >> (32-(count-i));

        mask = mask & rest;
         pos+= __builtin_popcount (mask);
        }
        return pos;
*/
        unsigned lower = 0;
        unsigned upper = count;
        do
        {
            unsigned mid = ((upper - lower) / 2) + lower;
            if (k < keys[mid])
            {
                upper = mid;
            }
            else if (k > keys[mid])
            {
                lower = mid + 1;
            }
            else
            {   
                //std::cout << "pos: " << pos << "mid: " << mid<< std::endl;
                return mid;
            }
        } while (lower < upper);
       // std::cout << "pos: " << pos << "lower: " << lower<< std::endl;
        return lower; 
    }
    Key* branchless_lower_bound(Key * begin, Key * end, const Key value)
    {
        size_t length = end - begin;
        if (length == 0)
            return end;
        size_t step = std::bit_floor(length);
        if (step != length && (begin[step] < value))
        {
            length -= step + 1;
            if (length == 0)
                return end;
            step = std::bit_ceil(length);
            begin = end - step;
        }
        for (step /= 2; step != 0; step /= 2)
        {
            if ((begin[step] < value))
                begin += step;
        }
        return begin + (*begin < value);
    }
    unsigned lowerBound_m(Key k)
    {
        unsigned lower = 0;
        unsigned upper = count;
        do
        {
            unsigned mid = ((upper - lower) / 2) + lower;
            if (k < keys[mid])
            {
                upper = mid;
            }
            else if (k > keys[mid])
            {
                lower = mid + 1;
            }
            else
            {   
                return mid;
            }
        } while (lower < upper);
        return lower; 
    }

    BTreeInner *split(Key &sep)
    {
        BTreeInner *newInner = new BTreeInner();
        newInner->count = count - (count / 2);
        count = count - newInner->count - 1;
        sep = keys[count];
        memcpy(newInner->keys, keys + count + 1, sizeof(Key) * (newInner->count + 1));
        memcpy(newInner->children, children + count + 1, sizeof(NodeBase *) * (newInner->count + 1));
        return newInner;
    }

    void insert(Key k, NodeBase *child)
    {
        assert(count < maxEntries - 1);
        unsigned pos = lowerBound(k);
        memmove(keys + pos + 1, keys + pos, sizeof(Key) * (count - pos + 1));
        memmove(children + pos + 1, children + pos, sizeof(NodeBase *) * (count - pos + 1));
        keys[pos] = k;
        children[pos] = child;
        std::swap(children[pos], children[pos + 1]);
        count++;
    }
};

template <class Key, class Value> struct BTree
{

    /*
    std::unordered_map<Key, Value> umap;

    void insert(Key k, Value v) {
        umap[k] = v;
    }
    
    bool lookup(Key k, Value &result) {
        result = umap.at(k);
        return true;
    }
    */


    std::atomic<NodeBase *> root;

    BTree() { 
        root = new BTreeLeaf<Key, Value>(); 
        //umap.reserve(500000000);
    }

    ~BTree() { delete root.load(); }

    std::size_t height()
    {
        auto height = 0U;
        if (root) {
            auto *current = root.load();
            while(current && current->type == PageType::BTreeInner) {
                ++height;
                current = reinterpret_cast<BTreeInner<Key>*>(current)->children[0U];
            }
        }

        return height + 1U;
    }

    void makeRoot(Key k, NodeBase *leftChild, NodeBase *rightChild)
    {
        auto inner = new BTreeInner<Key>();
        inner->count = 1;
        inner->keys[0] = k;
        inner->children[0] = leftChild;
        inner->children[1] = rightChild;
        root = inner;
    }

    void yield(int count)
    {
        if (count > 3)
            sched_yield();
        else
            builtin::pause();
    }

    void insert_m(const std::vector<benchmark::NumericTuple>& _load, int from, int to) {

    }

    void insert(Key k, Value v)
    {
        int restartCount = 0;
    restart:
        if (restartCount++)
            yield(restartCount);
        bool needRestart = false;

        // Current node
        NodeBase *node = root;
        uint64_t versionNode = node->readLockOrRestart(needRestart);
        if (needRestart || (node != root))
            goto restart;

        // Parent of current node
        BTreeInner<Key> *parent = nullptr;
        uint64_t versionParent;

        while (node->type == PageType::BTreeInner)
        {
            auto inner = static_cast<BTreeInner<Key> *>(node);

            // Split eagerly if full
            if (inner->isFull())
            {
                // Lock
                if (parent)
                {
                    parent->upgradeToWriteLockOrRestart(versionParent, needRestart);
                    if (needRestart)
                        goto restart;
                }
                node->upgradeToWriteLockOrRestart(versionNode, needRestart);
                if (needRestart)
                {
                    if (parent)
                        parent->writeUnlock();
                    goto restart;
                }
                if (!parent && (node != root))
                { // there's a new parent
                    node->writeUnlock();
                    goto restart;
                }
                // Split
                Key sep;
                BTreeInner<Key> *newInner = inner->split(sep);
                if (parent)
                    parent->insert(sep, newInner);
                else
                    makeRoot(sep, inner, newInner);
                // Unlock and restart
                node->writeUnlock();
                if (parent)
                    parent->writeUnlock();
                goto restart;
            }

            if (parent)
            {
                parent->readUnlockOrRestart(versionParent, needRestart);
                if (needRestart)
                    goto restart;
            }

            parent = inner;
            versionParent = versionNode;

            node = inner->children[inner->lowerBound(k)];
            inner->checkOrRestart(versionNode, needRestart);
            if (needRestart)
                goto restart;
            versionNode = node->readLockOrRestart(needRestart);
            if (needRestart)
                goto restart;
        }

        auto leaf = static_cast<BTreeLeaf<Key, Value> *>(node);

        // Split leaf if full
        if (leaf->count == leaf->maxEntries)
        {
            // Lock
            if (parent)
            {
                parent->upgradeToWriteLockOrRestart(versionParent, needRestart);
                if (needRestart)
                    goto restart;
            }
            node->upgradeToWriteLockOrRestart(versionNode, needRestart);
            if (needRestart)
            {
                if (parent)
                    parent->writeUnlock();
                goto restart;
            }
            if (!parent && (node != root))
            { // there's a new parent
                node->writeUnlock();
                goto restart;
            }
            // Split
            Key sep;
            BTreeLeaf<Key, Value> *newLeaf = leaf->split(sep);
            if (parent)
                parent->insert(sep, newLeaf);
            else
                makeRoot(sep, leaf, newLeaf);
            // Unlock and restart
            node->writeUnlock();
            if (parent)
                parent->writeUnlock();
            goto restart;
        }
        else
        {
            // only lock leaf node
            node->upgradeToWriteLockOrRestart(versionNode, needRestart);
            if (needRestart)
                goto restart;
            if (parent)
            {
                parent->readUnlockOrRestart(versionParent, needRestart);
                if (needRestart)
                {
                    node->writeUnlock();
                    goto restart;
                }
            }
            leaf->insert(k, v);
            node->writeUnlock();
            return; // success
        }
    }

    template <size_t N>
    void lookup_m(const std::vector<benchmark::NumericTuple>& _load, std::array<Value, N>& results, int from, int to) {
        int size = to - from;

            int restartCount = 0;
    restart:
        if (restartCount++)
            yield(restartCount);
        bool needRestart = false;

        std::array<NodeBase*, N> nodes;
        std::array<uint64_t, N> versionNodes;
        std::array<BTreeInner<Key>*, N> parents;
        std::array<uint64_t, N> parentVersions;
        std::array<Key, N> keys;
        std::array<unsigned, N> posStored;
        for (size_t i = 0; i < size; i++)
        {
            nodes[i] = root;
            versionNodes[i] = nodes[i]->readLockOrRestart(needRestart);

            if (needRestart || (nodes[i] != root))
                goto restart;

            parents[i] = nullptr;
            //parentVersions[i] = 0;
            keys[i] = _load[from + i].key();
            //posStored[i] = -1;
        }
        

        while (nodes[0]->type == PageType::BTreeInner)
        {
            for (size_t i = 0; i < size; i++)
            {
                auto inner = static_cast<BTreeInner<Key> *>(nodes[i]);

                if (parents[i])
                {
                    parents[i]->readUnlockOrRestart(parentVersions[i], needRestart);
                    if (needRestart)
                        goto restart;
                }

                parents[i] = inner;
                parentVersions[i] = versionNodes[i];


                posStored[i] = inner->lowerBound(keys[i]);

                __builtin_prefetch(&(inner->children[posStored[i]]));
                //
            } 

            for (size_t i = 0; i < size; i++) {
                auto inner = static_cast<BTreeInner<Key> *>(nodes[i]);

                nodes[i] = inner->children[posStored[i]];

                inner->checkOrRestart(versionNodes[i], needRestart);
                if (needRestart)
                    goto restart;

                versionNodes[i] = nodes[i]->readLockOrRestart(needRestart);
                if (needRestart)
                    goto restart;
            } 

            for (size_t i = 0; i < size; i++)
            {
                
            }
            

        }

        for (size_t i = 0; i < size; i++)
        {
            BTreeLeaf<Key, Value> *leaf = static_cast<BTreeLeaf<Key, Value> *>(nodes[i]);
            unsigned p = leaf->lowerBound(keys[i]);

            __builtin_prefetch(&(leaf->keys[p]));
            __builtin_prefetch(&(leaf->payloads[p]));
            posStored[i] = p;
        }

        for (size_t i = 0; i < size; i++)
        {
            BTreeLeaf<Key, Value> *leaf = static_cast<BTreeLeaf<Key, Value> *>(nodes[i]);
            bool success;
            if ((posStored[i] < leaf->count) && (leaf->keys[posStored[i]] == keys[i]))
            {
                success = true;
                results[i] = leaf->payloads[posStored[i]];
            }
            if (parents[i])
            {
                parents[i]->readUnlockOrRestart(parentVersions[i], needRestart);
                if (needRestart)
                    goto restart;
            }
            nodes[i]->readUnlockOrRestart(versionNodes[i], needRestart);
            if (needRestart)
                goto restart;
        }
        
        
        //return success;
    }

    bool lookup(Key k, Value &result)
    {
        int restartCount = 0;
    restart:
        if (restartCount++)
            yield(restartCount);
        bool needRestart = false;

        NodeBase *node = root;
        uint64_t versionNode = node->readLockOrRestart(needRestart);
        if (needRestart || (node != root))
            goto restart;

        // Parent of current node
        BTreeInner<Key> *parent = nullptr;
        uint64_t versionParent;

        while (node->type == PageType::BTreeInner)
        {
            auto inner = static_cast<BTreeInner<Key> *>(node);

            if (parent)
            {
                parent->readUnlockOrRestart(versionParent, needRestart);
                if (needRestart)
                    goto restart;
            }

            parent = inner;
            versionParent = versionNode;

            node = inner->children[inner->lowerBound(k)];
            inner->checkOrRestart(versionNode, needRestart);
            if (needRestart)
                goto restart;
            versionNode = node->readLockOrRestart(needRestart);
            if (needRestart)
                goto restart;
        }

        BTreeLeaf<Key, Value> *leaf = static_cast<BTreeLeaf<Key, Value> *>(node);
        unsigned pos = leaf->lowerBound(k);
        bool success;
        if ((pos < leaf->count) && (leaf->keys[pos] == k))
        {
            success = true;
            result = leaf->payloads[pos];
        }
        if (parent)
        {
            parent->readUnlockOrRestart(versionParent, needRestart);
            if (needRestart)
                goto restart;
        }
        node->readUnlockOrRestart(versionNode, needRestart);
        if (needRestart)
            goto restart;

        return success;
    }

    uint64_t scan(Key k, int range, Value *output)
    {
        int restartCount = 0;
    restart:
        if (restartCount++)
            yield(restartCount);
        bool needRestart = false;

        NodeBase *node = root;
        uint64_t versionNode = node->readLockOrRestart(needRestart);
        if (needRestart || (node != root))
            goto restart;

        // Parent of current node
        BTreeInner<Key> *parent = nullptr;
        uint64_t versionParent;

        while (node->type == PageType::BTreeInner)
        {
            auto inner = static_cast<BTreeInner<Key> *>(node);

            if (parent)
            {
                parent->readUnlockOrRestart(versionParent, needRestart);
                if (needRestart)
                    goto restart;
            }

            parent = inner;
            versionParent = versionNode;

            node = inner->children[inner->lowerBound(k)];
            inner->checkOrRestart(versionNode, needRestart);
            if (needRestart)
                goto restart;
            versionNode = node->readLockOrRestart(needRestart);
            if (needRestart)
                goto restart;
        }

        BTreeLeaf<Key, Value> *leaf = static_cast<BTreeLeaf<Key, Value> *>(node);
        unsigned pos = leaf->lowerBound(k);
        int count = 0;
        for (unsigned i = pos; i < leaf->count; i++)
        {
            if (count == range)
                break;
            output[count++] = leaf->payloads[i];
        }

        if (parent)
        {
            parent->readUnlockOrRestart(versionParent, needRestart);
            if (needRestart)
                goto restart;
        }
        node->readUnlockOrRestart(versionNode, needRestart);
        if (needRestart)
            goto restart;

        return count;
    }
    NodeBase *get_root() const {
        return root;
    }

    uint64_t get_page_size() const {
        return pageSize;
    }

};

}