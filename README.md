# Parallel quicksort
## Usage

    Usage: 
    quicksort size granularity
    size - size of the array to sort
    granularity - requlates the size of a block for sequential sorting 
        (block = size / granularity)
## Launches
Achieved approximately x1.5-2.5 speed on size = 10^8 and granularity = {10, 100, 1000}
# Cube graph generator
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
#Parallel BFS
## Usage
    Usage: 
    bfs graph_file launches
    graph_file - file containing a graph
    launches - number of launches of the algorithms
## Launches
Succeeded to reach x2 speed compared to 
the sequential algorithm on cube graph with side 500 and x3.5 with side 600

Examples of launches:

    ./bfs cube500 5
    Loading graph...
    Graph loaded in 61771ms
    Launch #0
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 22439ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 11442ms
        Speed: 1.96111x
    Launch #1
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 22830ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 10087ms
        Speed: 2.26331x
    Launch #2
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 22423ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 10385ms
        Speed: 2.15917x
    Launch #3
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 21193ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 7721ms
        Speed: 2.74485x
    Launch #4
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 21327ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 7972ms
        Speed: 2.67524x
    Avg. Seq time: 22042ms
    Avg. Par time: 9521ms
    Speed of avg. time: 2.31509x
    Avg. speed: 2.36074x
    

    ./bfs cube600 5
    Loading graph...
    Graph loaded in 101825ms
    Launch #0
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 56368ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 16089ms
        Speed: 3.50351x
    Launch #1
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 57359ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 16237ms
        Speed: 3.53261x
    Launch #2
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 56178ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 16469ms
        Speed: 3.41114x
    Launch #3
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 56262ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 16167ms
        Speed: 3.48005x
    Launch #4
        Graph covered by seq: 1
        Distances are good: 1
        Seq time: 57481ms
        Graph covered by par: 1
        Distances are good: 1
        Par time: 16416ms
        Speed: 3.50152x
    Avg. Seq time: 56729ms
    Avg. Par time: 16275ms
    Speed of avg. time: 3.48565x
    Avg. speed: 3.48577x