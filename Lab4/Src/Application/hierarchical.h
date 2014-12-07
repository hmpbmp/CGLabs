#ifndef HIERARCHICAL_H
#define HIERARCHICAL_H
/**
@file     hierarchical.h
@brief    Class Node definition
@date     Created on 03/12/2014
@project  D3DBase
@author   Semyon Kozyrev
*/

// *******************************************************************
// includes

#include "basic.h"
#include "objectGenerator.h"
#include "lightGenerator.h"
#include <vector>

// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

class Node
{
public:

  //Constructor
  Node(ObjectGenerator *generator) :geometry(generator)
  {
    if (generator != NULL)
    {
      matrix = *(generator->worldMatrix());
    }
  }

  //Adds child to list
  void addChild(Node * ch) { childrenList.push_back(ch); }
  //Sets local matrix
  void setMatrix(D3DXMATRIX M) { matrix = M; }
  //Recursively update world and local matricies
  void updateMatrix(D3DXMATRIX parentM,D3DXMATRIX *rot) 
  { 
    D3DXMATRIX world;
    D3DXMatrixMultiply(&matrix, &matrix, rot);
    D3DXMatrixMultiply(&world, &matrix, &parentM);
    geometry->setWorldMatrix(world);
    if (light != NULL)
    {
      light->setPointLightPosition(D3DXVECTOR3(world(3, 0), world(3, 1), world(3, 2)));
    }
    for (Node *ch : childrenList)
    {
      ch->updateMatrix(world,rot);
    }
  }

  //Renders object
  void render(Camera *cam) 
  { 
    if (geometry != NULL) { geometry->render(cam); }
    for (Node *ch : childrenList) 
      ch->render(cam); 
  }
  //Children list
  std::vector<Node *> children() { return childrenList; }

  //Connect object and light
  void connectToLight(LightGenerator *l) { light = l; }

  void recDelete() 
  {
    if (geometry != NULL)
    {
      delete geometry;
    }
    for (Node *ch :childrenList)
    {
      ch->recDelete();
      delete ch;
    }
  }
private:
  D3DXMATRIX matrix;
  ObjectGenerator *geometry;
  std::vector<Node *> childrenList;
  LightGenerator *light = NULL;

};

#endif