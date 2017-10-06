build: main.o ClassGraphicGrid.o Button.o
	g++ -g -std=c++14 main.o ClassGraphicGrid.o Button.o -lsfml-graphics -lsfml-window -lsfml-system -o sfml

main.o: main.cpp ClassGraphicGrid.hpp Button.h
	g++ -c -g -std=c++14 -O3 main.cpp
	
ClassGraphicGrid.o: ClassGraphicGrid.hpp ClassGraphicGrid.cpp
	g++ -c -g -std=c++14 -O3 ClassGraphicGrid.cpp

Button.o: Button.h Button.cc
	g++ -c -g -std=c++14 -O3 Button.cc
	
clean:
	rm main.o ClassGraphicGrid.o sfml
