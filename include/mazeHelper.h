#define side_right 0
#define side_down 1
#define side_left 2
#define side_top 3

void drawNode(sf::RenderWindow &window, Node nodeList[], int i, int j, int nodeSizePx, bool isCurrent = false)
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
        window.draw(cell);

        sf::RectangleShape wall;
        wall.setFillColor(sf::Color::White);
        if (!node->walls[side_right])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness + innerNodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (!node->walls[side_left])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx, j * nodeSizePx + wallThickness));
            wall.setSize(sf::Vector2f(wallThickness, innerNodeSizePx));
            window.draw(wall);
        }
        if (!node->walls[side_top])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
        if (!node->walls[side_down])
        {
            wall.setPosition(sf::Vector2f(i * nodeSizePx + wallThickness, j * nodeSizePx + wallThickness + innerNodeSizePx));
            wall.setSize(sf::Vector2f(innerNodeSizePx, wallThickness));
            window.draw(wall);
        }
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
