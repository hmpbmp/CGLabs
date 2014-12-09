/**
@file     objectGenerator.cpp
@brief    Class objectGenerator methods
@date     Created on 03/10/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes
#include "objectGenerator.h"

#include <cmath>
// *******************************************************************
// defines

#define E_PI 3.1415926535897932384626433832795

//*************************************************************************
//ObjectGenerator class

//*************************************************************************************
//TexturedObjectGenerator class

void TexturedObjectGenerator::LoadTexture(char *filename, char **mipmaps, int number)
{
  texture = new LPDIRECT3DTEXTURE9;
  size_t size = strlen(filename) + 1;
  wchar_t *wFileName = new wchar_t[size];
  size_t convChars = 0;
  mbstowcs_s(&convChars, wFileName, size, filename, _TRUNCATE);
  HRESULT hr = D3DXCreateTextureFromFile(pDev, wFileName, texture);
  if (hr == S_OK)
  {
    texturized = 1;
    pDev->SetTexture(0, *texture);

    if (mipmaps != NULL)
    {
      LPDIRECT3DSURFACE9 surf = NULL;
      for (int i = 0; i < number; ++i)
      {
        (*texture)->GetSurfaceLevel(i, &surf);
        size_t size = strlen(mipmaps[number - i - 1]) + 1;
        wchar_t *wFileName = new wchar_t[size];
        size_t convChars = 0;
        mbstowcs_s(&convChars, wFileName, size, mipmaps[number - i - 1], _TRUNCATE);
        D3DXLoadSurfaceFromFile(surf, NULL, NULL, wFileName, NULL, D3DX_DEFAULT, 0, NULL);
        surf->Release();
      }
    }
  }
  else
  {
    delete texture;
    texture = NULL;
  }
}


DWORD *TexturedObjectGenerator::TexMin()
{
  return &minfilter;
}

DWORD *TexturedObjectGenerator::TexMag()
{
  return &magfilter;
}

DWORD *TexturedObjectGenerator::TexMip()
{
  return &mipfilter;
}

float *TexturedObjectGenerator::TexBias()
{
  return &mipmapbias;
}


//*************************************************************************************
//****************************************************************************************
//GeometricObjectGenerator class


//Constructor
GeometricObjectGenerator::GeometricObjectGenerator(LPDIRECT3DDEVICE9 pDevice)
{
  pDev = pDevice;
  D3DXMatrixTranslation(worldMatrix(), 0.0f, 0.0f, 0.0f);

  mipfilter = D3DTEXF_LINEAR;
  magfilter = minfilter = D3DTEXF_LINEAR;
  mipmapbias = 0.0f;

}

void GeometricObjectGenerator::Geometry(int triangleM, int triangleN, Figure *F)
{
  vertex *tempVertexBuffer;
  int *tempIndexBuffer;

  M = triangleM;
  N = triangleN;
  vertNum = (M + 1) * (N + 1);
  triNum = M * N * 2;

  
  //Init and Fill vertex and index buffers
  pDev->CreateVertexBuffer(vertNum * sizeof(vertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pVertBuff, NULL);
  pDev->CreateIndexBuffer(3 * triNum * sizeof(int), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIndBuff, NULL);
  HRESULT hRes1 = pVertBuff->Lock(0, 0, (void **)&tempVertexBuffer, 0);
  HRESULT hRes2 = pIndBuff->Lock(0, 0, (void **)&tempIndexBuffer, 0);



  if ((hRes1 == D3D_OK) && (hRes2 == D3D_OK))
  {
    //Create torus representation in vertex-index buffer
    float iScale = 1.0f / M;
    float jScale = 1.0f / N;
    for (int i = 0; i < M + 1; i++)
    {
      for (int j = 0; j < N + 1; j++)
      {
        tempVertexBuffer[i * (N + 1) + j].p = F->point(i * iScale, j * jScale);
        tempVertexBuffer[i * (N + 1) + j].diffuse = 0xffffffff;//colorFromPoint(tempVertexBuffer[i * (N + 1) + j].p);
        tempVertexBuffer[i * (N + 1) + j].specular = 0;//tempVertexBuffer[i * (N + 1) + j].diffuse;
        tempVertexBuffer[i * (N + 1) + j].normal = F->normalAtPoint(i * iScale, j * jScale);
        tempVertexBuffer[i * (N + 1) + j].tu = (float)i / 20 ;
        tempVertexBuffer[i * (N + 1) + j].tv = (N - (float)j) / 20;
      }
    }
    for (int i = 0; i < M; i++)
    {
      for (int j = 0; j < M; j++)
      {
        //Triangle #1 in square
        tempIndexBuffer[2 * 3 * (i * N + j)] = i * (N + 1) + j;
        tempIndexBuffer[2 * 3 * (i * N + j) + 1] = i * (N + 1) + j + 1;
        tempIndexBuffer[2 * 3 * (i * N + j) + 2] = (i + 1) * (N + 1) + j;

        //Triangle #2 in square
        tempIndexBuffer[2 * 3 * (i * N + j) + 3] = (i + 1) * (N + 1) + j;
        tempIndexBuffer[2 * 3 * (i * N + j) + 4] = i * (N + 1) + j + 1;
        tempIndexBuffer[2 * 3 * (i * N + j) + 5] = (i + 1) * (N + 1) + j + 1;
      }
    }

    pVertBuff->Unlock();
    pIndBuff->Unlock();
  }
}

//Destructor
GeometricObjectGenerator::~GeometricObjectGenerator()
{
  pVertBuff->Release();
  pIndBuff->Release();
}
//Color from coordinates
DWORD GeometricObjectGenerator::colorFromPoint(D3DXVECTOR3 point)
{
  D3DXVECTOR3 norm;
  D3DXVec3Normalize(&norm, &point);
  return D3DCOLOR_COLORVALUE(norm.z, norm.y, norm.x,1.0f);
}

//Rendering
void GeometricObjectGenerator::render(Camera *cam)
{
  //Applying current transforms
  cam->updateCamera(*worldMatrix());


  //Setting rendering options and drawing
  pDev->SetRenderState(D3DRS_ZENABLE, true);
  pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pDev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
  pDev->SetIndices(pIndBuff);
  pDev->SetStreamSource(0, pVertBuff, 0, sizeof(vertex));
  if (texturized)
  {
    pDev->SetTexture(0, *texture);
    pDev->SetSamplerState(0, D3DSAMP_MINFILTER, minfilter);
    pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, magfilter);
    pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, mipfilter);
    pDev->SetSamplerState(0,D3DSAMP_MIPMAPLODBIAS,*(LPDWORD)(&mipmapbias));
    pDev->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
    pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  }
  pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertNum, 0, triNum);
}



//****************************************************************************************
//****************************************************************************************
// MeshObjectGenerator class

MeshObjectGenerator::MeshObjectGenerator(LPDIRECT3DDEVICE9 pDevice, char *filename, D3DXVECTOR3 starting)
{
  pDev = pDevice;
  name = filename;
  mesh.load(filename,pDev);
  start = starting;
  
  D3DXMATRIX M;
  D3DXMatrixTranslation(worldMatrix(), 5.0f, 5.0f, 3.0f);
  D3DXMatrixRotationX(&M,  PI_F / 2);
  D3DXMatrixMultiply(worldMatrix(), &M, worldMatrix());
  D3DXMatrixRotationZ(&M, PI_F);
  D3DXMatrixMultiply(worldMatrix(), &M, worldMatrix());
  D3DXMatrixTranslation(&M,start.x,start.y,start.z);
  D3DXMatrixMultiply(worldMatrix(), worldMatrix(), &M);

  mipfilter = D3DTEXF_NONE;
  magfilter = minfilter = D3DTEXF_POINT;
  mipmapbias = 0.0f;

  D3DXMatrixRotationZ(&spin, 0.01f);
}

MeshObjectGenerator::~MeshObjectGenerator()
{
  mesh.release();
}
//Rendering function
void MeshObjectGenerator::render(Camera *cam)
{
  //Applying current transforms

  cam->updateCamera(*worldMatrix());

  pDev->SetRenderState(D3DRS_ZENABLE, true);
  pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  if (texturized) 
  { 
    pDev->SetTexture(0, *texture);
    pDev->SetSamplerState(0, D3DSAMP_MINFILTER, minfilter);
    pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, magfilter);
    pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, mipfilter);
    pDev->SetSamplerState(0,D3DSAMP_MIPMAPLODBIAS,(DWORD)mipmapbias);
    pDev->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  }
  mesh.render(pDev);
}

//Movement of mesh
void MeshObjectGenerator::circleMove()
{
  phi += 0.01f;
  D3DXMATRIX M;
  D3DXMatrixRotationX(worldMatrix(), PI_F / 2);
  //D3DXVECTOR3 *x = new D3DXVECTOR3(1.0, 0.0, 0.0);
  //float angle = D3DXVec3Dot(x, &start) / D3DXVec3Length(&start) / D3DXVec3Length(x);
  D3DXMatrixRotationZ(&M, PI_F + phi);
  D3DXMatrixMultiply(worldMatrix(), worldMatrix(),&M);
  D3DXVec3TransformCoord(&start,&start,&spin);
  D3DXMatrixTranslation(&M, start.x, start.y, start.z);
  D3DXMatrixMultiply(worldMatrix(), worldMatrix(), &M);
}
