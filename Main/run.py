# first open output file and display grid graph as heat map 
# 
# Gridxutomatically check both the generated wires as well as the "heat map" solution, and return the number of incorrectly or not routed nets, and violations or "shorts".
import matplotlib.pyplot as plt
import numpy as np
import sys

def read_out_file(filename):
    wiring = []
    with open(filename, 'r') as file:
        # Skip lines starting with ';' and empty lines until the first non-empty, non-comment line
        line = file.readline().strip()
        while line.startswith(';') or not line:
            line = file.readline().strip()
        
        # Extract M and N
        M, N = map(int, line.split())
        
        # Read data for array Gridx
        Gridx = np.zeros((M,N+1), dtype=int)
        for i in range(M):
            line = file.readline().strip()
            Gridx[i] = list(map(int, line.split()))
        
        # Read data for Gridy
        Gridy = np.zeros((M+1,N), dtype=int)
        for i in range(M+1):
            line = file.readline().strip()
            Gridy[i] = list(map(int, line.split()))
        
        # Read data for the list of arrays
        while True:
            line = file.readline().strip()
            if not line:
                break
            wiring.append(np.array(list(map(int, line.split()))))
    
    return Gridx, Gridy, wiring, M, N
filename = sys.argv[1]  
Gridx, Gridy, path, M, N = read_out_file(filename)
print(M,N)
# Making heatmap
Grid = np.zeros((M, N))
for i in range(M):
    for j in range(N):
        r = Gridx[i,j]
        l = Gridx[i,j+1]
        d = Gridy[i,j]
        u = Gridy[i+1,j]
        Grid[i,j] = (r+l+u+d)*0.25

def plot_path_on_grid(Grid, paths):
    # Plot the colormap of Grid
    plt.imshow(Grid, cmap='viridis', origin='lower')

    # Plot the path on top of the colormap
    for i in range(len(paths)):
        path=paths[i]
        plt.plot(path[::2], path[1::2], 'r-o', linewidth=2)

    # Add a colorbar
    plt.colorbar()

    # Show the plot
    plt.show()

plot_path_on_grid(Grid, path[0:2])
