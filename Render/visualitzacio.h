//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic VS2005 MONOFINESTRA amb interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Febrer 2010)
// visualitzacio.h : Interface de visualitzacio.cpp
//

#ifndef GLVIS_H
#define GLVIS_H

#include "../stdafx.h"
#include "../Logic/Rigging/Selection.h"
#include "../Logic/Muscles/MuscleManager.h"
#include "../Logic/Rigging/EditorManager.h"
#include "../Parla/Subtitles.h"
#include "../Parla/Parla.h"
#include "CDirectX.h"

/* ------------------------------------------------------------------------- */
/*                                Funcions                                   */
/* ------------------------------------------------------------------------- */

void RenderBox (float x1, float y1, float x2, float y2);
void SetRenderMuscle ( TypeMuscle numMuscle );

// G2: Projecció Perspectiva
void Perspectiva(float anglex,float angley,float R,char VPol,bool pant,D3DXVECTOR3 tr,
				 CColor col_fons,char objecte,bool TR,
				 CPunt3D VScl,CPunt3D VTr, CPunt3D VRot,bool oculta,bool testv,
				 bool bck_ln,bool filferros,bool textur,bool ifix,bool eix,
				 EditorManager* EdManager, Actor* ObOBJ, MuscleManager* MManager, bool flags,
				 CSubtitles* MSubtitles, bool subtitles, CParla* parla);


// G2: Esborrats de pantalla a fons variable (Fons), negre (Fons) o a blanc (FonsW)
void Fons(CColor int_fons);
void FonsB(void);
void FonsN(void);

// G2: Carregar fitxer imatge de formats admesos per la llibreria DevIL/OpenIL 
//		com a textura
bool loadIMA(CString filename, int texID);

GLdouble* GetProjectionMatrix (void);
GLint* GetViewportMatrix (void);
GLdouble* GetModelviewMatrix (void);

#endif