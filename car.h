#include "base_obj.h"
#include "mesh.h"

class Controller{
public:
  enum { LEFT=0, RIGHT=1, ACC=2, DEC=3, JUM=4, NKEYS=5};
  bool key[NKEYS];
  void Init();
  void EatKey(int keycode, int* keymap, bool pressed_or_released);
  Controller(){Init();} // costruttore
};


class Car:public Object3D{
public:
  // Metodi
  void Init(); // inizializza variabili
  void Render() const; // disegna a schermo
  void Hit(float fx, float fy, float fz);
  void DoStep(); // computa un passo del motore fisico
  Car(){Init();} // costruttore
 
  Controller controller;
 
  // STATO DELLA MACCHINA
  // (DoStep fa evolvere queste variabili nel tempo)
  float facing; // posizione e orientamento
  float mozzoA, mozzoP, sterzo; // stato interno

  // STATS DELLA MACCHINA
  // (di solito rimangono costanti)
  float velSterzo, velRitornoSterzo, accMax, attrito,
        raggioRuotaA, raggioRuotaP, grip,
        attritoX, attritoY, attritoZ; // attriti

private:
  Mesh* carlinga;
  Mesh* glass;
  Mesh* engine;
  Mesh* frontlight;
  Mesh* tyreFL;
  Mesh* tyreFR;
  Mesh* tyreBL;
  Mesh* tyreBR;
};
