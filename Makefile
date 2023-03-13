all: bin/exec

bin/exec: obj/main.o
	g++  -g -Wall -o bin/exec obj/main.o 

obj/main.o: src/main.cpp
	g++ -g -Wall -c -o obj/main.o src/main.cpp 

clean:
	rm -rf bin/*

cleanall:
	rm -rf bin/*
	rm -rf obj/*