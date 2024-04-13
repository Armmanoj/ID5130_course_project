#include "main.h"

void pattern_manager(int M, int N, int* Gx,int* Gy, <vector<vector<int>> bx1, <vector<vector<int>> by1,<vector<vector<int>> bx2,<vector<vector<int>> by2){
    float T = 1000;
    int count = bx1.size();
    <vector<vector<int>> cost(count);
    float tot_cost = std::numeric_limits<int>::max();
    float new_tot_cost = 0;
    for (int i=0; i<count;i++){
        cost[i] = std::vector<int>(count, std::numeric_limits<int>::max());
        new_tot_cost = pattern_route(Gx,Gy,M,N,bx1[0],by1[0],bx2[0],by2[0], bx1[0].size(),cost[0]);
    }
    
    return;
}