#include "main.h"
using namespace std;
    /*
    M rows
    N columns
    C Capacity
    V cost of changing direction
    Gx, Gy are grid graph weights
    net_x stores x coordinates of corners in each path
    net_y stores y coordinates of corners in each path

    Funtions called:
    Task Scheduler -> This splits the entire net to be routed into different independent batches
    SSSP Algorithm -> 
    Pattern router -> This takes care of patterns in routing. It's prototype is void pattern_manager(int M, int N, int* Gx,int* Gy, <vector<vector<int>> bx1, <vector<vector<int>> by1,<vector<vector<int>> bx2,<vector<vector<int>> by2);

    */

void task_scheduler(int M, int N, int* net_x, int* net_y, int** batches_x, int** batches_y, int* batch_sizes, int* num_batches);
void sssp_algorithm(int M, int N, int* Gx, int* Gy, int* paths_x, int* paths_y, int paths_count);
void pattern_router(int M, int N, int* Gx, int* Gy, int** batch_x1, int** batch_y1, int** batch_x2, int** batch_y2, int num_batches);

void Router(int M, int N, int C, int V, int* Gx, int* Gy, vector<int> net_x, vector<int> net_y, vector<int> x1,vector<int> y1,vector<int> x2,vector<int> y2) {
   /*
--- variables initialized- 
    batches
    num_batches
    count
--- functions called-
    task_scheduler
    pattern_route_manager
    */
    int *batches;q
    int num_batches;
    int count = x1.size();
    // Get batches of tasks to be routed, this will be stored in batches as
    task_scheduler_pr(M, N, x1,x2,x3,x4, batches, x1.size());

    // Allocate space for SSSP outputs, assuming it outputs in pairs (x1, y1, x2, y2)
    int **paths_x1 = malloc(num_batches * sizeof(int*));
    int **paths_y1 = malloc(num_batches * sizeof(int*));
    int **paths_x2 = malloc(num_batches * sizeof(int*));
    int **paths_y2 = malloc(num_batches * sizeof(int*));
    pattern_route_manager(M, N, C, V, Gx, Gy, paths_x1, paths_y1, paths_x2, paths_y2);
}

int main() {
    int M,N,C,V;

    int net_x[M+1];
    int net_y[N+1];

    Router(M, N, C, V, net_x, net_y);

    return 0;
}
