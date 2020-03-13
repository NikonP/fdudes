# Copyright (c) 2020, Nikon Podgorny <student@cs.karelia.ru>
# This code is licensed under a WTFPL.

fdudes: main.o hasher.o Makefile
	g++ -o fdudes main.o hasher.o -lstdc++fs -std=c++17

main.o: main.cpp hasher.h Makefile
	g++ -c main.cpp -o main.o -std=c++17

hasher.o: hasher.cpp hasher.h Makefile
	g++ -c hasher.cpp -o hasher.o -std=c++17

clean:
	rm *.o fdudes
