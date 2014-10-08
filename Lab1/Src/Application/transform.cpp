/**
@file     transform.cpp
@brief    Class transform methods
@date     Created on 04/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include "transform.h"

// *******************************************************************
// defines



//Constructor
Transformations::Transformations()
{
  World = new D3DXMATRIX;
  View = new D3DXMATRIX;
  Proj = new D3DXMATRIX;
}

Transformations::~Transformations()
{
  delete World;
  delete View;
  delete Proj;
}
//Gets current view matrix
D3DXMATRIX* Transformations::viewMatrix()
{
  /*D3DXMATRIX* M = new D3DXMATRIX(cxAxis.x, cxAxis.y, cxAxis.z, 0.0f,
                                 cyAxis.x, cyAxis.y, cyAxis.z, 0.0f,
                                 czAxis.x, czAxis.y, czAxis.z, 0.0f,
                                 cPosition.x, cPosition.y, cPosition.z, 1.0f);
  D3DXMATRIX* N = new D3DXMATRIX();
  D3DXMatrixInverse(N, NULL, M);*/
  D3DXMatrixLookAtRH(View,&cPosition,&dir,&up);
  return View;
}

//Gets current projection matrix
D3DXMATRIX* Transformations::projectionMatrix()
{
  D3DXMatrixPerspectiveFovRH(Proj, yAngle, tan(xAngle / 2.0f) / tan(yAngle / 2.0f), clBorder, farBorder);
  return Proj;
}

void Transformations::setCameraVectors(D3DXVECTOR3 l, D3DXVECTOR3 u) 
{
  lookat = l;
  up = u;
  dir = lookat - cPosition;
  D3DXVec3Normalize(&dir,&dir);

  D3DXVec3Cross(&right, &dir, &up);
  D3DXVec3Cross(&up, &right, &dir);

  D3DXVec3Normalize(&right, &right);
  D3DXVec3Normalize(&up, &up);
}
