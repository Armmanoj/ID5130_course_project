#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define MAX_X 4 // Maximum X dimension of the grid
#define MAX_Y 4 // Maximum Y dimension of the grid
#define NUM_NETS 14 // Total number of connections to generate

int main(int argc, char* argv[]) {
    int thread_count = 1;
    if (argc != 2) {
        thread_count = 8;
    } else {
        thread_count = strtol(argv[1], NULL, 10);
    }

    srand((unsigned int)time(NULL));
    
    FILE *file = fopen("sample_capacity.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    
    // Write dimensions
    fprintf(file, "%d %d\n", MAX_Y, MAX_X);

    // Generate and write demand values for each row
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X - 1; j++) {
            fprintf(file, "%d ", rand() % 2); // Random demand value between 0 and 1
        }
        fprintf(file, "\n");
    }

    // Generate and write demand values for each column
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y - 1; j++) {
            fprintf(file, "%d ", rand() % 2); // Random demand value between 0 and 1
        }
        fprintf(file, "\n");
    }

    // Generate nets (connections)
#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (int i = 0; i < NUM_NETS; i++) {
            int x1, y1, x2, y2;
            
            x1 = rand() % MAX_X;
            y1 = rand() % MAX_Y;
            x2 = rand() % MAX_X;
            y2 = rand() % MAX_Y;

#pragma omp critical
            fprintf(file, "%d %d %d %d\n", x1, y1, x2, y2);
        }
    }
    
    fclose(file);

    return 0;
}
