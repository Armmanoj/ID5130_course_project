
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

#include <stdio.h>
#include <stdlib.h> 

void task_scheduler(int M, int N, int* net_x, int* net_y, int*** batches_x, int*** batches_y, int* batch_sizes, int* num_batches);
void sssp_algorithm(int M, int N, int* Gx, int* Gy, int* paths_x, int* paths_y, int paths_count);
void pattern_router(int M, int N, int* Gx, int* Gy, int** batch_x1, int** batch_y1, int** batch_x2, int** batch_y2, int num_batches);

void Router(int M, int N, int C, int V, int* net_x, int* net_y) {
    int *Gx, *Gy; 
    int **batches_x, **batches_y;
    int *batch_sizes;
    int num_batches;

    // Gx = malloc(M * sizeof(int));
    // Gy = malloc(N * sizeof(int));
    // batches_x = malloc(num_batches * sizeof(int*));
    // batches_y = malloc(num_batches * sizeof(int*));
    // batch_sizes = malloc(num_batches * sizeof(int));

    task_scheduler(M, N, net_x, net_y, &batches_x, &batches_y, batch_sizes, &num_batches);

    int **paths_x1 = malloc(num_batches * sizeof(int*));
    int **paths_y1 = malloc(num_batches * sizeof(int*));
    int **paths_x2 = malloc(num_batches * sizeof(int*));
    int **paths_y2 = malloc(num_batches * sizeof(int*));

    for (int i = 0; i < num_batches; i++) {
        paths_x1[i] = malloc(batch_sizes[i] * sizeof(int));
        paths_y1[i] = malloc(batch_sizes[i] * sizeof(int));
        paths_x2[i] = malloc(batch_sizes[i] * sizeof(int));
        paths_y2[i] = malloc(batch_sizes[i] * sizeof(int));
        sssp_algorithm(M, N, Gx, Gy, batches_x[i], batches_y[i], batch_sizes[i]);
    }

    pattern_router(M, N, Gx, Gy, paths_x1, paths_y1, paths_x2, paths_y2, num_batches);
}

int main() {
    int M,N,C,V;
    int *net_x = malloc((M+1) * sizeof(int));
    int *net_y = malloc((N+1) * sizeof(int));

    Router(M, N, C, V, net_x, net_y);

    free(net_x);
    free(net_y);
    return 0;
}
