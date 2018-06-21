
/* Generate maze using Randomized Prim's algorithm */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

void addWalls(std::vector<Wall> *wallVec, Node nodeList[], int col, int row)
{
    Node* node1 = &(nodeList[col + row * GRID_WIDTH]);
    for(int side = 0; side < 4; ++side)
    {
        int next_col = nextCol(col, side);
        int next_row = nextRow(row, side);
        if (indexIsValid(next_col, next_row))
        {
            Wall wall;
            wall.node1 = node1;
            wall.node2 = &(nodeList[next_col + next_row * GRID_WIDTH]);
            wallVec->push_back(wall);
        }
    }
}

int main()
{
    srand(time(NULL));

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];

    int cur_col = rand() % GRID_WIDTH;
    int cur_row = rand() % GRID_HEIGHT;

    std::vector<Wall> wallVec;
    addWalls(&wallVec, nodeList, cur_col, cur_row);
    nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;

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

        if (!wallVec.empty())
        {
            int rndWall = rand() % wallVec.size();
            Wall wall = wallVec.at(rndWall);
            Node* node1 = wall.node1;
            Node* node2 = wall.node2;

            if((node1->visited && !node2->visited) || (!node1->visited && node2->visited))
            {
                joinNodes(nodeList, node1, node2);

                int node_index;
                if(!node1->visited) {
                    node_index = node1 - nodeList;
                    node1->visited = true;
                } else {
                    node_index = node2 - nodeList;
                    node2->visited = true;
                }

                cur_row = (int) node_index / GRID_WIDTH;
                cur_col = node_index % GRID_WIDTH;

                addWalls(&wallVec, nodeList, cur_col, cur_row);
            }

            // pushing item on std::vector will add it on the tail
            // so deleting at rndWall will delete at the correct location
            wallVec.erase(wallVec.begin() + rndWall);
        }

        for (int col = 0; col < GRID_WIDTH; ++col)
            for (int row = 0; row < GRID_HEIGHT; ++row)
                drawNode(window, nodeList, col, row,
                        (col == cur_col && row == cur_row));

        window.display();
    }
}
