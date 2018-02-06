# Graph SFML

In this repo I created a graph using C++ with the SFML library. This repo is created for educational purpose. For now I only focus on creating the graph. Some definitions that I used:

- A graph is an array of nodes.
- A node, defined by the `Node` class, mainly consisted of `x` and `y` coordinates.
- Each node also has neighbors, which is just a vector of pointers to other nodes.
- When we add a neighbor to a node, that neighbor will also add that node as its neighbor, except if that neighbor already has that node as its neighbor. In other words, there is no direction between the connection of two nodes.

In addition to that, I also use SFML to display the graph. The creation of the nodes has also been randomized.

In the near future, I will use this program to implement algorithms related with graphs.

## Shortest Path Algorithms

I have implemented one shortest path algorithm, the [Djikstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm). In `NodeDjikstra.cpp` and `NodeDjikstra.h` I created a class `NodeDj` that inherited the original `Node` class. This class enable the graph to be solvable using Djikstra's algorithm, such as the method to get the previous node, or the method to tell whether the node has been "visited" or not. The program will highlight lines that have been visited by the algorithm, and show the path from that node to the source (`start`) node. A green line indicates the path from the source node to the target (`dest`) node. By default the program will attempt to make a complete traversal (find shortest path for each node). But changing the variable `complete_traversal` to `false` will make the program only attempt to find the shortest find between the source node to the target node.
