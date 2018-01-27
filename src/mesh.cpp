// file mesh.cpp
//
// Implementazione dei metodi di Mesh

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "mesh.h"

/**************************
 * Material
 **************************/

/**
 * dice alle OpenGL di utilizzare il materiale corrente per un successivo
 * rendering di una mesh
 */
void Material::useMaterial()
{
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kd);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &Ns);
}

/**
 * Carica la texture da file da usare per il materiale corrente
 * @param  filename Path del file della texture
 */
bool Material::LoadTexture(char *filename)
{
  SDL_Surface *s = IMG_Load(filename);
  if (!s)
  {
    fprintf(stderr, "File not found: %s\n", filename);
    return false;
  }
  glGenTextures(1, &texture_index);
  glBindTexture(GL_TEXTURE_2D, texture_index);

  gluBuild2DMipmaps(
      GL_TEXTURE_2D,
      GL_RGB,
      s->w, s->h,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      s->pixels);
  glTexParameteri(
      GL_TEXTURE_2D,
      GL_TEXTURE_MAG_FILTER,
      GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(
      GL_TEXTURE_2D,
      GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  use_texture = true;
  return true;
}

/**************************
 * Mesh
 **************************/

/**
 * Calcola le normali di ogni faccia della mesh
 */
void Mesh::ComputeNormalsPerFace()
{
  for (int i = 0; i < f.size(); i++)
  {
    f[i].ComputeNormal();
  }
}

/**
 * Imposta un colore per la mesh
 * @param color Colore in RGB
 */
void Mesh::SetDiffuse(float *color)
{
  mtl[0]->Kd[0] = color[0];
  mtl[0]->Kd[1] = color[1];
  mtl[0]->Kd[2] = color[2];
}

/**
 * Computo normali per vertice
 * (come media rinormalizzata delle normali delle facce adiacenti)
 */
void Mesh::ComputeNormalsPerVertex()
{
  // uso solo le strutture di navigazione FV (da Faccia a Vertice)!

  // fase uno: ciclo sui vertici, azzero tutte le normali
  for (int i = 0; i < v.size(); i++)
  {
    v[i].n = Point3(0, 0, 0);
  }
  // fase due: ciclo sulle facce: accumulo le normali di F nei 3 V corrispondenti
  for (int i = 0; i < f.size(); i++)
  {
    f[i].v[0]->n = f[i].v[0]->n + f[i].n;
    f[i].v[1]->n = f[i].v[1]->n + f[i].n;
    f[i].v[2]->n = f[i].v[2]->n + f[i].n;
  }
  // fase tre: ciclo sui vertici; rinormalizzo
  // la normale media rinormalizzata e' uguale alla somma delle normnali, rinormalizzata
  for (int i = 0; i < v.size(); i++)
  {
    v[i].n = v[i].n.Normalize();
  }
}

/**
 * Rendering della mesh senza texture
 */
void Mesh::NoTexRender()
{
  glDisable(GL_TEXTURE_2D);
  // mandiamo tutti i triangoli a schermo
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < f.size(); i++)
  {

    if (f[i].mtl != NULL)
      f[i].mtl->useMaterial();

    //f[i].n.SendAsNormal(); // flat shading
    (f[i].v[0])->n.SendAsNormal();
    (f[i].v[0])->p.SendAsVertex();

    (f[i].v[1])->n.SendAsNormal();
    (f[i].v[1])->p.SendAsVertex();

    (f[i].v[2])->n.SendAsNormal();
    (f[i].v[2])->p.SendAsVertex();
  }
  glEnd();
}

/**
 * Rendering della mesh
 */
void Mesh::Render()
{
  if (mtl[0]->use_texture)
  {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, mtl[0]->texture_index);
  }
  // mandiamo tutti i triangoli a schermo
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < f.size(); i++)
  {

    if (f[i].mtl != NULL)
      f[i].mtl->useMaterial();

    //f[i].n.SendAsNormal(); // flat shading
    (f[i].v[0])->n.SendAsNormal();
    if (mtl[0]->use_texture)
      (f[i].vt[0])->SendAsTextureMap();
    (f[i].v[0])->p.SendAsVertex();

    (f[i].v[1])->n.SendAsNormal();
    if (mtl[0]->use_texture)
      (f[i].vt[1])->SendAsTextureMap();
    (f[i].v[1])->p.SendAsVertex();

    (f[i].v[2])->n.SendAsNormal();
    if (mtl[0]->use_texture)
      (f[i].vt[2])->SendAsTextureMap();
    (f[i].v[2])->p.SendAsVertex();
  }

  if (mtl[0]->use_texture)
  {
    glDisable(GL_TEXTURE_2D);
  }
  glEnd();
}

/**
 * Restituisce il materiale con il nome dato in input al metodo
 */
Material *Mesh::GetMaterial(char *name)
{
  for (int i = 0; i < mtl.size(); i++)
  {
    if (strcmp(mtl[i]->name, name) == 0)
      return mtl[i];
  }
  return NULL;
}

