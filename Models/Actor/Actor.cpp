#include "../../stdafx.h"
#include "Actor.h"
#include "../../Logic/Rigging/intersection.h"
#include "../../defines.h"

CUSTOMVERTEX* tempVerticesVector = new CUSTOMVERTEX[100000];
CUSTOMVERTEXTEXTURA* tempVertexTextures = new CUSTOMVERTEXTEXTURA[50000];

Actor::Actor(char* filename, int tipus) {
	this->materials = NULL;
	this->cares = NULL;
	this->punts = NULL;

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (tipus) {
		case TIPUS_OBJ: this->ModelDeOBJ(filename);
			break;
		case TIPUS_3DS: this->ModelDe3DS(filename);
			break;
		case TIPUS_X: this->ModelDeX(filename);
			break;
		default: printf("Tipus invalid\n");
	}
}

Actor::~Actor()
{
	delete [] cordTex;
	delete [] materials;
	delete [] punts;
	delete [] cares;

	cordTex = NULL;
	materials = NULL;
	punts = NULL;
	cares = NULL;
	

	listaTexturas.clear();
	for(size_t cont = 0; cont < vec_textures.size(); ++cont)
	{
		if(vec_numCaresByMat[cont] != 0)
		{
			CHECKED_RELEASE(vec_pVBGeomTexturaByMat[cont]);
			CHECKED_RELEASE(vec_pIBMeshByMat[cont]);
		}
	}

	vec_textures.clear();
	vec_numCaresByMat.clear();
	vec_materials.clear();
	vec_VerticesMesh.clear();
	vec_Geom.clear();
	g_PuntsMap.clear();
	CTextureManager::GetInstance()->CleanUp();
}

void Actor::ModelDeOBJ(char* filename) {
	int numpunts,numcares,i,j;
	Punt p;

	COBJModel* o = new COBJModel();
	o->LoadModel(filename,NULL);
	ModelOBJ ob = o->GetModel();

	numpunts = o->GetNumVertices();

	this->punts = new Punt[numpunts];
	this->nombrePunts = numpunts;

	this->resetMoviments();
	for (i = 0; i < numpunts; i++) {
		this->punts[i].cordenades = D3DXVECTOR3(ob.pVertices[i].fX,ob.pVertices[i].fY,ob.pVertices[i].fZ);
		if (ob.pTexCoords != NULL)
		{
			this->punts[i].cordTex.x = ob.pTexCoords[i].fX;
			this->punts[i].cordTex.y = ob.pTexCoords[i].fY;
		}
	}

	numcares = o->GetNumFaces();

	this->cares = new Cara[numcares];
	this->nombreCares = numcares;

	for (i = 0; i < numcares; ++i) {
		for (j = 0; j < 3; ++j) {
			this->cares[i].punts[j] = &(punts[buscarPunt(D3DXVECTOR3(ob.pFaces[i].pVertices[j].fX,ob.pFaces[i].pVertices[j].fY,ob.pFaces[i].pVertices[j].fZ))]);
			if (ob.pNormals != NULL)
			{
				this->cares[i].normals[j] = D3DXVECTOR3(ob.pFaces[i].pNormals[j].fX,ob.pFaces[i].pNormals[j].fY,ob.pFaces[i].pNormals[j].fZ);
			}
			// Controlar que tenen textures
			if (ob.pTexCoords != NULL)
			{
				this->cares[i].materialTextura = ob.pFaces[i].iMaterialIndex;
				this->cares[i].cordTex[j].x = ob.pFaces[i].pTexCoords[j].fX;
				this->cares[i].cordTex[j].y = ob.pFaces[i].pTexCoords[j].fY;
			}
		}
	}
	if (ob.pMaterials != NULL)
	{
		this->nombreMaterials = o->GetNumMaterials();
		this->materials = new O3DMaterial[this->nombreMaterials];
	}
	else
	{
		this->nombreMaterials = 0;
		this->materials = NULL;
	}

	memcpy(this->materials,ob.pMaterials,sizeof(O3DMaterial) * this->nombreMaterials);

	nombreTexturesVertexs = o->GetNumTexCoords();
	cordTex = new Point2D[nombreTexturesVertexs];
	for(int i = 0; i < nombreTexturesVertexs; ++i)
	{
		cordTex[i].x = ob.pTexCoords[i].fX;
		cordTex[i].y = ob.pTexCoords[i].fY;
	}

	delete o;

	LoadInfoInVectors(CDirectX::GetInstance()->GetDevice());
}

