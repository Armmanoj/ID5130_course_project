#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define MAX_X 100
#define MAX_Y 100
#define MIN_DISTANCE 10

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int main(int argc, char* argv[]) {
    int thread_count = 1;

    if (argc != 2) {
        printf("Please enter the number of cores you wanna use :)\n");
        return 1;
    } else {
        thread_count = strtol(argv[1], NULL, 10);
    }

    srand((unsigned int)time(NULL));
//printf(";grid graph, %d %d is M N is = dimensions of array\n", M, N);
//printf("; format is {demand of each of the 3 edges in 1st row\n 2nd row\n ...}\n");
//printf(";nets in form x1 y1 x2 y2 ... after the grid graph,  no comments allowed after the 1st non-comment line\n");

#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (int i = 0; i < 100; i++) {
            int x1, y1, x2, y2;
            
            x1 = rand() % (MAX_X + 1);
            y1 = rand() % (MAX_Y + 1);

            do {
                x2 = rand() % (MAX_X + 1);
                y2 = rand() % (MAX_Y + 1);
            } while (distance(x1, y1, x2, y2) > MIN_DISTANCE && ((x1 != 0) || (x2 != 0) || (y1 != 0) || (y2 != 0)));

            printf("(%d,%d,%d,%d)\n", x1, y1, x2, y2);
        }
    }

    return 0;
}

