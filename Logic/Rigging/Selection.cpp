#include "../../stdafx.h"
#include "Selection.h"
#include "../../matrix.h"
#include "../../Models/Actor/Actor.h"
#include "EditorManager.h"
#include "intersection.h"
#include "../../Render/visualitzacio.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "../../Render/CDirectX.h"

void inline swapFloat( float &x, float &y )
{
	 float temp = x; 
	 x = y;
	 y = temp;
}

Selection::Selection(Actor* obj, EditorManager* editor)
{
	ObOBJ = obj;
	editorM = editor;
	dominantSelect = false;
	m_pTriFlags = NULL;
	if (obj != NULL)
		this->SetFlagsTriangles();
}
Selection::~Selection()
{
	ObOBJ = NULL;
	free(editorM);
	delete[] m_pTriFlags;
}

//Al prémer el botó esquerre
void Selection::ButtonDown( float mouseX, float mouseY, int flag )
{
	nStartX = mouseX;
	nStartY = mouseY;
	nEndX = mouseX;
	nEndY = mouseY;
    buttonState = true;	

	if (ObOBJ != NULL)
	{
		GetLine( m_vLineP[0], m_vLineP[1], mouseX, mouseY );	//Obtenim la l�nia que pertany a on s'ha clickat
		switch (flag)
		{
		case 1:
			{
				SetSelectionMode( SELECT_ADD );	//Indica que els v�rtexs que es seleccionin s'han d'afegir a la selecci�
				break;
			}
		case 2:
			{
				SetSelectionMode( SELECT_SUB ); //Indica que els v�rtexs que es seleccionin s'han de borrar de la selecci�.
				break;
			}
		}

		LineSelect(m_vLineP[0],m_vLineP[1]);	//Agafem l'�ndex del punt m�s proper a la col�lisi� en aquell punt
	}
}

//Calcula el quadrat de sel·lecció al moure el ratolí
void Selection::ButtonMove( float mouseX, float mouseY )
{
	if ( buttonState )
	{
		if ( mouseX < 32000 ) 
			nEndX = mouseX; 
		else 
			nEndX = 0;
		if ( mouseY < 32000 ) 
			nEndY = mouseY; 
		else 
			nEndY = 0;
		// (reason for if : when the mouse is beyond the window in a negative direction we end up with a high number )		
	}
}

//Acció al deixar el d'apretar el botó esquerre del ratolí
void Selection::ButtonUp( void )
{
	buttonState = false;
	if ( abs( nEndX - nStartX ) < 4 && abs( nEndY - nStartY ) < 4 ) 
		return;
	if ( nEndX < nStartX ) 
		swapFloat( nEndX, nStartX );
	if ( nEndY < nStartY ) 
		swapFloat( nEndY, nStartY );
	
	if (ObOBJ != NULL)
	{
		D3DXVECTOR3 P[8];
		D3DXVECTOR3 Normals[6];
		GetFrustum(Normals,P);
		FrustumSelect(Normals, P);
		//Cridar als m�todes de EditorManager per tal d'afegir els v�rtexs all�
	}
}

//Selecciona el vèrtex dominant del muscle
void Selection::ButtonRDown( float mouseX, float mouseY )
{
	if (ObOBJ != NULL)
	{
		dominantSelect = true;
		GetLine( m_vLineP[0], m_vLineP[1], mouseX, mouseY );
		LineSelect(m_vLineP[0],m_vLineP[1]);
		dominantSelect = false;
	}
}

//Obté una línia en el món virtual a partir d'unes coordenades de pantalla fetes amb el ratolí
void Selection::GetLine( D3DXVECTOR3 &L1, D3DXVECTOR3 &L2, float mouseX, float mouseY )
{
	D3DVIEWPORT9 ViewPortMatrix;
	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
	D3DXMATRIX	WorldMatrix;
	D3DXVECTOR3 result;
	D3DXVECTOR3 screenPoint;

	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();

	Device->GetViewport(&ViewPortMatrix);
	Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	Device->GetTransform(D3DTS_PROJECTION, &ProjectionMatrix);

	D3DXMatrixIdentity(&WorldMatrix);

	screenPoint = D3DXVECTOR3(mouseX, (float)(ViewPortMatrix.Height - mouseY + 1.0), ViewPortMatrix.MinZ);
	D3DXVec3Unproject( &result, &screenPoint, &ViewPortMatrix, &ProjectionMatrix, &ViewMatrix, &WorldMatrix );
	L1 = D3DXVECTOR3(result.x,  result.y, result.z);

	screenPoint = D3DXVECTOR3(mouseX, mouseY, ViewPortMatrix.MaxZ);
	D3DXVec3Unproject( &result, &screenPoint, &ViewPortMatrix, &ProjectionMatrix, &ViewMatrix, &WorldMatrix );
	L2 = D3DXVECTOR3(result.x, result.y, result.z);
}

