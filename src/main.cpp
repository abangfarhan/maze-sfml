/* Generate maze using backtracking algorithm */

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#define side_right 0
#define side_down 1
#define side_left 2
#define side_top 3

const int gridWidth = 80;
const int gridHeight = 40;

struct Node
{
    // side_right, side_down, side_left, side_top
    bool walls[4] = { true, true, true, true };
    bool visited = false;
};

bool indexIsValid(int i, int j);
int getNext_i(int current_i, int side);
int getNext_j(int current_j, int side);
void setWall(Node nodeList[], int i, int j, int side, bool state);
void drawNode(sf::RenderWindow* window, Node nodeList[], int i, int j, int nodeSizePx, bool isCurrent = false);
bool hasUnvisitedNodes(Node nodeList[]);
bool hasUnvisitedNeighbor(Node nodeList[], int i, int j);

int main()
{
    srand(time(NULL));
    const int nodeSizePx = 10;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];

    int current_i = 0, current_j = 0;
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
        window.clear(sf::Color::White);

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

        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(&window, nodeList, i, j, nodeSizePx,
                        (i == current_i && j == current_j));

        // sf::sleep(sf::milliseconds(100));

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
    if ((side == side_down && current_j == gridHeight - 1) || (side == side_top && current_j == 0))
        return -1;

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

void drawNode(sf::RenderWindow* window, Node nodeList[], int i, int j, int nodeSizePx, bool isCurrent)
{
    Node* node = &(nodeList[i + j * gridWidth]);
    if (!(node->walls[0] && node->walls[1] && node->walls[2] && node->walls[3]))
    {
        float scale = 0.6;
        float innerNodeSizePx = nodeSizePx * scale;
        float wallThickness = (nodeSizePx - innerNodeSizePx) / 2;

        sf::RectangleShape cell;
        cell.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness));
        cell.setSize(sf::Vector2f(innerNodeSizePx, innerNodeSizePx));
        cell.setFillColor(sf::Color::Black);
        if (isCurrent)
            cell.setFillColor(sf::Color::Red);

        sf::RectangleShape wall;
        wall.setFillColor(sf::Color::Black);
        if (!node->walls[side_right])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness + innerNodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window->draw(wall);
        }
        if (!node->walls[side_left])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window->draw(wall);
        }
        if (!node->walls[side_top])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window->draw(wall);
        }
        if (!node->walls[side_down])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window->draw(wall);
        }

        window->draw(cell);
    }
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
