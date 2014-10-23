/**
@file     lightGenerator.cpp
@brief    Class lighttGenerator methods
@date     Created on 13/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include "lightGenerator.h"

// *******************************************************************
// defines

//*************************************************************************
//lightGenerator class

LightGenerator::LightGenerator(LPDIRECT3DDEVICE9 device) :pDev(device)
{
  int i;
  
//  pDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));
  pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

  for (i = 0; i < POINTNUM; i++)
  {
    ZeroMemory(&lights[i], sizeof(lights[i]));
    lights[i].Type = D3DLIGHT_POINT;
    lights[i].Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.0f);
    lights[i].Range = 10.0f;
    lightsMode[i] = TRUE;
  }

  lights[0].Position = D3DXVECTOR3(7.0f, 3.0f, 2.0f);
  lights[1].Position = D3DXVECTOR3(3.0f, 7.0f, 2.0f);
  lights[2].Position = D3DXVECTOR3(17.0f, 17.0f, 12.0f);

  i = dirIndex;
  ZeroMemory(&lights[i], sizeof(lights[i]));
  lights[i].Type = D3DLIGHT_DIRECTIONAL;
  lights[i].Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
  lights[i].Direction = D3DXVECTOR3(10.0f, 10.0f, -5.0f);
  lightsMode[i] = TRUE;
  
  i = spotIndex;
  ZeroMemory(&lights[i], sizeof(lights[i]));
  lights[i].Type = D3DLIGHT_SPOT;
  lights[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
  lights[i].Direction = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
  spotStart = D3DXVECTOR3(0.0f, 0.0f, 8.0f);
  lights[i].Position = spotStart;
  phi = atan2f(spotStart.y,spotStart.x);
  lights[i].Range = 10.0f;
  lightsMode[i] = TRUE;

}

void LightGenerator::setLight()
{
  for (int i = 0; i < LIGHTNUM; i++)
  {
    pDev->SetLight(i, &lights[i]);
    pDev->LightEnable(i, lightsMode[i]);
  }
}


void LightGenerator::setSpotLightPosition(D3DXVECTOR3 pos)
{
  lights[spotIndex].Position = pos;
  pDev->SetLight(spotIndex, &lights[spotIndex]);
}

void LightGenerator::switchLightByIndex(int index)
{
  lightsMode[index] = !lightsMode[index];
  pDev->LightEnable(index, lightsMode[index]);
}




void LightGenerator::spotCircleMove()
{
  phi += 0.01f;
  lights[spotIndex].Position = D3DXVECTOR3(center.x + R * cosf(phi) ,center.y + R * sinf(phi) ,center.z);
  lights[spotIndex].Direction = D3DXVECTOR3(center.x + R * cosf(PI_F / 2 + phi), center.y + R * sinf( PI_F / 2 + phi), center.z);
  pDev->SetLight(spotIndex, &lights[spotIndex]);
}
