#include "base_obj.h"
#include "mesh.h"


#define BALL_MAX_NUM 2

#define BALL_MESH 0
#define BALL_FACE 1
// #define BALL_ENV_MAP 2

class Ball: public Object3D{
public:
  // Metodi
  void Init(int x,int y, int z); // inizializza variabili
  void Render() const; // disegna a schermo
  void Hit(float fx, float fy, float fz);
  void ChangeBallType();
  void DoStep(); // computa un passo del motore fisico
  Ball(int x,int y, int z){Init(x,y,z);} // costruttore

  float facing; // posizione e orientamento

  float attritoX, attritoY, attritoZ; // attriti

private:
  void RenderFaceBall() const;
  Mesh* ball;
  GLuint face_tex;

  int ball_type;
};
