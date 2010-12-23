#ifndef ANIMATION_H
#define ANIMATION_H

#include "../Expressions/ExpressionManager.h"
#include "../Muscles/MuscleManager.h"
#include "../SPoint3D.h"
#include "../Models/Objecte3D.h"

class Animation
{
private:

	int sizeExpression;
	int transitionTime;
	float totalTime;
	float divisionTime;
	int step;
	bool animationActive;

	TypeExpression expression;

	SPoint3D* totalMovement;
	SPoint3D* partialMovement;
	SPoint3D* portionMovement;
	SPoint3D* backMovement;

	ExpressionManager* EManager;
	MuscleManager* MManager;

public:

	Animation(ExpressionManager* manager, MuscleManager* muscle);
	~Animation();

	void		SetTime				( int transitionTime, float totalTime );
	bool		IsActiveAnimation	( void );

	void		StartAnimation		( TypeExpression expression, Objecte3D* obj );
	void		NextStepAnimation	( void );
	void		FinalizeAnimation	( void );

	void		Render				( Objecte3D* obj );
};

#endif