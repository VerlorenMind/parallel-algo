# Parallel quicksort
## Usage

    Usage: 
    quicksort size granularity
    size - size of the array to sort
    granularity - requlates the size of a block for sequential sorting 
        (block = size / granularity)
## Launches
Achieved approximately x1.5-2.5 speed on size = 10^8 and granularity = {10, 100, 1000}
# gen-graph
## Usage
    Usage: 
    gen_cube side graph_file
    side - the side of cube graph to be generated
    graph_file - output file
Generates a cube graph with a given side (in vertices).
Output file is in the following form:

    number_of_vertices
    [vertex_i_degree neighbor_0 neighbor_1 ...]
    ...
# Parallel BFS
## Usage
    Usage: 
    bfs graph_file launches
    graph_file - file containing a graph
    launches - number of launches of the algorithms
## Launches
Succeeded to reach x2 speed compared to the sequential algorithm on cube graph with side 600

Example of launch:

    ./bfs cube600 5
    Launch #0
        Graph covered by seq: 1
        Seq time: 21786ms
        Graph covered by par: 1
        Par time: 10832ms
        Speed: 2.01126x
    Launch #1
        Graph covered by seq: 1
        Seq time: 21192ms
        Graph covered by par: 1
        Par time: 11408ms
        Speed: 1.85764x
    Launch #2
        Graph covered by seq: 1
        Seq time: 21133ms
        Graph covered by par: 1
        Par time: 10843ms
        Speed: 1.949x
    Launch #3
        Graph covered by seq: 1
        Seq time: 21426ms
        Graph covered by par: 1
        Par time: 10801ms
        Speed: 1.98371x
    Launch #4
        Graph covered by seq: 1
        Seq time: 21515ms
        Graph covered by par: 1
        Par time: 10791ms
        Speed: 1.99379x
    Avg. Seq time: 21410ms
    Avg. Par time: 10935ms
    Speed of avg. time: 1.95793x
    Avg. speed: 1.95908x

