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
#include "transform.h"
#include "myMesh.h"
// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

class ObjectGenerator
{
  public:
  //Rotation of object using keyboard
  void rotateObject(float dx, float dy);

  //Rotation of camera using mouse
  void rotateCamera(float dx, float dy);

  //Zooming using wheel
  void zoom(float dr);

  //Movement and rotation in terms of WASD (1st person) camera
  void moveWASDCamera(float dx, float dy);

  void rotateWASDCamera(float dx, float dy);

  //Change camera type
  void setCamera(bool freeCamera);

  virtual void render() = 0;

  protected:
  //Pointer to current device
  LPDIRECT3DDEVICE9 pDev;

  //Transformations class that is responsible for world,view,proj transforms
  Transformations Trans;

  static const int speedWASD = 10;
};





//Class that generates rendering of object(torus) and responsible for basic rendering
class GeometricObjectGenerator:public ObjectGenerator
{
public:
  //Constructor
  GeometricObjectGenerator(LPDIRECT3DDEVICE9 pDevice, int triangleM, int triangleN);

  //Destructor
  ~GeometricObjectGenerator();

  //Rendering function
  void render();

private:
  //Vertex and Index buffers to use
  IDirect3DVertexBuffer9 *pVertBuff;
  IDirect3DIndexBuffer9  *pIndBuff;

  //Number of squares for triangulation
  int M, N;

  //Number of vertices and triangles
  int vertNum, triNum;

  //Creates color value using 3D coordinates
  DWORD colorFromPoint(cartesian point);

};


class MeshObjectGenerator:public ObjectGenerator
{
public:
  //Constructor
  MeshObjectGenerator(LPDIRECT3DDEVICE9 pDevice,char *filename);

  //Destructor
  ~MeshObjectGenerator();

  //Rendering function
  void render();
  void initCircleMove(D3DXVECTOR3 c, float angle,float radius) {center = c;phi = angle;R = radius;}
  void  circleMove();
private:
  //Raw mesh
  MyMesh mesh;

  //Filename
  char *name;

  //Parameters for spot light movement
  D3DXVECTOR3 center;
  float phi;
  float R;
};

#endif