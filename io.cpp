#include "main.h"



// Gen-AI prompt
/*
User
I have a text file which stores data in the following format- All lines starting with a ";" character is to be ignored, the first line which 
is ignored has 4 integers seperated by a whitespace, which are M, N, C, and v. Every line after that which is not ignored consists of 4 integers
seperated by a white space, which are the ith elements of 4 vectors x1,y1,x2,y2. Write a cpp function that creates these 4 arrays as well as 
M,N,C and v. The input to the function are pointers to M,N,C,v and vectors x1,y1,x2,y2 and the function stores the values read in them.
*/
void readDataFromFile(const char* filename, uint16_t* M, uint16_t* N, int* C, int* v, std::vector<uint16_t>& x1, std::vector<uint16_t>& y1, std::vector<uint16_t>& x2, std::vector<uint16_t>& y2) {
    std::ifstream file(filename);
    std::string line;
    int lineNumber = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty() || line[0] == ';') {
                // Ignore comment lines
                continue;
            }
            
            std::istringstream iss(line);
            uint16_t a, b, c, d;
            iss >> a >> b >> c >> d;

            if (lineNumber == 0) {
                // First line contains M, N, C, v
                *M = a;
                *N = b;
                *C = c;
                *v = d;
            } else {
                // Remaining lines contain x1, y1, x2, y2
                x1.push_back(a);
                y1.push_back(b);
                x2.push_back(c);
                y2.push_back(d);
            }

            lineNumber++;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

/*
Gen-AI prompt-
Write a cpp function StoreToFile(a string filename,int M, int N,int* Gx, int* GY, vector<vector<int>> net_x, vector<vector<int>> net_y); 
 Gx is a flatenned array with M rows and N+1 columns, while Gy is a flattened array with M+1 rows and N columns. net_x and net_y have the 
 same number of sub-vectors, with corresponding sub-vectors having equall length The function should store the following data in the following format- 
 The first lines should have M N seperated by a whitespace, next Gx should be stored with elements in each row seperated by white spaces and rows in different 
 lines, and then Gy should be stored in the same format. The for each pair of subvectors in net_x and net_y, store the subvectors as net_x[0] net_y[0] net_x[1] 
 net_y[1] ... net_x[...] net_y[...]. seperated by whitespaces, 1 pair of subvectors per row

*/

void StoreToFile(const char* filename,Grid_Graph G, std::vector<Net> nets) {
    std::ofstream file(filename);
    if (file.is_open()) {
        // Write M and N to the file
        file << G.M << " " << G.N << std::endl;

        // Write Gx to the file
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N + 1; ++j) {
                file << G.Gx[i * (N + 1) + j];
                if (j < N) {
                    file << " ";
                }
            }
            file << std::endl;
        }

        // Write Gy to the file
        for (int i = 0; i < M + 1; ++i) {
            for (int j = 0; j < N; ++j) {
                file << G.Gy[i * N + j];
                if (j < N - 1) {
                    file << " ";
                }
            }
            file << std::endl;
        }

        // Write net_x and net_y pairs to the file
        for (size_t i = 0; i < nets.size(); ++i) {
            file << nets[i].x1 << " " << nets[i].y1 << " ";
            for (size_t j = 0; j < nets[i].Bends; ++j) {
                file << nets[i].route[j].x << " " << nets[i].route[j].y << " ";
            }
            file << nets[i].x2 << " " << nets[i].y2 << " ";
            file << std::endl;
        }
        file.close();
    } 
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

