#ifndef __OBJGEN_H__
#define __OBJGEN_H__

/**
@file     objectGenerator.h
@brief    Class objectGenerator definition
@date     Created on 03/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include "basic.h"
#include "camera.h"
#include "myMesh.h"
// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

class Camera;

class ObjectGenerator
{
public:

  virtual void render(Camera *cam) = 0 {};
  D3DXMATRIX *worldMatrix() { return &world; }
  void setWorldMatrix(D3DXMATRIX M) { world = M; }

protected:
  //Pointer to current device
  LPDIRECT3DDEVICE9 pDev;
  D3DXMATRIX world;
};


class TexturedObjectGenerator : public ObjectGenerator
{
public:
  virtual ~TexturedObjectGenerator() = 0 {};
  void LoadTexture(char *filename, char **mipmaps, int number);
  DWORD *TexMin();
  DWORD *TexMag();
  DWORD *TexMip();
  float *TexBias();

protected:
  LPDIRECT3DTEXTURE9 *texture;
  DWORD minfilter, magfilter, mipfilter;
  float mipmapbias;
  int texturized = 0;
};


//Class that generates rendering of object(torus) and responsible for basic rendering
class GeometricObjectGenerator:public TexturedObjectGenerator
{
public:
  //Constructor
  GeometricObjectGenerator(LPDIRECT3DDEVICE9 pDevice);

  //Destructor
  ~GeometricObjectGenerator();

  //Generate geometry
  void Geometry(int triangleM, int triangleN, Figure *F);

  //Rendering function
  void render(Camera *cam);

private:
  //Vertex and Index buffers to use
  IDirect3DVertexBuffer9 *pVertBuff;
  IDirect3DIndexBuffer9  *pIndBuff;

  //Number of squares for triangulation
  int M, N;

  //Number of vertices and triangles
  int vertNum, triNum;

  //Creates color value using 3D coordinates
  DWORD colorFromPoint(D3DXVECTOR3 point);

};


class MeshObjectGenerator:public TexturedObjectGenerator
{
public:
  //Constructor
  MeshObjectGenerator(LPDIRECT3DDEVICE9 pDevice,char *filename);

  //Destructor
  ~MeshObjectGenerator();

  //Rendering function
  void render(Camera *cam);
  void  circleMove();
private:
  //Raw mesh
  MyMesh mesh;

  //Filename
  char *name;

  //Parameters for spot light movement
  D3DXVECTOR3 start;
  D3DXMATRIX spin;

  float phi = 0.0f;
};

#endif