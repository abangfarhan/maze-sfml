/* Generate maze using backtracking algorithm */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#define side_right 0
#define side_down 1
#define side_left 2
#define side_top 3

const int gridWidth = 80;
const int gridHeight = 40;

struct Node
{
    // side_right, side_down, side_left, side_top
    bool walls[4] = { true, true, true, true };
    bool visited = false;
};
#include "mazeHelper.h"

int main()
{
    srand(time(NULL));
    const int nodeSizePx = 10;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];

    int current_i = rand() % gridWidth;
    int current_j = rand() % gridHeight;
    nodeList[current_i + current_j * gridWidth].visited = true;
    std::stack<int> nodeStack_i;
    std::stack<int> nodeStack_j;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Maze");
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        if (hasUnvisitedNodes(nodeList))
        {
            if (hasUnvisitedNeighbor(nodeList, current_i, current_j))
            {
                int nextSide;
                int next_i, next_j;
                do {
                    nextSide = rand() % 4;
                    next_i = getNext_i(current_i, nextSide);
                    next_j = getNext_j(current_j, nextSide);
                } while (!indexIsValid(next_i, next_j) ||
                        nodeList[next_i + next_j * gridWidth].visited);
                nodeStack_i.push(current_i);
                nodeStack_j.push(current_j);
                setWall(nodeList, current_i, current_j, nextSide, false);
                current_i = next_i;
                current_j = next_j;
                nodeList[current_i + current_j * gridWidth].visited = true;
            }
            else
            {
                current_i = nodeStack_i.top();
                current_j = nodeStack_j.top();
                nodeStack_i.pop();
                nodeStack_j.pop();
            }
        }

        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(window, nodeList, i, j, nodeSizePx,
                        (i == current_i && j == current_j));

        // sf::sleep(sf::milliseconds(100));

        window.display();
    }
}
