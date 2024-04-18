class Grid_Graph {
private:
    int* Gx;
    int* Gy;
    int M;
    int N;
    float v;

public:
    // Constructor
    Grid_Graph(int M_val, int N_val, int C_val, float v_val);

    // Destructor
    ~Grid_Graph();

    // Function to calculate number of elements greater than C
    int overflows(float C);
};