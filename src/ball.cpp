#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "ball.h"
#include "mesh.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

extern GLuint LoadTexture(const char *filename, GLint param);

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

/**
 * Cambia tipologia di palla (palla da calcio, sfera con immagine personale)
 */
void Ball::ChangeBallType() {
  ball_type = (ball_type + 1) % BALL_MAX_NUM;
}

/**
 * Inizializzazione della palla
 */
void Ball::Init(int x,int y, int z){
  ball = new Mesh((char *)"resource/models/", (char *)"ball1.obj");
  face_tex = LoadTexture("resource/texture/me.jpg", GL_REPEAT);

  ball_type=BALL_MESH;
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

/**
 * Render della palla con texture con immagine personale
 */
void Ball::RenderFaceBall() const{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, face_tex);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  float xPlane[4] = {0.0f, 0.0f, 0.5f,-0.5f};
  float zPlane[4] = {0.0f, -0.5f, 0.0f, -0.5f};
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv(GL_S, GL_OBJECT_PLANE, xPlane);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, zPlane);

  glutSolidSphere(1,20,20);

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_2D);
}


/**
 * Render della palla
 */
void Ball::Render() const{
  // sono nello spazio mondo
  glPushMatrix();

  glTranslatef(px,py,pz);
  glRotatef(facing, 0,1,0);

  switch (ball_type) {
    case BALL_MESH:
      ball->NoTexRender();
      break;
    case BALL_FACE:
      RenderFaceBall();
      break;
    default:
      break;
  }

  glPopMatrix();
}

/**
 * Imprime una spinta alla palla dopo un urto
 */
void Ball::Hit(float fx, float fy, float fz) {
  vx+=fx;
  vy+=fy;
  vz+=fz;
}
