# Maze with SFML

In this repo I share my implementation of making mazes with C++ and SFML as the graphic library. In the maze, each cell is represented by a struct called `Node`, with basic property (`walls`) of telling what sides of that cell are "blocked".

I will attempt to implement algorithms related to maze generation and maze solving.

The first algorithm that I implemented was the [backtracking algorithm](https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_backtracker) to generate the maze, based on the explanation on Wikipedia. Here is a short preview of the algorithm in action:

![recursive backtracking preview](./images/backtracking.gif)

The second algorithm that I implemented was the [Kruskall's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) based on the explanation on this [blog](http://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm). Here is the preview:

![kruskall preview](./images/kruskall.gif)
