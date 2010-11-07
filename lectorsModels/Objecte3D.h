#ifndef OBJECTE3D_H
#define OBJECTE3D_H

#define TIPUS_OBJ 1
#define TIPUS_3DS 2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <d3dx9.h>
#include <vector>

#include "Windows.h"
#include "../SPoint3D.h"
#include "objLoader.h"
#include "../Seleccions/intersection.h"
#include "../Render/CDirectX.h"
#include "TextureManager.h"

struct VERTICEXYZ_T2_NORMAL
{
    D3DXVECTOR3    pos;
    D3DXVECTOR3	normal;
    FLOAT tu,tv;
};

#define D3DFVF_VERTICEXYZ_T2_NORMAL (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


struct O3DMaterial
{
	char	szName[1024];		 // Name of the material
	char	szTexture[_MAX_PATH];// Filename of the texture
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
	SPoint3D cordenades,normal,moviment;
	Point2D cordTex;
};

// Les cares es considera que son triangles.
// Es guarda la ID relacionada a cada punt.
struct Cara {
	Punt *punts[3];
	SPoint3D normals[3];
	int materialTextura;
	Point2D cordTex[3];
};

class Objecte3D {
	public:
		Objecte3D(char* file,int tipus);
		~Objecte3D();

		void		mourePunt			( int punt , SPoint3D vectorMoviment );
		int			buscarPunt			( SPoint3D punt );
		int			buscarTex			( Point2D tex );
		void		Render				( void );
		void		resetMoviments		( void );

		void		Render				(LPDIRECT3DDEVICE9 Device);	

		// Mètodes GET
		int			PuntMesProxim		( SPoint3D );
		SPoint3D	GetPoint			( int punt );
		SPoint3D	GetMovement			( int punt );
		SPoint3D	GetNormalsFace		( int nFace );
		int			GetNumVertexs		( void );
		int			GetNumTriangles		( void );
		void		GetTriangle			( int index, SPoint3D* triangle );
		void		GetFaceCoords		( int nFace, SPoint3D* coords );	

private:
		Cara		*cares;
		Punt		*punts;
		SPoint3D	*moviment;
		O3DMaterial *materials;
		Point2D		*cordTex;

		bool teNormals;
		int nombrePunts, nombreMaterials;
		int nombreCares;
		int nombreTexturesVertexs;

		//VertexBuffer i IndexBuffer
		LPDIRECT3DVERTEXBUFFER9				m_pVB;
		LPDIRECT3DINDEXBUFFER9				m_pIB;
		std::vector<LPDIRECT3DTEXTURE9>		m_TextureList;

		//Dades per al RenderByAse
		std::vector< LPDIRECT3DVERTEXBUFFER9 >	vec_pVBMeshByMat;
		std::vector< LPDIRECT3DINDEXBUFFER9 >	vec_pIBMeshByMat;
		std::vector< LPDIRECT3DVERTEXBUFFER9 >	vec_pVBGeomTexturaByMat;
		std::vector< LPDIRECT3DTEXTURE9 >		vec_textures;
		std::vector< D3DMATERIAL9 >				vec_materials;
		std::vector< int >						vec_numCaresByMat;
		std::vector <std::string>				listaTexturas;

		VERTICEXYZ_T2_NORMAL *l_pV;
		int* l_pMF;
		
		
		void		Objecte3DDeOBJ			( char* filename );
		void		Objecte3DDe3DS			( char* filename );
		void		UseMaterial				( O3DMaterial pMaterial );
		void		CalcularNormalsVertex	( void );

		SPoint3D	GetFaceNormal			( const Cara* );

		bool		LoadInfoInVectors	( LPDIRECT3DDEVICE9 g_pd3dDevice );
};

#endif
