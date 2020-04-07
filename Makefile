main: main.o 
	g++ -std=c++0x main.o -o main
	
main.o: main.cpp BST.h
	g++ -std=c++0x -c main.cpp -o main.o

.PHONY: clean

clean:
	rm -rvf main.o main
	@echo "Cleaned previous outputs!"
