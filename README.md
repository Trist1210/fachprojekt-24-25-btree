## Build the project

    cmake .
    make

## Create the test dataset

    make ycsb

The workload settings can be found in wokload_specification. The default workload consists of 10 M insert statements and 10 M lookup statements. The lookup statements use a uniform distribution (other options are "zipfian" and "latest").
## Execute the benchmarks

    ./bin/b_tree

The tree will now read the files in the folder workloads and execute all insert statements and all lookup statements once. You can also use a different path for the workloads and change the number of repetitions. Use 

    ./bin/b_tree -h 

to get a full overview of the available parameters.


## Experimental Environment
### Numa Balancing: OFF

    echo 0 > /proc/sys/kernel/numa_balancing

### Performance Governor

    cpupower frequency-set -g performance

### Activate Performance Counter

    sysctl -w kernel.perf_event_paranoid=-1
