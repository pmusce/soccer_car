#ifndef FILE_BASE_OBJ_SEEN
#define FILE_BASE_OBJ_SEEN

class Object3D{
public:
  // Metodi
  virtual void Render() const{
  } // disegna a schermo
  bool DoesCollide() {
  	return collide;
  }

  float px,py,pz; // posizione
  float vx,vy,vz; // velocita' attuale

  void setPos(float x, float y, float z) {
    px = x; py = y; pz = z;
  }
  void setVel(float x, float y, float z) {
    vx = x; vy = y; vz = z;
  }
  float collisionRadius;
protected:
	bool collide;
};


#endif
