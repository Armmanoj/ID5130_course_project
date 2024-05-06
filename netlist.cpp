#include "netlist.hpp"
#include "Grid_Graph.hpp"
#include "batch.hpp"
#include "struct.hpp"

#define costfile "pattern_costs.txt"

// Constructor
Netlist::Netlist(Grid_Graph G, const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3, const std::vector<int>& v4) {
    int N = v1.size();
    std::cout << "here" << std::endl;
    for (int i = 0; i < N; ++i) {
        Point point = {v1[i], v4[i]};
        std::cout << "here" << std::endl;
        for (int y = v2[i]; y <= point.y; y++) {
            std::cout << "here" << std::endl;
            G.Gy[v1[i] * (G.M + 1) + y] += 1;
        }
        for (int y = point.y; y <= v2[i]; y++) {
            std::cout << "here" << std::endl;
            G.Gy[v1[i] * (G.M + 1) + y] += 1;
        }
        for (int x = point.x; x < v3[i]; x++) {
            std::cout << "here" << std::endl;
            G.Gx[v4[i] * (G.N + 1) + x] += 1;
        }
        for (int x = v3[i]; x < point.x; x++) {
            std::cout << "here" << std::endl;
            G.Gx[v4[i] * (G.N + 1) + x] += 1;
        }
        
        Net net {v1[i], v2[i], v3[i], v4[i], 16};
        if (!(((point.x==net.x1)&&(point.y==net.y1))||((point.x==net.x2)&&(point.y==net.y2)))){
            net.route.push_back(point);
        }
        nets.push_back(net);
    }
}

// Function to schedule patterns
void Netlist::pattern_schedule() {
    int N = nets.size();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = N - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(nets[i], nets[j]);
    }

    std::vector<int> clique(N, 0);
    std::vector<int> batch_count;
    int id = 1;

    for (int i = 0; i < N; ++i) {
        if (clique[i] == 0) {
            clique[i] = id;
            int count = 0;
            for (int j = i + 1; j < N; ++j) {
                if (clique[j] == 0 && checkRectangleIntersection(nets[i], nets[j])) {
                    clique[j] = id;
                    count++;
                }
            }
            batch_count.push_back(count + 1); // Include the first element
            id++;
        }
    }

    std::vector<int> sorted_indices(clique.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return clique[a] < clique[b];
    });

    std::vector<Net> sorted_nets(nets.size());
    for (size_t i = 0; i < sorted_indices.size(); ++i) {
        sorted_nets[i] = nets[sorted_indices[i]];
    }

    nets = std::move(sorted_nets);

    int loc = 0;
    for (int count : batch_count) {
        Batch batch(nets, loc, count);
        batches.push_back(batch);
        loc += count;
    }
}

// Function to schedule maze with parameter k
void Netlist::maze_schedule(Grid_Graph G,float k) {
    int N = nets.size();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = N - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(nets[i], nets[j]);
    }

    std::vector<int> clique(N, 0);
    std::vector<int> batch_count;
    int id = 1;

    for (int i = 0; i < N; ++i) {
        if (clique[i] == 0) {
            clique[i] = id;
            int count = 0;
            for (int j = i + 1; j < N; ++j) {
                if (clique[j] == 0 && !overlap(nets[i], nets[j], k)) {
                    clique[j] = id;
                    count++;
                }
            }
            batch_count.push_back(count + 1);
            id++;
        }
    }

    std::vector<int> sorted_indices(clique.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return clique[a] < clique[b];
    });
    std::cout << "All sorting is done " << std::endl;
    std::vector<Net> sorted_nets(nets.size());
    for (size_t i = 0; i < sorted_indices.size(); ++i) {
        sorted_nets[i] = nets[sorted_indices[i]];
    }

    nets = std::move(sorted_nets);

    int loc = 0;
    int count;
    batches.clear();
    std::cout << "batches are cleared" << std::endl;
    for (int cg = 0; cg < batch_count.size();cg++) {
        count = batch_count[cg];
        Batch batch(nets, loc, count);
        batches.push_back(batch);
        loc += count;
    }
    std::cout << "Done making new batch" << std::endl;
    return;
}

