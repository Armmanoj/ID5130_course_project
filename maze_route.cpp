#include "maze_route.h"
#include "Grid_Graph.h"

const int MazeRouter::dx[] = {-1, 0, 1, 0};
const int MazeRouter::dy[] = {0, 1, 0, -1};
const string MazeRouter::directions[] = {"North", "East", "South", "West"};

// Constructor initializes the router with a reference to a grid graph and its dimensions
MazeRouter::MazeRouter(Grid_Graph& grid) : gridGraph(grid), M(grid.M), N(grid.N) {}

// Checks if the specified cell is within the grid bounds and not blocked
bool MazeRouter::isValid(int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N && gridGraph.Gx[x * N + y] != 0;
}

// Perform maze routing using BFS to find the shortest path from src to dst
vector<Move> MazeRouter::route(Point src, Point dst) {
    vector<vector<bool>> visited(M, vector<bool>(N, false));
    vector<vector<Point>> parent(M, vector<Point>(N, {-1, -1}));
    queue<Point> q;

    visited[src.x][src.y] = true;
    q.push(src);
    
    while (!q.empty()) {
        Point current = q.front(); q.pop();
        
        // Check if destination is reached
        if (current.x == dst.x && current.y == dst.y) {
            break;
        }
        
        // Explore neighbors in the four possible directions
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i], ny = current.y + dy[i];
            if (isValid(nx, ny) && !visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = current;
                q.push({nx, ny});
            }
        }
    }

    vector<Move> path;
    if (!visited[dst.x][dst.y]) {
        return path;  // No path found, return empty vector
    }

    // Backtrack from destination to source using the parent matrix to construct the path
    for (Point at = dst; at.x != src.x || at.y != src.y; at = parent[at.x][at.y]) {
        Point par = parent[at.x][at.y];
        int dir_index = 0;
        for (int i = 0; i < 4; ++i) {
            if (par.x + dx[i] == at.x && par.y + dy[i] == at.y) {
                dir_index = i;
                break;
            }
        }
        path.push_back({1, directions[dir_index]});
    }
    reverse(path.begin(), path.end());

    return path;
}
