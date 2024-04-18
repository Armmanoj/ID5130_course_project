#include "main.h"
using namespace std;
inline int connected(int* x1,int* y1,int* x2,int* y2,int i,int j){
    return !((x1[i]<x1[j]<x2[i])|(x1[i]<x2[j]<x2[i]))&((y1[i]<y1[j]<y2[i])|(y1[i]<y2[j]<y2[i]));
}

void task_manager(int* x1,
    int* x2,
    int* y1,
    int* y2,
    int* batches, int N){
    // initialize batches, the array that stores the batch id for each net
    batches = (int*)calloc(N*sizeof(int));
        // N is the number of nets
    // randomly jumble the nets using Fisher-Yates
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int temp;
    for (int i = N - 1; i > 0; --i) {
        // Generate a random index between 0 and i
        int j = std::rand() % (i+1);
        
        // Swap arr[i] and arr[j]
        temp = x1[i]; x1[i] = x1[j]; x1[j] = temp;
        temp = y1[i]; y1[i] = y1[j]; y1[j] = temp;
        temp = x2[i]; x2[i] = x2[j]; x2[j] = temp;
        temp = y2[i]; y2[i] = y2[j]; y2[j] = temp;
    }

    // greedy algorithm for minimal clique cover
    int id = 1;
    for (int i=0; i<N;i++){
        if (batches[i]==0){
            batches[i] = id;
            for (int j=i+1; j<N;j++){
                if (batches[j]==0){
                    if (connected(x1,y1,x2,y2,i,j)){
                        batches[j] = id;
                    }
                }
            }
        id++;
        }
    }

    // sort the nets, ideally using radix sort if GPU or CPU, and merge or odd-even sort for MPI 
    bubbleSort(batches, x1, y1, x2, y2, N);
    free(batches);
    batches=NULL;
} 

void bubbleSort(int* arr,int* x1,int* y1,int* x2,int* y2, int n) {
    bool swapped;
    do {
        swapped = false;
        int temp;
        for (int i = 0; i < n - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                temp = x1[i]; x1[i] = x1[i+1]; x1[i+1] = temp;
                temp = y1[i]; y1[i] = y1[i+1]; y1[i+1] = temp;
                temp = x2[i]; x2[i] = x2[i+1]; x2[i+1] = temp;
                temp = y2[i]; y2[i] = y2[i+1]; y2[i+1] = temp;
                temp = arr[i]; arr[i] = arr[i+1]; arr[i+1] = temp;
            }
            swapped = true;
        }
    } while (swapped);
}
vector<int> count_batches