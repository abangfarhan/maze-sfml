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

    int cur_col = rand() % GRID_WIDTH;
    int cur_row = rand() % GRID_HEIGHT;
    nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;
    std::stack<int> stack_col;
    std::stack<int> stack_row;

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
            if (hasUnvisitedNeighbor(nodeList, cur_col, cur_row))
            {
                int nextSide;
                int next_col, next_row;
                do {
                    nextSide = rand() % 4;
                    next_col = nextCol(cur_col, nextSide);
                    next_row = nextRow(cur_row, nextSide);
                } while (!indexIsValid(next_col, next_row) ||
                        nodeList[next_col + next_row * GRID_WIDTH].visited);
                stack_col.push(cur_col);
                stack_row.push(cur_row);
                setWall(nodeList, cur_col, cur_row, nextSide, false);
                cur_col = next_col;
                cur_row = next_row;
                nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;
            }
            else
            {
                cur_col = stack_col.top();
                cur_row = stack_row.top();
                stack_col.pop();
                stack_row.pop();
            }
        }

        for (int col = 0; col < GRID_WIDTH; ++col)
            for (int row = 0; row < GRID_HEIGHT; ++row)
                drawNode(window, nodeList, col, row,
                        (col == cur_col && row == cur_row));

        // sf::sleep(sf::milliseconds(100));

        window.display();
    }
}
