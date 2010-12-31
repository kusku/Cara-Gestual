#ifndef OBJX_H
#define OBJX_H

#include <vector>
#include <string>

class CObjX
{
public:

	CObjX();
	~CObjX();

	void	LoadModel				( std::string filename );
	void	LoadModelManually		( std::string filename );

	LPD3DXMESH				GetMesh				( ) { return m_Mesh; };
	D3DMATERIAL9*			GetMaterials		( ) { return m_MeshMaterials; };
	LPDIRECT3DTEXTURE9*		GetTextures			( ) { return m_MeshTextures; };
	int						GetNumMaterials		( ) { return (int) m_NumMaterials; };
	std::vector<int>		GetMaterialMeshList	( ) { return m_MaterialMeshList; };
	std::vector<std::string>GetTextureName		( ) { return m_TextureName; };

private:

	LPD3DXMESH m_Mesh;
	D3DMATERIAL9* m_MeshMaterials;
	LPDIRECT3DTEXTURE9* m_MeshTextures;
	DWORD m_NumMaterials;
	std::vector<std::string> m_TextureName;

	std::vector<int> m_MaterialMeshList;

	int m_NumVertices;
	int m_NumFaces;

	std::string	CreateTexturePath		( const std::string filename, std::string texturename );
	void		ParserMaterialMeshList	( std::string filename );

};

#endif