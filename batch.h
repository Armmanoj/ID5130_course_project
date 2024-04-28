#include"Grid_Graph.h"
#include "main.h"
#include "route.h"

// Class representing a batch
class Batch {
private:
    std::vector<Net> nets;  // Vector of nets
    int N; // size of batch

public:
    // Constructor
    Batch(const std::vector<Net>& netVector, int i, int n);

    // Function to perform pattern route
    float pattern_route(Grid_Graph G, uint16_t* bestLx, vuint16_t* bestLy,uint16_t* Lx, vuint16_t* Ly,float T);

    // Function to perform maze route
    void maze_route(Grid_Graph G, float k, float c,float*  Sdist1,char*  Sdir1,float*  Sdis2,char*  Sdir2);
    // k is the ratio of bounding box size to net size
    void batch::save_patterns(uint16_t* bestLx, vuint16_t* bestLy);
};

