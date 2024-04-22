#include "Batch.h" // Include the header file to access the class definition
#include "Grid_Graph.h"
#include "main.h"

// Constructor
Batch(const std::vector<Net>& netVector, const std::vector<Route>& routeVector, int i, int n) : N(n) {
        // Store elements from netVector starting from index i up to index i+n into a vector named nets
        for (int j = i; j < i + n && j < netVector.size(); ++j) {
            nets.push_back(netVector[j]);
        }
    }

void batch::save_patterns(uint16_t* bestLx, uint16_t* bestLy){
    for (int i=0; i<N; i++){
        Point temp = {bestLx[i],bestLy[i]};
        nets[i].route[0] = temp;
    }
    return;
}

// Function to perform pattern route
void Batch::pattern_route(Grid_Graph G, uint16_t* bestLx, vuint16_t* bestLy,uint16_t* Lx, vuint16_t* Ly, float T) {
    // Implement pattern route logic here
}

// Function to perform maze route
void Batch::maze_route(Grid_Graph G, float k, float c) {
    // k represents bounding box size, c is a factor that determine hou severely overflow is penalized 
    // Implement maze route logic here
}


