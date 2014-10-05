#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

/**
@file     transform.h
@brief    Class transform definition
@date     Created on 04/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include "basic.h"

// *******************************************************************
// defines

// *******************************************************************
// classes 

//Class that contains world,view,proj matrices  and responsible for transformations
class Transformations
{
public:

  //constructor
  Transformations();

  //destructor
  ~Transformations();

  //Set camera position
  void setCameraPosition(D3DXVECTOR3 pos) { cPosition = pos; }

  //Set camera lookat and up vectors
  void setCameraVectors(D3DXVECTOR3 l, D3DXVECTOR3 u) { lookat = l; up = u; }

  //Sets projection matrix using two angles and  two borders
  void setProjection(float Ax, float Ay, float zN, float zF) { xAngle = Ax; yAngle = Ay; clBorder = zN; farBorder = zF; }

  //Sets world matrix
  void setWorldMatrix(D3DXMATRIX *M) {World = M; }

  //Corresponding getters for data and setters
  D3DXMATRIX * projectionMatrix();
  D3DXMATRIX * viewMatrix();
  D3DXMATRIX * worldMatrix() { return World; }
  D3DXVECTOR3 * lookatVector() {return &lookat;}
  D3DXVECTOR3 * upVector() { return &up; }
  D3DXVECTOR3 * cameraPos() { return &cPosition; }
private:
  //camera position
  D3DXVECTOR3 cPosition;
  //camera vectors
  D3DXVECTOR3 lookat, up;
  //projections parameters
  float xAngle, yAngle, clBorder, farBorder;
  //world matrix
  D3DXMATRIX * World, *View, *Proj;
};

#endif