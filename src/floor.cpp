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

#include "floor.h"
#include "point3.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

GLuint cubeText[6];

GLuint LoadTexture(const char *filename){
  GLuint textbind;

  SDL_Surface *s = IMG_Load(filename);
  if(!s) exit(1);

  glGenTextures(1, &textbind);
  glBindTexture(GL_TEXTURE_2D, textbind);

  gluBuild2DMipmaps(
    GL_TEXTURE_2D,
    GL_RGB,
    s->w, s->h,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    s->pixels
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    GL_LINEAR );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  return textbind;
}

void Floor::Init(){
  collide=false;
  SDL_Surface *s = IMG_Load("resource/texture/fussball.jpg");

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  gluBuild2DMipmaps(
    GL_TEXTURE_2D,
    GL_RGB,
    s->w, s->h,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    s->pixels
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
     GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
     GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  cubeText[0] = LoadTexture("resource/texture/jajlands1_rt.jpg");
  cubeText[1] = LoadTexture("resource/texture/jajlands1_lf.jpg");
  cubeText[2] = LoadTexture("resource/texture/jajlands1_up.jpg");
  cubeText[3] = LoadTexture("resource/texture/jajlands1_dn.jpg");
  cubeText[4] = LoadTexture("resource/texture/jajlands1_bk.jpg");
  cubeText[5] = LoadTexture("resource/texture/jajlands1_ft.jpg");
}

void Floor::RenderSky() const{
  int S = 80;
  glEnable(GL_TEXTURE_2D);
  glColor3f(1,1,1);
  glDepthMask(GL_FALSE);
  glDisable(GL_LIGHTING);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  // right
  glBindTexture(GL_TEXTURE_2D,cubeText[0]);
  glBegin(GL_QUADS);
    glNormal3f(  0,0,+1  );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( +S,+S,+S );
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -S,+S,+S );
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -S,-S,+S );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( +S,-S,+S );
  glEnd();

  // left
  glBindTexture(GL_TEXTURE_2D,cubeText[1]);
  glBegin(GL_QUADS);
    glNormal3f(  0,0,-1  );
    glTexCoord2f(1.0f, 1.0f); glVertex3f( +S,-S,-S );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -S,-S,-S );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -S,+S,-S );
    glTexCoord2f(1.0f, 0.0f); glVertex3f( +S,+S,-S );
  glEnd();

  // up
  glBindTexture(GL_TEXTURE_2D,cubeText[2]);
  glBegin(GL_QUADS);
    glNormal3f(  0,+1,0  );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( +S,+S,+S );
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -S,+S,+S );
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -S,+S,-S );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( +S,+S,-S );
  glEnd();

  // back
  glBindTexture(GL_TEXTURE_2D,cubeText[4]);
  glBegin(GL_QUADS);
    glNormal3f( +1,0,0  );
    glTexCoord2f(1.0f, 0.0f); glVertex3f( +S,+S,+S );
    glTexCoord2f(1.0f, 1.0f); glVertex3f( +S,-S,+S );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( +S,-S,-S );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( +S,+S,-S );
  glEnd();

  // front
  glBindTexture(GL_TEXTURE_2D,cubeText[5]);
  glBegin(GL_QUADS);
    glNormal3f( -1,0,0 );
    glTexCoord2f(1.0f, 0.0f); glVertex3f( -S,+S,-S );
    glTexCoord2f(1.0f, 1.0f); glVertex3f( -S,-S,-S );
    glTexCoord2f(0.0f, 1.0f); glVertex3f( -S,-S,+S );
    glTexCoord2f(0.0f, 0.0f); glVertex3f( -S,+S,+S );
  glEnd();

  glEnable(GL_LIGHTING);
  glDepthMask(GL_TRUE);
  glDisable(GL_TEXTURE_2D);
}

void Floor::RenderField() const{
  const float H=0; // altezza
  const float S=20; // size

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glBegin(GL_QUADS);
    glNormal3f(0,1,0);

    glTexCoord2f(0.0f, 0.0f); glVertex3d(-S*1.5, H, -S);
    glTexCoord2f(1.0f, 0.0f); glVertex3d(+S*1.5, H, -S);
    glTexCoord2f(1.0f, 1.0f); glVertex3d(+S*1.5, H, +S);
    glTexCoord2f(0.0f, 1.0f); glVertex3d(-S*1.5, H, +S);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

void Floor::RenderGround() const{
  float grey[4] = {0.6, 0.6, 0.6, 1.0};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
  glBegin(GL_QUADS);
    glNormal3f(0,1,0);

    glVertex3d(-80, -0.001, -80);
    glVertex3d(+80, -0.001, -80);
    glVertex3d(+80, -0.001, +80);
    glVertex3d(-80, -0.001, +80);
  glEnd();
}


static Vector3 ComputeNormal(Point3 p0, Point3 p1, Point3 p2) {
  return -( (p1 - p0) % (p2 - p0) ).Normalize();
}

void Floor::RenderBorder() const{
    Point3 v[10];

    v[0] = Point3(33, 0, -23);
    v[1] = Point3(34, 2, -24);
    v[2] = Point3(33, 0, 23);
    v[3] = Point3(34, 2, 24);
    v[4] = Point3(-33, 0, 23);
    v[5] = Point3(-34, 2, 24);
    v[6] = Point3(-33, 0, -23);
    v[7] = Point3(-34, 2, -24);
    v[8] = Point3(33, 0, -23);
    v[9] = Point3(34, 2, -24);
    Vector3 n;

    float yellow[4] = {0.6, 0.6, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
    glShadeModel(GL_FLAT);
    glBegin(GL_QUAD_STRIP);
      for(int i=0; i<8; i+=2) {
        n = ComputeNormal(v[i],v[i+1],v[i+2]);
        n.SendAsNormal();

        if(i==0) {
          v[0].SendAsVertex();
          v[1].SendAsVertex();
        }

        v[i+2].SendAsVertex();
        v[i+3].SendAsVertex();
      }
    glEnd();
    glShadeModel(GL_SMOOTH);
}


// disegna a schermo
void Floor::Render() const{
  RenderSky();
  RenderField();
  RenderGround();
  RenderBorder();
}
