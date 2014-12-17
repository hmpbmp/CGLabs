#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

/**
@file     camera.h
@brief    Class camera definition
@date     Created on 04/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include "objectGenerator.h"
#include "basic.h"

// *******************************************************************
// defines

// *******************************************************************
// classes 

//Class that contains world,view,proj matrices  and responsible for transformations

class ObjectGenerator;


class Camera
{
public:

  //constructor
  Camera(LPDIRECT3DDEVICE9 pDevice);

  //destructor
  ~Camera();


  //Rotation of object using keyboard
  void rotateObject(ObjectGenerator *obj,float dx, float dy);

  //Rotation of camera using mouse
  void rotateCamera(float dx, float dy);

  //Zooming using wheel
  void zoom(float dr);

  //Movement and rotation in terms of WASD (1st person) camera
  void moveWASDCamera(float dx, float dy);

  void rotateWASDCamera(float dx, float dy);

  //Change camera type
  void setCamera(bool freeCamera);

  //Set camera position
  void setCameraPosition(D3DXVECTOR3 pos) { cPosition = pos; }

  //Set camera lookat and up vectors
  void setCameraVectors(D3DXVECTOR3 l, D3DXVECTOR3 u);

  //Sets projection matrix using two angles and  two borders
  void setProjection(float Ax, float Ay, float zN, float zF) { xAngle = Ax; yAngle = Ay; clBorder = zN; farBorder = zF; }

  //Sets world matrix
  void setWorldMatrix(D3DXMATRIX *M) { World = M; }

  void updateCamera(D3DXMATRIX World);

  //Corresponding getters for data and setters
  D3DXMATRIX * projectionMatrix();
  D3DXMATRIX * viewMatrix();
  D3DXMATRIX * worldMatrix() { return World; }

  D3DXVECTOR3 * lookatVector() {return &lookat;}
  D3DXVECTOR3 * upVector() { return &up; }
  D3DXVECTOR3 * cameraPos() { return &cPosition; }
  D3DXVECTOR3 *dirVector() { return &dir; }
  D3DXVECTOR3 *rightVector(){ return &right; }

private:
  //camera position
  D3DXVECTOR3 cPosition;
  //camera vectors
  D3DXVECTOR3 lookat, up,dir,right;
  //projections parameters
  float xAngle, yAngle, clBorder, farBorder;
  //world matrix
  D3DXMATRIX * World, *View, *Proj;

  LPDIRECT3DDEVICE9 pDev;

  static const int speedWASD = 10;
};

#endif