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
