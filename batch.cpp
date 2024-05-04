#include "batch.hpp"
#include "Grid_Graph.hpp"
#include "struct.hpp"

// Constructor
/*
vector<route> is bsed on old way  of storing data, has to be fixed
*/

Batch::Batch(std::vector<Net> netVector, int i, int n) : N(n) {
    // Store elements from netVector starting from index i up to index i+n into a vector named nets
    for (int j = i; j < i + n && j < netVector.size(); ++j) {
        nets.push_back(netVector[j]);
    }
}
Batch::Batch() : N(0) {} // Initializes N to 0

/*
The below function is likely a small mess up, this has to be corrected to account for how data is stored in struct Net, it is called by Netlist.SA_patternroute();
*/
void Batch::save_patterns(std::vector<Point>& L, int k){
    for (int i=0; i<N; i++){
        Point temp = {L[i+k].x,L[i+k].y};
        nets[i].route[0] = temp;
    }
    return;
}

// Function to perform pattern route
/*
L is basically storing the set of routes for pattern routing, is this needed? There may be some issue here
*/
float Batch::pattern_route(Grid_Graph G,std::vector<Point>& L, int k,float T, float p) {
    float tot_cost = 0;
    float cost;
    float cost_new;

    for (int i = 0; i < N ; i++){
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


/*
Gen-AI prompt-
User
A net is a struct of 4 uint16_t , x1,y1,x2,y2, where (x1,y1) form a point on a grid, and (x2,y2) forms another. An L shaped connection is the set of points 
that lie on an L shaped path that goes from the first point in net to the second, excluding the start and end points. Int Orientation = 0  for an L shaped path
 that takes a turn at (x1,y2), and Orientation = 1 if it turns at (x2,y1). write a code that traverses first the path with a given orientation, then the path 
 with the other orientation, and then updates the orientation according to some function, and then traverses according to that orientation 
*/

float Batch::ripL(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (int y = net.y1 + 1; y < net.y2; ++y) {
            G.Gy[net.x1*(G.M+1)+y] -= 1;
            cost += 1;
        }
        for (int x = net.x1 + 1; x <= net.x2; ++x) {
            G.Gx[net.y2*(G.N+1)+x] -= 1;
            cost += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (int x = net.x1 + 1; x < net.x2; ++x) {
            G.Gx[net.y1*(G.N+1)+x] -= 1;
            cost += 1;
        }
        for (int y = net.y1; y <= net.y2; ++y) {
            G.Gy[net.x2*(G.M+1)+y] -= 1;
            cost += 1;
        }
    }
    return cost;
}

void Batch::routeL(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (int y = net.y1 + 1; y < net.y2; ++y) {
            G.Gy[net.x1*(G.M+1)+y] += 1;
        }
        for (int x = net.x1 + 1; x <= net.x2; ++x) {
            G.Gx[net.y2*(G.N+1)+x] += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (int x = net.x1 + 1; x < net.x2; ++x) {
            G.Gx[net.y1*(G.N+1)+x] += 1;
        }
        for (int y = net.y1; y <= net.y2; ++y) {
            G.Gy[net.x2*(G.M+1)+y] += 1;
        }
    }
}

float Batch::survey(Grid_Graph G,Net net,int orientation){
    float cost = 0;
    if (orientation == 0) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (int y = net.y1 + 1; y < net.y2; ++y) {
            //G.Gy[net.x1*(G.M+1)+y] -= 1;
            cost += 1;
        }
        for (int x = net.x1 + 1; x <= net.x2; ++x) {
            //G.Gx[net.y2*(G.N+1)+x] -= 1;
            cost += 1;
        }
    } else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (int x = net.x1 + 1; x < net.x2; ++x) {
            //G.Gx[net.y1*(G.N+1)+x] -= 1;
            cost += 1;
        }
        for (int y = net.y1; y <= net.y2; ++y) {
            //G.Gy[net.x2*(G.M+1)+y] -= 1;
            cost += 1;
        }
    }
    return cost;
}
// Function to perform maze route
void Batch::maze_route(Grid_Graph G, float k, float c,std::vector<float>& Sdist1,std::vector<char>&  Sdir1,std::vector<float>& Sdist2,std::vector<char>&  Sdir2) {
    // 2 pieces of memory are allocated, a big array storing the paths, an array of pointers to starting points
    // access each path by difference of 2 pointers
    // net_pointers only initializes the memory, is not used later
    // path is the array of pointers to net_pointers
    /*
    Look more carefully at if the below double allocation is really necessary
    */
   // First all variables necessary for maze routing are made
   /*
    x,y -> source
    x1,y1 -> destination
    xl,yl -> lower left corner of bounding box
    xu, yu -> upper left corner of bounding box
    Sdist -> stores distance from source to grid point
    Sdir -> stores direction to enter a cell
    Sdist, Sdir are allocated outside the function
    */ 
     
    for (int i=0; i<N; i++){
        Point source {nets[i].x1,nets[i].x2};
        Point dest = {nets[i].y1,nets[i].y2};
        // calculate corners of bounding box
        int centerX1 = (source.x + dest.x) / 2;
        int centerY1 = (source.y + dest.y) / 2;
        int width1 = std::ceil(k * std::abs(source.x- dest.x));
        int height1 = std::ceil(k * std::abs(source.y - dest.y));
        Point cornerl {centerX1 - width1 / 2,centerY1 - height1 / 2};
        Point cornerh {centerX1 + width1 / 2, centerY1 + height1 / 2};
        // next rip up the current route
        // rip up the full batch
        for (int i=0; i<N; i++){
            rip_wire(G, source,dest,nets[i].route, nets[i].route.size());
        }
        // Initializing shortest paths inside the bounding box, use 2 Sdists and Sdirs
        /*
        Sdir1- If a cell is the source, then Sdir = 'x', if it is anywhere else on the source column, then it is 'u', if it is to the right of the source
        column, then it is 'l', else it is 'r'
        Sdir2- If it is at the source, then it is 'd', if it is not on the source column, then it is 'd', if it is on the source column and above the source, 
        then it is 's', else it is 'n'
        Sdist1- If it is the source, then it is 0, if it is on the source column, then it is equall to v plus the cost to reach the cell above it, else it is
        the sum of costs to reach the cell from the projection of the cell onto the source column, plus the cost to reach the projection on the source column,
        Sdist2- If it is on the source column, then it is v + sum of costs to reach the cell from the cell above the source, else it is Sdist1 at the same (i,j)+v
        */
       // Initializing Sdir1
        for (int k = cornerl.y; k<cornerh.y; k++){
            for (int j = cornerl.x; j<cornerh.x; j++){
                if (k==source.y && j == source.x){
                    Sdir1[G.N*k+j] = 'x';
                }
                else if(j == source.x){
                    Sdir1[G.N*k+j] = 'u';
                }
                else if(j > source.x){
                    Sdir1[G.N*k+j] = 'l';
                }
                else{
                    Sdir1[G.N*k+j] = 'r';
                }
            }
        }
        // Initializing Sdir2
        for (int j = cornerl.x; j<cornerh.x; j++){
            for (int k = cornerl.y; k<cornerh.y; k++){
                if (j==source.x){
                    if (k==source.y && j == source.x){
                        Sdir2[G.M*j+k] = 'd';
                    }
                    else if (k > source.y && j == source.x){
                        Sdir2[G.M*j+k] = 's'; // 's' for south
                    }
                    else if (k < source.y && j == source.x){
                        Sdir2[G.M*j+k] = 'n';
                    }
                }
                else{
                    Sdir2[G.M*j+k] = 'd';
                }
            }
        }
        // Initializing Sdist2 on the source column
        float sum_cost = G.v ;
        int j = source.x;
        for (int k = source.y+1; k<cornerh.y; k++){
            sum_cost+=weight(G.Gy[(G.M+1)*j+k+1],G.C,c);
            Sdist2[G.M*j+k] = sum_cost;
        }
        sum_cost = G.v;
        for (int k = source.y-1; k>cornerl.y; k--){
            sum_cost+=weight(G.Gy[(G.M+1)*j+k+1],G.C,c);
            Sdist2[G.M*j+k] = sum_cost;
        }
        Sdist2[G.M*j+source.y] = G.v;
        // Initializing Sdist1 
        for (int k = cornerl.y; k<cornerh.y; k++){
            int j = source.x;
            Sdist1[G.N*j+k] = G.v + Sdist2[G.N*k+j];
            float sum_cost = Sdist1[G.N*j+k];
            for (j = source.x+1; j<cornerh.x; j++){
                sum_cost += weight(G.Gx[(G.N+1)*k+j],G.C,c); 
                Sdist1[G.N*j+k] = sum_cost;
            }
            j = source.x;
            sum_cost = Sdist1[G.N*j+k];
            for (j = source.x-1; j>cornerl.x; j--){
                sum_cost += weight(G.Gx[(G.N+1)*k+j+1],G.C,c);
                 Sdist1[G.N*j+k] = sum_cost;
            }
        }
        Sdist1[G.N*source.x+source.y] = 0;
        // Initializing Sdist2 on the rest of the columns
        for (int j = cornerl.x; j<cornerh.x; j++){
            for (int k = cornerl.y; k<cornerh.y; k++){
                if (j != source.x){
                    Sdist2[G.M*j+k] = G.v+Sdist1[G.N*j+k];
                }
            }
        }

        // Then do Bellman-Ford
        bool flag = 1; // to keep track of if the relaxation step has caused any change to the distances and routes or not
        while (flag){
            flag = 0;
            // Relaxing Sdir1 (rows)
            for (int k = cornerl.y; k<cornerh.y; k++){
                // left to right
                for (int j = cornerl.x+1; j<cornerh.x; j++){
                    if (Sdist1[G.N*k+j] > Sdist1[G.N*k+j-1]+weight(G.Gx[(G.N+1)*k+j],G.C,c)){
                        Sdist1[G.N*k+j] > Sdist1[G.N*k+j-1]+weight(G.Gx[(G.N+1)*k+j],G.C,c);
                        Sdir1[G.N*k+j] = 'l';
                        flag = 1;
                    }
                }
                // right to left
                for (int j = cornerh.x-1; j>cornerl.x; j--){
                    if (Sdist1[G.N*k+j] > Sdist1[G.N*k+j+1]+weight(G.Gx[(G.N+1)*k+j+1],G.C,c)){
                        Sdist1[G.N*k+j] > Sdist1[G.N*k+j+1]+weight(G.Gx[(G.N+1)*k+j+1],G.C,c);
                        Sdir1[G.N*k+j] = 'r';
                        flag = 1;
                    }
                }
            }
            // Relaxing Sdir2 (cols)
            for (int j = cornerl.x; j<cornerh.x; j++){
                // south to  north
                for (int k = cornerl.y+1; k<cornerh.y; k++){
                    if (Sdist2[G.M*j+k] > Sdist2[G.M*j+k-1]+weight(G.Gy[(G.M+1)*j+k],G.C,c)){
                        Sdist2[G.M*j+k] > Sdist2[G.M*j+k-1]+weight(G.Gy[(G.M+1)*j+k],G.C,c);
                        Sdir2[G.M*j+k] = 's';
                        flag = 1;
                    }
                }
                // right to left
                for (int k = cornerh.y-1; k>cornerl.y; k--){
                    if (Sdist2[G.M*j+k] > Sdist2[G.M*j+k+1]+weight(G.Gy[(G.M+1)*j+k+1],G.C,c)){
                        Sdist2[G.M*j+k] > Sdist2[G.M*j+k+1]+weight(G.Gy[(G.M+1)*j+k+1],G.C,c);
                        Sdir2[G.M*j+k] = 'n';
                        flag = 1;
                    }
                }
            }
            // via sweep
            for (int k = cornerl.y; k<cornerh.y; k++){
                for (int j = cornerl.x; j<cornerh.x; j++){
                    if (Sdist1[G.N*k+j] > Sdist2[G.N*j+k]+G.v){
                        Sdist1[G.N*k+j] = Sdist2[G.N*j+k]+G.v;
                        Sdir1[G.N*k+j] =  'u';
                        flag = 1;
                    }
                    if (Sdist1[G.N*k+j] + G.v < Sdist2[G.N*j+k]){
                        Sdist2[G.N*j+k] = Sdist1[G.N*k+j]+G.v;
                        Sdist2[G.N*j+k] =  'd';
                        flag = 1;
                    }
                }
            }
        }

        // Backtrack and store the results
        Point here = dest;
        int layer = 0;
        int b = 0;
        char dir = 'x';
        char diro = 'y';
        while ((here.x != source.x) && (here.y != source.y)){
            if (layer==0){
                dir = Sdir1[G.N*here.y+here.x];
            }
            else{
                dir = Sdir2[G.M*here.x+here.y];
            }
            if (dir != diro){
                if (b<nets[i].route.size()){
                    nets[i].route[b] = here;
                }
                else{
                    nets[i].route.push_back(here);
                }
            }
            if (layer==0){
                if (Sdir1[G.N*(here.x+1)+here.y] == 'u'){
                    layer = 1;
                    diro = 'u';
                }
                else if (Sdir1[G.N*here.x+here.y] == 'r'){
                    here.x += 1;
                    diro  = 'r';
                }
                else if (Sdir1[G.N*here.x+here.y] == 'l'){
                    here.x -= 1;
                    diro = 'l';
                }
            }
            else{
                if (Sdir2[G.M*here.y+here.x] == 'd'){
                    layer = 0;
                    diro = 'd';
                }
                else if (Sdir1[G.M*here.y+here.x] == 's'){
                    here.y -= 1;
                    diro = 's';
                }
                else if (Sdir1[G.M*here.y+here.x] == 'n'){
                    here.x += 1;
                    diro = 'n';
                }
            }
            b++;
        }
    } 
    return;
}