void Actor::ModelDe3DS(char* filename)
{
	Obj_3DS *o = new Obj_3DS();
	t3DObject *objecte;
	int i,j,objectes;
	int offsetPunts, offsetCares,npunts,ncares;


	o->Carregar3DS(filename);
	this->nombreCares = 0;
	this->nombrePunts = 0;

	for (objectes = 0; objectes < o->g_3DModel.numOfObjects; objectes++) {
		objecte = &(o->g_3DModel.pObject[objectes]);
		offsetPunts = this->nombrePunts;
		offsetCares = this->nombreCares;

		npunts = objecte->numOfVerts;
		this->nombrePunts += objecte->numOfVerts;
		this->punts = (Punt*) realloc(this->punts,this->nombrePunts*sizeof(Punt));
		for (i = 0; i < npunts; i++) {
			this->punts[i + offsetPunts].cordenades = D3DXVECTOR3(objecte->pVerts[i].x,objecte->pVerts[i].y,objecte->pVerts[i].z);
		}

		ncares = objecte->numOfFaces;
		this->nombreCares += objecte->numOfFaces;
		this->cares = (Cara*) realloc(this->cares,this->nombreCares*sizeof(Cara));
		for (i = 0; i < ncares; i++) {
			for (j = 0; j < 3; j++) {
				this->cares[i + offsetCares].punts[j] = &(this->punts[objecte->pFaces[i].vertIndex[j] + offsetPunts]);
				this->cares[i + offsetCares].cordTex[j].x = objecte->pTexVerts[objecte->pFaces[i].vertIndex[j]].x;
				this->cares[i + offsetCares].cordTex[j].y = objecte->pTexVerts[objecte->pFaces[i].vertIndex[j]].y;
				this->cares[i + offsetCares].materialTextura = objecte->materialID;
			}
		}
	}

	this->nombreMaterials = o->g_3DModel.numOfMaterials;
	this->materials = new O3DMaterial[this->nombreMaterials];
	for (i = 0; i < this->nombreMaterials; i++) {
		strcpy_s(this->materials[i].szTexture, MAX_PATH_TEXTURE, o->g_3DModel.pMaterials[i].strFile);
		strcpy_s(this->materials[i].szName, MAX_NAME_TEXTURE, o->g_3DModel.pMaterials[i].strName);
		this->materials[i].iTextureID = o->g_Texture[o->g_3DModel.pMaterials[i].texureId];
	}

	this->resetMoviments();

	o->EliminarMemoria();
	delete o;

	LoadInfoInVectors(CDirectX::GetInstance()->GetDevice());
}

