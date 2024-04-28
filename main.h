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
#include <ctime>
#include <algorithm>

// main
void Router(int M,int N,int C,int v,int* Gx,int* Gy,vector<vector<int>> net_x,vector<vector<int>> net_y, float* cost, int* iter_sssp,int* x1, int* y1,int* x2.int* y2);

// Task_schedular.cpp
inline int connected(int* x1,int* y1,int* x2,int* y2,int i,int j);
void task_manager(int* x1,int* x2,int* y1,int* y2,int* batches, int N);
void bubbleSort(int* arr,int* x1,int* y1,int* x2,int* y2, int n);

// i/o.cpp
void readDataFromFile(const std::string& filename, int* M, int* N, int* C, int* v) ;void readDataFromFile(const char* filename, int* M, int* N, int* C, int* v, std::vector<int>& x1, std::vector<int>& y1, std::vector<int>& x2, std::vector<int>& y2) ;
void StoreToFile(const char* filename,Grid_Graph G, std::vector<Net> nets); 

// patter_route_manager
void pattern_manager(int M, int N, int C, int v int* Gx,int* Gy, <vector<vector<int>> bx1, <vector<vector<int>> by1,<vector<vector<int>> bx2,<vector<vector<int>> by2);

//PatternRoute
pattern_route(int* Gx,int* Gy,int M,int N,int* bx1[0],int* by1[0],int* bx2[0],int* by2[0], int count,float cost,float T);

