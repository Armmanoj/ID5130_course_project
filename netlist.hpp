struct Net;
struct Point;
class Grid_Graph;
class Batch;
#include "main.hpp"

// Class representing the netlist
class Netlist {
public:
    std::vector<Net> nets;      // Vector of nets
    std::vector<Batch> batches; // Vector of batches
    // Constructor
    Netlist(Grid_Graph G,const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3, const std::vector<int>& v4);

    // Function to schedule patterns
    void pattern_schedule();

    // Function to schedule maze with parameter k
    void maze_schedule(Grid_Graph G,float k);

    // Function to perform simulated annealing for pattern route
    float SA_patternroute(Grid_Graph G);

    // Function to perform maze routing
    void mazer(Grid_Graph G,float k);

    inline bool overlap(const Net& net1, const Net& net2, float k);
    inline bool checkRectangleIntersection(const Net& net1, const Net& net2); 
    
};
