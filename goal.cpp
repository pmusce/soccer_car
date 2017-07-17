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

void Goal::Init(float x, float y, float z, float* col){
  px = x; py = y; pz = z;
  color[0] = col[0]; color[1] = col[1]; color[2] = col[2];
  collide=false;
  goal = new Mesh((char *)"./", (char *)"goal.obj");
  goal->SetDiffuse(color);
}

void Goal::DoStep() {

}



// disegna a schermo
void Goal::Render() const{
  glPushMatrix();
  glRotatef(90,0,1,0);
  glTranslatef(px,py,pz);
  //glutSolidCube(9);
  glScalef(1.55,1.55,1.55);

  goal->NoTexRender();
  glPopMatrix();
}

