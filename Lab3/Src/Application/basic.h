#ifndef BASIC_H_INCLUDED
#define BASIC_H_INCLUDED

/**
@file     basic.h
@brief    Definiton of some basic classes that will be used
@date     Created on 03/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

// *******************************************************************
// defines & constants

const float  PI_F = 3.14159265358979f;

// *******************************************************************
// classes 

//Vertex class
struct basicvertex
{
  D3DXVECTOR3 p;
  D3DCOLOR diffuse;
};

struct vertex
{
  D3DXVECTOR3 p;
  D3DXVECTOR3 normal;
  D3DCOLOR diffuse;
  D3DCOLOR specular;

  float tu, tv;
};




//Torus points generator class
class Torus
{
public:
  //constructor
  Torus(float smallR = 1.0f, float bigR = 2.0f) :r(smallR), R(bigR) {}
  //Converts from parameters to point in 3D
  D3DXVECTOR3 point(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    D3DXVECTOR3 p((R + r * cos(n)) * sin(m), (R + r * cos(n)) * cos(m), r * sin(n));
    return p;
  }

  D3DXVECTOR3 normalAtPoint(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    D3DXVECTOR3 norm(sinf(n)*cos(m), sin(m), cos(n)*cos(m));
    return norm;
  }

private:
  float r, R;
};

class Plane
{
public:
  //constructor
  Plane(D3DXVECTOR3 X,D3DXVECTOR3 Y):x(X),y(Y)
  {
    D3DXVec3Cross(&norm,&x,&y);
    D3DXVec3Normalize(&norm,&norm);
  }

  D3DXVECTOR3 point(float u,float v)
  {
    return (u * x + v * y);
  }

  D3DXVECTOR3 normalAtPoint(float u,float v)
  {
    return norm;
  }
private:
  D3DXVECTOR3 x,y,norm;
  float n,m;
};

//Plane points generator class


#endif