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



class Figure
{
public:
  virtual D3DXVECTOR3 point(float u, float v) = 0 {};
  virtual D3DXVECTOR3 normalAtPoint(float u, float v) = 0 {};
};



//Torus points generator class
class Torus : public Figure
{
public:
  //constructor
  Torus(float smallR = 1.0f, float bigR = 2.0f) :r(smallR), R(bigR) {}
  //Converts from parameters to point in 3D
  D3DXVECTOR3 point(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    return (D3DXVECTOR3 ((R + r * cos(n)) * sin(m), (R + r * cos(n)) * cos(m), r * sin(n)));
  }

  D3DXVECTOR3 normalAtPoint(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    return (D3DXVECTOR3 (sin(n)*cos(m), sin(m), cos(n)*cos(m)));
  }

private:
  float r, R;
};

//Plane points generator class
class Plane : public Figure
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


//Sphere points generator class
class Sphere : public Figure
{
public:
  Sphere(float radius = 1.0f) :R(radius) {};

  D3DXVECTOR3 point(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    return (D3DXVECTOR3 (R * cos(n) * sin(m), R * sin(n) * sin(m), R * cos(m)));
  }

  D3DXVECTOR3 normalAtPoint(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    return (D3DXVECTOR3 (cos(n) * sin(m), sin(n) * sin(m), cos(m)));
  }
private:
  float R;
};

class Cylinder : public Figure
{
public:
  Cylinder(float radius = 0.5f, float height = 1.0f) :R(radius), H(height) {};

  D3DXVECTOR3 point(float u, float v)
  {
    float n = 2 * PI_F * u;
    return (D3DXVECTOR3(R * cos(n), R * sin(n), H * (v - 0.5f)));
  }

  D3DXVECTOR3 normalAtPoint(float u, float v)
  {
    float n = 2 * PI_F * u;
    D3DXVECTOR3 norm(R * cos(n), R * sin(n), H * (v - 0.5f));
    D3DXVec3Normalize(&norm, &norm);
    return norm;
  }

private:
  float R, H;
};



#endif