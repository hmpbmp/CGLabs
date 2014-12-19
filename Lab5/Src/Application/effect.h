#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

/**
@file     effect.h
@brief    Class effect definition
@date     Created on 14/12/2014
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

//Class that contains shader effects

class Effect
{
public:
  Effect(char *filename, LPDIRECT3DDEVICE9 pDevice);
  ~Effect();
  bool isValid() { return valid; }
  ID3DXEffect* D3DXPointer() { return mFX; }
  void setLightType(bool type) { lightType = type; }
  bool type() { return lightType; }
private:
  LPDIRECT3DDEVICE9 pDev;
  char *name;

  bool valid = false;
  ID3DXEffect* mFX = 0;
  ID3DXBuffer* errors = 0;
  bool lightType = false;
};

#endif