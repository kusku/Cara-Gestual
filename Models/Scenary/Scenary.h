#ifndef SCENARY_H
#define SCENARY_H

#include <vector>

class Scenary
{
private:
	
	std::string m_Path;
	std::string m_Name;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Rotation;

	LPD3DXMESH m_Mesh;
	D3DMATERIAL9* m_Materials;
	LPDIRECT3DTEXTURE9* m_Textures;
	int m_NumMaterials;

	void	LoadModelX	();

public:

	Scenary( const std::string name = "", const std::string path = "" );
	~Scenary();

	void	Render		( LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 pan = D3DXVECTOR3(0.f, 0.f, 0.f));

	void	SetPath		( std::string path ) { m_Path = path; };
	void	SetName		( std::string name ) { m_Name = name; };
	void	SetPosition	( D3DXVECTOR3 pos ) { m_Position = pos; };
	void	SetScale	( D3DXVECTOR3 sca ) { m_Scale = sca; };
	void	SetRotation	( D3DXVECTOR3 rot ) { m_Rotation = rot; };
};

#endif


