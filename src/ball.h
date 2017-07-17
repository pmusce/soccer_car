#include "base_obj.h"
#include "mesh.h"

class Ball: public Object3D{
public:
  // Metodi
  void Init(int x,int y, int z); // inizializza variabili
  void Render() const; // disegna a schermo
  void Hit(float fx, float fy, float fz);
  void DoStep(); // computa un passo del motore fisico
  Ball(int x,int y, int z){Init(x,y,z);} // costruttore
 
  // STATO DELLA SCATOLA
  // (DoStep fa evolvere queste variabili nel tempo)
  float facing; // posizione e orientamento
  
  // STATS DELLA MACCHINA
  // (di solito rimangono costanti)
  float attritoX, attritoY, attritoZ; // attriti
  
private:
  Mesh* ball;
};