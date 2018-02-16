/* Generate maze using backtracking algorithm */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

const int gridWidth = 20;
const int gridHeight = 10;

struct Node
{
    // side_right, side_down, side_left, side_top
    bool walls[4] = { true, true, true, true };
    bool visited = false;
};
#include "mazeHelper.h"

bool indexIsValid(int i, int j);
int getNext_i(int current_i, int side);
int getNext_j(int current_j, int side);
void setWall(Node nodeList[], int i, int j, int side, bool state);
bool hasUnvisitedNodes(Node nodeList[]);
bool hasUnvisitedNeighbor(Node nodeList[], int i, int j);

int main()
{
    // srand(time(NULL));
    srand(0);
    const int nodeSizePx = 10;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];
    int direction = side_down;
    int wallSide = side_left;
    int solver_i = 0;
    int solver_j = 0;
    int target_i = gridWidth - 1;
    int target_j = gridHeight - 1;

    // nodeList[1 + 3 * gridWidth].prevSide = side_left;
    // nodeList[0 + 3 * gridWidth].prevSide = side_top;
    // nodeList[0 + 2 * gridWidth].prevSide = side_top;
    // nodeList[0 + 1 * gridWidth].prevSide = side_top;

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

        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(window, nodeList, i, j, nodeSizePx,
                        (i == current_i && j == current_j));

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
        else if (!(solver_i == target_i && solver_j == target_j))
        {
            int di = (4 - (wallSide - direction)) % 4;
            for (int i = 0; i < 4 * di; i += di)
            {
                int nextSide = (wallSide + i) % 4;
                if (nodeList[solver_i + solver_j * gridWidth].walls[nextSide])
                    continue;
                direction = nextSide;
                wallSide = (nextSide + di * 3) % 4;
                int next_i = getNext_i(solver_i, nextSide);
                int next_j = getNext_j(solver_j, nextSide);
                // nodeList[next_i + next_j * gridWidth].prevSide = (nextSide + 2) % 4;
                solver_i = next_i;
                solver_j = next_j;
                break;
            }
        }

        drawNode(window, nodeList, 0, 0, nodeSizePx, true);
        drawNode(window, nodeList, target_i, target_j, nodeSizePx, true);
        drawNode(window, nodeList, solver_i, solver_j, nodeSizePx, true);
        // drawPath(window, nodeList, target_i, target_j, nodeSizePx);

        sf::sleep(sf::milliseconds(10));
        window.display();
    }
}

bool indexIsValid(int i, int j)
{
    if (i < 0 || i >= gridWidth) return false;
    if (j < 0 || j >= gridHeight) return false;
    return true;
}

int getNext_i(int current_i, int side)
{
    int delta_i = 0;
    if (side == side_right)
        delta_i++;
    else if (side == side_left)
        delta_i--;
    return current_i + delta_i;
}

int getNext_j(int current_j, int side)
{
    int delta_j = 0;
    if (side == side_down)
        delta_j++;
    else if (side == side_top)
        delta_j--;
    return current_j + delta_j;
}

void setWall(Node nodeList[], int i, int j, int side, bool state)
{
    int next_i = getNext_i(i, side);
    int next_j = getNext_j(j, side);
    if (!indexIsValid(next_i, next_j)) return;

    nodeList[i + j * gridWidth].walls[side] = state;
    int oppositeSide = (side + 2) % 4;
    nodeList[next_i + next_j * gridWidth].walls[oppositeSide] = state;
}


bool hasUnvisitedNodes(Node nodeList[])
{
    for (int i = 0; i < gridWidth * gridHeight; ++i)
        if (!nodeList[i].visited)
            return true;
    return false;
}

bool hasUnvisitedNeighbor(Node nodeList[], int i, int j)
{
    for (int side = 0; side < 4; ++side)
    {
        int next_i = getNext_i(i, side);
        int next_j = getNext_j(j, side);
        if (!indexIsValid(next_i, next_j)) continue;
        if (!nodeList[next_i + next_j * gridWidth].visited)
            return true;
    }
    return false;
}
