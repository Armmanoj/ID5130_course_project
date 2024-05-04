#include "netlist.hpp" // Include the header file to access the class definition
#include "Grid_Graph.hpp"
#include "batch.hpp"
#include "struct.hpp"

#define costfile "pattern_costs.txt"
// WE use vectors for all memory that will be on cpu only, and dynamically allocated arrays for memory that may be transferred to GPU and/or back

// Constructor
Netlist::Netlist(Grid_Graph G,const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3, const std::vector<int>& v4, float v) {
    // Create nets from input vectors
    // Assuming vectors are of equal length
    int N = v1.size();
    for (int i = 0; i < N; ++i) {

        Point point = {v1[i],v4[i]};

        for (int y = v2[i]; y < point.y+1; y++){
            G.Gy[v1[i]*(G.M+1)+y] +=1;
        }
        for (int x = point.x; x < v3[i]; x++){
            G.Gx[v4[i]*(G.N+1)+x] +=1;
        }
        Net net {v1[i],v2[i],v3[i],v4[i],16};
        net.route.push_back(point);
        // net.route only stores intermediate points in the path
        nets.push_back(net);
    } 
    // no need to exlicitly initialize constructors
}

// Function to schedule patterns, this is all fine
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
    float T = 1000;
    int N = nets.size();
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    // (uniform distribution between 0 and 1)
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<Point> L(N); 
    // items that may be stored inside GPU, handle with pointers and arrays
    std::vector<float> costs(0,(int)(5/log10(1/0.995+1)+1));

    float tot_cost = std::numeric_limits<float>::max();
    float new_tot_cost = 0;
    for (int i=0; i<N;i++){
        new_tot_cost += batches[i].pattern_route(G,L,0,T,1);
    }
    tot_cost = new_tot_cost;
    int k = 0;
    while (T>0.01){
        k=0;
        for (int i=0; i<N;i++){
            new_tot_cost += batches[i].pattern_route(G,L,0,T,dist(gen));
            k+=batches[i].N;
        }
        tot_cost = new_tot_cost;
        new_tot_cost = 0;
        T = 0.995*T;
        costs.push_back(tot_cost);
    }
    // initializing the routes
    k=0;
    for (int i=0; i<N;i++){
        batches[i].save_patterns(L,k);
        k += batches[i].N;
    }

    // Storing the costs to pattern_costs.txt
    std::ofstream outputFile(costfile);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return -1.0;
    }

    // Write the vector elements to the file
    for (size_t i = 0; i < costs.size(); ++i) {
        outputFile << costs[i]; // Write the float value
        if (i != costs.size() - 1) {
            outputFile << " "; // Add space separator if not the last element
        }
    }

    // Close the file
    outputFile.close();
    return tot_cost;
}

void Netlist::mazer(Grid_Graph G) {
    std::vector<float>  Sdist1(G.M*G.N);
    std::vector<char> Sdir1(G.M*G.N);
    std::vector<float>  Sdist2(G.M*G.N); 
    std::vector<char> Sdir2(G.M*G.N);
    float k = 1.5;
    for (int i = 1000; i > 0; i--) {
        for (int j = 0; j < nets.size(); j++) {
            batches[j].maze_route(G, k, 2, Sdist1,Sdir1,Sdist2,Sdir2);
        }
    }
    return; 
}

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

inline bool checkRectangleIntersection(const Net& net1, const Net& net2) {
    // Check if rectangles intersect
    bool xOverlap = net1.x1 <= net2.x2 && net1.x2 >= net2.x1;
    bool yOverlap = net1.y1 <= net2.y2 && net1.y2 >= net2.y1;

    // Check if one rectangle is fully inside another
    bool fullyInside = (net1.x1 >= net2.x1 && net1.x2 <= net2.x2 && net1.y1 >= net2.y1 && net1.y2 <= net2.y2) ||
                       (net2.x1 >= net1.x1 && net2.x2 <= net1.x2 && net2.y1 >= net1.y1 && net2.y2 <= net1.y2);

    return xOverlap && yOverlap && !fullyInside;
}
