# Makefile used to compile the project shortest-path-in-cpp.
# Authors: Carlos Domínguez García, Juan Pablo Claros Romero, Cristian Abrante Dorta.

# Varibles used for the compilation.
CXX = g++
BINARY = shortest-path-in-cpp

IDIR = include
ODIR = obj

_DEPS = ClassGraphicGrid.hpp Button.hpp ProblemSpecification.hpp GridCamera.hpp
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o ClassGraphicGrid.o Button.o ProblemSpecification.o GridCamera.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

CXXFLAGS = -g -std=c++14 -I$(IDIR)
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# If any option is selected the program will build.
all: $(BINARY)

# Creation of objects under obj/ folder.
$(ODIR)/%.o: src/%.cpp $(DEPS)

		@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

		$(CXX) -c -o $@ $< $(CXXFLAGS)

# Compilation of the binary program.
$(BINARY): $(OBJ)
		$(CXX) -o $@ $^ $(CXXFLAGS) $(SFMLFLAGS)

clean:
	rm -f $(ODIR)/*.o $(BINARY)
	rm -r $(ODIR)
