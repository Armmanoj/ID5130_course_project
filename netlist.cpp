#include "Netlist.h" // Include the header file to access the class definition
#include "main.h"
#include "Grid_Graph.h"

// WE use vectors for all memory that will be on cpu only, and dynamically allocated arrays for memory that may be transferred to GPU and/or back

// Constructor
Netlist::Netlist(const std::vector<uint16_t>& v1, const std::vector<uint16_t>& v2, const std::vector<uint16_t>& v3, const std::vector<uint16_t>& v4, float v) {
    // Create nets from input vectors
    // Assuming vectors are of equal length
    size_t N = v1.size();
    for (size_t i = 0; i < N; ++i) {
        Net net = {v1[i], v2[i], v3[i], v4[i], (int)((fabs(v3[i]-v1[i])+fabs(v4[i]-v2[i]))/(2*v))};
        Point point = {v1[i],v4[i]};
        net.route[0] = point;
        // net.route only stores intermediate points in the path
        nets.push_back(net);
    } 
    // no need to exlicitly initialize constructors
}

// Function to schedule patterns
void Netlist::pattern_schedule() {
    // Randomly shuffle the nets using Fisher-Yates
    int N = nets.size();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = N - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(nets[i], nets[j]);
    }

    // Greedy algorithm for minimal clique cover

    std::vector<int> clique(N); // Initialize clique vector with size N
    std::vector<int> batch_count(N);
    int id = 1;
    int count = 0;
    for (int i = 0; i < N; ++i) {
        if (clique[i] == 0) {
            clique[i] = id;
            count = 0;
            for (int j = i + 1; j < N; ++j) {
                if (clique[j] == 0 && checkRectangleIntersection(nets[i], nets[j])) {
                    // make sure the boxes dont intersect even at the edges
                    clique[j] = id;
                    count++;
                }
            }
            batch_count.push_back(count);
            id++;
        }
    }

    // maybe redo the chatgot writing
    std::vector<int> sorted_indices(clique.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0); // Fill with 0, 1, 2, ..., n-1
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return clique[a] < clique[b]; // Sorting based on values in clique vector
    });
    std::sort(clique.begin(), clique.end());

    // Apply the same permutation of indices to net vector
    std::vector<Net> sorted_nets(nets.size());

    for (size_t i = 0; i < sorted_indices.size(); ++i) {
        sorted_nets[i] = nets[sorted_indices[i]];
    }

    // Update original vector with sorted one
    nets = std::move(sorted_nets);

    // Create new batches
    int loc = 0;
    for (int i = 0; i < batch_count.size(); i++) {
        Batch batch(nets, loc, batch_count[i]);
        batches.push_back(batch);
        loc += batch_count[i];
    }
}

// Function to schedule maze with parameter k
void Netlist::maze_schedule(float k) {
    // Randomly shuffle the nets using Fisher-Yates
    int N = nets.size();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = N - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(nets[i], nets[j]);
    }

    // Greedy algorithm for minimal clique cover
    std::vector<int> clique(N); // Initialize clique vector with size N
    std::vector<int> batch_count;
    int id = 1;
    int count = 0;
    for (int i = 0; i < N; ++i) {
        if (clique[i] == 0) {
            clique[i] = id;
            count = 0;
            for (int j = i + 1; j < N; ++j) {
                if (clique[j] == 0 && overlap(nets[i], nets[j], k)) {
                    clique[j] = id;
                    count++;
                }
            }
            batch_count.push_back(count);
            id++;
        }
    }
    std::vector<int> sorted_indices(clique.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0); // Fill with 0, 1, 2, ..., n-1
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return clique[a] < clique[b]; // Sorting based on values in clique vector
    });
    std::sort(clique.begin(), clique.end());

    // Apply the same permutation of indices to nets vector
    std::vector<Net> sorted_nets(nets.size());

    for (size_t i = 0; i < sorted_indices.size(); ++i) {
        sorted_nets[i] = nets[sorted_indices[i]];
    }

    // Update original vector with sorted one
    nets = std::move(sorted_nets);

    // Update the batches, by creating them again
    int loc = 0;
    delete batches;
    batches(batch_count.size());
    for (int i = 0; i < batch_count.size(); i++) {
        Batch batch(nets, loc, batch_count[i]);
        batches.push_back(batch);
        loc += batch_count[i];
    }
}

