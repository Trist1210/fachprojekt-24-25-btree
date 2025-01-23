#pragma once

// #include <perfcpp/event_counter.h>
#include <benchmark/workload_set.h>
#include <tree/btree_olc.h>
#include <chrono>
#include <omp.h>
#include <thread>

namespace benchmark {

template <typename Tree> class BtreeOLCBenchmark 
{
public:

    static const int elementsPer = 20;

    BtreeOLCBenchmark(NumericWorkloadSet &&workload, const std::uint16_t iterations)
        : _workload(std::move(workload)), _iterations(iterations)
    {}

    ~BtreeOLCBenchmark() = default;

    void set_up(phase phase)
    {
        if (phase == phase::INSERT)
        {
            this->_tree = new Tree{};
        }
    }

    void tear_down(phase phase)
    {
        if (phase == phase::MIXED)
        {
            delete std::exchange(this->_tree, nullptr);
        }
    }

    static void insertElements(const std::vector<NumericTuple>& _load, Tree *_tree, int from, int to) {
        std::cout << "insert   " << "   " << from << "  " << to << std::endl;
        for (auto i = from; i < to; ++i) {
            const auto &request = _load[i];
            _tree->insert(request.key(), request.value());
        }
        std::cout << "insert   " << "   " << from << "  " << to << " ready" << std::endl;
    }

    static void getElements(const std::vector<NumericTuple>& _load, Tree *_tree, int from, int to) {
        std::cout << "get   " << "   " << from << "  " << to << std::endl;
        for (auto i = from; i < to; i += elementsPer) {
            std::array<std::int64_t, elementsPer> results;
            _tree->lookup_m(_load, results, i, i+elementsPer);

            for (size_t j = 0; j < elementsPer; j++) {
                builtin::DoNotOptimize(results[i]);
            }
            
        }
        std::cout << "get   " << "   " << from << "  " << to << " ready" << std::endl;
    }

    void execute_single_run(const benchmark::phase phase) {
        auto from = 0U;
        auto to = _workload[phase].size();

        if (phase == benchmark::phase::INSERT) {
            //omp_set_num_threads(6);
            #pragma omp parallel for
            for (auto i = from; i < to; ++i) {
                const auto &request = _workload[phase][i];
                this-> _tree->insert(request.key(), request.value());
            }
        }
        else {
            //omp_set_num_threads(6);
            #pragma omp parallel for
            for (auto i = from; i < to; i += elementsPer) {
                std::array<std::int64_t, elementsPer> results;
                _tree->lookup_m(_workload[phase], results, i, i+elementsPer);

                //builtin::DoNotOptimize(results[0]);
                
                for (size_t j = 0; j < elementsPer; j++) {
                    builtin::DoNotOptimize(results[j]);
                }
                
            }

            /*
            omp_set_num_threads(4);
            #pragma omp parallel for
            for (auto i = from; i < to; ++i) {
                const auto &request = _workload[phase][i];
                std::int64_t value;
                this->_tree->lookup(request.key(), value);
                builtin::DoNotOptimize(value);
            }
            */
        }
    }

    void execute_single_run_t(const benchmark::phase phase)
    {
        auto from = 0U;
        auto to = _workload[phase].size();

        int numThreads = 4;

        std::thread threads[numThreads];
        if (phase == benchmark::phase::INSERT) {
            uint64_t bound = 0U;
            uint64_t perThreadSize = (_workload[phase].size()) / numThreads;
            for (size_t i = 0; i < numThreads; i++) {
                threads[i] = std::thread(insertElements, _workload[phase], this->_tree, bound, bound+perThreadSize);
                bound += perThreadSize;
            }
            for (size_t i = 0; i < numThreads; i++) {
                threads[i].join();
            }
            insertElements(_workload[phase], this->_tree, bound, _workload[phase].size());
        }
        else 
        {
            uint64_t bound = 0U;
            uint64_t perThreadSize = (_workload[phase].size()) / numThreads;
            for (size_t i = 0; i < numThreads; i++) {
                threads[i] = std::thread(getElements, _workload[phase], this->_tree, bound, bound+perThreadSize);
                bound += perThreadSize;
            }
            for (size_t i = 0; i < numThreads; i++) {
                threads[i].join();
            }
            getElements(_workload[phase], this->_tree, bound, _workload[phase].size());
        }
    }

    void validate_tree()
    {
        for (auto i=0; i<_workload[benchmark::phase::INSERT].size(); i+=elementsPer)
        {
            std::array<std::int64_t, elementsPer> results;
            _tree->lookup_m(_workload[benchmark::phase::INSERT], results, i, i+elementsPer);

            for (auto j = 0; j < elementsPer; j++) {
                const auto &request = _workload[benchmark::phase::INSERT][i+j];
                std::int64_t value = results[j];
                if(value != request.value()) {
                    std::cout << "Error, the tree does not return the correct value. " << request.value() << " expected, but " << value << " returned!\nAborting!" << std::endl;
                    exit(1);
                }
                /*
                 else {
                    std::cout << (i+j) << "real: " << request.value() << " got:" << value << std::endl;
                }
                */
                
            }
        }
    }

    void execute_benchmark() 
    {
        for (auto i = 0U; i < this->_iterations; ++i)
        {


            /// Create the btree
            this->set_up(phase::INSERT);



            /// Here starts the insert phase.
            auto startInsert = std::chrono::high_resolution_clock::now();
            this->execute_single_run(phase::INSERT);
            auto endInsert = std::chrono::high_resolution_clock::now();
            
            //validate_tree();

            /// Print the results
            auto durationInsert = std::chrono::duration_cast<std::chrono::microseconds>(endInsert - startInsert).count();
            std::cout << "Insert duration: " << durationInsert << " microseconds " << std::endl;
            


            /// Here starts the lookup phase.
            auto startLookup = std::chrono::high_resolution_clock::now();
            this->execute_single_run(phase::MIXED);            
            auto endLookup = std::chrono::high_resolution_clock::now();

            /// Print the results
            auto durationLookup = std::chrono::duration_cast<std::chrono::microseconds>(endLookup - startLookup).count();
            std::cout << "Lookup duration: " << durationLookup << " microseconds " << std::endl;



            /// Destroy the btree
            this->tear_down(phase::MIXED);
        }
    }

private:
    Tree *_tree{nullptr};
    const std::uint16_t _iterations;
    NumericWorkloadSet _workload;
};

} // namespace benchmark