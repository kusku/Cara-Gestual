// Obj_3DS.cpp: implementation of the Obj_3DS class.
//
//////////////////////////////////////////////////////////////////////

#include "../../stdafx.h"
#include "Obj3DS.h"
#include <stack>
#include <il\ilut.h>		// Header File For OpenIL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// Constructor de la classe
//
Obj_3DS::Obj_3DS()
{
		
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;

	fet3DS=false;
}

//
// Funció d'inicialització de la classe Obj_3DS
//
void Obj_3DS::Inicialitzar()
{
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;
	
	fet3DS=false;
}

//
// Funció d'alliberament de memória de la classe Obj_3DS
//

void Obj_3DS::EliminarMemoria()
{
	//g_3DModel.pMaterials.clear();
	//fitxerstextures.clear();
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
// Alliberem les cares, normals, vertex i coordenades de textures
		delete [] g_3DModel.pObject[i].pFaces;
		delete [] g_3DModel.pObject[i].pNormals;
		delete [] g_3DModel.pObject[i].pVerts;
		delete [] g_3DModel.pObject[i].pTexVerts;
	}
	
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	/*for (int i=0;i<MAX_TEXTURES;i++){
		if (g_Texture[i]!=-1){
			const unsigned int *a=&g_Texture[i];
			glDeleteTextures(1,a);
		}
	}
	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;*/
	fet3DS=false;

}

//
// Destructor
//

Obj_3DS::~Obj_3DS()
{
	EliminarMemoria();
}

//
// Carrega un fitxer en format 3DS
//
bool Obj_3DS::Carregar3DS(char *nom)
{
//	strcpy(titol,nom);
	CLoad3DS g_Load3ds;
	// Primer necessitem carregar el fitxer 3DS. Li passem l'adreça de la
	// nostre estructura t3DModel i el nom del fitxer. 
	if (!g_Load3ds.Import3DS(&g_3DModel, nom))			// Load our .3DS file into our model structure
		return false;
	// Carregem les textures. Només podem carregar textures en format .BMP

	// Per a tots els materials
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// Comprovem si hi ha un fitxer per carregar per aquest material
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{
			// Utilitzem el nom de la textura per carregar el bitmap
			// i ens retorna el seu ID de textura que l'afegim al array de ID's de  textures
			//g_Texture[i]=CreateTexture(g_3DModel.pMaterials[i].strFile);
			g_Texture[i] = ilLoadImage(g_3DModel.pMaterials[i].strFile);	// Load The Images
			struct nomfitxer a;
			a.num=i;
			strcpy_s(a.nom,g_3DModel.pMaterials[i].strFile);
			fitxerstextures.push_back(a);
			if (g_Texture[i]==NULL)return false;
		}
		// Establim el ID de textura per aquest material
		g_3DModel.pMaterials[i].texureId = i;
	}
	
	fet3DS=true;
	return true;
}

