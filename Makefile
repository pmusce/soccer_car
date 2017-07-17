# Project: progettoCar3

CPP  = g++
CC   = gcc

SRCDIR = src
ODIR = build

TARGET  = play

_OBJ =  main.o camera.o mesh.o car.o floor.o ball.o goal.o hud.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L/usr/X11R6 -lGL -lGLU -lSDL2_image -lSDL2_ttf -lSDL2 -lm -lglut -ggdb
INCS = -I. -I/usr/X11R6/include
CXXINCS=#
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f

all: $(TARGET)

clean:
	${RM} $(ODIR)/*.o $(TARGET)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJ)
	$(CPP) $^ -o $@ $(LIBS)
