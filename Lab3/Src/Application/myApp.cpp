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

  char * mipmapStraw[5][16] = { "straw32.jpg", "straw64.jpg", "straw128.jpg", "straw256.jpg", "straw512.jpg" };
}

// *******************************************************************
// Methods

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
: cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
, generator(m_pD3D->getDevice(), 100, 100)
, air(m_pD3D->getDevice(), "airplane00.x", D3DXVECTOR3(5.0f, 0.0f, 4.0f))
, camera(m_pD3D->getDevice())
, light(m_pD3D->getDevice())
{
  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;

  camera.setCameraPosition(D3DXVECTOR3(10.0f, 10.0f, 7.0f));
  camera.setCameraVectors(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));
  camera.setProjection(2 * atan(16.0f / 9.0f), PI_F / 2, 1.0f, 1000.0f);


  light.setLight();
  generator.LoadTexture("straw512.jpg",(char**)mipmapStraw,mipmapNumber);
  air.LoadTexture("zebra.jpg",NULL,0);

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
                     light.switchLightByIndex(3);
                     break;
                   case '2':
                     light.switchLightByIndex(0);
                     light.switchLightByIndex(1);
                     light.switchLightByIndex(2);
                     break;
                   case '3':
                     light.switchLightByIndex(4);
                     break;
                   case 'M':
                     *generator.TexMip() = *generator.TexMip() > 0 ? (*generator.TexMip() - 1) : 2;
                     *air.TexMip() = *air.TexMip() > 0 ? (*air.TexMip() - 1) : 2;
                     break;
                   case 'F':
                     *generator.TexMin() = 3 - *generator.TexMin();
                     *air.TexMin() = 3 - *air.TexMin();
                     break;
                   case 'G': 
                     *generator.TexMag() = 3 - *generator.TexMag();
                     *air.TexMag() = 3 - *air.TexMag();
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
  camera.rotateObject(&generator,dx, dy);

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

  if (m_keysPressed[VK_SHIFT])
  {
    ;
  }

  if (m_keysPressed[VK_OEM_PLUS] && m_keysPressed[VK_SHIFT])
  {
    *generator.TexBias() += 0.2f;
    *air.TexBias() += 0.2f;
  }

  if (m_keysPressed[VK_OEM_MINUS] && m_keysPressed[VK_SHIFT])
  {
    *generator.TexBias() -= 0.2f;
    *air.TexBias() -= 0.2f;
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

  
  //Object rendering
  air.circleMove();
  light.spotCircleMove();
  air.render(&camera);
  generator.render(&camera);

}