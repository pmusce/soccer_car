# Project: progettoCar3

CPP  = g++
CC   = gcc
BIN  = play

OBJ  = main.o camera.o mesh.o car.o floor.o ball.o goal.o
LINKOBJ  = main.o camera.o mesh.o car.o floor.o ball.o goal.o
LIBS = -L/usr/X11R6 -lGL -lGLU -lSDL2_image -lSDL2_ttf -lSDL2 -lm -lglut -ggdb
INCS = -I. -I/usr/X11R6/include
CXXINCS=#
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)  
RM = rm -f

all: $(BIN)


clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

camera.o: camera.cpp
	$(CPP) -c camera.cpp -o camera.o $(CXXFLAGS)

mesh.o: mesh.cpp
	$(CPP) -c mesh.cpp -o mesh.o $(CXXFLAGS)

car.o: car.cpp
	$(CPP) -c car.cpp -o car.o $(CXXFLAGS)

floor.o: floor.cpp
	$(CPP) -c floor.cpp -o floor.o $(CXXFLAGS)

ball.o: ball.cpp
	$(CPP) -c ball.cpp -o ball.o $(CXXFLAGS)

goal.o: goal.cpp
	$(CPP) -c goal.cpp -o goal.o $(CXXFLAGS)