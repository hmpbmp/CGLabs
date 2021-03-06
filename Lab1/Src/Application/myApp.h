#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
@file     cglapp.h
@brief    Class cglApp definition
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/

// *******************************************************************
// includes

#include "Library/cglApp.h"
#include "Library/cglD3D.h"
#include "objectGenerator.h"

// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

// Application class
class myApp : public cglApp
{
public:
  // Constructor
  myApp(int nW, int nH, void* hInst, int nCmdShow);
  // Destructor
  virtual ~myApp() {}
  // This function performs input processing. Returns true if input is handled
  virtual bool processInput(unsigned int nMsg, int wParam, long lParam);
protected:
  virtual void update();
  virtual wchar_t const *getWindowText() { return L"First CG lab by Kozyrev."; };
  virtual void renderInternal();
private:
  enum
  {
    MAX_KEYS = 128
  };
  unsigned int m_nClearColor;
  int   m_nPrevMouseX;
  int   m_nPrevMouseY;
  bool  m_keysPressed[MAX_KEYS];

  void rotateObject(float dx, float dy);
  void rotateCamera(float dx, float dy);
  void zoom(float dr);
  objectGenerator generator;


};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
