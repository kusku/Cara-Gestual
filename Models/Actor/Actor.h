#ifndef Actor_H
#define Actor_H

#define TIPUS_OBJ 1
#define TIPUS_3DS 2
#define TIPUS_X 3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3dx9.h>
#include <vector>
#include <map>

#include "Windows.h"
#include "../Readers/objLoader.h"
#include "../Readers/Obj3DS.h"
#include "../Readers/ObjX.h"
#include "../../Logic/Rigging/intersection.h"
#include "../../Render/CDirectX.h"
#include "../TextureManager/TextureManager.h"
#include "../../defines.h"
#include "../../Common/Timer/Timer.h"

struct VERTICEXYZ_T2_NORMAL
{
    D3DXVECTOR3    pos;
    D3DXVECTOR3	normal;
    FLOAT tu,tv;
};

#define D3DFVF_VERTICEXYZ_T2_NORMAL (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


struct O3DMaterial
{
	char	szName[MAX_NAME_TEXTURE];		 // Name of the material
	char	szTexture[MAX_PATH_TEXTURE];// Filename of the texture
	int	iTextureID;			 // OpenGL name of the texture
	float fDiffuse[3];		 // Diffuse component
	float fAmbient[3];		 // Ambient component
	float fSpecular[3];		 // Specular component
	float fEmmissive[3];		 // Emmissive component
	float fShininess;			 // Specular exponent
};

struct Point2D {
	float x,y;
};

struct Punt {
	D3DXVECTOR3 cordenades,normal,moviment;
	Point2D cordTex;
};

// Les cares es considera que son triangles.
// Es guarda la ID relacionada a cada punt.
struct Cara {
	Punt *punts[3];
	D3DXVECTOR3 normals[3];
	int materialTextura;
	Point2D cordTex[3];
};

class Actor {
	public:
		Actor(char* file,int tipus);
		~Actor();

		void		mourePunt			( int punt , D3DXVECTOR3 vectorMoviment );
		int			buscarPunt			( D3DXVECTOR3 punt );
		int			buscarTex			( Point2D tex );
		void		resetMoviments		( void );

		void		Render				(LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 pan = D3DXVECTOR3(0.0f,0.0f,0.0f));	

		void		SetPosition			( D3DXVECTOR3 pos ) { m_Position = pos; };
		void		SetScale			( D3DXVECTOR3 sca ) { m_Scale = sca; };
		void		SetRotation			( D3DXVECTOR3 rot ) { m_Rotation = rot; };

		// Mètodes GET
		int			PuntMesProxim		( D3DXVECTOR3 );
		D3DXVECTOR3 GetPosition			( ) { return m_Position; };
		D3DXVECTOR3	GetPoint			( int punt );
		D3DXVECTOR3	GetMovement			( int punt );
		D3DXVECTOR3	GetNormalsFace		( int nFace );
		int			GetNumVertexs		( void );
		int			GetNumTriangles		( void );
		void		GetTriangle			( int index, D3DXVECTOR3* triangle );
		void		GetFaceCoords		( int nFace, D3DXVECTOR3* coords );

		HRESULT		LoadInfoInVectors	( LPDIRECT3DDEVICE9 g_pd3dDevice );
		HRESULT		LoadVertexsBuffers	( LPDIRECT3DDEVICE9 g_pd3dDevice );

private:
		Cara		*cares;
		Punt		*punts;
		D3DXVECTOR3	*moviment;
		O3DMaterial *materials;
		Point2D		*cordTex;

		bool teNormals;
		int nombrePunts, nombreMaterials;
		int nombreCares;
		int nombreTexturesVertexs;

		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_Scale;
		D3DXVECTOR3 m_Rotation;

		//Dades per al RenderByAse
		std::vector< LPDIRECT3DINDEXBUFFER9 >	vec_pIBMeshByMat;
		std::vector< LPDIRECT3DVERTEXBUFFER9 >	vec_pVBGeomTexturaByMat;
		std::vector< LPDIRECT3DTEXTURE9 >		vec_textures;
		std::vector< D3DMATERIAL9 >				vec_materials;
		std::vector< int >						vec_numCaresByMat;
		std::vector <std::string>				listaTexturas;
		std::map<D3DXVECTOR3, int, D3DXVECTOR3_LessThan>			g_PuntsMap;

		std::vector < std::vector <CUSTOMVERTEX> >			vec_VerticesMesh;
		std::vector < std::vector <CUSTOMVERTEXTEXTURA> >	vec_Geom;
	
		void		ModelDeOBJ			( char* filename );
		void		ModelDe3DS			( char* filename );
		HRESULT		ModelDeX			( char* filename );
		void		UseMaterial				( O3DMaterial pMaterial );
		void		CalcularNormalsVertex	( void );

		D3DXVECTOR3	GetFaceNormal			( const Cara* );
};

#endif
