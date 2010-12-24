//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic VS2005 MONOFINESTRA amb interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Febrer 2010)
// visualitzacio.cpp : Funcions de visualització i pantalla
// FUNCIONS:	- Iluminació (Ilumina)
//				- Projecció Ortogràfica (ProjeccioOrto i Ortografica)
//				- Projecció Perspectiva (ProjeccioPerspectiva,Perspectiva)
//				- Dibuixar els eixos (deixos)
//				- Dibuixar fons: blanc (FonsW) i negre (Fons)
//				- Carregar imatges textura (loadBMP)
//				- Carregar imatges de formats compatibles amb DevIL/OpenIL com a textura (loadIMA)
//				- Inicialitzar imatges textura per l'objecte Truck
//

#include "../stdafx.h"
#include "visualitzacio.h"
#include "escena.h"
#include "../defines.h"
#include "../Logic/Rigging/Selection.h"
#include "../Logic/Muscles/MuscleManager.h"

TypeMuscle muscle = NONE_MUSCLE;
float wx1 = 0.0,wy1 = 0.0,wx2 = 0.0, wy2 = 0.0;


void SetRenderMuscle ( TypeMuscle numMuscle )
{
	muscle = numMuscle;
}

void RenderBox (float x1, float y1, float x2, float y2)
{
	wx1 = x1; wy1 = y1; wx2 = x2;wy2 = y2;
}

// Perspectiva: Definició gluLookAt amb possibilitat de moure 
//				el punt de vista interactivament amb el ratolí, 
//				ilumina i dibuixa l'escena
void Perspectiva(float anglex,float angley,float R,char VPol,bool pant,D3DXVECTOR3 tr, bool TR, 
				 D3DXVECTOR3 VScl,D3DXVECTOR3 VTr, D3DXVECTOR3 VRot,bool oculta,bool testv,
				 bool bck_ln,bool filferros,bool textur,bool ifix,bool eix,
				 EditorManager* EdManager, Actor* ObOBJ, MuscleManager* MManager, bool flags,
				 CSubtitles* MSubtitles, bool subtitles, CParla* parla)
{    
	D3DXVECTOR3 cam,up;
	CDirectX* l_DX = CDirectX::GetInstance();
	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();

// Conversió angles radians -> graus
   	angley=angley*2*D3DX_PI/360;
    anglex=anglex*2*D3DX_PI/360;
//    R=300.0+zoom; 
	if(R<1.0) R=1.0;

	// Posició càmera i vector cap amunt
  /*  cam[0]=R*cos(angley)*cos(anglex);
	cam[1]=R*sin(angley)*cos(anglex);
	cam[2]=R*sin(anglex);
    up[0]=-cos(angley)*sin(anglex);
	up[1]=-sin(angley)*sin(anglex);
	up[2]=cos(anglex);*/

	if (VPol==POLARZ) { cam[0]=R*cos(angley)*cos(anglex);
						cam[1]=R*sin(angley)*cos(anglex);
						cam[2]=R*sin(anglex);
						up[0]=-cos(angley)*sin(anglex);
						up[1]=-sin(angley)*sin(anglex);
						up[2]=cos(anglex);	}
		else if (VPol==POLARY) {	cam[0]=R*sin(angley)*cos(anglex);
									cam[1]=R*sin(anglex);
									cam[2]=R*cos(angley)*cos(anglex);
									up[0]=-sin(angley)*sin(anglex);
									up[1]=cos(anglex);
									up[2]=-cos(angley)*sin(anglex);	}
			else {	cam[0]=R*sin(anglex);
					cam[1]=R*cos(angley)*cos(anglex);
					cam[2]=R*sin(angley)*cos(anglex);
					up[0]=cos(anglex);
					up[1]=-cos(angley)*sin(anglex);
					up[2]=-sin(angley)*sin(anglex);	}

	// Especificació del punt de vista
	l_DX->SetEye(cam);
	l_DX->SetUpVector(up);
	l_DX->SetLookAt(D3DXVECTOR3(0.f, 0.f, 0.f));
	
	//Habilita o no el ZBuffer
	l_DX->SetZBuffer(oculta);

	//Estableix la visiualització del mode del model (FILFERROS o SOLID)
	l_DX->SetPaintSolid(!filferros);

	//Estableix el test de visibilitat (Culling Face)
	l_DX->SetCullingFace(testv);

	//Activa la il·luminació
	if (ifix)
		l_DX->PointLight(D3DXVECTOR3(15.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f));
	else
		l_DX->PointLight(cam, D3DXVECTOR3(0.f, 0.f, 0.f));

	//Activa el Render en DirectX
	l_DX->BeginRenderDX();
	l_DX->SetupMatrices();

	//Establim matrius de transformacions
	{
		D3DXMATRIX l_Matrix;
		D3DXMatrixIdentity(&l_Matrix);
				
		if (pant)	//Fa una translació si el PAN està activat
			D3DXMatrixTranslation (&l_Matrix, tr.x, tr.y, tr.z);

		l_DX->GetDevice()->SetTransform(D3DTS_WORLD, &l_Matrix);
	}

	//Dibuixa els eixos de coordenades
	if (eix)
		l_DX->RenderAxis(8.0f);

	//Dibuixa el model
	if (ObOBJ != NULL)
		ObOBJ->Render(Device);	

	renderSphereSelection(Device, l_DX, EdManager, muscle);

	l_DX->RenderSquare(D3DXVECTOR2(wx1, wy1), D3DXVECTOR2(wx2, wy2));
	
//// Transformacions geomètriques sobre objecte (Traslació, Rotacions i Escalatge)
//	if (TR) 
//		{	glTranslatef(VTr.x,VTr.y,VTr.z);
//			glRotatef(VRot.x,1,0,0);
//			glRotatef(VRot.y,0,1,0);
//			glRotatef(VRot.z,0,0,1);
//			glScalef(VScl.x,VScl.y,VScl.z);
//		}
//	// RenderSelectedMuscle(muscle,MManager,ObOBJ);
//	renderSphereSelection(EdManager, muscle);
//
//	drawSelectionBox(wx1,wy1,wx2,wy2);
//
//	// Dibuixar els subtítols
//	if (subtitles && parla->IsTalking())
//	{
//		Iluminacio(FILFERROS,textur,objecte,bck_ln);
//		MSubtitles->RenderSubtitles();
//		Iluminacio(iluminacio,textur,objecte,bck_ln);
//	}

	CDirectX::GetInstance()->EndRenderDX();
}
