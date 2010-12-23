//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic VS2005 MONOFINESTRA amb interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Debora Gil, Enric Martí (Febrer 2010)
// escena.cpp : Aqui es on ha d'anar el codi de les funcions que 
//              dibuixin les escenes.

#include "../stdafx.h"
#include "escena.h"
#include "../PracticaView.h"
#include "visualitzacio.h"
#include "../Seleccions/Selection.h"
#include "../Models/Objecte3D.h"
#include "../Muscles/MuscleManager.h"
#include "../SPoint3D.h"

void renderSphereSelection (LPDIRECT3DDEVICE9 Device, CDirectX* DX, EditorManager* EdManager, TypeMuscle muscle)
{
	int tamany, dominant;
	SPoint3D* llista;

	if (EdManager != NULL && muscle != NONE_MUSCLE)
	{
		llista = EdManager->GetPointList(&tamany, &dominant);
		for (int i = 0; i < tamany; ++i)
		{
			D3DXMATRIX l_Matrix;
			D3DXMatrixIdentity(&l_Matrix);
			D3DXMatrixTranslation (&l_Matrix, llista[i].x, llista[i].y, llista[i].z);
			DX->GetDevice()->SetTransform(D3DTS_WORLD, &l_Matrix);

			if (i == dominant)
				DX->RenderSphere(0.05,0xffff0000,8);
			else
				DX->RenderSphere(0.05,0xffffffff,8);	
		}

		delete[] llista;
	}
}
//
//void drawSelectionBox (float x1, float y1, float x2, float y2)
//{
//	float m_nHeight = GetViewportMatrix()[3];
//	y1 = m_nHeight-y1;
//	y2 = m_nHeight-y2;
//
//	SetProjection2D();
//
//	glColor3f(1.0,1.0,1.0);
//	glPushMatrix();
//		glLineStipple( 2, 43690 );
//		glEnable( GL_LINE_STIPPLE);
//		WireRect(x1,y1,x2,y2);
//		glLineStipple( 1, 65535);
//		glDisable( GL_LINE_STIPPLE);
//	glPopMatrix();
//
//	SetProjection3D();
//}
//void WireRect( float x1, float y1, float x2, float y2 )
//{
//    DrawLine( SPoint3D(x1,y1,0), SPoint3D(x2,y1,0) );
//    DrawLine( SPoint3D(x2,y1,0), SPoint3D(x2,y2,0) );
//    DrawLine( SPoint3D(x1,y2,0), SPoint3D(x2,y2,0) );
//    DrawLine( SPoint3D(x1,y1,0), SPoint3D(x1,y2,0) );
//}
//
//void DrawLine( const SPoint3D &P1, const SPoint3D &P2 )
//{
//	glBegin( GL_LINES );
//	glVertex3f( P1.x, P1.y, P1.z );
//	glVertex3f( P2.x, P2.y, P2.z );
//	glEnd();
//}
//
//void SetProjection3D()
//{
//	float m_nWidth = GetViewportMatrix()[2];
//	float m_nHeight = GetViewportMatrix()[3];
//
//	glMatrixMode( GL_PROJECTION );
//	glLoadIdentity();	
//	gluPerspective( 60, (GLdouble)m_nWidth/(GLdouble)m_nHeight, 5.0f, 30000.0f );
//	glMatrixMode( GL_MODELVIEW );
//	glLoadIdentity();	
//}
//
//void SetProjection2D()
//{
//	float m_nWidth = GetViewportMatrix()[2];
//	float m_nHeight = GetViewportMatrix()[3];
//
//	glMatrixMode( GL_PROJECTION );
//	glLoadIdentity();	
//	gluOrtho2D( 0.0, (GLdouble)m_nWidth, 0.0, (GLdouble)m_nHeight );
//	glMatrixMode( GL_MODELVIEW );
//	glLoadIdentity();	
//}
