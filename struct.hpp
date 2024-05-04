
// struct Point
struct Point{
    int x;
    int y;
};
typedef struct Point Point;


// Struct representing a net
struct Net {
    int x1, y1, x2, y2;
    std::vector<Point> route;
    Net(int v1, int v2, int v3, int v4, size_t size) 
        : x1(v1), y1(v2), x2(v3), y2(v4), route(size) {}
};
typedef struct Net Net;



