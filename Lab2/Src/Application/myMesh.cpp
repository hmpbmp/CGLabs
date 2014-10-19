#include "myMesh.h"
#include <cstdlib>

//Loading mesh from file
void MyMesh::load(char *filename, IDirect3DDevice9 *device)
{
  ID3DXBuffer *matBuff = NULL;
  size_t size = strlen(filename) + 1;
  wchar_t *wFileName = new wchar_t[size];
  size_t convChars = 0;
  mbstowcs_s(&convChars,wFileName,size,filename,_TRUNCATE);
  HRESULT hr = D3DXLoadMeshFromX(wFileName, 0, device, NULL, &matBuff, NULL, &matNum, &mesh);
  
  int r = mesh->GetFVF() & D3DFVF_NORMAL;

  D3DXMATERIAL *D3DXMat = (D3DXMATERIAL *)matBuff->GetBufferPointer();
  materials = new D3DMATERIAL9[matNum];
  for (DWORD i = 0; i < matNum; i++)
  {
    materials[i] = D3DXMat[i].MatD3D;
    materials[i].Ambient = materials[i].Diffuse;
  }

  if (matBuff) { matBuff->Release(); }

  mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
  delete[] wFileName;
}

//Rendering
void MyMesh::render(IDirect3DDevice9 *device)
{
  for (DWORD i = 0; i < matNum; i++)
  {
    device->SetMaterial(&(materials[i]));
    mesh->DrawSubset(i);
  }
}

//Release
void MyMesh::release()
{
  mesh->Release();
  delete[] materials;
}