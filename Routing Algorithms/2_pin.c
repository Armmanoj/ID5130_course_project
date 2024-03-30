#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_POINTS 10 
#define GRID_SIZE 100
#define MAX_ROUTES 10

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point src;
    Point dest;
} Route;

typedef struct Gen {
    int x, y;
    int num_routes;
    int g_cost;
    int h_cost;
    int f_cost;
    struct Gen* parent;
} Gen;

Gen nodes[GRID_SIZE][GRID_SIZE];

void initializeNodes();
int heuristic(Point a, Point b);
void aStar(Point src, Point dest);
void reconstructPath(Gen* current);

Route routes[NUM_POINTS] = {
    {{68, 84}, {70, 77}},
   // {{67, 83}, {69, 78}}, // Comment this line and run the algorithm again, it won't show segmentation fault
    {{44, 31}, {42, 31}},
    {{86, 47}, {94, 42}},
    {{20, 9}, {22, 6}},
    {{63, 64}, {63, 71}},
    {{88, 34}, {90, 25}},
    {{33, 22}, {32, 13}},
    {{44, 17}, {45, 25}},
    {{79, 70}, {80, 66}},
};

int main() {
    initializeNodes();
    for (int i = 0; i < NUM_POINTS; i++) {
        aStar(routes[i].src, routes[i].dest);
        printf("\n");
    }
    return 0;
}

void initializeNodes() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            nodes[i][j].x = i;
            nodes[i][j].y = j;
            nodes[i][j].num_routes = 0;
            nodes[i][j].g_cost = INT_MAX;
            nodes[i][j].h_cost = 0; // This will be calculated for each node when it is processed
            nodes[i][j].f_cost = INT_MAX;
            nodes[i][j].parent = NULL;
        }
    }
}

int heuristic(Point a, Point b) {
 
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void aStar(Point src, Point dest) {
    Gen* openSet[GRID_SIZE * GRID_SIZE];
    int openSetSize = 0;
    
    Gen* startNode = &nodes[src.x][src.y];
    startNode->g_cost = 0;
    startNode->h_cost = heuristic(src, dest);
    startNode->f_cost = startNode->g_cost + startNode->h_cost;
    openSet[openSetSize++] = startNode;

    while (openSetSize > 0) {
        Gen* current = openSet[0]; // For simplification, this example doesn't properly select the lowest f_cost node
        int currentIndex = 0;
        for (int i = 1; i < openSetSize; i++) {
            if (openSet[i]->f_cost < current->f_cost) {
                current = openSet[i];
                currentIndex = i;
            }
        }

        // Remove current from openSet
        for (int i = currentIndex; i < openSetSize - 1; i++) {
            openSet[i] = openSet[i + 1];
        }
        openSetSize--;

        if (current->x == dest.x && current->y == dest.y) {
            reconstructPath(current);
            return;
        }

        current->num_routes++;
        if (current->num_routes > MAX_ROUTES) continue; // Skip if max routes exceeded

        Point directions[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; 
        for (int i = 0; i < 4; i++) {
            int nx = current->x + directions[i].x;
            int ny = current->y + directions[i].y;

            if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
                Gen* neighbour = &nodes[nx][ny];
                if (neighbour->num_routes >= MAX_ROUTES) continue; 

                int tentative_g_cost = current->g_cost + 1; // Assume cost between any two nodes is 1
                if (tentative_g_cost < neighbour->g_cost) {
                    neighbour->parent = current;
                    neighbour->g_cost = tentative_g_cost;
                    neighbour->h_cost = heuristic((Point){nx, ny}, dest);
                    neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;

                    
                    int inOpenSet = 0;
                    for (int j = 0; j < openSetSize; j++) {
                        if (openSet[j] == neighbour) {
                            inOpenSet = 1;
                            break;
                        }
                    }
                    if (!inOpenSet) {
                        openSet[openSetSize++] = neighbour;
                    }
                }
            }
        }
    }
}

void reconstructPath(Gen* current) {
    if (current->parent != NULL) {
        reconstructPath(current->parent);
    }
    printf("(%d, %d) -> ", current->x, current->y);
}