void Mesh::ComputeBoundingBox()
{
  if (!v.size())
    return;
  bbmin = bbmax = v[0].p;
  for (int i = 0; i < v.size(); i++)
  {
    for (int k = 0; k < 3; k++)
    {
      if (bbmin.coord[k] > v[i].p.coord[k])
        bbmin.coord[k] = v[i].p.coord[k];
      if (bbmax.coord[k] < v[i].p.coord[k])
        bbmax.coord[k] = v[i].p.coord[k];
    }
  }
}

/**
 * Carica per la mesh i materiali da file .mtl
 */
bool Mesh::LoadMaterials(char *path, char *filename)
{
  char fullpath[80] = "";
  strcat(fullpath, path);
  strcat(fullpath, filename);

  FILE *file = fopen(fullpath, "rt"); // apriamo il file in lettura
  if (!file)
  {
    fprintf(stderr, "File not found: %s\n", fullpath);
    return false;
  }
  char buf[128];
  // char name[20];
  Material *m = NULL;

  while (fscanf(file, "%s", buf) != EOF)
  {
    if (strcmp(buf, "#") == 0)
    {
      fgets(buf, sizeof(buf), file);
      continue;
    }

    if (strcmp(buf, "newmtl") == 0)
    {
      if (m != NULL)
      {
        mtl.push_back(m);
      }

      m = new Material;
      // m->use_texture = false;
      fscanf(file, "%s", m->name);
      continue;
    }
    if (strcmp(buf, "Ns") == 0)
    {
      fscanf(file, "%f", &m->Ns);
      continue;
    }
    if (strcmp(buf, "Ka") == 0)
    {
      fscanf(file, "%f %f %f", &m->Ka[0], &m->Ka[1], &m->Ka[2]);
      continue;
    }
    if (strcmp(buf, "Kd") == 0)
    {
      fscanf(file, "%f %f %f", &m->Kd[0], &m->Kd[1], &m->Kd[2]);
      continue;
    }
    if (strcmp(buf, "Ks") == 0)
    {
      fscanf(file, "%f %f %f", &m->Ks[0], &m->Ks[1], &m->Ks[2]);
      continue;
    }
    if (strcmp(buf, "Ke") == 0)
    {
      fscanf(file, "%f %f %f", &m->Ke[0], &m->Ke[1], &m->Ke[2]);
      continue;
    }
    if (strcmp(buf, "Ni") == 0)
    {
      fscanf(file, "%f", &m->Ni);
      continue;
    }
    if (strcmp(buf, "d") == 0)
    {
      fscanf(file, "%f", &m->Kd[3]);
      continue;
    }
    if (strcmp(buf, "illum") == 0)
    {
      fscanf(file, "%d", &m->illum);
      continue;
    }
    if (strcmp(buf, "map_Kd") == 0)
    {
      char tx_fname[30];
      fscanf(file, "%s", tx_fname);
      m->LoadTexture(tx_fname);
      continue;
    }
  }
  if (m != NULL)
  {
    mtl.push_back(m);
  }
  fclose(file);
  return true;
}

/**
 * Carica mesh da file .obj
 */
