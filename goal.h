#include "base_obj.h"
#include "mesh.h"

class Goal: public Object3D {
public:
  // Metodi
  void Init(float x,float y, float z, float* col); // inizializza variabili
  void Render() const; // disegna a schermo
  void DoStep(); // computa un passo del motore fisico
  Goal(float x,float y, float z, float* col){Init(x,y,z,col);} // costruttore

private:
	Mesh* goal;
	float color[3];
};
