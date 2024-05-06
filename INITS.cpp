// Initializing shortest paths inside the bounding box, use 2 Sdists and Sdirs
        /*
        Sdir1- If a cell is the source, then Sdir = 'x', if it is anywhere else on the source column, then it is 'u', if it is to the right of the source
        column, then it is 'l', else it is 'r'
        Sdir2- If it is at the source, then it is 'd', if it is not on the source column, then it is 'd', if it is on the source column and above the source, 
        then it is 's', else it is 'n'
        Sdist1- If it is the source, then it is 0, if it is on the source column, then it is equall to v plus the cost to reach the cell above it, else it is
        the sum of costs to reach the cell from the projection of the cell onto the source column, plus the cost to reach the projection on the source column,
        Sdist2- If it is on the source column, then it is v + sum of costs to reach the cell from the cell above the source, else it is Sdist1 at the same (i,j)+v
        */
       // Initializing Sdir1
       dispG(G);
        for (int k = cornerl.y; k<cornerh.y+1; k++){
            for (int j = cornerl.x; j<cornerh.x+1; j++){
                if (k==source.y && j == source.x){
                    Sdir1[G.N*k+j] = 'x';
                }
                else if(j == source.x){
                    Sdir1[G.N*k+j] = 'u';
                }
                else if(j > source.x){
                    Sdir1[G.N*k+j] = 'l';
                }
                else{
                    Sdir1[G.N*k+j] = 'r';
                }
            }
        }
        std::cout << "Sdir1 is initialized " << std::endl;
        // Initializing Sdir2
        for (int j = cornerl.x; j<cornerh.x+1; j++){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                if (j==source.x){
                    if (k==source.y && j == source.x){
                        Sdir2[G.M*j+k] = 'd';
                    }
                    else if (k > source.y && j == source.x){
                        Sdir2[G.M*j+k] = 's'; // 's' for south
                    }
                    else if (k < source.y && j == source.x){
                        Sdir2[G.M*j+k] = 'n';
                    }
                }
                else{
                    Sdir2[G.M*j+k] = 'd';
                }
            }
        }
        std::cout << "Sdir2 is initialized " << std::endl;
        disp_s(Sdist1,Sdist2,Sdir1,Sdir2,G.M,G.N);
        // Initializing Sdist2 on the source column
        float sum_cost = G.v ;
        int j = source.x;
        for (int k = source.y+1; k<cornerh.y+1; k++){
            sum_cost+=weight(G.Gy[(G.M+1)*j+k+1],G.C,c);
            Sdist2[G.M*j+k] = sum_cost;
        }
        sum_cost = G.v;
        for (int k = source.y-1; k>cornerl.y+1; k--){
            sum_cost+=weight(G.Gy[(G.M+1)*j+k+1],G.C,c);
            Sdist2[G.M*j+k] = sum_cost;
        }
        Sdist2[G.M*j+source.y] = G.v;
        // Initializing Sdist1 
        for (int k = cornerl.y; k<cornerh.y+1; k++){
            int j = source.x;
            Sdist1[G.N*j+k] = G.v + Sdist2[G.N*k+j];
            float sum_cost = Sdist1[G.N*j+k];
            for (j = source.x+1; j<cornerh.x+1; j++){
                sum_cost += weight(G.Gx[(G.N+1)*k+j],G.C,c); 
                Sdist1[G.N*j+k] = sum_cost;
            }
            j = source.x;
            sum_cost = Sdist1[G.N*j+k];
            for (j = source.x-1; j>cornerl.x-1; j--){
                sum_cost += weight(G.Gx[(G.N+1)*k+j+1],G.C,c);
                 Sdist1[G.N*j+k] = sum_cost;
            }
        }
        std::cout << "Sdist1 is initialized " << std::endl;
        Sdist1[G.N*source.y+source.x] = 0;
        // Initializing Sdist2 on the rest of the columns
        for (int j = cornerl.x; j<cornerh.x+1; j++){
            for (int k = cornerl.y; k<cornerh.y+1; k++){
                if (j != source.x){
                    Sdist2[G.M*j+k] = G.v+Sdist1[G.N*j+k];
                }
            }
        }
        std::cout << "Sdist2 is initialized " << std::endl;
    }