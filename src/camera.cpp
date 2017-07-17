#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
#include "car.h"
#include "ball.h"

float angle=0.0;
float viewAlpha=20, viewBeta=40; // angoli che definiscono la vista
float eyeDist=7.0;
extern Car* car;
extern Ball* football;
static int currentCamera = CAMERA_BALL;

int nextCamera() {
  currentCamera = (currentCamera+1) % CAMERA_MAX_NUM;
  return currentCamera;
}


void setMouseCamera() {
  glTranslatef(0,0,-eyeDist);
  glRotatef(viewBeta,  1,0,0);
  glRotatef(viewAlpha, 0,1,0);
}

void setCarCamera() {
  double px = car->px;
  double py = car->py;
  double pz = car->pz;
  double angle = car->facing;
  double cosf = cos(angle*M_PI/180.0);
  double sinf = sin(angle*M_PI/180.0);
  double camd, camh, ex, ey, ez, cx, cy, cz;
  double cosff, sinff;

  camd = 2.5;
  camh = 0.8;
  ex = px + camd*sinf;
  ey = py + camh;
  ez = pz + camd*cosf;
  cx = px - camd*sinf;
  cy = py + camh;
  cz = pz - camd*cosf;
  gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
}

void setBallCamera() {
  double px = car->px;
  double py = car->py;
  double pz = car->pz;
  double cx = football->px;
  double cy = football->py;
  double cz = football->pz;

  double deltaX = px - cx;
  double deltaZ = pz - cz;
  double dist = sqrt(deltaX*deltaX+deltaZ*deltaZ);


  double cosf = deltaZ / dist;
  double sinf = deltaX / dist;
  double camd, camh, ex, ey, ez;

  
  camd = 2.5;
  camh = 0.8;
  ex = px + camd*sinf;
  ey = py + camh;
  ez = pz + camd*cosf;
  gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
}

void setCamera() {
  switch(currentCamera) {
    case CAMERA_MOUSE:
      setMouseCamera();
      break;
    case CAMERA_CAR:
      setCarCamera();
      break;
    case CAMERA_BALL:
      setBallCamera();
      break;
    default:
      break;
  }
}