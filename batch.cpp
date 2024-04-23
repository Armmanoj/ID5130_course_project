#include "Batch.h" // Include the header file to access the class definition
#include "Grid_Graph.h"
#include "main.h"

// Constructor
Batch(const std::vector<Net>& netVector, const std::vector<Route>& routeVector, int i, int n) : N(n) {
        // Store elements from netVector starting from index i up to index i+n into a vector named nets
        for (int j = i; j < i + n && j < netVector.size(); ++j) {
            nets.push_back(netVector[j]);
        }
    }

void batch::save_patterns(Point* bestL){
    for (int i=0; i<N; i++){
        Point temp = {bestL[i].x,bestLy[i].y};
        nets[i].route[0] = temp;
    }
    return;
}

// Function to perform pattern route
float Batch::pattern_route(Grid_Graph G,Point* L, float T) {
    tot_cost = 0;
    float cost;
    float cost_new;

    for (int = 0;i<N;i++){
        Point point = L[i];
        int orientation;
        if (point.x==nets[i].x1 && point.y==nets[i].y2){ orientation == 0;}
        else{orientation == 1;}

        cost = ripL(G,nets[i],orientation);
        cost_new = survey(G,nets[i],1-orientation);
        if (cost_new<cost){
            routeL(G,nets[i],1-orientation);
            point = {nets[i].x2,nets[i].y1};
            L[i] = point;
        }
        else{
            if (exp((cost-cost_new)/T)>p){
                routeL(G,nets[i],1-orientation);
                point = {nets[i].x2,nets[i].y1};
                L[i] = point;
            }
            else{
                routeL(G,nets[i],orientation);
            }
        }
        tot_cost += cost_new;
    }
    return tot_cost;
}

float Batch::ripL(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (uint16_t y = net.y1 + 1; y < net.y2; ++y) {
            G.Gy[net.x1*(G.M+1)+y] -= 1;
            cost += 1;
        }
        for (uint16_t x = net.x1 + 1; x <= net.x2; ++x) {
            G.Gx[net.y2*(G.N+1)+x] -= 1;
            cost += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (uint16_t x = net.x1 + 1; x < net.x2; ++x) {
            G.Gx[net.y1*(G.N+1)+x] -= 1;
            cost += 1;
        }
        for (uint16_t y = net.y1; y <= net.y2; ++y) {
            G.Gy[net.x2*(G.M+1)+y] -= 1;
            cost += 1;
        }
    }
}

void Batch::routeL(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (uint16_t y = net.y1 + 1; y < net.y2; ++y) {
            G.Gy[net.x1*(G.M+1)+y] += 1;
        }
        for (uint16_t x = net.x1 + 1; x <= net.x2; ++x) {
            G.Gx[net.y2*(G.N+1)+x] += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (uint16_t x = net.x1 + 1; x < net.x2; ++x) {
            G.Gx[net.y1*(G.N+1)+x] += 1;
        }
        for (uint16_t y = net.y1; y <= net.y2; ++y) {
            G.Gy[net.x2*(G.M+1)+y] += 1;
        }
    }
}

float Batch::survey(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (uint16_t y = net.y1 + 1; y < net.y2; ++y) {
            //G.Gy[net.x1*(G.M+1)+y] -= 1;
            cost += 1;
        }
        for (uint16_t x = net.x1 + 1; x <= net.x2; ++x) {
            //G.Gx[net.y2*(G.N+1)+x] -= 1;
            cost += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (uint16_t x = net.x1 + 1; x < net.x2; ++x) {
            //G.Gx[net.y1*(G.N+1)+x] -= 1;
            cost += 1;
        }
        for (uint16_t y = net.y1; y <= net.y2; ++y) {
            //G.Gy[net.x2*(G.M+1)+y] -= 1;
            cost += 1;
        }
    }
}

// Function to perform maze route
void Batch::maze_route(Grid_Graph G, float k, float c) {
    // k represents bounding box size, c is a factor that determine hou severely overflow is penalized 
    // Implement maze route logic here
}


