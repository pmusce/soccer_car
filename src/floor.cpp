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

GLuint LoadTexture(const char *filename, GLint param){
  GLuint textbind;

  SDL_Surface *s = IMG_Load(filename);
  if(!s) {
    fprintf(stderr, "%s not found\n", filename);
    exit(1);
  }
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
  return textbind;
}


void Floor::initTessField() {
  int K=200;
  int S=20;
  for (int x=0; x<K; x++)
    for (int z=0; z<K; z++) {
      float x0=-S + 2*(x+0)*S/K;
      float x1=-S + 2*(x+1)*S/K;
      float z0=-S + 2*(z+0)*S/K;
      float z1=-S + 2*(z+1)*S/K;
      tess_field_x[x] = x0;
      tess_field_z[z] = z0;
    }
}

void Floor::Init(){
  collide=false;

  field_tex = LoadTexture("resource/texture/fussball.jpg", GL_CLAMP_TO_EDGE);

  cubeText[0] = LoadTexture("resource/texture/jajlands1_rt.jpg", GL_CLAMP_TO_EDGE);
  cubeText[1] = LoadTexture("resource/texture/jajlands1_lf.jpg", GL_CLAMP_TO_EDGE);
  cubeText[2] = LoadTexture("resource/texture/jajlands1_up.jpg", GL_CLAMP_TO_EDGE);
  cubeText[3] = LoadTexture("resource/texture/jajlands1_dn.jpg", GL_CLAMP_TO_EDGE);
  cubeText[4] = LoadTexture("resource/texture/jajlands1_bk.jpg", GL_CLAMP_TO_EDGE);
  cubeText[5] = LoadTexture("resource/texture/jajlands1_ft.jpg", GL_CLAMP_TO_EDGE);

  initTessField();
}

void Floor::RenderSky() const{
  int S = 80;
  glEnable(GL_TEXTURE_2D);
  glColor3f(1,1,1);
  glDepthMask(GL_FALSE);
  // glDisable(GL_LIGHTING);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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

  // glEnable(GL_LIGHTING);
  glDepthMask(GL_TRUE);
  glDisable(GL_TEXTURE_2D);
}


void Floor::RenderField() const{
  const float H=0; // altezza
  const float S=20; // size
  float grey[4] = {0.6, 0.6, 0.6, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, field_tex);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  int K = 100;
  glBegin(GL_QUADS);
    for (int x=0; x<K; x++) {
      for (int z=0; z<K; z++) {
        float x0=(-S + 2*(x+0)*S/K)*1.5;
        float x1=(-S + 2*(x+1)*S/K)*1.5;
        float z0=-S + 2*(z+0)*S/K;
        float z1=-S + 2*(z+1)*S/K;

        glNormal3f(0,1,0);
        glTexCoord2f((float)x/K, (float)z/K);
        glVertex3d(x0, H, z0);

        glNormal3f(0,1,0);
        glTexCoord2f((float)(x+1)/K, (float)(z)/K);
        glVertex3d(x1, H, z0);

        glNormal3f(0,1,0);
        glTexCoord2f((float)(x+1)/K, (float)(z+1)/K);
        glVertex3d(x1, H, z1);

        glNormal3f(0,1,0);
        glTexCoord2f((float)x/K, (float)(z+1)/K);
        glVertex3d(x0, H, z1);
      }
    }
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

  float yellow[4] = {0.95, 0.9, 0.51, 1.0};
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
