#all: bin/exec

#Debug
bin/exec_debug: obj/main.o
	g++ -g -Wall -o bin/exec_debug obj/main.o

# Executable
bin/exec: obj/main.o obj/plateau.o obj/Graph.o obj/bloc.o obj/utils.o obj/Sommets.o
	g++ -Wall -std=c++20 -o $@ $^ 

bin/test: obj/test.o obj/bloc.o obj/plateau.o obj/utils.o 
	g++ -Wall -std=c++20 -o $@ $^

# Objets
obj/main.o: src/main.cpp
	g++ -c -Wall -std=c++20 -o $@ $<

obj/test.o: src/test.cpp
	g++ -c -Wall -std=c++20 -o $@ $<

obj/%.o: src/%.cpp src/%.hpp
	g++ -c -Wall -std=c++20 -o $@ $<


# Utiliataire

clean:
	rm -rf bin/*

cleanall:
	rm -rf bin/*
	rm -rf obj/*