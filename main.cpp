
/*
open input files in specified file formats, into list of nets, create grid graphs, initiaizing all edge weights, and capacity, order the nets, then run routing algorithm over all the nets,
test with an input file containing just a single net. 
*/
#include "main.h"
#define fileout "route.out"



int main(int argc, char *argv[]) {
    /*
--- variables initialized- 
    M rows
    N cols
    C capacity
    V cost of bending
    x1,x2,y1,y2
    Gx,Gy grid graph edge weights
    net_x stores x coordinates of corners in each path
    net_y stores y coordinates of edges in each path
--- functions called-
    ReadDataFromFile
    Router
    */
    if (argc != 3) {
        std::cerr << "Correct format is ./" << argv[0] << " <input filename> <output filename\n";
        return 1;
    }

    // Get the filename from command line argument
    char* filename = argv[1];

    // read data from file
    int M, N, C, v;
    std::vector<int> x1, y1, x2, y2;
    readDataFromFile(filename, &M, &N, &C, &v, x1, y1, x2, y2);
    Netlist Netlist(x1,y1,x2,y2);

    // create the return data structures, Gx, Gy are dynamically allocated with pointers due to large size, and as it will be passed to GPU
    Grid_Graph G(M,N,C,v);

    //Do the routing
    float cost;
    cost = Netlist.SA_patternroute(G); 
    int iterations_sssp;
    cost = Netlist.mazer(G, 1.5) ;
    cout << "Final cost is " << cost << std::endl; 
    // Store the result
    StoreToFile(fileout, G, Netlist);
    return 0;
}


t