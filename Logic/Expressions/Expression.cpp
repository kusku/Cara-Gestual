#include "../../stdafx.h"
#include "Expression.h"
#include "../Muscles/MuscleManager.h"

Expression::Expression(MuscleManager* MMan)
{
	MManager = MMan;
	movements = (D3DXVECTOR3*) malloc (MManager->getNumMuscles()*sizeof(D3DXVECTOR3));
	resetMuscles();
}

Expression::~Expression()
{
	delete[] movements;
	MManager = NULL;
}

void Expression::modifyMuscle( TypeMuscle muscle, D3DXVECTOR3 movement )
{
	movements[muscle] = movement;
}

D3DXVECTOR3 Expression::getMovement( TypeMuscle muscle )
{
	return movements[muscle];
}

void Expression::resetMuscles( void )
{
	for (int i=0; i<MManager->getNumMuscles(); ++i)
	{
		movements[i] = D3DXVECTOR3(0.f,0.f,0.f);
	}
}

void Expression::RenderExpression( void )
{
	for (int i=0; i<MManager->getNumMuscles(); ++i)
	{
		MManager->moveAMuscle((TypeMuscle)i, movements[i]);
	}
}

void Expression::ExternalRender(D3DXVECTOR3* newNovements)
{
	for (int i=0; i<MManager->getNumMuscles(); ++i)
	{
		MManager->moveAMuscle((TypeMuscle)i, newNovements[i]);
	}
}