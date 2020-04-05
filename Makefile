aprile: aprile.o 
	g++ aprile.o -o aprile
	
aprile.o: main.cpp BST.h
	g++ -c main.cpp -o aprile.o

.PHONY: clean

clean:
	rm -rvf aprile.o aprile
