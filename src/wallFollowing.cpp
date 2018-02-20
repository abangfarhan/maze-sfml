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
    bool walls[4] = { true, true, true, true }; // for maze generating
    bool visited = false; // for maze generating
    bool paths[4] = { false, false, false, false }; // for maze solving
    int n_visited = 0; // for maze solving
};
#include "mazeHelper.h"

void drawNodePath(sf::RenderWindow &window, Node nodeList[], int i, int j, int nodeSizePx);

int main()
{
    srand(time(NULL));
    // srand(0);
    const int nodeSizePx = 10;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];
    int direction = side_down;
    int wallSide = side_left;
    int solver_i = 0;
    int solver_j = 0;
    nodeList[solver_i + solver_j * gridWidth].n_visited++;
    int target_i = gridWidth - 1;
    int target_j = gridHeight - 1;

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
            {
                drawNode(window, nodeList, i, j, nodeSizePx,
                        (i == current_i && j == current_j));
                drawNodePath(window, nodeList, i, j, nodeSizePx);
            }

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
            current_i = -1, current_j = -1;

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
                nodeList[solver_i + solver_j * gridWidth].paths[nextSide] = true;
                nodeList[next_i + next_j * gridWidth].paths[(nextSide + 2) % 4] = true;
                nodeList[next_i + next_j * gridWidth].n_visited++;
                // nodeList[next_i + next_j * gridWidth].prevSide = (nextSide + 2) % 4;
                solver_i = next_i;
                solver_j = next_j;
                break;
            }
        }

        // sf::sleep(sf::milliseconds(10));
        window.display();
    }
}

void drawNodePath(sf::RenderWindow &window, Node nodeList[], int i, int j, int nodeSizePx)
{
    Node* node = &(nodeList[i + j * gridWidth]);
    //if (node->paths[0] || node->paths[1] || node->paths[2] || node->paths[3])
    if (node->n_visited > 0)
    {
        float scale = 0.6;
        float innerNodeSizePx = nodeSizePx * scale;
        float wallThickness = (nodeSizePx - innerNodeSizePx) / 2;

        int alpha = std::min(node->n_visited * 100, 255);
        // number > 255 will be modulo-ed, so the opacity will cycle
        // so we need to use std::min to make sure it doesn't go above 255
        sf::Color nodeColor(255, 0, 0, alpha);
        sf::RectangleShape cell;
        cell.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness));
        cell.setSize(sf::Vector2f(innerNodeSizePx, innerNodeSizePx));
        cell.setFillColor(nodeColor);
        window.draw(cell);

        sf::RectangleShape wall;
        wall.setFillColor(nodeColor);
        if (node->paths[side_right])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness + innerNodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (node->paths[side_left])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (node->paths[side_top])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
        if (node->paths[side_down])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
    }
}

