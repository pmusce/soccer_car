#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include "hud.h"
#include "base_obj.h"

extern Object3D *football, *car;
extern int score[];
extern bool scoring;
extern int scoringTeam;

bool is_showing_keymap = false;

static void printText(const char* t, int x, int y) {
  glRasterPos2f(x, y);
  for(int i=0; i<strlen(t); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, t[i]);
}

static void displayKeymap(int w, int h) {
  int margin = 100;

  // gray background
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

static void displayMap() {
  int margin = 20;
  int scaling = 3;
  int w = 55;
  int h = 35;

  // field
  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
    glVertex2d(margin+w*scaling, margin);
    glVertex2d(margin, margin);
    glVertex2d(margin, margin+h*scaling);
    glVertex2d(margin+w*scaling, margin+h*scaling);
  glEnd();

  // ball and car as points on map
  glPointSize(5.0f);
  glBegin(GL_POINTS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2d(margin+(w/2-football->px)*scaling, margin+(h/2-football->pz)*scaling);
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    glVertex2d(margin+(w/2-car->px)*scaling, margin+(h/2-car->pz)*scaling);
  glEnd();
}

static void displayScore(int w, int h) {
  char t[30];
  sprintf(t, "Red %d - %d Blue", score[0], score[1]);

  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  printText(t,50,h-50);
}

static void displayGoal(int w, int h) {
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  printText("GOAL!", w/2 - 20, h/2);
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

  displayScore(m_viewport[2], m_viewport[3]);
  displayMap();
  if(is_showing_keymap) displayKeymap(m_viewport[2], m_viewport[3]);
  if(scoring) displayGoal(m_viewport[2], m_viewport[3]);

  glPopMatrix();
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

}
