#include <iostream>
#include <vector>
#include <time.h>
#include <SFML/Graphics.hpp>

const int gridWidth = 40;
const int gridHeight = 20;
const int n_walls = (gridWidth - 1) * gridHeight + gridWidth * (gridHeight - 1);

struct Node {
    // side_right, side_down, side_left, side_top
    bool walls[4] = { true, true, true, true };
    int group;
    bool visited; // FIXME because mazeHelper.h assumed this
};
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
    const int nodeSizePx = 20;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];
    for (int i = 0; i < gridWidth * gridHeight; ++i)
        nodeList[i].group = i;

    std::vector<Wall> wallVec;
    for (int i = 0; i < gridWidth; ++i)
        for (int j = 0; j < gridHeight; ++j)
        {
            Node* node1 = &(nodeList[i + j * gridWidth]);
            // only check side_right and side_down
            for (int side = 0; side < 2; ++side)
            {
                int next_i = getNext_i(i, side);
                int next_j = getNext_j(j, side);
                if (indexIsValid(next_i, next_j))
                {
                    Wall wall;
                    wall.node1 = node1;
                    wall.node2 = &(nodeList[next_i + next_j * gridWidth]);
                    wallVec.push_back(wall);
                }
            }
        }

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

        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(window, nodeList, i, j, nodeSizePx);

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
        side = side_right;
        break;
    case -1:
        side = side_left;
        break;
    case gridWidth:
        side = side_down;
        break;
    case -gridWidth:
        side = side_top;
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
    for (int i = 0; i < gridWidth * gridHeight; ++i)
    {
        if (nodeList[i].group == group2)
            nodeList[i].group = group1;
    }
}
