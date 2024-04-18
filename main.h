#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <opencc.h>


// main
void Router(int M,int N,int C,int v,int* Gx,int* Gy,vector<vector<int>> net_x,vector<vector<int>> net_y, float* cost, int* iter_sssp,int* x1, int* y1,int* x2.int* y2);

// Task_schedular.cpp
inline int connected(int* x1,int* y1,int* x2,int* y2,int i,int j);
void task_manager(int* x1,int* x2,int* y1,int* y2,int* batches, int N);
void bubbleSort(int* arr,int* x1,int* y1,int* x2,int* y2, int n);

// i/o.cpp
void readDataFromFile(const std::string& filename, int* M, int* N, int* C, int* v) ;void readDataFromFile(const char* filename, int* M, int* N, int* C, int* v, std::vector<int>& x1, std::vector<int>& y1, std::vector<int>& x2, std::vector<int>& y2) ;

// patter_route_manager
void pattern_manager(int M, int N, int C, int v int* Gx,int* Gy, <vector<vector<int>> bx1, <vector<vector<int>> by1,<vector<vector<int>> bx2,<vector<vector<int>> by2);

//PatternRoute
pattern_route(int* Gx,int* Gy,int M,int N,int* bx1[0],int* by1[0],int* bx2[0],int* by2[0], int count,float cost,float T);


/*

prompts-

1)
User
I want to make a software tool that does global routing, which is a step in electronic design automation, which invloves finding the optimum route for each net out of a netlist, through a routing grid of dimension MxN. The flow of tasks will be as follows. First the input file containing the netlist will be loaded, then the set of all nets will be grouped using a minimum clique cover algorithm into batches, where each batch consists of nets whose bounding boxes don't intersect. Then all the batches will be routed using simulated anealing algorithm, whose move operation is pattern routing. Routing of each batch may later be parallelized on a gpu. Then the nets are divided into batches once more, this time in a manner optimum for maze routing. Then the nets are greedily routed using maze routing. So the nets will be routed, batch after batch, till all nets are routed, and this will be repeated as many times as needed, improving routing quality each time, till there are no more violations. Then the routing solution will be made into the output format, and stored. Dring the algorithm, the routing paths must be represented in a manner that is not too memory occupying, but is still very fast to access and update. Design and structure this project into modules, classes, etc. 
*/


///--------------------xxx---------------------Classes--------------------xxx----------------------------------------------------------------------------------

typedef struct {
    int x1; int y1; int x2; int y2;
} net;


