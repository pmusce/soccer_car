#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "camera.h"
#include "mesh.h"
#include "car.h"
#include "floor.h"
#include "ball.h"
#include "goal.h"

int xOrigin,yOrigin;

static GLuint texName;

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

bool is_showing_keymap = false;

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
  GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white
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

void printText(const char* t, int x, int y) {
  glRasterPos2f(x, y);
  for(int i=0; i<strlen(t); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, t[i]);
}

void displayKeymap(int w, int h) {
  int margin = 100;

  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
    glVertex2d(w-margin, 100+margin);
    glVertex2d(margin, 100+margin);
    glVertex2d(margin, h-margin);
    glVertex2d(w-margin, h-margin);
  glEnd(); 

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  printText("W - accellerate", 150, h-150-0);
  printText("L - turn left", 150, h-150-1*20);
  printText("D - turn right", 150, h-150-2*20);
  printText("S - break", 150, h-150-3*20);
  printText("SPACE - jump", 150, h-150-4*20);
  printText("F1 - change camera", 150, h-150-5*20);
  
  
}

void displayMap() {
  int margin = 20;
  int scaling = 3;
  int w = 55;
  int h = 35;

  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
    glVertex2d(margin+w*scaling, margin);
    glVertex2d(margin, margin);
    glVertex2d(margin, margin+h*scaling);
    glVertex2d(margin+w*scaling, margin+h*scaling);
  glEnd(); 

  glPointSize(5.0f);
  glBegin(GL_POINTS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2d(margin+(w/2-football->px)*scaling, margin+(h/2-football->pz)*scaling);
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    glVertex2d(margin+(w/2-car->px)*scaling, margin+(h/2-car->pz)*scaling);
  glEnd();

}

void displayScore() {
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  glRasterPos2f(50, 550);

  char t[30];
  sprintf(t, "Red %d - %d Blue", score[0], score[1]);

  for(int i=0; i<strlen(t); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, t[i]);
}

void drawHUD() {
  GLint m_viewport[4];

  glGetIntegerv( GL_VIEWPORT, m_viewport );
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D( 0, m_viewport[2], 0, m_viewport[3]);

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  
  displayScore();
  displayMap();
  if(is_showing_keymap) displayKeymap(m_viewport[2], m_viewport[3]);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
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

void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(0);
      break;
    case 9:
      is_showing_keymap = true;
      break;
    default:
      static int keymap[Controller::NKEYS] = {'a', 'd', 'w', 's', ' '};
      car->controller.EatKey(key, keymap, true);
      break;
  }
}

void keyUp (unsigned char key, int x, int y)
{
  if(key == 9) {
    is_showing_keymap = false;
    return;
  }
  static int keymap[Controller::NKEYS] = {'a', 'd', 'w', 's', ' '};
  car->controller.EatKey(key, keymap, false);
}

void keyboardSpecial (int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_F1:
      nextCamera();
      break;
    default:
      break;
  }
}

void mouseButton(int button, int state, int x, int y) {
  // Wheel reports as button 3(scroll up) and button 4(scroll down)
  if(button == 3) {
    eyeDist=eyeDist*0.9;
    if (eyeDist<1) eyeDist = 1;
  }
  if(button == 4) {
    eyeDist=eyeDist/0.9;
  }

  if (button == GLUT_LEFT_BUTTON) {

    // when the button is released
    if (state == GLUT_UP) {
      xOrigin = -1;
      yOrigin = -1;
    }
    else  {// state = GLUT_DOWN
      xOrigin = x;
      yOrigin = y;
    }
  }
}

void mouseMove(int x, int y) {
  viewAlpha+= x - xOrigin;
  viewBeta += y - yOrigin;

  xOrigin = x;
  yOrigin = y;

  if (viewBeta<+5) viewBeta=+5; //per non andare sotto la macchina
  if (viewBeta>+90) viewBeta=+90;
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
  int guardia=0; // sicurezza da loop infinito

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