// Function for simulated annealing pattern routing
float Netlist::SA_patternroute(Grid_Graph G) {
    std::cerr << "Starting SA_patternroute..." << std::endl;

    float T = 1000;
    int N = nets.size();
    std::cerr << "Initial temperature: " << T << ", Number of nets: " << N << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<float> costs;

    float tot_cost = std::numeric_limits<float>::max();
    float new_tot_cost = 0;
    
    // Debug: Processing batches initially
    std::cerr << "Processing batches initially..." << std::endl;
    for (auto& batch : batches) {
        new_tot_cost += batch.pattern_route(G, 0, T, 1);
    }
    tot_cost = new_tot_cost;
    std::cerr << "Initial total cost: " << tot_cost << std::endl;

    // Start the simulated annealing process
    std::cerr << "Starting simulated annealing process..." << std::endl;
    while (T > 0.01) {
        new_tot_cost = 0;
        
        // Debug: Processing each batch
        std::cerr << "Processing batches at T=" << T << std::endl;
        for (auto& batch : batches) {
            new_tot_cost += batch.pattern_route(G, 0, T, dist(gen));
        }
        tot_cost = new_tot_cost;
        T *= 0.995;
        costs.push_back(tot_cost);
        std::cerr << "Current cost after processing: " << tot_cost << std::endl;
    }

    // Save patterns for each batch
    std::cerr << "Saving patterns..." << std::endl;
    int k = 0;
    for (auto& batch : batches) {
        std::cerr << "Saving pattern for batch, starting at index " << k << std::endl;
        k += batch.N;
    }

    // Write costs to output file
    std::cerr << "Writing costs to output file: " << costfile << std::endl;
    std::ofstream outputFile(costfile);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return -1.0f;
    }

    for (size_t i = 0; i < costs.size(); ++i) {
        outputFile << costs[i];
        if (i != costs.size() - 1) {
            outputFile << " ";
        }
    }

    outputFile.close();
    std::cerr << "Finished writing costs." << std::endl;

    std::cerr << "Total final cost: " << tot_cost << std::endl;
    std::cerr << "Finished SA_patternroute." << std::endl;
    return tot_cost;
}

// Function to perform maze routing
void Netlist::mazer(Grid_Graph G,float k) {
    std::vector<float> Sdist1(G.M * G.N);
    std::vector<char> Sdir1(G.M * G.N);
    std::vector<float> Sdist2(G.M * G.N);
    std::vector<char> Sdir2(G.M * G.N);
    Batch batch;
    for (int i=0; i<MAZE_ROUTE_ITER; i++){
        for (int j=0; j<batches.size(); j++) {
            batch = batches[j];
            std::cout << "Maze routing a batch " << std::endl;
            batch.maze_route(G, k, 2, Sdist1, Sdir1, Sdist2, Sdir2);
        }
    }
}

// Function to check if two nets overlap
inline bool Netlist::overlap(const Net& net1, const Net& net2, float k) {
    int centerX1 = (net1.x1 + net1.x2) / 2;
    int centerY1 = (net1.y1 + net1.y2) / 2;
    int centerX2 = (net2.x1 + net2.x2) / 2;
    int centerY2 = (net2.y1 + net2.y2) / 2;

    int width1 = std::max((int)std::ceil(k * std::abs(net1.x1 - net1.x2)),BOX_MIN_DIM);
    int height1 = std::max((int)std::ceil(k * std::abs(net1.y1 - net1.y2)),BOX_MIN_DIM);
    int width2 = std::max((int)std::ceil(k * std::abs(net2.x1 - net2.x2)),BOX_MIN_DIM);
    int height2 = std::max((int)std::ceil(k * std::abs(net2.y1 - net2.y2)),BOX_MIN_DIM);

    int minX1 = centerX1 - width1 / 2;
    int maxX1 = centerX1 + width1 / 2;
    int minY1 = centerY1 - height1 / 2;
    int maxY1 = centerY1 + height1 / 2;

    int minX2 = centerX2 - width2 / 2;
    int maxX2 = centerX2 + width2 / 2;
    int minY2 = centerY2 - height2 / 2;
    int maxY2 = centerY2 + height2 / 2;

    return !(minX1 > maxX2 || maxX1 < minX2 || minY1 > maxY2 || maxY1 < minY2);
}

// Function to check if two nets' rectangles intersect
inline bool Netlist::checkRectangleIntersection(const Net& net1, const Net& net2) {
    bool xOverlap = net1.x1 <= net2.x2 && net1.x2 >= net2.x1;
    bool yOverlap = net1.y1 <= net2.y2 && net1.y2 >= net2.y1;

    bool fullyInside = (net1.x1 >= net2.x1 && net1.x2 <= net2.x2 && net1.y1 >= net2.y1 && net1.y2 <= net2.y2) ||
                       (net2.x1 >= net1.x1 && net2.x2 <= net1.x2 && net2.y1 >= net1.y1 && net2.y2 <= net1.y2);

    return xOverlap && yOverlap && !fullyInside;
}

