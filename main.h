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
