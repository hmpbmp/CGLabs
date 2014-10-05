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

//Point or vector in 3D class
class cartesian
{
public:
  cartesian() :x(0.0f), y(0.0f), z(0.0f){}
  cartesian(float X, float Y, float Z) :x(X), y(Y), z(Z) {}
  //Gets norm of vector multiplied by int
  cartesian normalized(int scale = 1)
  {
    float k = 1.0f / sqrt(x * x + y * y + z * z);
    return cartesian(scale * x * k, scale * y * k, scale * z * k);
  }

  float x, y, z;
};


//Vertex class
struct vertex
{
  cartesian p;
  DWORD color;
};

//Torus points generator class
class Torus
{
public:
  //constructor
  Torus(float smallR = 1.0f, float bigR = 2.0f) :r(smallR), R(bigR) {}
  //Converts from parameters to point in 3D
  cartesian point(float u, float v)
  {
    float n = 2 * PI_F * u, m = 2 * PI_F * v;
    cartesian p((R + r * cos(n)) * sin(m), (R + r * cos(n)) * cos(m), r * sin(n));
    return p;
  }
private:
  float r, R;
};

#endif