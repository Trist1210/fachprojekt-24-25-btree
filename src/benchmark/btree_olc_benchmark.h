#pragma once

// #include <perfcpp/event_counter.h>
#include <benchmark/workload_set.h>
#include <tree/btree_olc.h>
#include <chrono>

namespace benchmark {

template <typename Tree> class BtreeOLCBenchmark 
{
public:
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


    void execute_single_run(const benchmark::phase phase)
    {
        auto from = 0U;
        auto to = _workload[phase].size();

        if (phase == benchmark::phase::INSERT)
        {
            for (auto i = from; i < to; ++i)
            {
                const auto &request = _workload[phase][i];
                this->_tree->insert(request.key(), request.value());
            }
        }
        else 
        {
            for (auto i = from; i < to; ++i)
            {
                const auto &request = _workload[phase][i];
                std::int64_t value;
                this->_tree->lookup(request.key(), value);
                builtin::DoNotOptimize(value);
            }
        }
    }

    void validate_tree()
    {
        for (auto i=0; i<_workload[benchmark::phase::INSERT].size(); ++i)
        {
            const auto &request = _workload[benchmark::phase::INSERT][i];
            std::int64_t value;
            this->_tree->lookup(request.key(), value);
            if(value != request.value())
            {
                std::cout << "Error, the tree does not return the correct value. " << request.value() << " expected, but " << value << " returned!\nAborting!" << std::endl;
                exit(1);
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
            
            validate_tree();

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