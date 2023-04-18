all: release test

# Executable
release: obj/main.o obj/plateau.o obj/graph.o obj/bloc.o obj/utils.o obj/sommet.o obj/console_app.o obj/RPLconsole.o obj/RPLcolor.o
	g++ -Wall -std=c++20 -o bin/release $^ 

test: obj/test.o obj/plateau.o obj/graph.o obj/bloc.o obj/utils.o obj/sommet.o obj/console_app.o obj/RPLconsole.o obj/RPLcolor.o
	g++ -g -Wall -std=c++20 -o bin/test $^

# Objets
obj/main.o: src/main.cpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/test.o: src/test.cpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/%.o: src/%.cpp src/%.hpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

obj/%.o: src/RPL/%.cpp src/RPL/%.hpp
	g++ -g -c -Wall -std=c++20 -o $@ $<

# Utiliataire

clean:
	rm -rf bin/*

cleanall:
	rm -rf bin/*
	rm -rf obj/*