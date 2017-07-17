// car.cpp
// implementazione dei metodi definiti in car.h

#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "mesh.h"
#include "car.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

// da invocare quando e' stato premuto/rilasciato il tasto numero "keycode"
void Controller::EatKey(int keycode, int* keymap, bool pressed_or_released)
{
  for (int i=0; i<NKEYS; i++){
    if (keycode==keymap[i]) key[i]=pressed_or_released;
  }
}

void Car::Hit(float x, float y, float z) {
}

// DoStep: facciamo un passo di fisica (a delta-t costante)
//
// Indipendente dal rendering.
//
// ricordiamoci che possiamo LEGGERE ma mai SCRIVERE
// la struttura controller da DoStep
void Car::DoStep(){
  // computiamo l'evolversi della macchina
 
  float vxm, vym, vzm; // velocita' in spazio macchina
 
  // da vel frame mondo a vel frame macchina
  float cosf = cos(facing*M_PI/180.0);
  float sinf = sin(facing*M_PI/180.0);
  vxm = +cosf*vx - sinf*vz;
  vym = vy;
  vzm = +sinf*vx + cosf*vz;
 
  // gestione dello sterzo
  if (controller.key[Controller::LEFT]) sterzo+=velSterzo;
  if (controller.key[Controller::RIGHT]) sterzo-=velSterzo;
  sterzo*=velRitornoSterzo; // ritorno a volante fermo
#ifdef DEBUG
  cout << sterzo << endl;
#endif
 
  if (controller.key[Controller::ACC]) vzm-=accMax; // accelerazione in avanti
  if (controller.key[Controller::DEC]) vzm+=accMax; // accelerazione indietro

  if (controller.key[Controller::JUM] && py < 0.001) vym+=0.05;
  else vym-=0.0011;;
  // attriti (semplificando)
  vxm*=attritoX; 
  vym*=attritoY;
  vzm*=attritoZ;

  // l'orientamento della macchina segue quello dello sterzo
  // (a seconda della velocita' sulla z)
  facing = facing - (vzm*grip)*sterzo;
 
  // rotazione mozzo ruote (a seconda della velocita' sulla z)
  float da ; //delta angolo
  da=(180.0*vzm)/(M_PI*raggioRuotaA);
  mozzoA+=da;
  da=(180.0*vzm)/(M_PI*raggioRuotaP);
  mozzoP+=da;
 
  // ritorno a vel coord mondo
  vx = +cosf*vxm + sinf*vzm;
  vy = vym;
  vz = -sinf*vxm + cosf*vzm;
 
  // posizione = posizione + velocita * delta t (ma e' delta t costante)
  px+=vx;
  py+=vy;
  if(py<0) { 
    py=0; 
    if (vy<0) vy=0;
  }
  pz+=vz;
}


void Controller::Init(){
  for (int i=0; i<NKEYS; i++) key[i]=false;
}

void Car::Init(){
  collisionRadius = 1;
  
  carlinga = new Mesh((char *)"../car/", (char *) "car2.obj");
  glass = new Mesh((char *)"../car/", (char *) "glass.obj");
  engine = new Mesh((char *)"../car/", (char *) "engine.obj");
  frontlight = new Mesh((char *)"../car/", (char *) "frontlight.obj");
  tyreFL = new Mesh((char *)"../car/", (char *) "tyre1.obj");
  tyreFR = new Mesh((char *)"../car/", (char *) "tyre2.obj");
  tyreBL = new Mesh((char *)"../car/", (char *) "tyre3.obj");
  tyreBR = new Mesh((char *)"../car/", (char *) "tyre4.obj");

  collide=true;
  // inizializzo lo stato della macchina
  px=py=pz=facing=0; // posizione e orientamento
  mozzoA=mozzoP=sterzo=0;   // stato
  vx=vy=vz=0;      // velocita' attuale
  // inizializzo la struttura di controllo
  controller.Init();
 
  velSterzo=3.4;         // A
//  velSterzo=2.26;       // A
  velRitornoSterzo=0.93; // B, sterzo massimo = A*B / (1-B)
 
  accMax = 0.0011;
  //accMax = 0.0055;
 
  // attriti: percentuale di velocita' che viene mantenuta
  // 1 = no attrito
  // <<1 = attrito grande
  attritoZ = 0.991;  // piccolo attrito sulla Z (nel senso di rotolamento delle ruote)
  attritoX = 0.8;  // grande attrito sulla X (per non fare slittare la macchina)
  attritoY = 0.991;  // attrito sulla y nullo

  // Nota: vel max = accMax*attritoZ / (1-attritoZ)
 
  raggioRuotaA = 0.25;
  raggioRuotaP = 0.35;
 
  grip = 0.45; // quanto il facing macchina si adegua velocemente allo sterzo
}


// disegna a schermo
void Car::Render() const{
  
  // sono nello spazio mondo

  glPushMatrix();
 
  glTranslatef(px,py,pz);
  glRotatef(180+facing, 0,1,0);

  glScalef(.15,.15,.15);
  carlinga->Render();
  engine->Render();
  glass->Render();
  frontlight->Render();
  
  // Front Left
  glPushMatrix();
  glTranslatef(2.67879,1.11009,4.2345);
  glRotatef(sterzo,0,1,0);
  glRotatef(-mozzoA,1,0,0);
  tyreFL->Render();
  glPopMatrix();
  
  // Front Right
  glPushMatrix();
  glTranslatef(-2.77312,1.11009,4.2345);
  glRotatef(sterzo,0,1,0);
  glRotatef(-mozzoA,1,0,0);
  tyreFR->Render();
  glPopMatrix();

  // Back Left
  glPushMatrix();
  glTranslatef(2.67879,1.11009,-3.31136);
  glRotatef(-mozzoP,1,0,0);
  tyreBL->Render();
  glPopMatrix();

  // Back Right
  glPushMatrix();
  glTranslatef(-2.77312,1.11009,-3.31136);
  glRotatef(-mozzoP,1,0,0);
  tyreBR->Render();
  glPopMatrix();

  glPopMatrix();
}
