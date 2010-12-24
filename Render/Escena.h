//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic VS2005 MONOFINESTRA amb interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Febrer 2010)
// escena.h : interface de escena.cpp
//

#ifndef ESCENA_H
#define ESCENA_H

#include "../SPoint3D.h"
#include "../Logic/Rigging/Selection.h"
#include "../Models/Actor/Actor.h"
#include "../Logic/Muscles/MuscleManager.h"
#include "../Logic/Rigging/EditorManager.h"
#include "../Render/CDirectX.h"


/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
void renderSphereSelection (LPDIRECT3DDEVICE9 Device, CDirectX* DX,EditorManager* EdManager, TypeMuscle muscle);
//void drawSelectionBox	(float x1, float y1, float x2, float y2);
//void WireRect( float x1, float y1, float x2, float y2 );
//void SetProjection3D();
//void SetProjection2D();
#endif