#include "../../stdafx.h"
#include "ObjX.h"
#include "../../Render/CDirectX.h"

CObjX::CObjX()
{
	m_NumMaterials = 0;
	m_MeshTextures = NULL;
	m_MeshMaterials = NULL;
	m_Mesh = NULL;
	m_NumVertices = 0;
	m_NumFaces = 0;
}

CObjX::~CObjX()
{
}

void CObjX::LoadModel(std::string filename)
{
	std::string nomTextura;
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
				nomTextura = d3dxMaterials[i].pTextureFilename;
				nomTextura = CreateTexturePath(filename, nomTextura);
				m_TextureName.push_back(nomTextura);
				hr = D3DXCreateTextureFromFile(Device, nomTextura.c_str(), &m_MeshTextures[i]);
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
	int numTextures = 0;
	int numFaces = 0;
	errno_t err;
	FILE* fitxer;
	char seq[200];
	std::string MessageToCompare = "MeshMaterialList mtls {";
	std::string MessageToRead;

	err  = fopen_s(&fitxer,filename.c_str(),"r");
	if (err != 0)
		return;

	while (!feof(fitxer))
	{
		fgets(seq,200,fitxer);
		MessageToRead = seq;
		//Mira si la línia del fitxer llegida és la de MessageToCompare.
		//Si ho és, es comença a llegir les següents línies
		if (MessageToRead.find(MessageToCompare) != -1)
		{
			fscanf_s(fitxer,"%d;\n",&numTextures);
			fscanf_s(fitxer,"%d;\n",&numFaces);

			//Llegeix tots els valors d'identificador de textura per a cada cara
			int valor = 0;
			for (int i=0; i<numFaces-1;++i)
			{
				fscanf_s(fitxer,"%d,\n",&valor);
				m_MaterialMeshList.push_back(valor);
			}
			fscanf_s(fitxer,"%d;\n",&valor);
			m_MaterialMeshList.push_back(valor);
		}
	}
	fclose(fitxer);
}