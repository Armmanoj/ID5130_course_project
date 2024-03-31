#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

// main
// Define a struct to store the data
struct Net {
    int a, b, c, d;
};

// Task_schedular.cpp
inline int connected(int* x1,int* y1,int* x2,int* y2,int i,int j);
void task_manager(int* x1,int* x2,int* y1,int* y2,int* batches, int N);
void bubbleSort(int* arr,int* x1,int* y1,int* x2,int* y2, int n);

// i/o 
void storeusageToFile(const std::string& filename, int M, int N, int* usagex, int* usagey);
std::vector<Net> readFromFile(const std::string& filename, int* M, int* N, int* C, int* v) ;

