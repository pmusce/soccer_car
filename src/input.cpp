#include <GL/glut.h>
#include "car.h"
#include "ball.h"
#include "camera.h"
#include "hud.h"

extern Car* car;
extern Ball* football;

static int xOrigin,yOrigin;

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
    case GLUT_KEY_F2:
      football->ChangeBallType();
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
