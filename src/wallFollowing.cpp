/* 
 * Generate maze using backtracking algorithm
 * and solve it using wall following algorithm
 */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

void drawNodePath(sf::RenderWindow &window, Node nodeList[], int col, int row);

int main()
{
    srand(time(NULL));

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];
    int direction = SIDE_DOWN;
    int wallSide = SIDE_LEFT;
    int solver_col = 0;
    int solver_row = 0;
    nodeList[solver_col + solver_row * GRID_WIDTH].n_visited++;
    int target_col = GRID_WIDTH - 1;
    int target_row = GRID_HEIGHT - 1;

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

        for (int col = 0; col < GRID_WIDTH; ++col)
            for (int row = 0; row < GRID_HEIGHT; ++row)
            {
                drawNode(window, nodeList, col, row,
                        (col == cur_col && row == cur_row));
                drawNodePath(window, nodeList, col, row);
            }

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
        else if (!(solver_col == target_col && solver_row == target_row))
        {
            cur_col = -1, cur_row = -1;

            int di = (4 - (wallSide - direction)) % 4;
            for (int i = 0; i < 4 * di; i += di)
            {
                int nextSide = (wallSide + i) % 4;
                if (nodeList[solver_col + solver_row * GRID_WIDTH].walls[nextSide])
                    continue;
                direction = nextSide;
                wallSide = (nextSide + di * 3) % 4;
                int next_col = nextCol(solver_col, nextSide);
                int next_row = nextRow(solver_row, nextSide);
                nodeList[solver_col + solver_row * GRID_WIDTH].paths[nextSide] = true;
                nodeList[next_col + next_row * GRID_WIDTH].paths[(nextSide + 2) % 4] = true;
                nodeList[next_col + next_row * GRID_WIDTH].n_visited++;
                // nodeList[next_col + next_row * GRID_WIDTH].prevSide = (nextSide + 2) % 4;
                solver_col = next_col;
                solver_row = next_row;
                break;
            }
        }

        // sf::sleep(sf::milliseconds(10));
        window.display();
    }
}

void drawNodePath(sf::RenderWindow &window, Node nodeList[], int col, int row)
{
    Node* node = &(nodeList[col + row * GRID_WIDTH]);
    //if (node->paths[0] || node->paths[1] || node->paths[2] || node->paths[3])
    if (node->n_visited > 0)
    {
        float scale = 0.6;
        float innerNodeSizePx = NODE_SIZE * scale;
        float wallThickness = (NODE_SIZE - innerNodeSizePx) / 2;

        int alpha = std::min(node->n_visited * 100, 255);
        // number > 255 will be modulo-ed, so the opacity will cycle
        // so we need to use std::min to make sure it doesn't go above 255
        sf::Color nodeColor(255, 0, 0, alpha);
        sf::RectangleShape cell;
        cell.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE + wallThickness));
        cell.setSize(sf::Vector2f(innerNodeSizePx, innerNodeSizePx));
        cell.setFillColor(nodeColor);
        window.draw(cell);

        sf::RectangleShape wall;
        wall.setFillColor(nodeColor);
        if (node->paths[SIDE_RIGHT])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness + innerNodeSizePx, row * NODE_SIZE + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (node->paths[SIDE_LEFT])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE, row * NODE_SIZE + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (node->paths[SIDE_TOP])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
        if (node->paths[SIDE_DOWN])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
    }
}

