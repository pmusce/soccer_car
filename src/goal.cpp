// car.cpp
// implementazione dei metodi definiti in car.h

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <string>
#include <iostream>

#include "goal.h"
#include "mesh.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

/**
 * Inizializzazione di una nuova porta
 * @param x   Posizione asse X
 * @param y   Posizione asse Y
 * @param z   Posizione asse Z
 * @param col Colore RGB della porta
 */
void Goal::Init(float x, float y, float z, float* col){
  px = x; py = y; pz = z;
  color[0] = col[0]; color[1] = col[1]; color[2] = col[2];

  goal = new Mesh((char *)"resource/models/", (char *)"goal.obj");
  goal->SetDiffuse(color);
}

/**
 * Renderizza a schermo
 */
void Goal::Render() const{
  glPushMatrix();
  glRotatef(90,0,1,0);
  glTranslatef(px,py,pz);
  glScalef(1.55,1.55,1.55);

  goal->NoTexRender();
  glPopMatrix();
}
