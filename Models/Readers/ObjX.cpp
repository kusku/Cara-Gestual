#include "../../stdafx.h"
#include "ObjX.h"
#include "../../Render/CDirectX.h"

CObjX::CObjX()
{
	m_NumMaterials = 0;
	m_MeshTextures = NULL;
	m_MeshMaterials = NULL;
	m_Mesh = NULL;
}

CObjX::~CObjX()
{
	delete [] m_MaterialMeshList;
}

void CObjX::LoadModel(std::string filename)
{
	LPD3DXBUFFER materialBuffer;
	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();

	HRESULT hr = D3DXLoadMeshFromX(	filename.c_str(),D3DXMESH_SYSTEMMEM, Device, NULL,
									&materialBuffer, NULL, &m_NumMaterials, &m_Mesh);

	if (hr == D3D_OK)
	{
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

		m_MeshMaterials = new D3DMATERIAL9[m_NumMaterials];
		m_MeshTextures = new LPDIRECT3DTEXTURE9[m_NumMaterials];

		for (DWORD i=0; i<m_NumMaterials; ++i)
		{
			// Copy the material
			m_MeshMaterials[i] = d3dxMaterials[i].MatD3D;

			// Set the ambient color for the material (D3DX does not do this)
			m_MeshMaterials[i].Ambient = m_MeshMaterials[i].Diffuse;
			  
			// Create the texture if it exists - it may not
			m_MeshTextures[i] = NULL;
			if (d3dxMaterials[i].pTextureFilename)
			{
				m_TextureName = d3dxMaterials[i].pTextureFilename;
				m_TextureName = CreateTexturePath(filename, m_TextureName);
				hr = D3DXCreateTextureFromFile(Device, m_TextureName.c_str(), &m_MeshTextures[i]);
				if (hr == D3DXERR_INVALIDDATA)
					return;
			}

		}
		ParserMaterialMeshList(filename);
		materialBuffer->Release();
	}
}

std::string CObjX::CreateTexturePath(const std::string filename, std::string texturename)
{
	std::string file = filename;
	int pos = (int) filename.rfind("/");
	
	file.replace(pos,file.length(),"/" + texturename);
	
	return file;
}

void CObjX::ParserMaterialMeshList(std::string filename)
{
	int numTextures;
	int numFaces;
	errno_t err;
	FILE* fitxer;
	char* seq;

	err  = fopen_s(&fitxer,filename.c_str(),"r");
	//TODO: Buscar la seqüència de carècters MeshMaterialList mtls i després llegir els valors
	while (!feof(fitxer))
	{
		fscanf_s(fitxer, "%s", &seq);
		if (strcmp(seq,"MeshMaterialList mtls {") == 0)
		{

		}
	}

}