HRESULT Actor::ModelDeX(char* filename)
{
	CObjX* reader  = new CObjX();
	std::string file = filename;
	reader->LoadModel(file);

	//////////////////////////
	//Creació dels materials//
	//////////////////////////
	D3DMATERIAL9* m_Materials = reader->GetMaterials();
	if (m_Materials != NULL)
	{
		nombreMaterials = reader->GetNumMaterials();
		materials = new O3DMaterial[nombreMaterials];
	}
	else
	{
		this->nombreMaterials = 0;
		this->materials = NULL;
	}
	
	std::vector<std::string> nomTextures = reader->GetTextureName();
	for (int i=0; i<nombreMaterials; ++i)
	{
		strcpy_s(materials[i].szTexture, MAX_PATH_TEXTURE, const_cast <char *>(nomTextures[i].c_str()));
		materials[i].fAmbient[0] = m_Materials->Ambient.r;
		materials[i].fAmbient[1] = m_Materials->Ambient.g;
		materials[i].fAmbient[2] = m_Materials->Ambient.b;

		materials[i].fDiffuse[0] = m_Materials->Diffuse.r;
		materials[i].fDiffuse[1] = m_Materials->Diffuse.g;
		materials[i].fDiffuse[2] = m_Materials->Diffuse.b;

		materials[i].fEmmissive[0] = m_Materials->Emissive.r;
		materials[i].fEmmissive[1] = m_Materials->Emissive.g;
		materials[i].fEmmissive[2] = m_Materials->Emissive.b;

		materials[i].fSpecular[0] = m_Materials->Specular.r;
		materials[i].fSpecular[1] = m_Materials->Specular.g;
		materials[i].fSpecular[2] = m_Materials->Specular.b;

		materials[i].fShininess = m_Materials->Power;
	}
	
	///////////////////////
	//Creació de la malla//
	///////////////////////
	LPD3DXMESH m_Mesh = reader->GetMesh();
	IDirect3DIndexBuffer9 *ib;
	IDirect3DVertexBuffer9 *vb;

	nombrePunts = (int)m_Mesh->GetNumVertices();
	nombreCares = (int)m_Mesh->GetNumFaces();

	punts = new Punt[nombrePunts];
	cares = new Cara[nombreCares];
	cordTex = new Point2D[nombrePunts];

	std::vector<int> MaterialMeshList = reader->GetMaterialMeshList();

	unsigned short* indices;
	unsigned char* vertices;

	m_Mesh->GetIndexBuffer(&ib);
	m_Mesh->GetVertexBuffer(&vb);

	ib->Lock(0,0,(void **)&indices,0);
	vb->Lock(0,0,(void **)&vertices,0);

	for(unsigned int i=0; i<(unsigned int)nombrePunts; ++i)
	{
		D3DXVECTOR3 *pVertex = (D3DXVECTOR3 *)&vertices[m_Mesh->GetNumBytesPerVertex()*i];
		punts[i].cordenades = pVertex[0];
		punts[i].moviment = D3DXVECTOR3(0.f, 0.f, 0.f);
		punts[i].normal = pVertex[1];
		punts[i].cordTex.x = pVertex[2].y;
		punts[i].cordTex.y = -pVertex[2].z;
		cordTex[i].x = pVertex[2].y;
		cordTex[i].y = -pVertex[2].z;
	}

	for (unsigned int i = 0; i < (unsigned int)nombreCares; ++i)
	{
		for (unsigned short j = 0; j < 3; ++j)
		{
			D3DXVECTOR3 *pVertex = (D3DXVECTOR3 *)&vertices[m_Mesh->GetNumBytesPerVertex()*indices[i*3 + j]];
			
			int index = buscarPunt(pVertex[0]);

			cares[i].punts[j] = &punts[index];
			cares[i].normals[j] = punts[index].normal;
			cares[i].cordTex[j].x = pVertex[2].y;
			cares[i].cordTex[j].y = -pVertex[2].z;
		}
		cares[i].materialTextura = MaterialMeshList[i];
	}

	vb->Unlock();
	ib->Unlock();

	delete reader;
	delete m_Materials;

	LoadInfoInVectors(CDirectX::GetInstance()->GetDevice());
	return true;
}

int Actor::buscarPunt(D3DXVECTOR3 punt) {
	
	if(g_PuntsMap.empty())
	{
		for (int i = nombrePunts-1;i>=0; i--)
		{
			g_PuntsMap[punts[i].cordenades] = i;
		}
	}
	return g_PuntsMap[punt];
}

int	Actor::buscarTex( Point2D tex )
{
	int i,j;
	for (j=0; j<nombreCares; ++j)
	{
		for(i=0; i<3; ++i)
		{
			if (cares[j].cordTex[i].x == tex.x && cares[j].cordTex[i].y == tex.y)
			{
				return 3*j+i;
			}
		}
	}

	return 3*j+i;
}

int Actor::PuntMesProxim(D3DXVECTOR3 p)
{
	int millorPunt = 0;
	double distancia;
	double distMin = Distance(this->punts[0].cordenades, p);
	for (int i = 1; i < this->nombrePunts; i++) {
		distancia = Distance(this->punts[i].cordenades,p);
		if (distancia < distMin) {
			millorPunt = i;
			distMin = distancia;
		}
	}
	return millorPunt;
}

D3DXVECTOR3 Actor::GetPoint(int punt)
{
	return this->punts[punt].cordenades;
}

D3DXVECTOR3 Actor::GetMovement(int punt)
{
	return this->punts[punt].moviment;
}

int Actor::GetNumVertexs ( void )
{
	return nombrePunts;
}

// TODO: Recalcular les normals
void Actor::mourePunt(int punt, D3DXVECTOR3 vectorMoviment)
{
	this->punts[punt].moviment = vectorMoviment;
}

// Funcions Privades
void Actor::UseMaterial(const O3DMaterial pMaterial)
{
	glColor3f(1.0,1.0,1.0);
	if (pMaterial.iTextureID > -1) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pMaterial.iTextureID);
	}
	else
		glDisable(GL_TEXTURE_2D);
}

D3DXVECTOR3 Actor::GetFaceNormal(const Cara *cara)
{
	D3DXVECTOR3 p1,p2,normal;
	p1 = cara->punts[0]->cordenades - cara->punts[1]->cordenades;
	p2 = cara->punts[1]->cordenades - cara->punts[2]->cordenades;
	normal = D3DXVECTOR3(p1.y*p2.z - p1.z*p2.y,p1.z*p2.x - p1.x*p2.z,p1.x*p2.y - p1.y*p2.x);
	D3DXVec3Normalize(&normal, &normal);
	return normal;
}

