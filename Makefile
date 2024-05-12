all: clean compile generate run_serial run_parallel

clean:
	rm -f cout_serial.txt cout.txt cerr.txt cerr_serial.txt

compile:
	gcc -fopenmp rand_test_generator.c -o rand_test_generator.o -lm
	g++ -g -fopenmp main.cpp netlist.cpp batch_copy.cpp Grid_Graph.cpp -o Working_code.o

generate:
	./rand_test_generator.o

run_serial:
	./Working_code.o sample_test.txt sampul.txt 15 1 1 1> cout_serial.txt 2> cerr_serial.txt

run_parallel:
	./Working_code.o sample_test.txt sampul.txt 15 1 6 1> cout.txt 2> cerr.txt

run:
	
	./Working_code.o sample_test.txt sampul.txt 15 1 1 1> cout_serial.txt 2> cerr_serial.txt
	./Working_code.o sample_test.txt sampul.txt 15 1 6 1> cout.txt 2> cerr.txt

default:
	rm -f cout_serial.txt cout.txt cerr.txt cerr_serial.txt
