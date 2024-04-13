#include <openacc.h>
#include <cmath>

void SSSP(int (*Gx)[], int (*Gy)[], int x,int y,int x1,int y1,int xl, int yl, int xu, int yu, int (*cost)[], char (*Sdir)[], int (*hist)[]){
    /*
    Gx, Gy -> generates grid graph
    x,y -> source
    x1,y1 -> destination
    xl,yl -> lower left corner of bounding box
    xu, yu -> upper left corner of bounding box
    cost -> stores distace from source to grid point
    Sdir -> stores direction to enter a cell
    hist -> stores history of lengths of previous routes
    Sdist, Sdir are initialized in a previous function
    */
    int u = 1;
    int N = yu-yl;
    int M = xu-xl;
    while (u){
        u=0;
        // left to right
        for (int j = 0; j<N;j++){
            for (int i=1; i<M; i++){
                if (cost[i][j]>cost[i-1][j]+1){
                    cost[i][j] = cost[i-1][j]+1;
                    Sdir[i][j] = 'l';
                    u=1;
                }
            }
        }
        // right to left
        for (int j = 0; j<N;j++){
            for (int i=M-2; i>-1; i--){
                if (cost[i][j]>cost[i+1][j]+1){
                    cost[i][j] = cost[i+1][j]+1;
                    Sdir[i][j] = 'r';
                    u=1;
                }
            }
        }
        // down to up
        for (int i = 0; i<M;i++){
            for (int j=1; j<N; j++){
                if (cost[i][j]>cost[i][j-1]+1){
                    cost[i][j] = cost[i][j-1]+1;
                    Sdir[i][j] = 'd';
                    u=1;
                }
            }
        }
        // up to down
        for (int i = 0; i<M;i++){
            for (int j=M-2; j>-1; j--){
                if (cost[i][j]>cost[i][j+1]+1){
                    cost[i][j] = cost[i][j+1]+1;
                    Sdir[i][j] = 'u';
                    u=1;
                }
            }
        }
    }
    return;
}

