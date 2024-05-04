#ifndef GRID_GRAPH_HPP
#define GRID_GRAPH_HPP

class Grid_Graph {
public:
    int* Gx;
    int* Gy;
    int C;
    int M;
    int N;
    float v;

    // Constructor
    Grid_Graph(int M_val, int N_val, int C_val, float v_val);

    // Destructor
    ~Grid_Graph();

    // Function to calculate number of elements greater than C
    int overflows();
};

#endif // GRID_GRAPH_HPP

