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
// *******************************************************************
// defines & constants

// *******************************************************************
// classes 


//Class that generates rendering of object(torus) and responsible for basic rendering
class objectGenerator
{
public:
  //Constructor
  objectGenerator(LPDIRECT3DDEVICE9 pDevice, int triangleM, int triangleN);

  //Destructor
  ~objectGenerator();

  //Rendering function
  void render();

  //Rotation of object using keyboard
  void rotateObject(float dx, float dy);

  //Rotation of camera using mouse
  void rotateCamera(float dx, float dy);

  //Zooming using wheel
  void zoom(float dr);

private:
  //Pointer to current device
  LPDIRECT3DDEVICE9 pDev;

  //Vertex and Index buffers to use
  IDirect3DVertexBuffer9 *pVertBuff;
  IDirect3DIndexBuffer9  *pIndBuff;

  //Number of squares for triangulation
  int M, N;

  //Number of vertices and triangles
  int vertNum, triNum;

  //Transformations class that is responsible for world,view,proj transforms
  Transformations Trans;

  //Creates color value using 3D coordinates
  DWORD colorFromPoint(cartesian point);

};


#endif