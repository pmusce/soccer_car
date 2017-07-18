#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "camera.h"
#include "hud.h"
#include "input.h"

#include "mesh.h"
#include "car.h"
#include "floor.h"
#include "ball.h"
#include "goal.h"

int nstep=0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP=10; // numero di millisec che un passo di fisica simula

float light_v[4] = {0.1,1,2,0};

Car* car;
Floor* ground;
Ball* football;
Goal* goal1;
Goal* goal2;

int score[2] = {0,0};
bool scoring=false;
Uint32 scoreTime;

void reset_scene() {
  football->setPos(0.0,0.0,0.0);
  football->setVel(0.0,0.0,0.0);

  car->setPos(7.0,0.0,0.0);
  car->setVel(0.0,0.0,0.0);
  car->facing = 90;
}

int detect_collision(Object3D *obj1, Object3D *obj2) {
  float distance;
  float deltaX, deltaY, deltaZ;

  deltaX = obj1->px - obj2->px;
  deltaY = obj1->py - obj2->py;
  deltaZ = obj1->pz - obj2->pz;
  distance = (float) sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

  return distance <= obj1->collisionRadius+obj2->collisionRadius;
}

void light (void) {
  GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
  GLfloat blackAmbientLight[] = {0.2, 0.2, 0.2}; //set the light ambient to black
  GLfloat whiteDiffuseLight[] = {1.0, 1.0, 0.863}; //set the diffuse light to white
  glLightfv(GL_LIGHT0, GL_POSITION, light_v );
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
}

void initObjects(void) {
  float red[] = {1.0, 0.0, 0.0};
  float blue[] = {0.0, 0.0, 1.0};

  car=new Car();
  ground = new Floor();
  football=new Ball(3,1,3);
  goal1 = new Goal(0,0,27,red);
  goal2 = new Goal(0,0,-27,blue);
  reset_scene();
}

void init(void)
{
  glClearColor (0.0,0.0,0.0,0.0);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);

  initObjects();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  setCamera();
  light();

  ground->Render();
  football->Render();
  car->Render();
  goal1->Render();
  goal2->Render();

  drawHUD();

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 180.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -3.6);
}

bool point_for_red() {
  if(football->px > 26.75 && football->px < 27.25)
    if(football->pz < 4.5 && football->pz > -4.5) {
      return true;
    }
  return false;
}

bool point_for_blue() {
  if(football->px < -26.75 && football->px > -27.25)
    if(football->pz < 4.5 && football->pz > -4.5) {
      return true;
    }
  return false;
}

void check_scores() {
  if(point_for_red()) {
    if(!scoring) {
      score[0]++;
      scoreTime = SDL_GetTicks();
    }
    scoring = true;
  } else if(point_for_blue()) {
    if(!scoring) {
      score[1]++;
      scoreTime = SDL_GetTicks();
    }
    scoring = true;
  }

  if(scoring && scoreTime+1000 < SDL_GetTicks()) {
    reset_scene();
    scoring = false;
  }
}

void My_timer_routine( int t )
{
  // Update display
  glutPostRedisplay();
  // Reset timer
  glutTimerFunc( 50, My_timer_routine, 0);
}

void idleFunc() {
  Uint32 timeNow=SDL_GetTicks(); // che ore sono?
  bool doneSomething=false;

  // finche' il tempo simulato e' rimasto indietro rispetto
  // al tempo reale...
  while (nstep*PHYS_SAMPLING_STEP < timeNow ) {
    car->DoStep();
    football->DoStep();

    if(detect_collision(car, football)) {
      double deltaX = football->px - car->px;
      double deltaY = football->py - car->py;
      double deltaZ = football->pz - car->pz;
      double dist = sqrt(deltaX*deltaX+deltaZ*deltaZ);

      double vel_car = sqrt(car->vx*car->vx+car->vz*car->vz);

      double n_vx = deltaX / dist * vel_car * 3;
      double n_vy = deltaY / dist * vel_car * 3;
      double n_vz = deltaZ / dist * vel_car * 3;

      football->Hit(n_vx, n_vy, n_vz);
    }


    check_scores();

    nstep++;
    doneSomething=true;
    timeNow=SDL_GetTicks();
    // if (guardia++>1000) {done=true; break;} // siamo troppo lenti!
  }

  if (doneSomething) {
    display();
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("A texture map");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idleFunc);
  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboardSpecial);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyUp);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutTimerFunc( 100, My_timer_routine, 0);
  glutMainLoop();
  return 0;
}
