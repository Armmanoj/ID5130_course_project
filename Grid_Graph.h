

class Grid_Graph {
private:
    uint8_t* Gx;
    uint8_t* Gy;
    int C;
    uint16_t M;
    uint16_t N;
    float v;

public:
    // Constructor
    Grid_Graph(uint16_t M_val, uint16_t N_val, uint8_t C_val, float v_val);

    // Destructor
    ~Grid_Graph();

    // Function to calculate number of elements greater than C
    int overflows(float C);
};