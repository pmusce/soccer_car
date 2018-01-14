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

#define RED_TEAM 0
#define BLUE_TEAM 1

int nstep=0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP=10; // numero di millisec che un passo di fisica simula

float light_v[4] = {0.1,1,2,0}; // posizione della luce principale

Car* car; // automobile
Floor* ground; // campo
Ball* football; // palla
Goal* goal1; // porta rossa
Goal* goal2; // porta blu

int score[2] = {0, 0}; // punteggi
bool scoring=false; // true se è stato appena segnato un goal
Uint32 scoreTime; // timer fra il goal e la reinizializzazione della scena
int scoringTeam = 0; // squadra che ha segnato goal

/**
 * Funzione di reset della scena. Mette la palla al centro e l'auto in direzione
 * della porta della squadra che ha appena segnato.
 */
void reset_scene() {
  football->setPos(0.0,0.0,0.0);
  football->setVel(0.0,0.0,0.0);

  car->setVel(0.0,0.0,0.0);

  if(scoringTeam == BLUE_TEAM) {
    car->setPos(-7.0,0.0,0.0);
    car->facing = -90;
  } else {
    car->setPos(7.0,0.0,0.0);
    car->facing = 90;
  }
}

/**
 * Rileva collisione fra due oggetti, approssimati come sfere.
 * @param  obj1
 * @param  obj2
 * @return True se la collisione è avvenuta, False altrimenti
 */
int detect_collision(Object3D *obj1, Object3D *obj2) {
  float distance;
  float deltaX, deltaY, deltaZ;

  deltaX = obj1->px - obj2->px;
  deltaY = obj1->py - obj2->py;
  deltaZ = obj1->pz - obj2->pz;
  distance = (float) sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

  return distance <= obj1->collisionRadius+obj2->collisionRadius;
}

/**
 * Controlla se la palla è dentro la porta blu
 * @return True / False
 */
bool point_for_red() {
  if(football->px > 26.75 && football->px < 27.25)
    if(football->pz < 4.5 && football->pz > -4.5) {
      return true;
    }
  return false;
}

/**
 * Controlla se la palla è dentro la porta rossa
 * @return True / False
 */
bool point_for_blue() {
  if(football->px < -26.75 && football->px > -27.25)
    if(football->pz < 4.5 && football->pz > -4.5) {
      return true;
    }
  return false;
}

/**
 * Controlla se è stato segnato un goal, aggiorna i punteggi e resetta la scena
 */
void check_scores() {
  if(point_for_red()) {
    scoringTeam = RED_TEAM;
    if(!scoring) {
      score[0]++;
      scoreTime = SDL_GetTicks();
    }
    scoring = true;
  } else if(point_for_blue()) {
    scoringTeam = BLUE_TEAM;
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

/**
 * Inizializzazione degli oggetti di scena
 */
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

/**
 * Funzione di inizializzazione
 */
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

/**
 * Inizializza le luci
 */
void light (void) {
  GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
  GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
  GLfloat whiteDiffuseLight[] = {1.0, 1.0, 0.863}; //set the diffuse light to white

  glLightfv(GL_LIGHT0, GL_POSITION, light_v );
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
}

/**
 * Funzione chiamata da GLUT per il rendering della scena.
 * Pulisce la scena, inizializza camera e luci, renderizza gli oggetti, disegna
 * l'HUD e scambia il back-buffer
 */
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

/**
 * Funzione richiamata da GLUT quando viene ridimensionata la finestra
 */
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

/**
 * Funzione richiamata da GLUT ogni 50 ms, per forzare la chiamata alla funzione
 * di display
 */
void My_timer_routine( int t )
{
  // Update display
  glutPostRedisplay();
  // Reset timer
  glutTimerFunc( 50, My_timer_routine, 0);
}

/**
 * Funzione richiamata da GLUT quando in stato di idle.
 * Fa muovere gli oggetti di scena in base alle loro velocità, controlla le
 * eventuali collisioni e reagisce di conseguenza, controlla se è stato segnato
 * un goal
 */
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
  }

  if (doneSomething) display();
}

/**
 * Main
 * Inizializza le librerie e definisce le funzioni richiamate dalle librerie GLUT
 * nel ciclo principale
 */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Soccer Car");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idleFunc);
  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboardSpecial);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyUp);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutTimerFunc( 50, My_timer_routine, 0);
  glutMainLoop();
  return 0;
}
