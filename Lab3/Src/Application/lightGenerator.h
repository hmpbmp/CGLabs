#ifndef LIGTHGEN_H_
#define LIGHTGEN_H_

/**
@file     lightGenerator.h
@brief    Class objectGenerator definition
@date     Created on 13/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>

#define LIGHTNUM 5
#define POINTNUM 3

class LightGenerator
{
public:
  LightGenerator(LPDIRECT3DDEVICE9 device);
  void setLight();
  void setSpotLightPosition(D3DXVECTOR3 pos);
  void switchLightByIndex(int index);
  void spotCircleMove();

private:
  D3DLIGHT9 lights[LIGHTNUM];
  int lightsMode[LIGHTNUM];
  int sign = 1;
  D3DXVECTOR3 spotStart;
  D3DXMATRIX spin;

  IDirect3DDevice9 *pDev;
  const int spotIndex = LIGHTNUM - 1;
  const int dirIndex = LIGHTNUM - 2;

};

#endif