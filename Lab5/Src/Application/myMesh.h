#ifndef MY_MESH_INCLUDED
#define MY_MESH_INCLUDED

/**
@file     myMesh.h
@brief    User-defined mesh class definition
@date     Created on 13/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include <stdlib.h>

// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

class MyMesh
{
public:
  //Loading mesh from file
  void load(char *filename, IDirect3DDevice9 *device);
  //Rendering
  void render(IDirect3DDevice9 *device);
  //Release mesh and materials
  void release();
private:
  ID3DXMesh *mesh;
  D3DMATERIAL9 *materials;
  DWORD matNum = 0;
  HRESULT meshLoadResult;
};



#endif