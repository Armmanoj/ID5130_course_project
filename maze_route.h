

#include <vector>
#include <queue>
#include <string>
#include <utility>

using namespace std;

// Move struct to store each move's distance and direction in the routing path
struct Move {
    int distance;  // Typically, this would be 1 in grid-based movement
    string direction;  // Direction of the movement
};

// MazeRouter class manages the process of finding a path through a grid
class MazeRouter {
public:
    // Constructor initializes MazeRouter with a reference to a grid graph and its dimensions
    MazeRouter(Grid_Graph& grid);

    // Routes a path from source (src) to destination (dst) using the BFS algorithm
    vector<Move> route(Point src, Point dst);

private:
    // Checks if the specified position (x, y) is within the grid bounds and not blocked
    bool isValid(int x, int y);

    // Reference to the grid graph for accessing the grid's state
    Grid_Graph& gridGraph;

    // Dimensions of the grid
    int M, N;

    // Array to store x-direction increments corresponding to movement directions
    static const int dx[];

    // Array to store y-direction increments corresponding to movement directions
    static const int dy[];

    // Array to store string representations of movement directions
    static const char directions[];
};

