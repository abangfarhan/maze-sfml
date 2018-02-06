# Maze with SFML

In this repo I share my implementation of making mazes with C++ and SFML as the graphic library. In the maze, each cell is represented by a struct called `Node`, with basic property (`walls`) of telling what sides of that cell are "blocked".

I will attempt to implement algorithms related to maze generation and maze solving. So far I have only implemented one maze generation algorithm: the [backtracking algorithm](https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_backtracker) based on the explanation on Wikipedia. Here is a short preview of the algorithm in action:

![recursive backtracking preview](./images/backtracking.gif)
