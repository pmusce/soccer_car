#ifndef __MESH_DEF_
#define __MESH_DEF_

#include <vector>
#include "point3.h"
// classe Vertex: 
// i vertici della mesh

class Vertex
{ 
public: 
  Point3 p; // posizione

  // attributi per vertice
  Vector3 n;
};

class Edge
{
public: 
  Vertex* v[2]; // due puntatori a Vertice (i due estremi dell'edge)
  // attributi per edge:
};

class Material
{
public:
  char name[20];
  GLfloat Ns;
  GLfloat Ka[3];
  GLfloat Kd[4];
  GLfloat Ks[3];
  GLfloat Ke[3];
  GLfloat Ni;
  int illum;
  bool use_texture;
  int texture_index;

  void useMaterial();
  bool LoadTexture(char* filename);
};

class Face
{
public: 
  Vertex* v[3]; // tre puntatori a Vertice (i tre vertici del triangolo)
  Point2* vt[3];
  Material* mtl;

  // costruttore
  Face(Vertex* a, Vertex* b, Vertex* c){
    v[0]=a; v[1]=b; v[2]=c;
    mtl = NULL;
  }

  void SetTexture(Point2* a, Point2* b, Point2* c){
    vt[0]=a; vt[1]=b; vt[2]=c;
  }
  
  // attributi per faccia
  Vector3 n; // normale (per faccia)
  
  // computa la normale della faccia
  void ComputeNormal() {
    n= -( (v[1]->p - v[0]->p) % (v[2]->p - v[0]->p) ).Normalize();
  }
  
  // attributi per wedge
};


class Mesh
{
  std::vector<Vertex> v; // vettore di vertici
  std::vector<Point2> vt;
  std::vector<Face> f;   // vettore di facce
  std::vector<Edge> e;   // vettore di edge (per ora, non usato)
  std::vector<Material*> mtl;

public:  
  
  // costruttore con caricamento
  Mesh(char *path, char *filename){
    LoadFromObj(path, filename);
    ComputeNormalsPerFace();
    ComputeNormalsPerVertex();
    ComputeBoundingBox();
  }
  
  // metodi
  void Render(); // manda a schermo la mesh
  void NoTexRender();
  
  bool LoadFromObj(char *path, char *filename); //  carica la mesh da un file OFF
  bool LoadMaterials(char *path, char *filename);
  
  void ComputeNormalsPerFace();
  void ComputeNormalsPerVertex();
  void ComputeBoundingBox();

  Material* GetMaterial(char *name);
  void SetDiffuse(float* color);
  
  // centro del axis aligned bounding box
  Point3 Center(){ return (bbmin+bbmax)/2.0; };
  
  Point3 bbmin,bbmax; // bounding box 
};

#endif