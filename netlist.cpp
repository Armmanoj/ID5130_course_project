#include "Netlist.h" // Include the header file to access the class definition
#include "main.h"
#include "Grid_Graph.h"
#define costfile "pattern_costs.txt"
// WE use vectors for all memory that will be on cpu only, and dynamically allocated arrays for memory that may be transferred to GPU and/or back

// Constructor
Netlist::Netlist(Grid_Graph G,const std::vector<uint16_t>& v1, const std::vector<uint16_t>& v2, const std::vector<uint16_t>& v3, const std::vector<uint16_t>& v4, float v) {
    // Create nets from input vectors
    // Assuming vectors are of equal length
    size_t N = v1.size();
    for (size_t i = 0; i < N; ++i) {

        Point point = {v1[i],v4[i]};

        for (int y = v2+1; y< point.y+1; y++){
            G.Gy[v1[i]*(M+1)+y] +=1;
        }
        for (int x = v1+1; x< v3+1; x++){
            G.Gx[v4[i]*(N+1)+x] +=1;
        }
        Net net;
        net.x1 = v1[i];
        net.y1 v2[i];
        net.x2 = v3[i];
        net.y2 = v4[i];
        net.Bends = 1; 
        net.route = 
        net.route[0] = new Point[nets.size()]; 
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
    std::vector<int> batch_count(N);
    batch_count.clear();
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

    // Update the batches, by clearing them
    int loc = 0;
    batches.clear();
    batches.resize(batch_count.size());
    for (int i = 0; i < batch_count.size(); i++) {
        Batch batch(nets, loc, batch_count[i]);
        batches.push_back(batch);
        loc += batch_count[i];
    }
}

float Netlist::SA_patternroute(Grid_Graph G){
    
    Point* L = new Point[nets.size()]; 
    // items that may be stored inside GPU, handle with pointers and arrays
    
    float T = 1000;
    int N = nets.size();
    vector<float> costs(0,(int)(5/log10(1/0.995)));

    float tot_cost = std::numeric_limits<float>::max();
    float new_tot_cost = 0;
    for (int i=0; i<N;i++){
        new_tot_cost += batches[i].pattern_route(G,bestL,L,T);
    }
    tot_cost = new_tot_cost;
    int k = 0;

    while (T>0.01){
        k=0;
        for (int i=0; i<N;i++){
            new_tot_cost += batches[i].pattern_route(G,bestL,L,T);
            k+=batches[i].N;
        }
        tot_cost = new_tot_cost;
        new_tot_cost = 0;
        T = 0.995*T;
        costs.push_back(tot_cost);
    }
    // iniitializing the routes
    for (int i=0; i<N;i++){
        batches[i].save_patterns(bestL+k);
    }
    delete[] L;

    // Storing the costs to pattern_costs.txt
    std::ofstream outputFile(costfile);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return -1.0;
    }

    // Write the vector elements to the file
    for (size_t i = 0; i < patternCosts.size(); ++i) {
        outputFile << patternCosts[i]; // Write the float value
        if (i != patternCosts.size() - 1) {
            outputFile << " "; // Add space separator if not the last element
        }
    }

    // Close the file
    outputFile.close();
    return tot_cost;
}

void Netlist::mazer(Grid_Graph G) {
    float*  Sdist1 = (float*)malloc(G.M*G.N*2*sizeof(float)); 
    char*  Sdir1 = (char*)malloc(G.M*G.N*2*sizeof(char));
    float*  Sdist2 = (float*)malloc(G.M*G.N*2*sizeof(float)); 
    char*  Sdir2 = (char*)malloc(G.M*G.N*2*sizeof(char));
    float k = 1.5;
    for (int i = 1000; i > 0; i--) {
        for (int j = 0; j < N; j++) {
            batches[j].maze_route(G, k, 2, Sdist1,Sdir1,Sdist2,Sdir2);
        }
    }
    free(Sdist1);
    free(Sdir1);
    free(Sdist2);
    free(Sdir2);
    return 
}

"""
Gen-AI prompt-
A Net is a struct consisting of 4 integers, x1,y1,x2,y2 where (x1,y1)  and (x2,y2) represents points in a 2d grid, now define the bounding box
of a net as the set of all grid points whose x coordinate is inside an interval of width k*abs(x1-x2) centered at (x1+x2)/2 and whose y coordinates 
is within an interval of k*abs(y1-y2) centered at (y1+y2)/2. Write a cpp function whose arguments are 2 Net objcts, and a float k, and which returns 
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

// Destructor to release memory allocated for the route

