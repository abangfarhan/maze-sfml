#include <iostream>
#include <vector>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

void mergeGroup(Node nodeList[], int group1, int group2);

int main()
{
    srand(time(NULL));

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
        nodeList[i].group = i;

    std::vector<Wall> wallVec;
    for (int col = 0; col < GRID_WIDTH; ++col)
        for (int row = 0; row < GRID_HEIGHT; ++row)
        {
            Node* node1 = &(nodeList[col + row * GRID_WIDTH]);
            int check_side[2] = {SIDE_RIGHT, SIDE_DOWN};
            for (int i = 0; i < 2; ++i)
            {
                int next_col = nextCol(col, check_side[i]);
                int next_row = nextRow(row, check_side[i]);
                if (indexIsValid(next_col, next_row))
                {
                    Wall wall;
                    wall.node1 = node1;
                    wall.node2 = &(nodeList[next_col + next_row * GRID_WIDTH]);
                    wallVec.push_back(wall);
                }
            }
        }

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
            Wall* wall = &(wallVec.at(rndWall));
            Node* node1 = wall->node1;
            Node* node2 = wall->node2;
            if (node1->group != node2->group)
            {
                joinNodes(nodeList, node1, node2);
                mergeGroup(nodeList, node1->group, node2->group);
            }

            wallVec.erase(wallVec.begin() + rndWall);
        }

        drawMaze(window, nodeList);

        // sf::sleep(sf::milliseconds(100));
        window.display();
    }
}

void mergeGroup(Node nodeList[], int group1, int group2)
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        if (nodeList[i].group == group2)
            nodeList[i].group = group1;
    }
}