void Actor::GetTriangle ( int index, D3DXVECTOR3* triangle )
{
	triangle[0] = this->cares[index].punts[0]->cordenades;
	triangle[1] = this->cares[index].punts[1]->cordenades;
	triangle[2] = this->cares[index].punts[2]->cordenades;
}

int Actor::GetNumTriangles ( void )
{
	return nombreCares;
}

void Actor::GetFaceCoords ( int nFace, D3DXVECTOR3* coords )
{
	coords[0] = this->cares[nFace].punts[0]->cordenades;
	coords[1] = this->cares[nFace].punts[1]->cordenades;
	coords[2] = this->cares[nFace].punts[2]->cordenades;
}

D3DXVECTOR3 Actor::GetNormalsFace ( int nFace )
{
	return this->GetFaceNormal(&this->cares[nFace]);
}

void Actor::resetMoviments()
{
	for (int i = 0; i < this->nombrePunts; i++) {
		this->punts[i].moviment = D3DXVECTOR3(0,0,0);
	}
}

void Actor::CalcularNormalsVertex()
{
	int i,j;
	D3DXVECTOR3 p;
	for (i = 0; i < this->nombrePunts; i++) {
		this->punts[i].normal = D3DXVECTOR3(0.0, 0.0, 0.0);
	}
	for (i = 0; i < this->nombreCares; i++) {
		p = this->GetNormalsFace(i);
		for (j = 0; j < 3; j++) {
			this->cares[i].punts[j]->normal += p;
		}
	}
	for (i = 0; i < this->nombrePunts; i++) {
		D3DXVec3Normalize(&this->punts[i].normal, &this->punts[i].normal);
	}
}

///////////////////////////////////////
//// RENDER WITH DIRECTX //////////////
///////////////////////////////////////
void Actor::Render(LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 pan)
{
	VOID* pMesh=NULL;
	unsigned short* indexs=NULL;
	CUSTOMVERTEXTEXTURA* text=NULL;

	D3DXVECTOR3 pos = m_Position + pan;
	D3DXMATRIX l_MatrixT, l_Matrix;
	D3DXMatrixIdentity(&l_MatrixT);
	D3DXMatrixIdentity(&l_Matrix);
	D3DXMatrixScaling (&l_MatrixT,m_Scale.x, m_Scale.y, m_Scale.z);
	l_Matrix *= l_MatrixT;
	D3DXMatrixRotationYawPitchRoll(&l_MatrixT, DEG2RAD(m_Rotation.y), DEG2RAD(m_Rotation.x), DEG2RAD(m_Rotation.z));
	l_Matrix *= l_MatrixT;
	D3DXMatrixTranslation (&l_MatrixT, pos.x, pos.y, pos.z);
	l_Matrix *= l_MatrixT;
	Device->SetTransform(D3DTS_WORLD, &l_Matrix);

	for(int cont = 0; cont < (int)vec_textures.size(); cont++)
	{
		if(vec_numCaresByMat[cont] != 0)
		{
			Device->SetStreamSource( 0, vec_pVBGeomTexturaByMat[cont], 0, sizeof(CUSTOMVERTEXTEXTURA) );
			Device->SetFVF( D3DFVF_CUSTOMVERTEXTEXTURA);
			Device->SetTexture (0, vec_textures[cont]);
			Device->SetMaterial(&vec_materials[cont]);
			Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,vec_numCaresByMat[cont]);
			//Recuperar els índexs
			/*vec_pIBMeshByMat[cont]->Lock( 0, sizeof(unsigned short)*vec_numCaresByMat[cont]*3, (void**)&pMesh, 0 );
				indexs = (unsigned short*)pMesh;
			vec_pIBMeshByMat[cont]->Unlock();

			vec_pVBGeomTexturaByMat[cont]->Lock( 0, sizeof(CUSTOMVERTEXTEXTURA)*vec_numCaresByMat[cont]*3, (void**)&pMesh, 0 );
				text = (CUSTOMVERTEXTEXTURA*)pMesh;
			vec_pVBGeomTexturaByMat[cont]->Unlock();

			Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,vec_Geom[cont].size(),1,
							indexs,D3DFMT_INDEX16,text, sizeof(CUSTOMVERTEXTEXTURA));*/
		}
	}
	Device->SetTexture (0, NULL);
}

