#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

void drawNode(sf::RenderWindow &window, Node nodeList[], int col, int row, bool isCurrent)
{
    Node* node = &(nodeList[col + row * GRID_WIDTH]);
    if (!(node->walls[0] && node->walls[1] && node->walls[2] && node->walls[3]))
    {
        float scale = 0.6;
        float innerNodeSizePx = NODE_SIZE * scale;
        float wallThickness = (NODE_SIZE - innerNodeSizePx) / 2;

        sf::RectangleShape cell;
        cell.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE + wallThickness));
        cell.setSize(sf::Vector2f(innerNodeSizePx, innerNodeSizePx));
        cell.setFillColor(sf::Color::White);
        if (isCurrent)
            cell.setFillColor(sf::Color::Red);
        window.draw(cell);

        sf::RectangleShape wall;
        wall.setFillColor(sf::Color::White);
        if (!node->walls[SIDE_RIGHT])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness + innerNodeSizePx, row * NODE_SIZE + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (!node->walls[SIDE_LEFT])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE, row * NODE_SIZE + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (!node->walls[SIDE_TOP])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
        if (!node->walls[SIDE_DOWN])
        {
            wall.setPosition(sf::Vector2f(col * NODE_SIZE + wallThickness, row * NODE_SIZE + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
    }
}

void drawMaze(sf::RenderWindow &window, Node nodeList[])
{
    int col, row;
    for (col = 0; col < GRID_WIDTH; ++col)
        for (row = 0; row < GRID_HEIGHT; ++row)
            drawNode(window, nodeList, col, row, false);
}

bool indexIsValid(int col, int row)
{
    if (col < 0 || col >= GRID_WIDTH) return false;
    if (row < 0 || row >= GRID_HEIGHT) return false;
    return true;
}

int nextCol(int cur_col, int side)
{
    int dcol = 0;
    if (side == SIDE_RIGHT)
        dcol++;
    else if (side == SIDE_LEFT)
        dcol--;
    return cur_col + dcol;
}

int nextRow(int cur_row, int side)
{
    int drow = 0;
    if (side == SIDE_DOWN)
        drow++;
    else if (side == SIDE_TOP)
        drow--;
    return cur_row + drow;
}

void setWall(Node nodeList[], int col, int row, int side, bool state)
{
    int next_col = nextCol(col, side);
    int next_row = nextRow(row, side);
    if (!indexIsValid(next_col, next_row)) return;

    nodeList[col + row * GRID_WIDTH].walls[side] = state;
    int oppositeSide = (side + 2) % 4;
    nodeList[next_col + next_row * GRID_WIDTH].walls[oppositeSide] = state;
}

bool hasUnvisitedNodes(Node nodeList[])
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
        if (!nodeList[i].visited)
            return true;
    return false;
}

bool hasUnvisitedNeighbor(Node nodeList[], int col, int row)
{
    for (int side = 0; side < 4; ++side)
    {
        int next_col = nextCol(col, side);
        int next_row = nextRow(row, side);
        if (!indexIsValid(next_col, next_row)) continue;
        if (!nodeList[next_col + next_row * GRID_WIDTH].visited)
            return true;
    }
    return false;
}

int connectingSide(int node1_index, int node2_index)
{
    int side;
    switch (node2_index - node1_index)
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
        // ERROR: nodes not adjacent!
        side = -1;
        break;
    }
    return side;
}

void joinNodes(Node nodeList[], Node* node1, Node* node2)
{
    int node1_index = node1 - nodeList;
    int node2_index = node2 - nodeList;
    int side = connectingSide(node1_index, node2_index);
    int oppositeSide = (side + 2) % 4;
    node1->walls[side] = false;
    node2->walls[oppositeSide] = false;
}
