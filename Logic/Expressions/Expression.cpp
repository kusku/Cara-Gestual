#include "../../stdafx.h"
#include "Expression.h"
#include "../Muscles/MuscleManager.h"

Expression::Expression()
{
	movements = (D3DXVECTOR3*) malloc (MuscleManager::GetInstance()->getNumMuscles()*sizeof(D3DXVECTOR3));
	resetMuscles();
}

Expression::~Expression()
{
	delete[] movements;
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
	int numMuscles = MuscleManager::GetInstance()->getNumMuscles();
	for (int i=0; i<numMuscles; ++i)
	{
		movements[i] = D3DXVECTOR3(0.f,0.f,0.f);
	}
}

void Expression::RenderExpression( void )
{
	int numMuscles = MuscleManager::GetInstance()->getNumMuscles();
	for (int i=0; i<numMuscles; ++i)
	{
		MuscleManager::GetInstance()->moveAMuscle((TypeMuscle)i, movements[i]);
	}
}

void Expression::ExternalRender(D3DXVECTOR3* newNovements)
{
	int numMuscles = MuscleManager::GetInstance()->getNumMuscles();
	for (int i=0; i<numMuscles; ++i)
	{
		MuscleManager::GetInstance()->moveAMuscle((TypeMuscle)i, newNovements[i]);
	}
}