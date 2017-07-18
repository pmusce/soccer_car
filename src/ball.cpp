#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ball.h"
#include "mesh.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

// DoStep: facciamo un passo di fisica (a delta-t costante)
//
// Indipendente dal rendering.
//
// ricordiamoci che possiamo LEGGERE ma mai SCRIVERE
// la struttura controller da DoStep
void Ball::DoStep(){

  vy-=0.003;
  // attriti (semplificando)
  vx*=attritoX;
  vy*=attritoY;
  vz*=attritoZ;

  // posizione = posizione + velocita * delta t (ma e' delta t costante)
  px+=vx;
  py+=vy;
  if(py<collisionRadius) {
    py=1;
    if (vy<0) vy*=-0.8;
    if (vy<0.01) vy=0;
  }
  pz+=vz;

  // bounce against border
  if(pz>17 && vz>0) vz*=-1;
  if(pz<-17 && vz<0) vz*=-1;
  if(pz < -4.5 || pz > 4.5) { // goal
    if(px>27 && vx>0) vx*=-1;
    if(px<-27 && vx<0) vx*=-1;
  }
}


void Ball::Init(int x,int y, int z){
  ball = new Mesh((char *)"../1/mesh/", (char *)"ball1.obj");
  collide=true;
  px=x;
  py=y;
  pz=z;

  collisionRadius = 1;
  // inizializzo lo stato della scatola
  facing=0; // posizione e orientamento
  vx=vy=vz=0;      // velocita' attuale

  attritoZ = 0.991;  // piccolo attrito sulla Z (nel senso di rotolamento delle ruote)
  attritoX = 0.991;  // grande attrito sulla X (per non fare slittare la macchina)
  attritoY = 0.991;  // attrito sulla y nullo
}


// disegna a schermo
void Ball::Render() const{
  // sono nello spazio mondo
  glPushMatrix();

  glTranslatef(px,py,pz);
  glRotatef(facing, 0,1,0);

  ball->NoTexRender();

  glColor3f(.4,.4,.4);


  glPopMatrix();
}

void Ball::Hit(float fx, float fy, float fz) {
  vx+=fx;
  vy+=fy;
  vz+=fz;
}
