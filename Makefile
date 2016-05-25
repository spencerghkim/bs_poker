all: bspoker.cpp
	g++ -std=c++14 -O3 -funroll-loops bspoker.cpp -o bspoker
clean:
	rm -rf bspoker