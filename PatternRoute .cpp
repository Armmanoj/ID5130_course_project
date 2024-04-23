#include <cmath>
#include <random>
#include "netlist.h"
using namespace std;

float pattern_route(Grid_Graph G,Point* bestL,Point* L, float T, uniform_real_distribution<float> dis,mt19937 gen){
    float p = dis(gen); // generate a new probability value, will have to change for gpu
    float c_max = cost_edge2(capacity, capacity,k);
    int cost_new;        
    cost_new == routeL(Gx,Gy,x,y,x1,y1,1-orientation);
    if (cost_new<cost){
        ripL(Gx,Gy,x,y,x1,y1,orientation);
    }
    else{
        if (exp((cost-cost_new)/T)>p){
            ripL(Gx,Gy,x,y,x1,y1,orientation);
        }
        else{
            ripL(Gx,Gy,x,y,x1,y1,1-orientation);
        }
    }
    return;
}

inline void ripL(int (*Gx)[], int (*Gy)[], int x,int y,int x1,int y1,char orientation){
    return;
}

inline int routeL(int (*Gx)[], int (*Gy)[], int x,int y,int x1,int y1,char orientation){
    int cost;
    return cost;
;
}

// note that the cost functions calculate the change in global cost due to a connection
inline float cost_edge1(int capacity, int demand, float k){
    float cost = demand+pow(2,k*(demand-capacity-1));
    return cost;
}

inline float cost_edge2(int capacity, int demand, float k){
    float cost = 1+8*pow(2,k*(demand-capacity));
    return cost;
}