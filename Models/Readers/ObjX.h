#ifndef OBJX_H
#define OBJX_H

#include <vector>
#include <string>

class CObjX
{
public:

	CObjX();
	~CObjX();

	void	LoadModel	( std::string filename );

	LPD3DXMESH			GetMesh			( ) { return m_Mesh; };
	D3DMATERIAL9*		GetMaterials	( ) { return m_MeshMaterials; };
	LPDIRECT3DTEXTURE9*	GetTextures		( ) { return m_MeshTextures; };
	int					GetNumMaterials ( ) { return (int) m_NumMaterials; };
	std::string			GetTextureName	( ) { return m_TextureName; };

private:

	LPD3DXMESH m_Mesh;
	D3DMATERIAL9* m_MeshMaterials;
	LPDIRECT3DTEXTURE9* m_MeshTextures;
	DWORD m_NumMaterials;
	std::string m_TextureName;

	std::string	CreateTexturePath	( const std::string filename, std::string texturename );
	
};

#endif