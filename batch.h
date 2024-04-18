#include"Grid_Graph.h"
#include "main.h"
#include "route.h"

// Class representing a batch
class Batch {
private:
    std::vector<Net> nets;  // Vector of nets
    std::vector<Route> routes;  // Vector of routes

public:
    // Constructor
    Batch(const std::vector<Net>& netVector, int i, int N);

    // Function to perform pattern route
    void pattern_route(Grid_Graph G, float T);

    // Function to perform maze route
    void maze_route(Grid_Graph G, float k);
};

