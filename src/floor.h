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
  void initTessField();

	GLuint field_tex;

  float tess_field_x[201];
  float tess_field_z[201];

};
