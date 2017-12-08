# Project: soccer_car

CPP  = g++
CC   = gcc

SRCDIR = src
ODIR = build

TARGET  = play

_OBJ =  main.o camera.o hud.o input.o mesh.o car.o floor.o ball.o goal.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -L/usr/X11R6 -lGL -lGLU -lSDL2_image -lSDL2_ttf -lSDL2 -lm -lglut -ggdb
INCS = -I. -I/usr/X11R6/include
CXXINCS=#
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -rf

all: $(ODIR) $(TARGET)

clean:
	${RM} $(ODIR) $(TARGET)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJ)
	$(CPP) $^ -o $@ $(LIBS)
