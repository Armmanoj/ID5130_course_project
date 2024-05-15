#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define MAX_X 4
#define MAX_Y 4
#define MAX_CAP 10

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

    // Write demand values for rows
    for (int i = 0; i < MAX_Y; i++) {
        if (i == 0 || i == MAX_Y - 1) {
            // First and last rows are entirely zeroes
            for (int j = 0; j < MAX_X+1; j++) {
                fprintf(file, "0 ");
            }
        } else {
            // Other rows start with zero and then random values
            fprintf(file, "0 ");
            for (int j = 1; j < MAX_X+1; j++) {
                fprintf(file, "%d ", rand() % (MAX_CAP + 1));
            }
        }
        fprintf(file, "\n");
    }

    // Write demand values for columns
    for (int i = 0; i < MAX_Y + 1; i++) {
        fprintf(file, "0 ");  // Start each column line with a zero
        for (int j = 1; j < MAX_X; j++) {
            if (i == 0 || i == MAX_Y) {
                // First and last rows of columns are entirely zeroes
                fprintf(file, "0 ");
            } else {
                fprintf(file, "%d ", rand() % (MAX_CAP + 1));
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}
