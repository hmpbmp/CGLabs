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
  //Creation of light sources: 1 point,1 directional, 0 spot
  //For each source creates color , position(if necessary) , direction(if necessary) and range 
  pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
 
  i = 0;
  ZeroMemory(&lights[i], sizeof(lights[i]));
  lights[i].Type = D3DLIGHT_SPOT;
  lights[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
  lights[i].Range = 7.0f;
  lights[i].Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  lightsMode[i] = TRUE;

  lights[0].Position = D3DXVECTOR3(2.0f, 2.0f, 7.0f);
  
  
  /*i = dirIndex;
  ZeroMemory(&lights[i], sizeof(lights[i]));
  lights[i].Type = D3DLIGHT_DIRECTIONAL;
  lights[i].Diffuse = D3DXCOLOR(0.7f, 0.7f,0.2f, 1.0f);
  lights[i].Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
  lights[i].Range = 15.0f;
  lightsMode[i] = TRUE;*/
  
  /*i = spotIndex;
  ZeroMemory(&lights[i], sizeof(lights[i]));
  lights[i].Type = D3DLIGHT_SPOT;
  lights[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
  lights[i].Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
  spotStart = D3DXVECTOR3(21.0f, 2.5f, 0.0f);
  lights[i].Position = spotStart;
  lights[i].Range = 10.0f;
  lightsMode[i] = TRUE;*/

  //D3DXMatrixRotationZ(&spin, 0.01f);
}

void LightGenerator::setLight()
{
  //Enable all lights
  for (int i = 0; i < LIGHTNUM; i++)
  {
    pDev->SetLight(i, &lights[i]);
    pDev->LightEnable(i, lightsMode[i]);
  }
}

//Sets position of spot light
void LightGenerator::setPointLightPosition(D3DXVECTOR3 pos)
{

  lights[0].Position = pos;
  pDev->SetLight(0, &lights[0]);
}

//Switches light with number 'index'
void LightGenerator::switchLightByIndex(int index)
{
  lightsMode[index] = !lightsMode[index];
  pDev->LightEnable(index, lightsMode[index]);
}



//Step of spot light circle move
void LightGenerator::spotCircleMove()
{
  D3DXVECTOR3 V = lights[spotIndex].Position;
  D3DXVec3TransformCoord(&V, &V, &spin);
  lights[spotIndex].Position = V;
  V = lights[spotIndex].Direction;
  D3DXVec3TransformCoord(&V, &V, &spin);
  lights[spotIndex].Direction = V;
  pDev->SetLight(spotIndex, &lights[spotIndex]);
  pDev->LightEnable(spotIndex, lightsMode[spotIndex]);
}
