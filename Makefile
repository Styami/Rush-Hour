#all: bin/exec

#Debug
bin/exec_debug: obj/main.o
	g++ -g -Wall -o bin/exec_debug obj/main.o

# Executable
bin/exec: obj/main.o
	g++ -Wall -o bin/exec obj/main.o 

bin/test: obj/test.o obj/bloc.o obj/utils.o
	g++ -Wall -o $@ $^

# Objets

obj/main.o: src/main.cpp src/Graph.tpp src/Graph.hpp src/Sommets.tpp src/Sommets.hpp
	g++ -c -Wall -o $@ $<

obj/test.o: src/test.cpp
	g++ -c -Wall -o $@ $<

obj/%.o: src/%.cpp src/%.hpp
	g++ -c -Wall -o $@ $<


# Utiliataire

clean:
	rm -rf bin/*

cleanall:
	rm -rf bin/*
	rm -rf obj/*