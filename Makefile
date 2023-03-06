# Executable

bin/test: obj/test.o obj/bloc.o obj/plateau.o obj/utils.o 
	g++ -Wall -o $@ $^

# Objets

obj/main.o: src/main.cpp
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