float Netlist::SA_patternroute(Grid_Graph G){
    uint16_t* bestLx = (uint16_t*)malloc(nets.size()*sizeof(uint16_t));
    uint16_t* Lx = (uint16_t*)malloc(nets.size()*sizeof(uint16_t));
    uint16_t* bestLy = (uint16_t*)malloc(nets.size()*sizeof(uint16_t));
    uint16_t* Ly = (uint16_t*)malloc(nets.size()*sizeof(uint16_t));
    // items that may be stored inside GPU, handle with pointers and arrays
    
    float T = 1000;
    int N = nets.size();

    float tot_cost = std::numeric_limits<float>::max();
    float new_tot_cost = 0;
    for (int i=0; i<N;i++){
        new_tot_cost += batches[i].pattern_route(G,bestLx, bestLy,Lx, Ly,T);
    }
    tot_cost = new_tot_cost;
    int k = 0;

    while (T>0.01){
        k=0;
        for (int i=0; i<N;i++){
            new_tot_cost += batches[i].pattern_route(G,bestLx+k,bestLy+k,Lx+k, Ly+k,T);
            k+=batches[i].N;
        }
        tot_cost = new_tot_cost;
        new_tot_cost = 0;
        T = 0.995*T;
    }
    
    for (int i=0; i<N;i++){
        batches[i].save_patterns(bestLx+k,bestLy+k);
        k+ = batches[i].N;
    }
    free(bestLx);
    free(bestLy);
    free(Lx);
    free(Ly);
    return tot_cost;
}

float Netlist::mazer(Grid_Graph G) {
    float k = 1.5;
    for (int i = 1000; i > 0; i--) {
        for (int j = 0; j < N; j++) {
            batches[j].maze_route(G, k, 2);
        }
    }
    return 0.0
}

"""
Gen-AI prompt-
A Net is a struct consisting of 4 integers, x1,y1,x2,y2 where (x1,y1)  and (x2,y2) represents points in a 2d grid, now define the bounding box
of a net as the set of all grid points whose x coordinate is inside an interval of width k*abs(x1-x2) centered at (x1+x2)/2 and whose y coordinates 
is within an interval of k*abs(y1-y2) centered at (y1+y2)/2. Write a cpp function whose arguments are 2 Net objcts, and a foat k, and which returns 
a bool true if the bounding boxes of the 2 nets dont overlap, and false if they do.
"""
inline bool Netlist::overlap(const Net& net1, const Net& net2, float k) {
    // Calculate the center of bounding boxes for both nets
    int centerX1 = (net1.x1 + net1.x2) / 2;
    int centerY1 = (net1.y1 + net1.y2) / 2;
    int centerX2 = (net2.x1 + net2.x2) / 2;
    int centerY2 = (net2.y1 + net2.y2) / 2;

    // Calculate the width and height of bounding boxes for both nets
    int width1 = std::ceil(k * std::abs(net1.x1 - net1.x2));
    int height1 = std::ceil(k * std::abs(net1.y1 - net1.y2));
    int width2 = std::ceil(k * std::abs(net2.x1 - net2.x2));
    int height2 = std::ceil(k * std::abs(net2.y1 - net2.y2));

    // Calculate the boundaries of bounding boxes for both nets
    int minX1 = centerX1 - width1 / 2;
    int maxX1 = centerX1 + width1 / 2;
    int minY1 = centerY1 - height1 / 2;
    int maxY1 = centerY1 + height1 / 2;

    int minX2 = centerX2 - width2 / 2;
    int maxX2 = centerX2 + width2 / 2;
    int minY2 = centerY2 - height2 / 2;
    int maxY2 = centerY2 + height2 / 2;

    // Check for overlap
    return !(minX1 > maxX2 || maxX1 < minX2 || minY1 > maxY2 || maxY1 < minY2);
}

"""
Gen-AI Prompt-
User
A Net is a struct consisting of 4 integers, x1,y1,x2,y2 where (x1,y1)  and (x2,y2) represents points in a 2d grid. Each net has a "boundary", which 
is the set of all points that lie on the rectangle with its corners given by (x1,y1)(x2,y2),(x1,y2),(x2,y1). Write a cpp function whose arguments are 2
Net objects, and a float k, and which returns a bool true if the rectangles of the 2 nets intersect each other, that is, share any common point, else return false.
Have you remembered that if a rectangle is fully inside another, then too they dont intersect?
"""
inline bool checkRectangleIntersection(const Net& net1, const Net& net2) {
    // Check if rectangles intersect
    bool xOverlap = net1.x1 <= net2.x2 && net1.x2 >= net2.x1;
    bool yOverlap = net1.y1 <= net2.y2 && net1.y2 >= net2.y1;

    // Check if one rectangle is fully inside another
    bool fullyInside = (net1.x1 >= net2.x1 && net1.x2 <= net2.x2 && net1.y1 >= net2.y1 && net1.y2 <= net2.y2) ||
                       (net2.x1 >= net1.x1 && net2.x2 <= net1.x2 && net2.y1 >= net1.y1 && net2.y2 <= net1.y2);

    return xOverlap && yOverlap && !fullyInside;
}