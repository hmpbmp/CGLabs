/**
@file     myApp.cpp
@brief    User's application class
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/

// *******************************************************************
// includes

#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>


#include "myApp.h"
#include "Library/cglApp.h"

// *******************************************************************
// defines

namespace
{

  const float s_rMouseMove2Rotate = 0.001f;
  const float s_rMouseWheel2Zoom = 0.001f;
  const float s_rKbd2Rotate = 1.16f;
  const float s_rKbd2Zoom = 1.16f;


  void shiftColor(unsigned &color, float delta, int shift)
  {
    int nComp = (color >> shift) & 0xFF;
    nComp += int(delta * 255.0f);
    if (nComp > 0xFF)
      nComp = 0xFF;
    else if (nComp < 0)
      nComp = 0;
    color &= ~(0xFF << shift);
    color |= nComp << shift;
  }

  const int mipmapNumber = 5;
  const int maxRotationNumber = 50;
  const float rotationAngle = 0.003f;
  char * mipmapStraw[5][16] = { "straw32.jpg", "straw64.jpg", "straw128.jpg", "straw256.jpg", "straw512.jpg" };
}

// *******************************************************************
// Methods

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
: cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
, camera(m_pD3D->getDevice())
, light(m_pD3D->getDevice())
, mesh(m_pD3D->getDevice(), "Dwarf.x")
{

  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;

  D3DXMATRIX M1,M2;
  D3DXMatrixRotationX(&M1, PI_F / 2);
  D3DXMatrixRotationZ(&M2, -PI_F / 4);
  D3DXMatrixMultiply(&M1,&M1,&M2);
  mesh.setWorldMatrix(M1);
  light.setLight();

  camera.setCameraPosition(D3DXVECTOR3(1.50f, 1.50f, 2.0f));
  camera.setCameraVectors(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));
  camera.setProjection(2 * atan(16.0f / 9.0f), PI_F / 2, 1.0f, 1000.0f);


  D3DXCreateEffectFromFile(m_pD3D->getDevice(), L"shader.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors);
  if (errors)
  {
    size_t size = strlen((char*)errors->GetBufferPointer()) + 1;
    wchar_t *wErrName = new wchar_t[size];
    size_t convChars = 0;
    mbstowcs_s(&convChars, wErrName, size, (char*)errors->GetBufferPointer(), _TRUNCATE);
    MessageBox(0, wErrName, 0, 0);
  }

  hWorldMatrix = mFX->GetParameterByName(0, "World");
  hViewMatrix  = mFX->GetParameterByName(0, "View");
  hProjMatrix  = mFX->GetParameterByName(0, "Proj");
  hCameraPos   = mFX->GetParameterByName(0, "cameraPos");
  hLightPos    = mFX->GetParameterByName(0, "lightPos[0]");
  hLightColor  = mFX->GetParameterByName(0, "lightColor[0]");

  hMinnaert  = mFX->GetTechniqueByName("Minnaert");
  hDiffuse   = mFX->GetTechniqueByName("Diffuse");
  // Set Parameters
}


myApp::~myApp()
{
  mFX->Release();
}

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
  // Handle input
  switch (nMsg)
  {
    // Process mouse move message
  case WM_MOUSEMOVE:
  {
                     // Get mouse x & y (to the upper-left corner of the client area)
                     int xPos = GET_X_LPARAM(lParam);
                     int yPos = GET_Y_LPARAM(lParam);

                     if (freeCamera && (wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0) // 
                     {
                         rotateCamera(s_rMouseMove2Rotate*(xPos - m_nPrevMouseX),
                         s_rMouseMove2Rotate*(yPos - m_nPrevMouseY));
                     }

                     if ((!freeCamera) && (wParam & MK_LBUTTON) != 0 && (m_nPrevMouseX >= 0))
                     {
                         rotateWASDCamera(s_rMouseMove2Rotate*(xPos - m_nPrevMouseX),
                         s_rMouseMove2Rotate*(yPos - m_nPrevMouseY));        
                     }

                     m_nPrevMouseX = xPos;
                     m_nPrevMouseY = yPos;
                     break;
  }

    // Process mouse wheel message
  case WM_MOUSEWHEEL:
  {
                      int zDelta = (int)((signed short)(HIWORD(wParam)));
                      zoom(zDelta * s_rMouseWheel2Zoom);
                      break;
  }

  case WM_KEYDOWN:
  {
                   // Save flags for continuous controls
                   if (wParam < MAX_KEYS)
                     m_keysPressed[wParam] = true;
                   break;
  }

  case WM_KEYUP:
  {
                 // Save flags for continuous controls
                 if (wParam < MAX_KEYS)
                   m_keysPressed[wParam] = false;
                 // Process switch controls
                 switch (wParam)
                 {
                   case '1':
                     lightType = false;
                     break;
                   case '2':
                     lightType = true;
                     break;
                   case '3':
                     break;
                   case 'M':
                     break;
                   case 'F':
                     break;
                   case 'G': 
                     break;
                   case VK_SPACE:
                     freeCamera = !freeCamera;
                     camera.setCamera(freeCamera);
                     break;
                 }
                 
  }
  }

  return cglApp::processInput(nMsg, wParam, lParam);
}

//Rotate object using keyboard
void myApp::rotateObject(float dx, float dy)
{
  camera.rotateObject(&mesh,dx, dy);

}

//Zoom using wheel
void myApp::zoom(float dr)
{
  camera.zoom(dr);

}

//Rotate camera using mouse
void myApp::rotateCamera(float dx, float dy)
{
  camera.rotateCamera(dx, dy);

}

void myApp::moveWASDCamera(float dx, float dy)
{
  camera.moveWASDCamera(dx, dy);
}


void myApp::rotateWASDCamera(float dx,float dy)
{
  camera.rotateWASDCamera(dx, dy);
}


//Update frame
void myApp::update()
{
  // Call predecessor update
  cglApp::update();

  // Process keyboard
  float dx = 0.0f;
  float dy = 0.0f;
  float dr = 0.0f;


  if (m_keysPressed[VK_OEM_PLUS] && m_keysPressed[VK_SHIFT])
  {
//    *plane.TexBias() += 0.2f;
  }

  if (m_keysPressed[VK_OEM_MINUS] && m_keysPressed[VK_SHIFT])
  {
//    *plane.TexBias() -= 0.2f;
  }


  if (freeCamera)
  {
    if (m_keysPressed[VK_LEFT])
      dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])
      dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])
      dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])
      dy += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_SUBTRACT])
      dr -= s_rKbd2Zoom * m_timer.getDelta();
    if (m_keysPressed[VK_ADD])
      dr += s_rKbd2Zoom * m_timer.getDelta();

    rotateObject(dx, dy);
    zoom(dr);
  }
  else
  {
    if (m_keysPressed['A'])
      dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed['D'])
      dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed['W'])
      dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed['S'])
      dy += s_rKbd2Rotate * m_timer.getDelta();
    moveWASDCamera(dx,dy);
  }

  /*
  float rI = 0.0f;
  for (int i = 0; i < 10000000;i++)
  rI += sin(i);*/
}

