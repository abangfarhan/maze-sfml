#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#define right 0
#define down 1
#define left 2
#define top 3

const int gridWidth = 20;
const int gridHeight = 10;

struct Node
{
    // right, down, left, top
    bool walls[4] = { true, true, true, true };
    bool visited = false;
};

int getNext_i(int current_i, int side)
{
    if ((side == right && current_i == gridWidth - 1) || (side == left && current_i == 0))
        return -1;

    int delta_i = 0;
    if (side == right)
        delta_i++;
    else if (side == left)
        delta_i--;
    return current_i + delta_i;
}

int getNext_j(int current_j, int side)
{
    if ((side == down && current_j == gridHeight - 1) || (side == top && current_j == 0))
        return -1;

    int delta_j = 0;
    if (side == down)
        delta_j++;
    else if (side == top)
        delta_j--;
    return current_j + delta_j;
}

void setWall(Node nodeList[], int i, int j, int side, bool state)
{
    int next_i = getNext_i(i, side);
    int next_j = getNext_j(j, side);
    if (next_i == -1 || next_j == -1) return;

    nodeList[i + j * gridWidth].walls[side] = state;
    int oppositeSide = (side + 2) % 4;
    nodeList[next_i + next_j * gridWidth].walls[oppositeSide] = state;
    // if ((side == right && i < gridWidth - 1) ||
    //     (side == left  && i > 1) ||
    //     (side == down  && j < gridHeight - 1) ||
    //     (side == top   && j > 1))
    // {
    //     nodeList[i + j * gridWidth].walls[side] = state;
    //     int oppositeSide = (side + 2) % 4;
    //     switch(side)
    //     {
    //     case right:
    //         i++; break;
    //     case down:
    //         j++; break;
    //     case left:
    //         i--; break;
    //     case top:
    //         j--; break;
    //     }
    //     nodeList[i + j * gridWidth].walls[oppositeSide] = state;
    // }
}


void drawNode(sf::RenderWindow* window, Node node, int i, int j, int nodeSizePx)
{
    if (!(node.walls[0] && node.walls[1] && node.walls[2] && node.walls[3]))
    {
        float scale = 0.6;
        float innerNodeSizePx = nodeSizePx * scale;
        float wallThickness = (nodeSizePx - innerNodeSizePx) / 2;

        sf::RectangleShape cell;
        cell.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness));
        cell.setSize(sf::Vector2f(innerNodeSizePx, innerNodeSizePx));
        cell.setFillColor(sf::Color::Black);

        sf::RectangleShape wall;
        wall.setFillColor(sf::Color(50, 50, 50));
        if (!node.walls[right])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness + innerNodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window->draw(wall);
        }
        if (!node.walls[left])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window->draw(wall);
        }
        if (!node.walls[top])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window->draw(wall);
        }
        if (!node.walls[down])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window->draw(wall);
        }

        window->draw(cell);
    }
}

bool notAllNodesVisited(Node nodeList[])
{
    for (int i = 0; i < gridWidth * gridHeight; ++i)
        if (!nodeList[i].visited)
            return true;
    return false;
}

int main()
{
    srand(time(NULL));
    const int nodeSizePx = 50;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];

    setWall(nodeList, 1, 1, right, false);
    setWall(nodeList, 2, 2, top, false);

    int currentNode[2] = {0, 0};
    nodeList[currentNode[0] + currentNode[1] + gridWidth].visited = true;
    std::stack<int*> nodeStack;

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
        // for (int i = 0; i < gridWidth; ++i)
        //     for (int j = 0; j < gridHeight; ++j)
        //     {
        //         for (int side = 0; side < 4; ++side)
        //             if (rand() < RAND_MAX/4)
        //                 setWall(nodeList, i, j, side, !nodeList[i + j * gridWidth].walls[side]);
        //     }


        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(&window, nodeList[i + j * gridWidth], i, j, nodeSizePx);

        sf::sleep(sf::milliseconds(500));

        window.display();
    }
}
