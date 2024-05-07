# compile random generator f

g++ -fopenmp main.cpp netlist.cpp batch.cpp Grid_Graph.cpp 
for i in $(seq 1 10); do 
   ./a.out $i 
done
