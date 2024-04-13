#include "main.h"
void storeusageToFile(const std::string& filename, int M, int N, int* usagex, int* usagey) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << M << " " << N << std::endl;

        // Write usagex data
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j <= N; ++j) {
                file << usagex[i * (N + 1) + j];
                if (j < N) {
                    file << " ";
                }
            }
            file << std::endl;
        }

        // Write usagey data
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= M; ++j) {
                file << usagey[i * (M + 1) + j];
                if (j < M) {
                    file << " ";
                }
            }
            file << std::endl;
        }

        file.close();
        std::cout << "Data stored in file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// Function to read data from file and store it into a vector of structs
std::vector<Net> readFromFile(const std::string& filename, int* M, int* N, int* C, int* v) {
    std::vector<Net> data; // Vector to store the data
    std::ifstream file(filename); // Open the file

    if (file.is_open()) {
        std::string line;
        bool found_first_line = false;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == ';') {
                continue; // Ignore empty lines or lines starting with ';'
            }

            // If the first line with four integers is found
            if (!found_first_line) {
                std::istringstream iss(line);
                int m, n, c, V;
                if (iss >> m >> n >> c >> V) {
                    *M = m; *N = n; *C = c; *v = V;
                    found_first_line = true;
                } else {
                    std::cerr << "Error: First line should contain four integers\n";
                    return data; // Return empty data if first line is not as expected
                }
            } else {
                // Parse subsequent lines and store data in the vector
                std::istringstream iss(line);
                int a, b, c, d;
                if (iss >> a >> b >> c >> d) {
                    data.push_back({a, b, c, d});
                } else {
                    std::cerr << "Error: Invalid data format\n";
                    return data; // Return data collected so far even if a line is not as expected
                }
            }
        }

        file.close(); // Close the file
    } else {
        std::cerr << "Error: Unable to open file\n";
    }

    return data;
}
