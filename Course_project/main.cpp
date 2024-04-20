#include "main.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input filename> <output filename>" << std::endl;
        return 1;
    }

    int M, N, C;
    float v;
    std::vector<int> x1, y1, x2, y2;

    readDataFromFile(argv[1], M, N, C, v, x1, y1, x2, y2);
    Grid_Graph grid(M, N, C, v);
    Netlist nets(x1, y1, x2, y2);

    nets.patternRouteManager(grid);

    // Optionally, save results or perform further actions
    return 0;
}

