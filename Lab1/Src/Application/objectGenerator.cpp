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
#include "transform.h"

#include <cmath>
// *******************************************************************
// defines

//Constructor
objectGenerator::objectGenerator(LPDIRECT3DDEVICE9 pDevice, int squaresM, int squaresN) :pDev(pDevice), M(squaresM), N(squaresN)
{
  vertex *tempVertexBuffer;
  int *tempIndexBuffer;

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
    Torus T;
    float iScale = 1.0f / M;
    float jScale = 1.0f / N;
    for (int i = 0; i < M + 1; i++)
    {
      for (int j = 0; j < N + 1; j++)
      {
        tempVertexBuffer[i * (N + 1) + j].p = T.point(i * iScale, j * jScale);
        tempVertexBuffer[i * (N + 1) + j].color = colorFromPoint(tempVertexBuffer[i * (N + 1) + j].p);
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

  //Setting default world,view,proj matrices
  D3DXMatrixTranslation(Trans.worldMatrix(), 4.0f, 4.0f, 0.0f);
  Trans.setCameraPosition(D3DXVECTOR3(4.0f, 4.0f, 7.0f));
  Trans.setCameraVectors(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));
  Trans.setProjection(2 * atan(16.0f / 9.0f), PI_F / 2, 1.0f, 1000.0f);

}

//Destructor
objectGenerator::~objectGenerator()
{
  pVertBuff->Release();
  pIndBuff->Release();
}
//Color from coordinates
DWORD objectGenerator::colorFromPoint(cartesian point)
{
  cartesian norm = point.normalized(255);
  return D3DCOLOR_XRGB((int)norm.x, (int)norm.y, (int)norm.z);
}

//Rendering
void objectGenerator::render()
{
  //Applying current transforms
  pDev->SetTransform(D3DTS_WORLD, Trans.worldMatrix());
  pDev->SetTransform(D3DTS_PROJECTION, Trans.projectionMatrix());
  pDev->SetTransform(D3DTS_VIEW, Trans.viewMatrix());

  //Setting rendering options and drawing
  pDev->SetRenderState(D3DRS_ZENABLE, true);
  pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pDev->SetRenderState(D3DRS_LIGHTING, false);
  pDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  pDev->SetIndices(pIndBuff);
  pDev->SetStreamSource(0, pVertBuff, 0, sizeof(vertex));
  
  pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertNum, 0, triNum);
}

//Rotate object using keyboard
void objectGenerator::rotateObject(float dx, float dy)
{
  D3DXMATRIX Rx, Ry;
  D3DXMatrixRotationX(&Rx, dx);
  D3DXMatrixRotationY(&Ry, dy);
  *Trans.worldMatrix() = Rx * Ry * (*Trans.worldMatrix());
}

//Rotate camera using mouse
void objectGenerator::rotateCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    //Rotating around up vector
    //Creating transform matrix
    D3DXMATRIX Rz;
    D3DXMatrixRotationAxis(&Rz,Trans.upVector(),dx);
    //Updating lookat and up vectors and position of camera

    D3DXVec3TransformCoord(Trans.dirVector(), Trans.dirVector(), &Rz);

    D3DXVec3TransformCoord(Trans.upVector(), Trans.upVector(), &Rz);

    D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
    float norm = D3DXVec3Length(&diff);
    *Trans.cameraPos() = *Trans.lookatVector() - *Trans.dirVector() * norm;

    D3DXVec3Normalize(Trans.dirVector(), Trans.dirVector());
    D3DXVec3Cross(Trans.rightVector(), Trans.dirVector(), Trans.upVector());
    D3DXVec3Normalize(Trans.rightVector(), Trans.rightVector());
    D3DXVec3Cross(Trans.upVector(), Trans.rightVector(), Trans.dirVector());

  }
  if (fabs(dy) > 0.001)
  {
    //Rotating around right vector
    //Creating transform matrix
    D3DXMATRIX Rz;
    D3DXMatrixRotationAxis(&Rz, Trans.rightVector(), -dy);
    //Updating lookat and up vectors and position of camera

    D3DXVec3TransformCoord(Trans.dirVector(), Trans.dirVector(), &Rz);

    D3DXVec3TransformCoord(Trans.rightVector(), Trans.rightVector(), &Rz);
	
    D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
    float norm = D3DXVec3Length(&diff);
    *Trans.cameraPos() = *Trans.lookatVector() - *Trans.dirVector() * norm;
  
    D3DXVec3Normalize(Trans.dirVector(), Trans.dirVector());
    D3DXVec3Cross(Trans.rightVector(), Trans.dirVector(), Trans.upVector());
    D3DXVec3Normalize(Trans.rightVector(), Trans.rightVector());
    D3DXVec3Cross(Trans.upVector(), Trans.rightVector(), Trans.dirVector());

  }
}

void objectGenerator::zoom(float dr)
{
  *Trans.cameraPos() = *(Trans.cameraPos()) - *(Trans.cameraPos()) * dr;
}