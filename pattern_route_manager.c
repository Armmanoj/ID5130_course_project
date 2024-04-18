#include "main.h"

void pattern_manager(int M, int N, int C, int v, int* Gx,int* Gy, <vector<vector<int>> bx1, <vector<vector<int>> by1,<vector<vector<int>> bx2,<vector<vector<int>> by2){
    float T = 1000;
    int count = bx1.size();
    <vector<vector<int>> Bestbx;
    <vector<vector<int>> Bestby;
    
    float tot_cost = std::numeric_limits<int>::max();
    float new_tot_cost = 0;
    for (int i=0; i<count;i++){
        new_tot_cost += pattern_route(Gx,Gy,M,N,bx1[i],by1[i],bx2[i],by2[i], count,cost[i],T);
    }
    // create variables to store the turning points in the best routes found
    <vector<vector<int>> Bestbx;
    <vector<vector<int>> Bestby;
    tot_cost = new_tot_cost;
    while (T>0.01){
        for (int i=0; i<count;i++){
            new_tot_cost += pattern_route(Gx,Gy,M,N,bx1[i],by1[i],bx2[i],by2[i], count,cost[i],T);
        }
        if (new_tot_cost < tot_cost){
            for (int i=0; i<count;i++){
                for (int j=0; j<b1[i].size();j++){
                    Bestbx1[i] = bx1[i];
                    Bestby1[i] = by1[i];
                    Bestbx2[i] = bx2[i];
                    Bestby2[i] = by2[i];
                }
            }
        }
        tot_cost = new_tot_cost;
        new_total_cost = 0;
        T = 0.995*T;
    }
    for (int i=0; i<count;i++){
        for (int j=0; j<b1[i].size();j++){
            bx1[i] = Bestbx1[i];
            by1[i] = Bestby1[i];
            bx2[i] = Bestbx2[i];
            by2[i] = Bestby2[i];
        }
    }
    return;
}