HRESULT Actor::LoadInfoInVectors( LPDIRECT3DDEVICE9 g_pd3dDevice  )
{
	struct CoordsText
	{
		float u,v;
	};

	CoordsText *g_VerticesTextura=NULL;
	VOID *pMesh, *pMeshIndices;
	unsigned long numBytes;

	CUSTOMVERTEX *g_VerticesMesh;
	CUSTOMVERTEXTEXTURA *Geom;
	unsigned short *g_IndicesMesh, *g_TIndicesMesh;

	std::vector<int>				IndexMaterial;

	std::vector<D3DXVECTOR3>				VertexBuffer;
	std::vector<D3DXVECTOR3>				CVertexBuffer;
	
	std::vector< std::vector<D3DXVECTOR3> >	VertexBufferByMat;
	std::vector< std::vector<D3DXVECTOR3> >	IndexBufferByMat;
	
	std::vector< std::vector<D3DXVECTOR3> >	CVertexBufferByMat;
	std::vector< std::vector<D3DXVECTOR3> >	CIndexBufferByMat;

	std::map< int, int >					VertexIndexOldNew;
	std::map< int, int >					CVertexIndexOldNew;

	LPDIRECT3DTEXTURE9 texture=NULL;
	
	if (!vec_numCaresByMat.empty())
	{
		for(size_t cont = 0; cont < vec_textures.size(); cont++)
		{
			if(vec_numCaresByMat[cont] != 0)
			{
				CHECKED_RELEASE(vec_pVBGeomTexturaByMat[cont]);
				CHECKED_RELEASE(vec_pIBMeshByMat[cont]);
			}
		}
		vec_numCaresByMat.clear();
		vec_pIBMeshByMat.clear();
		vec_pVBGeomTexturaByMat.clear();
		vec_textures.clear();
		vec_materials.clear();
		listaTexturas.clear();
		vec_VerticesMesh.clear();
		vec_Geom.clear();
	}

	//Llegeix les textures
	for(int i = 0; i < nombreMaterials; ++i)
	{
		listaTexturas.push_back(this->materials[i].szTexture);

		texture = CTextureManager::GetInstance()->LoadTexture(this->materials[i].szTexture,g_pd3dDevice);
		vec_textures.push_back(texture);	
		
		//Establir material
		{
			D3DMATERIAL9 l_Mat;

			l_Mat.Ambient.r = this->materials[i].fAmbient[0];
			l_Mat.Ambient.g = this->materials[i].fAmbient[1];
			l_Mat.Ambient.b = this->materials[i].fAmbient[2];
			l_Mat.Ambient.a = 1.0f;

			l_Mat.Diffuse.r = this->materials[i].fDiffuse[0];
			l_Mat.Diffuse.g = this->materials[i].fDiffuse[1];
			l_Mat.Diffuse.b = this->materials[i].fDiffuse[2];
			l_Mat.Diffuse.a = 1.0f;

			l_Mat.Specular.r = this->materials[i].fSpecular[0];
			l_Mat.Specular.g = this->materials[i].fSpecular[1];
			l_Mat.Specular.b = this->materials[i].fSpecular[2];
			l_Mat.Specular.a = 1.0f;

			l_Mat.Emissive.r = this->materials[i].fEmmissive[0];
			l_Mat.Emissive.g = this->materials[i].fEmmissive[1];
			l_Mat.Emissive.b = this->materials[i].fEmmissive[2];
			l_Mat.Emissive.a = 1.0f;

			l_Mat.Power = this->materials[i].fShininess;

			vec_materials.push_back(l_Mat);
		}

		std::vector<D3DXVECTOR3> new_vertexBuffer;
		VertexBufferByMat.push_back( new_vertexBuffer );
		std::vector<D3DXVECTOR3> new_CvertexBuffer;
		CVertexBufferByMat.push_back( new_CvertexBuffer );
		
		std::vector<D3DXVECTOR3> new_indexBuffer;
		IndexBufferByMat.push_back(  new_indexBuffer );	
		std::vector<D3DXVECTOR3> new_CindexBuffer;
		CIndexBufferByMat.push_back( new_CindexBuffer );
	}

	//Construeix el VertexBuffer de cares
	for (int i = 0; i < nombrePunts; ++i)
	{
		VertexBuffer.push_back(D3DXVECTOR3(	punts[i].cordenades.x + punts[i].moviment.x,
											punts[i].cordenades.y + punts[i].moviment.y,
											punts[i].cordenades.z + punts[i].moviment.z ));
	}

	//Construeix el IndexBuffer de cares
	for (int i = 0; i < nombreCares; ++i)
	{
		unsigned short material =(unsigned short)cares[i].materialTextura;
		
		int x = buscarPunt(cares[i].punts[0]->cordenades);
		int y = buscarPunt(cares[i].punts[1]->cordenades);
		int z = buscarPunt(cares[i].punts[2]->cordenades);

		D3DXVECTOR3 index_face = D3DXVECTOR3((float)x, (float)y, (float)z);
		IndexBufferByMat[material].push_back( index_face );
		IndexMaterial.push_back( material );
	}

	//Construeix el VertexBuffer de textures
	for (int i=0; i < nombreCares; ++i)
	{
		CVertexBuffer.push_back(D3DXVECTOR3(cares[i].cordTex[0].x, -cares[i].cordTex[0].y, 0.0f));
		CVertexBuffer.push_back(D3DXVECTOR3(cares[i].cordTex[1].x, -cares[i].cordTex[1].y, 0.0f));
		CVertexBuffer.push_back(D3DXVECTOR3(cares[i].cordTex[2].x, -cares[i].cordTex[2].y, 0.0f));
	}

	//Construeix el IndexBuffer de textures
	for (int i = 0; i < nombreCares; ++i)
	{
		D3DXVECTOR3 index_tFace;
		
		index_tFace.x = (float)buscarTex(cares[i].cordTex[0]);
		index_tFace.y = (float)buscarTex(cares[i].cordTex[1]);
		index_tFace.z = (float)buscarTex(cares[i].cordTex[2]);

		int mat = IndexMaterial[ i ];
		CIndexBufferByMat[mat].push_back( index_tFace );
	}

	///////////
	///////////
	std::map<int,int>::iterator it_VertexIndexOldNew;
	typedef std::pair<int,int> Pair_IndexOldNew;
	//Ahora vamos a reasignar los vertices geometricos y de color por material
	for(int cont_texture = 0; cont_texture < nombreMaterials; cont_texture++) //Recorrem totes les textures
	{
		int contador_vertices = 0;
		int contador_tvertices = 0;
		VertexIndexOldNew.clear();
		CVertexIndexOldNew.clear();
		//Recorremos todas las caras geometricas y de textura
		for(int cont_face = 0; cont_face < (int)IndexBufferByMat[cont_texture].size(); cont_face++) //Recorre totes les cares d'un material
		{
			D3DXVECTOR3 face_geom = IndexBufferByMat[cont_texture][cont_face];
			for(int j=0;j<3;j++) //Recorre la cara d'un material
			{
				it_VertexIndexOldNew = VertexIndexOldNew.find((int)face_geom[j]);
				if(it_VertexIndexOldNew != VertexIndexOldNew.end())
				{
					face_geom[j] = (float)it_VertexIndexOldNew->second;
				}
				else
				{
					VertexIndexOldNew.insert( Pair_IndexOldNew((int)face_geom[j], contador_vertices ) );
					VertexBufferByMat[cont_texture].push_back( VertexBuffer[(int)face_geom[j]] );
					face_geom[j] = (float)contador_vertices;
					contador_vertices++;
				}
			}
			
			D3DXVECTOR3 face_texture = CIndexBufferByMat[cont_texture][cont_face];
			for(int j=0;j<3;j++)
			{
				it_VertexIndexOldNew = CVertexIndexOldNew.find((int)face_texture[j]);
				if(it_VertexIndexOldNew != CVertexIndexOldNew.end())
				{
					face_texture[j]= (float)it_VertexIndexOldNew->second;
				}
				else
				{
					CVertexIndexOldNew.insert( Pair_IndexOldNew((int)face_texture[j], contador_tvertices ) );
					CVertexBufferByMat[cont_texture].push_back( CVertexBuffer[(int)face_texture[j]] );
					face_texture[j] = (float)contador_tvertices;
					contador_tvertices++;
				}
			}
			IndexBufferByMat[cont_texture][cont_face] = face_geom;
			CIndexBufferByMat[cont_texture][cont_face] = face_texture;

		}
	}
	CVertexBuffer.clear();
	VertexBuffer.clear();

	//Creamos una mesh para cada textura
	for(int cont_texture = 0; cont_texture < nombreMaterials; cont_texture++)
	{
		int numVertices = (int)VertexBufferByMat[cont_texture].size();
		int numCaras = (int)IndexBufferByMat[cont_texture].size();
		int numTVertices = (int)CVertexBufferByMat[cont_texture].size();
		int numTCaras = (int)CIndexBufferByMat[cont_texture].size();
        
		vec_numCaresByMat.push_back(numCaras);
		
		//inicilizamos en memoria la matriz de vertices e indices
		g_VerticesMesh=new CUSTOMVERTEX[numVertices];
		g_IndicesMesh=new unsigned short[numCaras*3];
		std::vector <CUSTOMVERTEX> verVector;
		for(size_t iv=0;iv<(size_t)numVertices;iv++)
		{
			D3DXVECTOR3 vertex = VertexBufferByMat[cont_texture][iv];
			switch(iv%3)
			{
				case 0:
					g_VerticesMesh[iv].color=0xffffffff;
					break;
				case 1:
					g_VerticesMesh[iv].color=0xff0000ff;
					break;
				case 2:
					g_VerticesMesh[iv].color=0xff00ffff;
					break;
			}
			g_VerticesMesh[iv].x=vertex.x;
			g_VerticesMesh[iv].y=vertex.y;
			g_VerticesMesh[iv].z=vertex.z;
			verVector.push_back(*g_VerticesMesh);
		}
		vec_VerticesMesh.push_back(verVector);
		verVector.clear();

		for(size_t ic=0;ic<(size_t)numCaras;ic++)
		{
			D3DXVECTOR3 face = IndexBufferByMat[cont_texture][ic];
			g_IndicesMesh[ic*3]=(int)face.x;
			g_IndicesMesh[1+ic*3]=(int)face.y;
			g_IndicesMesh[2+ic*3]=(int)face.z;
		}
		LPDIRECT3DVERTEXBUFFER9 pVBMesh=NULL;
		if(numCaras != 0)
		{
			if( FAILED( g_pd3dDevice->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*numVertices,0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &pVBMesh, NULL ) ) )
			{
				return E_FAIL;
			}
			
			if( FAILED( pVBMesh->Lock( 0, sizeof(CUSTOMVERTEX)*numVertices, (void**)&pMesh, 0 ) ) )
			{
				return E_FAIL; 
			}
			memcpy( pMesh, g_VerticesMesh, sizeof(CUSTOMVERTEX)*numVertices );
			pVBMesh->Unlock();
		}
		//vec_pVBMeshByMat.push_back(pVBMesh);
		
		numBytes=numCaras*3*sizeof(unsigned short);
		LPDIRECT3DINDEXBUFFER9 pIBMesh=NULL;
		if(numCaras != 0)
		{
			
				//D3DFMT_INDEX16 
			if( FAILED( g_pd3dDevice->CreateIndexBuffer( numBytes,0,D3DFMT_INDEX16,D3DPOOL_DEFAULT, &pIBMesh, NULL ) ) )
			{
				return E_FAIL;
			}
			if( FAILED( pIBMesh->Lock( 0, numBytes, (void**)&pMeshIndices, 0 ) ) )
			{
				return E_FAIL; 
			}
			memcpy( pMeshIndices, g_IndicesMesh, numBytes);
			//m_IndicesMesh.push_back(g_IndicesMesh);
			pIBMesh->Unlock();
		}
		vec_pIBMeshByMat.push_back(pIBMesh);

		g_VerticesTextura = new CoordsText[numTVertices];
		for(int iv=0;iv<numTVertices;iv++)
		{
			D3DXVECTOR3 texture_vertex = CVertexBufferByMat[cont_texture][iv];
			//lo tenemos que cargar asi, pq z es arriba para el max y x esta invertido por el max
			g_VerticesTextura[iv].u = texture_vertex.x;
			g_VerticesTextura[iv].v = texture_vertex.y;
		}

		g_TIndicesMesh = new unsigned short[numTCaras*3];
		for(int ic=0;ic<numTCaras;ic++)
		{
			D3DXVECTOR3 texture_face = CIndexBufferByMat[cont_texture][ic];
			g_TIndicesMesh[ic*3]= (int)texture_face.x;
			g_TIndicesMesh[1+ic*3]=(int)texture_face.y;
			g_TIndicesMesh[2+ic*3]=(int)texture_face.z;
		}

		Geom=new CUSTOMVERTEXTEXTURA [numCaras*3];
		std::vector <CUSTOMVERTEXTEXTURA> vecGeom;
		for(int iv=0;iv<numCaras;iv++)
		{
			int vid1, vid2, vid3,tvid1, tvid2, tvid3;
			D3DXVECTOR3 pos1,pos2,pos3;

			vid1=g_IndicesMesh[3*iv];
			vid2=g_IndicesMesh[3*iv+1];
			vid3=g_IndicesMesh[3*iv+2];
			tvid1=g_TIndicesMesh[3*iv];
			tvid2=g_TIndicesMesh[3*iv+1];
			tvid3=g_TIndicesMesh[3*iv+2];
			//COPIA COORDENADAS DE VERTICES 
			pos1.x = Geom[iv*3].x = g_VerticesMesh[vid1].x;
			pos1.y = Geom[iv*3].y = g_VerticesMesh[vid1].y;
			pos1.z = Geom[iv*3].z = g_VerticesMesh[vid1].z;

			pos2.x = Geom[iv*3+1].x = g_VerticesMesh[vid2].x;
			pos2.y = Geom[iv*3+1].y = g_VerticesMesh[vid2].y;
			pos2.z = Geom[iv*3+1].z = g_VerticesMesh[vid2].z;

			pos3.x = Geom[iv*3+2].x = g_VerticesMesh[vid3].x;
			pos3.y = Geom[iv*3+2].y = g_VerticesMesh[vid3].y;
			pos3.z = Geom[iv*3+2].z = g_VerticesMesh[vid3].z;
				
			//COPIA COORDENADAS DE TEXTURAS
			Geom[iv*3].u=g_VerticesTextura[tvid1].u;
			Geom[iv*3].v=g_VerticesTextura[tvid1].v;

			Geom[iv*3+1].u=g_VerticesTextura[tvid2].u;
			Geom[iv*3+1].v=g_VerticesTextura[tvid2].v;

			Geom[iv*3+2].u=g_VerticesTextura[tvid3].u;
			Geom[iv*3+2].v=g_VerticesTextura[tvid3].v;

			Geom[iv*3].color=0xffffffff;
			Geom[iv*3+1].color=0xffffffff;
			Geom[iv*3+2].color=0xffffffff;
		
			vecGeom.push_back(Geom[iv*3]);
			vecGeom.push_back(Geom[iv*3+1]);
			vecGeom.push_back(Geom[iv*3+2]);
		}
		vec_Geom.push_back(vecGeom);
		vecGeom.clear();


		LPDIRECT3DVERTEXBUFFER9 pVBGeomTextura=NULL;
		if(numCaras != 0)
		{
			DWORD usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
			if( FAILED( g_pd3dDevice->CreateVertexBuffer( sizeof(CUSTOMVERTEXTEXTURA)*numCaras*3,
													0, D3DFVF_CUSTOMVERTEXTEXTURA,
													D3DPOOL_DEFAULT, &pVBGeomTextura, NULL ) ) )
			{
				return E_FAIL;
			}

			if( FAILED( pVBGeomTextura->Lock( 0, sizeof(CUSTOMVERTEXTEXTURA)*numCaras*3, (void**)&pMesh, 0 ) ) )
				return E_FAIL; 
			memcpy( pMesh, Geom, sizeof(CUSTOMVERTEXTEXTURA)*numCaras*3 );
			pVBGeomTextura->Unlock();
		}
		vec_pVBGeomTexturaByMat.push_back(pVBGeomTextura);
	}

	delete [] g_IndicesMesh;
	delete [] g_VerticesMesh;
	delete [] g_TIndicesMesh;
	delete [] g_VerticesTextura;
	delete [] Geom;
	VertexIndexOldNew.clear();
	CVertexIndexOldNew.clear();

	return true;
}

