#ifndef NETLIST_HPP
#define NETLIST_HPP

#include "batch.hpp"
#include "Grid_Graph.hpp"
#include "struct.hpp"

class Netlist {
public:
    std::vector<Net> nets;
    std::vector<Batch> batches;

    // Constructor
    Netlist(Grid_Graph G, const std::vector<int>& v1, const std::vector<int>& v2,
            const std::vector<int>& v3, const std::vector<int>& v4, float v);

    // Function declarations
    void pattern_schedule();
    void maze_schedule(float k);
    float SA_patternroute(Grid_Graph G);
    void mazer(Grid_Graph G);

    inline bool overlap(const Net& net1, const Net& net2, float k);
    inline bool checkRectangleIntersection(const Net& net1, const Net& net2);
};

#endif // NETLIST_HPP

