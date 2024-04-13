/*
open input files in specified file formats, into list of nets, create grid graphs, initiaizing all edge weights, and capacity, order the nets, then run routing algorithm over all the nets,
test with an input file containing just a single net. 
*/

#include "main.h"




int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Correct format is ./" << argv[0] << " <input filename> <output filename\n";
        return 1;
    }

    // Get the filename from command line argument
    std::string filename = argv[1];

    // Call the function to read data from file
    int M,N,C,v;
    std::vector<Net> data = readFromFile(filename,&M,&N,&C,&v);
    int* usagex = (int*)malloc((N+1)*M*sizeof(int));
    for (int i = 0; i<(N+1)*M; i++){usagex[i] = 0;}
    int* usagey = (int*)malloc((M+1)*N*sizeof(int));
    for (int i = 0; i<(M+1)*N; i++){usagey[i] = 0;}
    
    free(usagex);
    free(usagey);
    // Print the stored data (for demonstration)
    for (const auto& net : data) {
        std::cout << net.a << " " << net.b << " " << net.c << " " << net.d << << " " << std::endl;
    }

    return 0;
}
