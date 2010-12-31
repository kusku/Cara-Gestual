#include "../../stdafx.h"
#include "Scenary.h"
#include "../Readers/ObjX.h"

Scenary::Scenary(const std::string name, const std::string path)
{
	m_Name = name;
	m_Path = path;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	LoadModelX();
}

Scenary::~Scenary()
{
}

void Scenary::Render(LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 pan)
{
	D3DXVECTOR3 pos = m_Position + pan;
	D3DXMATRIX l_Matrix, l_MatrixT;
	D3DXMatrixIdentity(&l_Matrix);
	D3DXMatrixIdentity(&l_MatrixT);
	D3DXMatrixScaling (&l_MatrixT, m_Scale.x, m_Scale.y, m_Scale.z);
	l_Matrix *= l_MatrixT;
	D3DXMatrixRotationYawPitchRoll(&l_MatrixT, DEG2RAD(m_Rotation.y), DEG2RAD(m_Rotation.x), DEG2RAD(m_Rotation.z));
	l_Matrix *= l_MatrixT;
	D3DXMatrixTranslation (&l_MatrixT, pos.x, pos.y, pos.z);
	l_Matrix *= l_MatrixT;
	Device->SetTransform(D3DTS_WORLD, &l_Matrix);

	for (int i=0; i<m_NumMaterials; ++i)
	{
		// Set the material and texture for this subset
		Device->SetMaterial(&m_Materials[i]);
		Device->SetTexture(0,m_Textures[i]);
	     
		// Draw the mesh subset
		m_Mesh->DrawSubset(i);
	}

	Device->SetTexture (0, NULL);
}

void Scenary::LoadModelX()
{
	CObjX* reader = new CObjX();
	reader->LoadModel(m_Path);

	m_NumMaterials = reader->GetNumMaterials();
	m_Mesh = reader->GetMesh();
	m_Materials = reader->GetMaterials();
	m_Textures = reader->GetTextures();
	

	delete reader;
}