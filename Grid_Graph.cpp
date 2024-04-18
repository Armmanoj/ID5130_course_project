#include "main.h"
#include "Grid_Graph.h"

// Constructor
Grid_Graph::Grid_Graph(int M_val, int N_val, int C_val, float v_val) : M(M_val), N(N_val), C(C_val), v(v_val) {
    Gx = (int*)calloc((N + 1) * M, sizeof(int));
    Gy = (int*)calloc((M + 1) * N, sizeof(int));
}

// Destructor to free memory automatically
Grid_Graph::~Grid_Graph() {
    free(Gx);
    free(Gy);
}

// calculate nuber of electrical shorts in the chip
int Grid_Graph::overflows() {
    int count = 0;
    for (int i = 0; i < (N + 1) * M; i++) {
        if (Gx[i] > C) {
            count++;
        }
    }
    for (int i = 0; i < (M + 1) * N; i++) {
        if (Gy[i] > C) {
            count++;
        }
    }
    return count;
}
