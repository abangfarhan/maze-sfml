#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>

#define side_right 0
#define side_down 1
#define side_left 2
#define side_top 3

const int gridWidth = 80;
const int gridHeight = 40;
const int n_walls = (gridWidth - 1) * gridHeight + gridWidth * (gridHeight - 1);

struct Node {
    // side_right, side_down, side_left, side_top
    bool walls[4] = { true, true, true, true };
    int group;
};

struct Wall {
    bool available = true;
    Node* node1;
    Node* node2;
};

int getNext_i(int current_i, int side);
int getNext_j(int current_j, int side);
bool indexIsValid(int i, int j);
void drawNode(sf::RenderWindow* window, Node nodeList[], int i, int j, int nodeSizePx, bool isCurrent = false);
void mergeGroup(Node nodeList[], int group1, int group2);
int diff2side(int diff);
void setWall(Node nodeList[], Node* node1, Node* node2, bool state);
bool wallsAvailable(Wall wallList[])
{
    for (int i = 0; i < n_walls; ++i)
        if (wallList[i].available)
            return true;
    return false;
}

int main()
{
    srand(time(NULL));
    const int nodeSizePx = 10;
    const int screenWidth = gridWidth * nodeSizePx;
    const int screenHeight = gridHeight * nodeSizePx;

    Node nodeList[gridWidth * gridHeight];
    for (int i = 0; i < gridWidth * gridHeight; ++i)
        nodeList[i].group = i;

    int wall_iter = 0;
    Wall wallList[n_walls];
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
                    wallList[wall_iter] = wall;
                    wall_iter++;
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

        if (wallsAvailable(wallList))
        {
            int rndWall;
            do { rndWall = rand() % n_walls; }
            while (!wallList[rndWall].available);

            Wall* wall = &(wallList[rndWall]);
            Node* node1 = wall->node1;
            Node* node2 = wall->node2;
            if (node1->group != node2->group)
            {
                setWall(nodeList, node1, node2, false);

                mergeGroup(nodeList, node1->group, node2->group);
            }

            wallList[rndWall].available = false;

        }

        for (int i = 0; i < gridWidth; ++i)
            for (int j = 0; j < gridHeight; ++j)
                drawNode(&window, nodeList, i, j, nodeSizePx);

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

bool indexIsValid(int i, int j)
{
    if (i < 0 || i >= gridWidth) return false;
    if (j < 0 || j >= gridHeight) return false;
    return true;
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
        cell.setFillColor(sf::Color::White);
        if (isCurrent)
            cell.setFillColor(sf::Color::Red);

        sf::RectangleShape wall;
        wall.setFillColor(sf::Color::White);
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

// void setWall(Node nodeList[], int i, int j, int side, bool state)
// {
//     int next_i = getNext_i(i, side);
//     int next_j = getNext_j(j, side);
//     if (!indexIsValid(next_i, next_j)) return;
// 
//     nodeList[i + j * gridWidth].walls[side] = state;
//     int oppositeSide = (side + 2) % 4;
//     nodeList[next_i + next_j * gridWidth].walls[oppositeSide] = state;
// }

void mergeGroup(Node nodeList[], int group1, int group2)
{
    for (int i = 0; i < gridWidth * gridHeight; ++i)
    {
        if (nodeList[i].group == group2)
            nodeList[i].group = group1;
    }
}