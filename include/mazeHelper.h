#define SIDE_RIGHT 0
#define SIDE_DOWN 1
#define SIDE_LEFT 2
#define SIDE_TOP 3

#define GRID_WIDTH 30
#define GRID_HEIGHT 20

#define NODE_SIZE 15

struct Node {
    bool walls[4] = { true, true, true, true };
    int group;
    bool visited = false;
    bool paths[4] = { false, false, false, false };
    int n_visited = 0;
};

void drawNode(sf::RenderWindow &window, Node nodeList[], int col, int row, bool isCurrent = false);

bool indexIsValid(int col, int row);

int nextCol(int cur_col, int side);

int nextRow(int cur_row, int side);

void setWall(Node nodeList[], int col, int row, int side, bool state);

bool hasUnvisitedNodes(Node nodeList[]);

bool hasUnvisitedNeighbor(Node nodeList[], int col, int row);
