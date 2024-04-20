#include "main.h"
#include <vector>
#include <iostream>
#include <limits>

Netlist::Netlist(const std::vector<int>& x1, const std::vector<int>& y1, const std::vector<int>& x2, const std::vector<int>& y2) {
    for (size_t i = 0; i < x1.size(); i++) {
        nets.push_back({x1[i], y1[i], x2[i], y2[i]});
    }
}

void Netlist::patternRouteManager(Grid_Graph& grid) {
    std::vector<std::vector<int>> bx1, by1, bx2, by2; // These should be filled based on some logic
    float T = 1000.0;
    float tot_cost = std::numeric_limits<float>::max();
    float new_tot_cost = 0;

    for (const auto& net : nets) {
        // dummy implementation for adding batches
        bx1.push_back({net.x1});
        by1.push_back({net.y1});
        bx2.push_back({net.x2});
        by2.push_back({net.y2});
    }

    int count = bx1.size();
    while (T > 0.01) {
        new_tot_cost = 0;
        for (int i = 0; i < count; i++) {
            // Implement pattern_route logic
            // new_tot_cost += pattern_route(grid.getGx(), grid.getGy(), grid.M, grid.N, bx1[i], by1[i], bx2[i], by2[i], count, cost[i], T);
            // Dummy addition

