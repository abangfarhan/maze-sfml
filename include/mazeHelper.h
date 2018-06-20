#define SIDE_RIGHT 0
#define SIDE_DOWN 1
#define SIDE_LEFT 2
#define SIDE_TOP 3

#define GRID_WIDTH 80
#define GRID_HEIGHT 40

struct Node {
    bool walls[4] = { true, true, true, true };
    int group;
    bool visited = false;
};

void drawNode(sf::RenderWindow &window, Node nodeList[], int i, int j, int nodeSizePx, bool isCurrent = false);

bool indexIsValid(int i, int j);

int getNext_i(int current_i, int side);

int getNext_j(int current_j, int side);

void setWall(Node nodeList[], int i, int j, int side, bool state);

bool hasUnvisitedNodes(Node nodeList[]);

bool hasUnvisitedNeighbor(Node nodeList[], int i, int j);
