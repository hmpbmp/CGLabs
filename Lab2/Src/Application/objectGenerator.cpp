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

#define E_PI 3.1415926535897932384626433832795

//*************************************************************************
//ObjectGenerator class



//Rotate object using keyboard
void ObjectGenerator::rotateObject(float dx, float dy)
{
  D3DXMATRIX Rx, Ry;
  D3DXMatrixRotationX(&Rx, dx);
  D3DXMatrixRotationY(&Ry, dy);
  *Trans.worldMatrix() = Rx * Ry * (*Trans.worldMatrix());
}

//Rotate camera using mouse
void ObjectGenerator::rotateCamera(float dx, float dy)
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

void ObjectGenerator::zoom(float dr)
{
  *Trans.cameraPos() = *(Trans.cameraPos()) + *(Trans.dirVector()) * dr * 5;
  D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
  D3DXVec3Normalize(Trans.dirVector(),&diff);
}

//Movement for 1st person camera using keyboard
void ObjectGenerator::moveWASDCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    *Trans.cameraPos() += dx * (*Trans.rightVector()) * speedWASD;
    *Trans.lookatVector() += dx * (*Trans.rightVector()) * speedWASD;
  }

  if (fabs(dy) > 0.001)
  {
    *Trans.cameraPos() -= dy * (*Trans.dirVector()) * speedWASD;
    *Trans.lookatVector() -= dy * (*Trans.dirVector()) * speedWASD;
  }
  D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
  D3DXVec3Normalize(Trans.dirVector(),&diff);
}

//Rotation of 1st person camera using mouse
void ObjectGenerator::rotateWASDCamera(float dx, float dy)
{
  if (fabs(dx) > 0.001)
  {
    *Trans.lookatVector() += dx * (*Trans.rightVector()) * 2 *speedWASD;
  }

  if (fabs(dy) > 0.001)
  {
    *Trans.lookatVector() -= dy * (*Trans.upVector()) * 2 *speedWASD;
  }
  D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
  D3DXVec3Normalize(Trans.dirVector(),&diff);
  D3DXVec3Cross(Trans.rightVector(), Trans.dirVector(), Trans.upVector());
  D3DXVec3Normalize(Trans.rightVector(), Trans.rightVector());
  D3DXVec3Cross(Trans.upVector(), Trans.rightVector(), Trans.dirVector());
}

//Change camera mode
void ObjectGenerator::setCamera(bool freeCamera)
{
  if (freeCamera)
    {
      *Trans.lookatVector() = D3DXVECTOR3(0.0f,0.0f,0.0f);
      D3DXVECTOR3 diff = *Trans.lookatVector() - *Trans.cameraPos();
      D3DXVec3Normalize(Trans.dirVector(),&diff);
      D3DXVec3Cross(Trans.rightVector(), Trans.dirVector(), Trans.upVector());
      D3DXVec3Normalize(Trans.rightVector(), Trans.rightVector());
      D3DXVec3Cross(Trans.upVector(), Trans.rightVector(), Trans.dirVector());
    }
}


//*************************************************************************************
//****************************************************************************************
//GeometricObjectGenerator class


//Constructor
GeometricObjectGenerator::GeometricObjectGenerator(LPDIRECT3DDEVICE9 pDevice, int squaresM, int squaresN) : M(squaresM), N(squaresN)
{
  vertex *tempVertexBuffer;
  int *tempIndexBuffer;

  pDev = pDevice;
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
GeometricObjectGenerator::~GeometricObjectGenerator()
{
  pVertBuff->Release();
  pIndBuff->Release();
}
//Color from coordinates
DWORD GeometricObjectGenerator::colorFromPoint(cartesian point)
{
  cartesian norm = point.normalized(255);
  return D3DCOLOR_XRGB((int)norm.x, (int)norm.y, (int)norm.z);
}

//Rendering
void GeometricObjectGenerator::render()
{
  //Applying current transforms
  pDev->SetTransform(D3DTS_WORLD, Trans.worldMatrix());
  pDev->SetTransform(D3DTS_PROJECTION, Trans.projectionMatrix());
  pDev->SetTransform(D3DTS_VIEW, Trans.viewMatrix());

  //Setting rendering options and drawing
  pDev->SetRenderState(D3DRS_ZENABLE, true);
  pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pDev->SetIndices(pIndBuff);
  pDev->SetStreamSource(0, pVertBuff, 0, sizeof(vertex));
  
  pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertNum, 0, triNum);
}


//****************************************************************************************
//****************************************************************************************
// MeshObjectGenerator class

MeshObjectGenerator::MeshObjectGenerator(LPDIRECT3DDEVICE9 pDevice, char *filename)
{
  pDev = pDevice;
  name = filename;
  mesh.load(filename,pDev);
  D3DXMATRIX M;
  D3DXMatrixRotationX(Trans.worldMatrix(),  PI_F / 2);
  D3DXMatrixRotationZ(&M, PI_F);
  D3DXMatrixMultiply(Trans.worldMatrix(), &M, Trans.worldMatrix());
  start = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
  D3DXMatrixTranslation(&M,start.x,start.y,start.z);
  D3DXMatrixMultiply(Trans.worldMatrix(), Trans.worldMatrix(), &M);
  
  Trans.setCameraPosition(D3DXVECTOR3(18.0f, 18.0f, 7.0f));
  Trans.setCameraVectors(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));
  Trans.setProjection(2 * atan(16.0f / 9.0f), PI_F / 2, 1.0f, 1000.0f);

  D3DXMatrixRotationZ(&spin, 0.01f);
}

MeshObjectGenerator::~MeshObjectGenerator()
{
  mesh.release();
}
//Rendering function
void MeshObjectGenerator::render()
{
  //Applying current transforms

  pDev->SetTransform(D3DTS_WORLD, Trans.worldMatrix());
  pDev->SetTransform(D3DTS_PROJECTION, Trans.projectionMatrix());
  pDev->SetTransform(D3DTS_VIEW, Trans.viewMatrix());

  pDev->SetRenderState(D3DRS_ZENABLE, true);
  pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  mesh.render(pDev);
}

//Movement of mesh
void MeshObjectGenerator::circleMove()
{
  phi += 0.01f;
  D3DXMATRIX M;
  D3DXMatrixRotationX(Trans.worldMatrix(), -PI_F / 2);
  D3DXMatrixRotationZ(&M, PI_F + phi);
  D3DXMatrixMultiply(Trans.worldMatrix(), Trans.worldMatrix(),&M);
  D3DXVec3TransformCoord(&start,&start,&spin);
  D3DXMatrixTranslation(&M, start.x, start.y, start.z);
  D3DXMatrixMultiply(Trans.worldMatrix(), Trans.worldMatrix(), &M);
}

