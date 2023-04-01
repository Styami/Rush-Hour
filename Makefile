#all: bin/exec

#Debug
bin/exec_debug: obj/main.o
	g++ -g -Wall -o bin/exec_debug obj/main.o

# Executable
bin/exec: obj/main.o
	g++ -Wall -o bin/exec obj/main.o 

bin/test: obj/test.o obj/bloc.o obj/plateau.o obj/utils.o obj/affichage.o obj/RPLconsole.o obj/RPLcolor.o
	g++ -g -Wall -std=c++20 -o $@ $^

# Objets
obj/main.o: src/main.cpp src/Graph.tpp src/Graph.hpp src/Sommets.tpp src/Sommets.hpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/test.o: src/test.cpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/%.o: src/%.cpp src/%.hpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/%.o: src/RPL/%.cpp src/RPL/%.h
	g++ -g -c -Wall -std=c++20 -o $@ $<

# Utiliataire

clean:
	rm -rf bin/*

cleanall:
	rm -rf bin/*
	rm -rf obj/*