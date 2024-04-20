#include "main.h"
#include <fstream>
#include <sstream>
#include <iostream>

void readDataFromFile(const std::string& filename, int& M, int& N, int& C, float& v, std::vector<int>& x1, std::vector<int>& y1, std::vector<int>& x2, std::vector<int>& y2) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;
        std::istringstream iss(line);
        if (x1.empty()) {
            iss >> M >> N >> C >> v;
        } else {
            int a, b, c, d;
            iss >> a >> b >> c >> d;
            x1.push_back(a);
            y1.push_back(b);
            x2.push_back(c);
            y2.push_back(d);
        }
    }
    file.close();
}

