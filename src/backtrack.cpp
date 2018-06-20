/* Generate maze using backtracking algorithm */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

int main()
{
    srand(time(NULL));

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];

    int current_i = rand() % GRID_WIDTH;
    int current_j = rand() % GRID_HEIGHT;
    nodeList[current_i + current_j * GRID_WIDTH].visited = true;
    std::stack<int> nodeStack_i;
    std::stack<int> nodeStack_j;

    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * NODE_SIZE, GRID_HEIGHT * NODE_SIZE), "Maze");
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
                        nodeList[next_i + next_j * GRID_WIDTH].visited);
                nodeStack_i.push(current_i);
                nodeStack_j.push(current_j);
                setWall(nodeList, current_i, current_j, nextSide, false);
                current_i = next_i;
                current_j = next_j;
                nodeList[current_i + current_j * GRID_WIDTH].visited = true;
            }
            else
            {
                current_i = nodeStack_i.top();
                current_j = nodeStack_j.top();
                nodeStack_i.pop();
                nodeStack_j.pop();
            }
        }

        for (int i = 0; i < GRID_WIDTH; ++i)
            for (int j = 0; j < GRID_HEIGHT; ++j)
                drawNode(window, nodeList, i, j,
                        (i == current_i && j == current_j));

        // sf::sleep(sf::milliseconds(100));

        window.display();
    }
}
