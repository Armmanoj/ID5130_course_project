#include "main.h"
#include "Grid_Graph.h"

// Struct representing a net
struct Net {
    uint16_t x1, y1, x2, y2;
    int Bends;
    Point* route;
};

struct Point {
    uint16_t x;
    uint16_t y;
}
// Class representing the netlist
class Netlist {
private:
    vector<Net> nets;      // Vector of nets
    vector<Batch> batches; // Vector of batches

public:
    // Constructor
    Netlist(Grid_Graph G,const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3, const std::vector<int>& v4, float v);

    // Function to schedule patterns
    void pattern_schedule();

    // Function to schedule maze with parameter k
    void maze_schedule(float k);

    // Function to perform simulated annealing for pattern route
    float SA_patternroute(Grid_Graph G);

    // Function to perform maze routing
    float mazer(Grid_Graph G);

    inline bool overlap(const Net& net1, const Net& net2, float k);
    inline bool checkRectangleIntersection(const Net& net1, const Net& net2) 
};
