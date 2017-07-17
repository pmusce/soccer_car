# Project: progettoCar3

CPP  = g++
CC   = gcc

SRCDIR = src
BUILDDIR = build
TARGET  = play

OBJ  = main.o camera.o mesh.o car.o floor.o ball.o goal.o
LIBS = -L/usr/X11R6 -lGL -lGLU -lSDL2_image -lSDL2_ttf -lSDL2 -lm -lglut -ggdb
INCS = -I. -I/usr/X11R6/include
CXXINCS=#
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f

all: $(TARGET)

clean:
	${RM} $(OBJ) $(BIN)

$(TARGET): $(OBJ)
	$(CPP) $^ -o $@ $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)
#
# main.o: main.cpp
# 	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)
#
# camera.o: camera.cpp
# 	$(CPP) -c camera.cpp -o camera.o $(CXXFLAGS)
#
# mesh.o: mesh.cpp
# 	$(CPP) -c mesh.cpp -o mesh.o $(CXXFLAGS)
#
# car.o: car.cpp
# 	$(CPP) -c car.cpp -o car.o $(CXXFLAGS)
#
# floor.o: floor.cpp
# 	$(CPP) -c floor.cpp -o floor.o $(CXXFLAGS)
#
# ball.o: ball.cpp
# 	$(CPP) -c ball.cpp -o ball.o $(CXXFLAGS)
#
# goal.o: goal.cpp
# 	$(CPP) -c goal.cpp -o goal.o $(CXXFLAGS)
