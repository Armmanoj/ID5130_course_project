#include "main.h"
#include "Grid_Graph.h"

// Struct representing a net
struct Net {
    int a, b, c, d;
};

// Class representing a batch
class Batch {
    // Add Batch implementation as needed
};

// Forward declaration of Grid_Graph class
class Grid_Graph;

// Class representing the netlist
class Netlist {
private:
    std::vector<Net> nets;      // Vector of nets
    std::vector<Batch> batches; // Vector of batches

    // Helper function to initialize batches
    void initializeBatches();

public:
    // Constructor
    Netlist(const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3, const std::vector<int>& v4);

    // Function to schedule patterns
    void pattern_schedule();

    // Function to schedule maze with parameter k
    void maze_schedule(float k);

    // Function to perform simulated annealing for pattern route
    float SA_patternroute(Grid_Graph G);

    // Function to perform maze routing
    float mazer(Grid_Graph G);
};
