#include "base_obj.h"

class Floor: public Object3D {
public:
  // Metodi
  void Init(); // inizializza variabili
  void Render() const; // disegna a schermo
  void DoStep(); // computa un passo del motore fisico
  Floor(){Init();} // costruttore

private:
	void RenderSky() const;
	void RenderField() const;
	void RenderGround() const;
	GLuint texture;
};