//Rendering
void myApp::renderInternal()
{
  D3DXMATRIX *M = new D3DXMATRIX;
  m_nClearColor = 0x00000000;
  m_pD3D->clear(m_nClearColor);

  //Coordinate system rendering
  float const axisLength = 20.0f;
  basicvertex const system[6] =
  {
    { D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF },
    { D3DXVECTOR3(axisLength, 0, 0), 0xFFFFFFFF},
    { D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF },
    { D3DXVECTOR3(0, axisLength, 0), 0xFFFFFFFF},
    { D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF },
    { D3DXVECTOR3(0, 0, axisLength), 0xFFFFFFFF},
  };
  m_pD3D->getDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
  m_pD3D->getDevice()->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(M));
  m_pD3D->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  m_pD3D->getDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 3, system, sizeof(basicvertex));
  m_pD3D->getDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
  delete M;

  

  if (lightType)
  {
    mFX->SetTechnique(hMinnaert);
  }
  else
  {
    mFX->SetTechnique(hDiffuse);
  }
  mFX->SetMatrix(hViewMatrix, camera.viewMatrix());
  mFX->SetMatrix(hProjMatrix, camera.projectionMatrix());
  mFX->SetFloatArray(hCameraPos, (float*)camera.cameraPos(),3);
  mFX->SetFloatArray(hLightPos, (float*)&light.lightPosition(),3);
  mFX->SetFloatArray(hLightColor, (float*)(&light.lightColor()),4);
  UINT numPasses = 1;
  mFX->Begin(&numPasses, 0);
  for (UINT i = 0; i < numPasses; i++)
  {
    mFX->BeginPass(i);

    mFX->SetMatrix(hWorldMatrix, mesh.worldMatrix());
    mFX->CommitChanges();
    mesh.render(&camera);
    mFX->EndPass();
  }
  
  mFX->End();
  //Object rendering

}