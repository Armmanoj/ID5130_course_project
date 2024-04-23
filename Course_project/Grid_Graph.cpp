#include "main.h"
#include <iostream>

Grid_Graph::Grid_Graph(int M_val, int N_val, int C_val, float v_val) : M(M_val), N(N_val), C(C_val), v(v_val) {
    Gx.resize((N + 1) * M, 0);
    Gy.resize((M + 1) * N, 0);
}

Grid_Graph::~Grid_Graph() {}

int Grid_Graph::overflows() const {
    int count = 0;
    for (int gx : Gx) if (gx > C) count++;
    for (int gy : Gy) if (gy > C) count++;
    return count;
}

int* Grid_Graph::getGx() {
    return Gx.data();
}

int* Grid_Graph::getGy() {
    return Gy.data();
}