bool Mesh::LoadFromObj(char *path, char *filename)
{
  char fullpath[80] = "";
  strcat(fullpath, path);
  strcat(fullpath, filename);

  FILE *file = fopen(fullpath, "rt"); // apriamo il file in lettura
  if (!file)
  {
    fprintf(stderr, "File not found: %s\n", fullpath);
    return false;
  }

  //make a first pass through the file to get a count of the number
  //of vertices, normals, texcoords & triangles
  char buf[128];
  int nv, nf, nt;
  float x, y, z;
  int va, vb, vc, vd;
  int na, nb, nc, nd;
  int ta, tb, tc, td;

  Material *current_mtl = NULL;

  nv = 0;
  nf = 0;
  nt = 0;
  while (fscanf(file, "%s", buf) != EOF)
  {
    switch (buf[0])
    {
    case '#': // comment
      // eat up rest of line
      fgets(buf, sizeof(buf), file);
      break;
    case 'v': // v, vn, vt
      switch (buf[1])
      {
      case '\0': // vertex
        // eat up rest of line
        fgets(buf, sizeof(buf), file);
        nv++;
        break;
      case 't': // vertex
        // eat up rest of line
        fgets(buf, sizeof(buf), file);
        nt++;
        break;
      default:
        break;
      }
      break;
    case 'f': // face
      fscanf(file, "%s", buf);
      // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
      if (strstr(buf, "//"))
      {
        // v//n
        sscanf(buf, "%d//%d", &va, &na);
        fscanf(file, "%d//%d", &vb, &nb);
        fscanf(file, "%d//%d", &vc, &nc);
        nf++;
        // nt++;
        while (fscanf(file, "%d//%d", &vd, &nd) > 0)
        {
          // nt++;
        }
      }
      else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3)
      {
        // v/t/n
        fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
        fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
        nf++;
        // nt++;
        while (fscanf(file, "%d/%d/%d", &vd, &td, &nd) > 0)
        {
          // nt++;
        }
      }
      else if (sscanf(buf, "%d/%d", &va, &ta) == 2)
      {
        // v/t
        fscanf(file, "%d/%d", &vb, &tb);
        fscanf(file, "%d/%d", &vc, &tc);
        nf++;
        // nt++;
        while (fscanf(file, "%d/%d", &vd, &td) > 0)
        {
          // nt++;
        }
      }
      else
      {
        // v
        fscanf(file, "%d", &va);
        fscanf(file, "%d", &vb);
        nf++;
        // nt++;
        while (fscanf(file, "%d", &vc) > 0)
        {
          // nt++;
        }
      }
      break;

    default:
      // eat up rest of line
      fgets(buf, sizeof(buf), file);
      break;
    }
  }

  //printf("dopo FirstPass nv=%d nf=%d nt=%d\n",nv,nf,nt);

  // allochiamo spazio per nv vertici
  v.resize(nv);

  vt.resize(nt);

  // rewind to beginning of file and read in the data this pass
  rewind(file);

  //on the second pass through the file, read all the data into the
  //allocated arrays

  nv = 0;
  nt = 0;
  while (fscanf(file, "%s", buf) != EOF)
  {
    if (strcmp(buf, "mtllib") == 0)
    {
      fscanf(file, "%s", buf);
      LoadMaterials(path, buf);
      continue;
    }
    if (strcmp(buf, "usemtl") == 0)
    {
      fscanf(file, "%s", buf);
      current_mtl = GetMaterial(buf);
      printf("%s\n", current_mtl->name);
      continue;
    }
    switch (buf[0])
    {
    case '#': // comment
      // eat up rest of line
      fgets(buf, sizeof(buf), file);
      break;
    case 'v': // v, vn, vt
      switch (buf[1])
      {
      case '\0': // vertex
        fscanf(file, "%f %f %f", &x, &y, &z);
        v[nv].p = Point3(x, y, z);
        nv++;
        break;
      case 't': // texture point
        // eat up rest of line
        fscanf(file, "%f %f", &x, &y);
        vt[nt] = Point2(x, y);
        nt++;
        break;
      default:
        break;
      }
      break;
    case 'f': // face
      fscanf(file, "%s", buf);
      // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
      if (strstr(buf, "//"))
      {
        // v//n
        sscanf(buf, "%d//%d", &va, &na);
        fscanf(file, "%d//%d", &vb, &nb);
        fscanf(file, "%d//%d", &vc, &nc);
        va--;
        vb--;
        vc--;
        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
        newface.mtl = current_mtl;
        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
        // nt++;
        vb = vc;
        while (fscanf(file, "%d//%d", &vc, &nc) > 0)
        {
          vc--;
          Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
          f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
          // nt++;
          vb = vc;
        }
      }
      else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3)
      {
        // v/t/n
        fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
        fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
        va--;
        vb--;
        vc--;
        ta--;
        tb--;
        tc--;
        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
        newface.mtl = current_mtl;
        newface.SetTexture(&(vt[ta]), &(vt[tc]), &(vt[tb]));
        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
        // nt++;
        vb = vc;
        tb = tc;
        while (fscanf(file, "%d/%d/%d", &vc, &tc, &nc) > 0)
        {
          vc--;
          tc--;
          Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
          newface.mtl = current_mtl;
          newface.SetTexture(&(vt[ta]), &(vt[tc]), &(vt[tb]));
          f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
          // nt++;
          vb = vc;
          tb = tc;
        }
      }
      else if (sscanf(buf, "%d/%d", &va, &ta) == 2)
      {
        // v/t
        fscanf(file, "%d/%d", &va, &ta);
        fscanf(file, "%d/%d", &va, &ta);
        va--;
        vb--;
        vc--;
        ta--;
        tb--;
        tc--;
        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
        newface.mtl = current_mtl;
        newface.SetTexture(&(vt[ta]), &(vt[tc]), &(vt[tb]));
        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
        // nt++;
        vb = vc;
        tb = tc;
        while (fscanf(file, "%d/%d", &vc, &tc) > 0)
        {
          vc--;
          tc--;
          Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
          newface.mtl = current_mtl;
          newface.SetTexture(&(vt[ta]), &(vt[tc]), &(vt[tb]));
          f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
          // nt++;
          vb = vc;
          tb = tc;
        }
      }
      else
      {
        // v
        sscanf(buf, "%d", &va);
        fscanf(file, "%d", &vb);
        fscanf(file, "%d", &vc);
        va--;
        vb--;
        vc--;
        Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
        newface.mtl = current_mtl;
        f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
        nt++;
        vb = vc;
        while (fscanf(file, "%d", &vc) > 0)
        {
          vc--;
          Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
          newface.mtl = current_mtl;
          f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
          nt++;
          vb = vc;
        }
      }
      break;

    default:
      // eat up rest of line
      fgets(buf, sizeof(buf), file);
      break;
    }
  }

  //printf("dopo SecondPass nv=%d nt=%d\n",nv,nt);

  fclose(file);
  return true;
}
