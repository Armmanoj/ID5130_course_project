#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define MAX_X 1000
#define MAX_Y 1000
#define MIN_DISTANCE 100

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int main(int argc, char* argv[]) {
    int thread_count = 1;

    if (argc != 2) {
        thread_count = 8;
    } else {
        thread_count = strtol(argv[1], NULL, 10);
    }

    srand((unsigned int)time(NULL));
    
    FILE *file = fopen("sample_test.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    
    fprintf(file, "%d %d %d %d\n", MAX_Y, MAX_X, 16, 50);

#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (int i = 0; i < 200000; i++) {
            int x1, y1, x2, y2;
            
            x1 = rand() % (MAX_X + 1);
            y1 = rand() % (MAX_Y + 1);

            do {
                x2 = rand() % (MAX_X + 1);
                y2 = rand() % (MAX_Y + 1);
            } while (distance(x1, y1, x2, y2) > MIN_DISTANCE && ((x1 != 0) || (x2 != 0) || (y1 != 0) || (y2 != 0)));

            fprintf(file, "%d %d %d %d\n", x1, y1, x2, y2);
        }
    }
    
    fclose(file);

    return 0;
}
