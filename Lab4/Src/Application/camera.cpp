/**
@file     camera.cpp
@brief    Class camera methods
@date     Created on 04/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include "camera.h"

// *******************************************************************
// defines



//Constructor
Camera::Camera(LPDIRECT3DDEVICE9 pDevice) :pDev(pDevice)
{
  World = new D3DXMATRIX;
  View = new D3DXMATRIX;
  Proj = new D3DXMATRIX;
}

Camera::~Camera()
{
  delete World;
  delete View;
  delete Proj;
}

//Rotate object using keyboard
void Camera::rotateObject(ObjectGenerator *obj,float dx, float dy)
{
  D3DXMATRIX Rx, Ry;
  D3DXMatrixRotationX(&Rx, dx);
  D3DXMatrixRotationY(&Ry, dy);
  *obj->worldMatrix() = Rx * Ry * (*obj->worldMatrix());
}

//Rotate camera using mouse
void Camera::rotateCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    //Rotating around up vector
    //Creating transform matrix
    D3DXMATRIX Rz;
    D3DXMatrixRotationAxis(&Rz, upVector(), dx);
    //Updating lookat and up vectors and position of camera

    D3DXVec3TransformCoord(dirVector(), dirVector(), &Rz);
    D3DXVec3TransformCoord(upVector(), upVector(), &Rz);

    D3DXVECTOR3 diff = *lookatVector() - *cameraPos();
    float norm = D3DXVec3Length(&diff);
    *cameraPos() = *lookatVector() - *dirVector() * norm;

    D3DXVec3Normalize(dirVector(), dirVector());
    D3DXVec3Cross(rightVector(), dirVector(), upVector());
    D3DXVec3Normalize(rightVector(), rightVector());
    D3DXVec3Cross(upVector(), rightVector(), dirVector());
  }
  if (fabs(dy) > 0.001)
  {
    //Rotating around right vector
    //Creating transform matrix
    D3DXMATRIX Rz;
    D3DXMatrixRotationAxis(&Rz, rightVector(), -dy);
    //Updating lookat and up vectors and position of camera

    D3DXVec3TransformCoord(dirVector(), dirVector(), &Rz);
    D3DXVec3TransformCoord(rightVector(), rightVector(), &Rz);

    D3DXVECTOR3 diff = *lookatVector() - *cameraPos();
    float norm = D3DXVec3Length(&diff);
    *cameraPos() = *lookatVector() - *dirVector() * norm;

    D3DXVec3Normalize(dirVector(), dirVector());
    D3DXVec3Cross(rightVector(), dirVector(), upVector());
    D3DXVec3Normalize(rightVector(), rightVector());
    D3DXVec3Cross(upVector(), rightVector(), dirVector());
  }
}

//Zoom camera
void Camera::zoom(float dr)
{
  *cameraPos() = *(cameraPos()) - *(cameraPos()) * dr;
  D3DXVECTOR3 diff =  *lookatVector() - *cameraPos();
  D3DXVec3Normalize(dirVector(), &diff);
}

//Movement for 1st person camera using keyboard
void Camera::moveWASDCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    *cameraPos() += dx * (*rightVector()) * speedWASD;
    *lookatVector() += dx * (*rightVector()) * speedWASD;
  }

  if (fabs(dy) > 0.001)
  {
    *cameraPos() -= dy * (*dirVector()) * speedWASD;
    *lookatVector() -= dy * (*dirVector()) * speedWASD;
  }
  D3DXVECTOR3 diff = *lookatVector() - *cameraPos();
  D3DXVec3Normalize(dirVector(), &diff);
}

//Rotation of 1st person camera using mouse
void Camera::rotateWASDCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    *lookatVector() += dx * (*rightVector()) * 2 * speedWASD;
  }

  if (fabs(dy) > 0.001)
  {
    *lookatVector() -= dy * (*upVector()) * 2 * speedWASD;
  }
  D3DXVECTOR3 diff = *lookatVector() - *cameraPos();
  D3DXVec3Normalize(dirVector(), &diff);
  D3DXVec3Cross(rightVector(), dirVector(), upVector());
  D3DXVec3Normalize(rightVector(), rightVector());
  D3DXVec3Cross(upVector(), rightVector(), dirVector());
}

//Change camera mode
void Camera::setCamera(bool freeCamera)
{
  if (freeCamera)
  {
    *lookatVector() = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 diff = *lookatVector() - *cameraPos();
    D3DXVec3Normalize(dirVector(), &diff);
    D3DXVec3Cross(rightVector(), dirVector(), upVector());
    D3DXVec3Normalize(rightVector(), rightVector());
    D3DXVec3Cross(upVector(), rightVector(), dirVector());
  }
}

//Update camera matrices
void Camera::updateCamera(D3DXMATRIX W)
{
  *World = W;
  Proj = projectionMatrix();
  View = viewMatrix();
  pDev->SetTransform(D3DTS_WORLD, World);
  pDev->SetTransform(D3DTS_PROJECTION, Proj);
  pDev->SetTransform(D3DTS_VIEW, View);
}


//Gets current view matrix
D3DXMATRIX* Camera::viewMatrix()
{
  D3DXMatrixLookAtRH(View,&cPosition,&lookat,&up);
  return View;
}

//Gets current projection matrix
D3DXMATRIX* Camera::projectionMatrix()
{
  D3DXMatrixPerspectiveFovRH(Proj, yAngle, tan(xAngle / 2.0f) / tan(yAngle / 2.0f), clBorder, farBorder);
  return Proj;
}

void Camera::setCameraVectors(D3DXVECTOR3 l, D3DXVECTOR3 u) 
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
