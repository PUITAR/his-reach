# his-reach
experiment code for `his-reach`. 

## INDEX:
1. inc
    - base_on_rocksdb
        - **debug.h:** use this file to print debug infomation.
        - **graph.h:** use this file to get `Nin(v)` and `Nout(v)`.
        - **order.h:** it is a data structure to approach `O(w)` and 'the i-th vertex of O'.
        - **till.h :** it is a data structure to approach `TILL-Index`.
        - **timer.h:** use this tool to get a precise computation time.

    - base_on_memory: this version is for memory.

2. src 
    - base_on_rocksdb 
        - **mkdb.cc          :** making a db for a graph dataset. (from txt)
        - **Online_Reach.cc  :** a baseline of `Online-Reach`.
        - **order.cc         :** to get the Order of all vertexes.
        - **testcase.cc      :** to get `n` testcases to computation.
        - **TILL_Construct.cc:** to construct a `TILL-Index`.

    - base_on_memory: this version is for memory.

3. Makefile
    to compile the file in `src`. and fix som cmds in this file.

## references
1. Efficiently Answering Span-Reachability Queries in Large Temporal Graphs *(ICDE 2020)*