#include "base_obj.h"
#include <GL/gl.h>

class Floor: public Object3D {
public:
  // Metodi
  void Init(); // inizializza variabili
  void Render() const; // disegna a schermo
  Floor(){Init();} // costruttore

private:
	void RenderSky() const;
	void RenderField() const;
	void RenderGround() const;
  void RenderBorder() const;

	GLuint field_tex;
};