void Selection::GetFrustum( D3DXVECTOR3 Normals[4], D3DXVECTOR3 P[8])
{
	GetLine( P[0], P[1], nStartX, nStartY );
	GetLine( P[2], P[3], nStartX, nEndY );
	GetLine( P[4], P[5], nEndX, nEndY );
	GetLine( P[6], P[7], nEndX, nStartY );
	D3DXVec3Cross(&Normals[0], &(P[0]-P[1]), &(P[2]-P[3]));
	D3DXVec3Cross(&Normals[1], &(P[2]-P[3]), &(P[4]-P[5]));
	D3DXVec3Cross(&Normals[2], &(P[4]-P[5]), &(P[6]-P[7]));
	D3DXVec3Cross(&Normals[3], &(P[6]-P[7]), &(P[0]-P[1]));

	for (int i = 0; i < 4; i++) 
		D3DXVec3Normalize(&Normals[i], &Normals[i]);
}


void Selection::Render()
{
	if (buttonState)
		RenderBox(nStartX, nStartY, nEndX, nEndY);
}

void Selection::NoRender()
{
	RenderBox(0.0,0.0,0.0,0.0);
}

void Selection::SetObj( Actor* obj )
{
	if (ObOBJ == NULL)
	{
		ObOBJ = obj;
		this->SetFlagsTriangles();
	}		
}

bool Selection::IsTriangleSelected ( int nTri )
{
	if (m_pTriFlags != NULL)
	{
		if (m_pTriFlags[nTri] == TF_SELECTED)
			return true;
		else
			return false;
	}
	else
		return false;
}

void Selection::SetFlagsTriangles ( void )
{
	if (m_pTriFlags != NULL)
	{
		delete [] m_pTriFlags;
		delete [] m_pTriBackFacing;
	}

	m_pTriFlags = new int[ObOBJ->GetNumTriangles()];
	m_pTriBackFacing = new int[ObOBJ->GetNumTriangles()];
}

void Selection::SetSelectionMode ( int nMode )
{
	m_nSelMode = nMode; 
}

void Selection::SelectTriangle	( int nTri )
{
	if ( nTri < 0 || nTri >= ObOBJ->GetNumTriangles() ) 
		return; //Surt si no s'ha seleccionat cap triangle

	if ( m_nSelMode == SELECT_ADD ) 
		m_pTriFlags[ nTri ] = TF_SELECTED; //Selecciona el triangle

	if ( m_nSelMode == SELECT_SUB )
	{
		m_pTriFlags[ nTri ] = NTF_SELECTED; //NO selecciona el triangle

	}
}

int Selection::FrustumSelect ( D3DXVECTOR3 Normals[4], D3DXVECTOR3 Points[8] )
{
	int nbHits = 0;
	D3DXVECTOR3 Tri[3];

	for (int nTri = 0; nTri < ObOBJ->GetNumTriangles(); ++nTri )
	{	
		int nV = nTri*3;
		ObOBJ->GetFaceCoords(nTri, Tri);

		if ( TriInFrustum( Tri, Normals, Points ) )
		{
			SelectTriangle( nTri );			
			++nbHits;
			for (int i=0; i < 3; ++i)
			{
				if ( PointInFrustum(Tri[i],Normals,Points) )
				{
					if (m_nSelMode == SELECT_ADD)
					{
						if (m_pTriFlags[nTri] == TF_SELECTED)
							editorM->AddVertex(Tri[i]);
					}
					else
					{
						if (m_pTriFlags[nTri] == NTF_SELECTED)
							editorM->DeleteVertex(Tri[i]);
					}
				}
			}
		}
	}
	
	return nbHits;
}

int Selection::LineSelect (const D3DXVECTOR3 &LP1, const D3DXVECTOR3 &LP2 )
{
	D3DXVECTOR3 HitP, pFace[3];
	int nbHits = 0;
	int nSelTri = -1;
	float fDistance = 1000000000.0f;
	
	for (int nTri = 0; nTri < ObOBJ->GetNumTriangles(); ++nTri )
	{
		if ( m_pTriBackFacing[ nTri ] == TF_BACKFACING ) 
			continue; // Check only front facing triangles

		int nV = nTri*3;	
	
		ObOBJ->GetFaceCoords(nTri, pFace);

		bool bHit = CheckLineTri( LP2, LP1, pFace[0], pFace[1], pFace[2], HitP );
		if ( bHit ) 
		{
			if ( Distance(HitP, LP1) < fDistance ) 
			{
				fDistance = (float) Distance(HitP, LP1 );
				nSelTri = nTri;

				if (dominantSelect)
				{
					editorM->SetDominantVertex(HitP,pFace);
				}
				else
				{
					if (m_nSelMode == SELECT_ADD)
						editorM->AddVertexFromTriangle(HitP,pFace);
					else
						editorM->DeleteVertexFromTriangle(HitP,pFace);
				}
			}
			++nbHits;
		}
	}

	SelectTriangle( nSelTri );
	
	return nbHits;
}
void Selection::SetZBufferTriangles( D3DXVECTOR3 camera )
{
	D3DXVECTOR3 coords[3];

	for (int nTri = 0; nTri < ObOBJ->GetNumTriangles(); nTri++ )
	{
		ObOBJ->GetTriangle(nTri, coords);

		if ( D3DXVec3Dot(&ObOBJ->GetNormalsFace(nTri), &(camera - coords[0])) < 0 ) 
			m_pTriBackFacing[ nTri ] = TF_BACKFACING;
		else
			m_pTriBackFacing[ nTri ] = NTF_BACKFACING;
	}
}

