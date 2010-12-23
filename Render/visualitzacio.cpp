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
#include "../constants.h"
#include "../Logic/Rigging/Selection.h"
#include "../Logic/Muscles/MuscleManager.h"

// TEXTURES: Vector de noms de textura
GLuint textures[NUM_MAX_TEXTURES]={0,1,2,3,4,5,6,7,8,9};

GLdouble projectionMatrix[16];
GLint viewportMatrix[4];
GLdouble ModelViewMatrix[16];
TypeMuscle muscle = NONE_MUSCLE;
float wx1 = 0.0,wy1 = 0.0,wx2 = 0.0, wy2 = 0.0;

GLdouble* GetProjectionMatrix (void)
{
	return projectionMatrix;
}

GLdouble* GetModelviewMatrix (void)
{
	return ModelViewMatrix;
}

GLint* GetViewportMatrix (void)
{
	return viewportMatrix;
}

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
void Perspectiva(float anglex,float angley,float R,char VPol,bool pant,D3DXVECTOR3 tr,
				 CColor col_fons,char objecte,bool TR, 
				 CPunt3D VScl,CPunt3D VTr, CPunt3D VRot,bool oculta,bool testv,
				 bool bck_ln,bool filferros,bool textur,bool ifix,bool eix,
				 EditorManager* EdManager, Actor* ObOBJ, MuscleManager* MManager, bool flags,
				 CSubtitles* MSubtitles, bool subtitles, CParla* parla)
{    
	D3DXVECTOR3 cam,up;
	CDirectX* l_DX = CDirectX::GetInstance();
	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();

// Conversió angles radians -> graus
   	angley=angley*2*pi/360;
    anglex=anglex*2*pi/360;
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

// Fons: Dibuixa el fons variable sefons el color int_fons
void Fons(CColor int_fons)
{
	glClearColor(int_fons.r,int_fons.g,int_fons.b,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();

}

// FonsN: Dibuixa el fons negre
void FonsN()
{
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glFlush();

}

// FonsB: Dibuixa el fons blanc
void FonsB()
{
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();

}
// TEXTURES------------------------------------------------------
// loadIMA: This function receives as input the image filename and an 
// integer identifier (0 for the first texture) and creates an OpenGL 
// texture which is stored in the global array 'textures'
// Paràmetres:
//		- filename: Fitxer que conté la imatge de qualsevol format:
//					BMP, JPG, TIFF, TGA, GIF, i d'altres suportats per OpenIL
//		- texID: Identificador dins la taula textures on volem
//                assignar la imatge
bool loadIMA(CString filename, int texID)				
{
	FILE *file=NULL;
	int errno;
	
// Open the image file for reading
// file=fopen(filename,"r");					// Funció Visual Studio 6.0
   errno=fopen_s(&file,filename,"r");			// Funció Visual 2005

// If the file is empty (or non existent) print an error and return false
// if (file == NULL)
   if (errno!=0)
 {
//	printf("Could not open file '%s'.\n",filename) ;

	 return false ;
 }

// Close the image file
 fclose(file);

// ilutGLLoadImage: Funció que llegeix la imatge del fitxer filename
//				si és compatible amb els formats DevIL/OpenIL (BMP,JPG,GIF,TIF,TGA,etc.)
//				i defineix la imatge com a textura OpenGL retornant l'identificador 
//				de textura OpenGL.
// GetBuffer: Funció de converió d'una variable CString -> char *
textures[texID] = ilutGLLoadImage(filename.GetBuffer(3));

// If execution arrives here it means that all went well. Return true
 return true;

}
