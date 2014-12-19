/**
@file     effect.cpp
@brief    Class effect methods
@date     Created on 14/12/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include "effect.h"

// *******************************************************************
// defines

//Constructor
Effect::Effect(char *filename, LPDIRECT3DDEVICE9 pDevice) :name(filename), pDev(pDevice)
{
  size_t size = strlen(name) + 1;
  wchar_t *wName = new wchar_t[size];
  size_t convChars = 0;
  mbstowcs_s(&convChars, wName, size, name, _TRUNCATE);
  D3DXCreateEffectFromFile(pDev, wName, 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors);
  delete[] wName;
  if (errors)
  {
    size_t size = strlen((char*)errors->GetBufferPointer()) + 1;
    wchar_t *wErrName = new wchar_t[size];
    size_t convChars = 0;
    mbstowcs_s(&convChars, wErrName, size, (char*)errors->GetBufferPointer(), _TRUNCATE);
    MessageBox(0, wErrName, 0, 0);
    delete[] wErrName;
    return;
  }
  else
  {
    valid = true;
  }
}


//Destructor
Effect::~Effect()
{
  if (valid)
  {
    mFX->Release();
  }
}