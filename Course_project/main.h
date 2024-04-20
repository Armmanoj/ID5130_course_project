#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <string>

struct Net {
    int x1, y1, x2, y2;
};

class Grid_Graph {
    int M, N, C;
    float v;
    std::vector<int> Gx, Gy;
public:
    Grid_Graph(int M_val, int N_val, int C_val, float v_val);
    ~Grid_Graph();
    int overflows() const;
    int* getGx();
    int* getGy();
};

class Netlist {
    std::vector<Net> nets;
public:
    Netlist(const std::vector<int>& x1, const std::vector<int>& y1, const std::vector<int>& x2, const std::vector<int>& y2);
    void patternRouteManager(Grid_Graph& grid);
};

void readDataFromFile(const std::string& filename, int& M, int& N, int& C, float& v, std::vector<int>& x1, std::vector<int>& y1, std::vector<int>& x2, std::vector<int>& y2);

#endif