HRESULT Actor::LoadVertexsBuffers( LPDIRECT3DDEVICE9 g_pd3dDevice )
{
	VOID *pMesh;
	CUSTOMVERTEXTEXTURA *l_VTMesh=NULL;

	int numFaces;
	int num = 0;

	for (int mat = 0; mat < nombreMaterials; ++mat)
	{
		if (mat == 1)
		{
		numFaces = (int)vec_Geom[mat].size();

		if( FAILED( vec_pVBGeomTexturaByMat[mat]->Lock( 0, sizeof(CUSTOMVERTEXTEXTURA)*numFaces, (void**)&pMesh, 0 ) ) )
			return E_FAIL; 

		l_VTMesh=(CUSTOMVERTEXTEXTURA *)pMesh;

			for(int i=0; i <numFaces; ++i)
			{
				l_VTMesh[i] =vec_Geom[mat][i]; 
				num = g_PuntsMap[D3DXVECTOR3(l_VTMesh[i].x, l_VTMesh[i].y, l_VTMesh[i].z)];
				l_VTMesh[i].x += punts[num].moviment.x;
				l_VTMesh[i].y += punts[num].moviment.y;
				l_VTMesh[i].z += punts[num].moviment.z;
			}		
		vec_pVBGeomTexturaByMat[mat]->Unlock();
		}
	}

	return true;
}