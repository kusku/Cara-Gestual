//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn b�sic VS2005 MONOFINESTRA amb interf�cie MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Febrer 2010)
// escena.h : interface de escena.cpp
//

#ifndef ESCENA_H
#define ESCENA_H

#include "../SPoint3D.h"
#include "../Seleccions/Selection.h"
#include "../Models/Objecte3D.h"
#include "../Muscles/MuscleManager.h"
#include "../Seleccions/EditorManager.h"
#include "../Render/CDirectX.h"


/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
void renderSphereSelection (LPDIRECT3DDEVICE9 Device, CDirectX* DX,EditorManager* EdManager, TypeMuscle muscle);
//void drawSelectionBox	(float x1, float y1, float x2, float y2);
//void WireRect( float x1, float y1, float x2, float y2 );
//void SetProjection3D();
//void SetProjection2D();
//void DrawLine( const SPoint3D &P1, const SPoint3D &P2 );
#endif