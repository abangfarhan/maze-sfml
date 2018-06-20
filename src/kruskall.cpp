#include <iostream>
#include <vector>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

struct Wall {
    Node* node1;
    Node* node2;
};

void mergeGroup(Node nodeList[], int group1, int group2);
int diff2side(int diff);
void setWall(Node nodeList[], Node* node1, Node* node2, bool state);
// FIXME setwall same name, different function
// also defined on mazeHelper.h

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
            // only check SIDE_RIGHT and SIDE_DOWN
            for (int side = 0; side < 2; ++side)
            {
                int next_col = nextCol(col, side);
                int next_row = nextRow(row, side);
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
                setWall(nodeList, node1, node2, false);
                mergeGroup(nodeList, node1->group, node2->group);
            }

            wallVec.erase(wallVec.begin() + rndWall);
        }

        for (int col = 0; col < GRID_WIDTH; ++col)
            for (int row = 0; row < GRID_HEIGHT; ++row)
                drawNode(window, nodeList, col, row);

        // sf::sleep(sf::milliseconds(100));
        window.display();
    }
}

int diff2side(int diff)
{
    int side;
    switch (diff)
    {
    case 1:
        side = SIDE_RIGHT;
        break;
    case -1:
        side = SIDE_LEFT;
        break;
    case GRID_WIDTH:
        side = SIDE_DOWN;
        break;
    case -GRID_WIDTH:
        side = SIDE_TOP;
        break;
    default:
        std::cout << "ERROR: nodes not adjacent" << std::endl;
        break;
    }
    return side;
}

void setWall(Node nodeList[], Node* node1, Node* node2, bool state)
{
    int node1_index = node1 - nodeList;
    int node2_index = node2 - nodeList;
    int side = diff2side(node2_index - node1_index);
    int oppositeSide = (side + 2) % 4;
    node1->walls[side] = state;
    node2->walls[oppositeSide] = state;
}

void mergeGroup(Node nodeList[], int group1, int group2)
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        if (nodeList[i].group == group2)
            nodeList[i].group = group1;
    }
}