/*
we need to rip multiple bends
*/
void Batch::rip_wire(Grid_Graph G, Point Src, Point Dest, std::vector<Point>& path, int Bends)
{
    int i = 0;
    if (Src.x == path[i].x) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (int y = Src.y + 1; y < path[i].y; ++y) {
            G.Gy[Src.x*(G.M+1)+y] -= 1;
        }
    } 
    else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (int x = Src.x + 1; x < path[i].x; ++x) {
            G.Gx[Src.y*(G.N+1)+x] -= 1;
        }
    }
    for (i=1; i<path.size()-1;i++)
    {
        if (path[i].x == path[i+1].x) {
            // Traverse the path with orientation 0 (turn at x1, y2)
            for (int y = path[i].y + 1; y < path[i+1].y; ++y) {
                G.Gy[path[i].x*(G.M+1)+y] -= 1;
            }
        } 
        else {
            // Traverse the path with orientation 1 (turn at x2, y1)
            for (int x = path[i].x + 1; x < path[i+1].x; ++x) {
                G.Gx[path[i].y*(G.N+1)+x] -= 1;
            }
        }
    }
    i = path.size()-1;
    if (path[i].x == Dest.x) {
        // Traverse the path with orientation 0 (turn at x1, y2)
        for (int y = path[i].y + 1; y < Dest.y; ++y) {
            G.Gy[path[i].x*(G.M+1)+y] -= 1;
        }
    } 
    else {
        // Traverse the path with orientation 1 (turn at x2, y1)
        for (int x = path[i].x + 1; x < Dest.x; ++x) {
            G.Gx[path[i].y*(G.N+1)+x] -= 1;
        }
    }
    return;
}

inline float Batch::weight(float demand, float capacity, float c){
    // the return values of this function can be memoised for further speedup
    return demand*c;
}

