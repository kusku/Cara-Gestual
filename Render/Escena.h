//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic VS2005 MONOFINESTRA amb interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Febrer 2010)
// escena.h : interface de escena.cpp
//

#ifndef ESCENA_H
#define ESCENA_H

#include "../SPoint3D.h"
#include "../Seleccions/Selection.h"
#include "../lectorsModels/Objecte3D.h"
#include "../Muscles/MuscleManager.h"
#include "../Seleccions/EditorManager.h"

class Escena
{
private:

	static Escena*		m_Escena;

	void		DrawLine(const D3DXVECTOR3 &PosA, const D3DXVECTOR3 &PosB, DWORD Color=0xffffffff);

public:
	
	Escena();
	~Escena();

	static Escena*		GetInstance			();

};

/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
//void renderSphereSelection (EditorManager* EdManager, TypeMuscle muscle);
//void drawSelectionBox	(float x1, float y1, float x2, float y2);
//void WireRect( float x1, float y1, float x2, float y2 );
//void SetProjection3D();
//void SetProjection2D();
//void DrawLine( const SPoint3D &P1, const SPoint3D &P2 );
#endif