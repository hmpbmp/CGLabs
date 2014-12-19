#include "myMesh.h"
#include <cstdlib>

//Loading mesh from file
void MyMesh::load(char *filename, IDirect3DDevice9 *device)
{
  ID3DXBuffer *matBuff = NULL;
  size_t size = strlen(filename) + 1;
  wchar_t *wFileName = new wchar_t[size];
  const char *bmp = "_bumpmap.dds";
  size_t convChars = 0;
  mbstowcs_s(&convChars,wFileName,size,filename,_TRUNCATE);
  meshLoadResult = D3DXLoadMeshFromX(wFileName, 0, device, NULL, &matBuff, NULL, &matNum, &mesh);
  delete[] wFileName;
  if (meshLoadResult == S_OK)
  {
    D3DXMATERIAL *D3DXMat = (D3DXMATERIAL *)matBuff->GetBufferPointer();
    materials = new D3DMATERIAL9[matNum];
    textures = new LPDIRECT3DTEXTURE9[matNum];
    bumpmaps = new LPDIRECT3DTEXTURE9[matNum];
    for (DWORD i = 0; i < matNum; i++)
    {
      materials[i] = D3DXMat[i].MatD3D;
      materials[i].Ambient = materials[i].Diffuse;
      if (D3DXMat[i].pTextureFilename)
      {
        size_t size = strlen(D3DXMat[i].pTextureFilename) + 1;
        wchar_t *wFileName = new wchar_t[size];
        size_t convChars = 0;
        mbstowcs_s(&convChars, wFileName, size, D3DXMat[i].pTextureFilename, _TRUNCATE);
        D3DXCreateTextureFromFile(device, wFileName, &textures[i]);
        delete[] wFileName;
        char *partName = new char[size + 8];
        unsigned j;
        for (j = 0; j < size - 5; j++)
        {
          partName[j] = D3DXMat[i].pTextureFilename[j];
        }
        for (j = size - 5; j < size + 8; j++)
        {
          partName[j] = bmp[j - size + 5];
        }
        size = strlen(partName) + 1;
        wchar_t *wbmpName = new wchar_t[size];
        convChars = 0;
        mbstowcs_s(&convChars, wbmpName, size, partName, _TRUNCATE);
        D3DXCreateTextureFromFile(device, wbmpName, &bumpmaps[i]);
        delete[] wbmpName;
        delete[] partName;
      }
      //D3DXComputeTangentFrame(mesh, NULL);
    }

    if (matBuff) { matBuff->Release(); }

    mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
  }
  
}

//Rendering
void MyMesh::render(IDirect3DDevice9 *device)
{
  if (meshLoadResult == S_OK)
  {
    for (DWORD i = 0; i < matNum; i++)
    {
      device->SetMaterial(&(materials[i]));
      device->SetTexture(0, textures[i]);
      mesh->DrawSubset(i);
    }
  }
}

void MyMesh::renderWithEffect(IDirect3DDevice9 *device, ID3DXEffect *eff, D3DXHANDLE hText, D3DXHANDLE hMat , D3DXHANDLE hBump)
{
  if (meshLoadResult == S_OK)
  {
    for (DWORD i = 0; i < matNum; i++)
    {
      //set text and mat by handler
      //device->SetMaterial(&(materials[i]));
      //device->SetTexture(0, textures[i]);
      eff->SetTexture(hText, textures[i]);
      eff->SetTexture(hBump, bumpmaps[i]);
      eff->SetFloatArray(hMat, (float*)D3DXCOLOR(materials[i].Diffuse),4);
      eff->CommitChanges();
      mesh->DrawSubset(i);
    }
  }
}

//Release
void MyMesh::release()
{
  if (meshLoadResult == S_OK)
  {
    mesh->Release();
    delete[] materials;
    delete[] textures;
  }
}