#ifndef ANIMATION_H
#define ANIMATION_H

#include "../Logic/Expressions/ExpressionManager.h"
#include "../Logic/Muscles/MuscleManager.h"
#include "../Models/Actor/Actor.h"

class Animation
{
private:

	static Animation* m_Animation;

	int sizeExpression;
	float transitionTime;
	float totalTime;
	float divisionTime;
	int step;
	bool animationActive;

	TypeExpression expression;

	D3DXVECTOR3* totalMovement;
	D3DXVECTOR3* partialMovement;
	D3DXVECTOR3* portionMovement;
	D3DXVECTOR3* backMovement;

public:

	Animation();
	~Animation();

	static Animation*	GetInstance ();
	void				CleanUp		();

	void		SetTime				( float transitionTime, float totalTime );
	bool		IsActiveAnimation	( void );

	void		StartAnimation		( TypeExpression expression, Actor* obj );
	void		NextStepAnimation	( void );
	void		FinalizeAnimation	( void );

	void		Render				( Actor* obj );
};

#endif