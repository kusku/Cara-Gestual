#include "../stdafx.h"
#include "Animation.h"
#include "../Models/Actor/Actor.h"

Animation::Animation()
{
	step = 0;
	transitionTime = 4;
	totalTime = 0.5;
	divisionTime = 0.f;

	animationActive = false;

	expression = NONE_EXPRESSION;

	sizeExpression = MuscleManager::GetInstance()->getNumMuscles();
	totalMovement = new D3DXVECTOR3 [sizeExpression];
	partialMovement = new D3DXVECTOR3 [sizeExpression];
	portionMovement = new D3DXVECTOR3[sizeExpression];
	backMovement = new D3DXVECTOR3[sizeExpression];

	for (int i=0; i<sizeExpression; ++i)
	{
		totalMovement[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		partialMovement[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		backMovement[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

Animation::~Animation()
{
}

Animation* Animation::m_Animation = NULL;

Animation* Animation::GetInstance()
{
	if (m_Animation == NULL)
		m_Animation = new Animation();

	return m_Animation;
}

void Animation::CleanUp()
{
	delete [] totalMovement;
	delete [] partialMovement;
	delete [] portionMovement;
	delete [] backMovement;

	delete m_Animation;
}

void Animation::SetTime(float transitionTime, float totalTime)
{
	this->transitionTime = transitionTime;
	this->totalTime = totalTime;
}

void Animation::StartAnimation(TypeExpression expression, Actor* obj)
{
	step = 0;
	if (expression != NONE_EXPRESSION)
	{
		divisionTime = totalTime / ((float)transitionTime*0.001f);

		this->expression = expression;
		for (int i=0; i < sizeExpression; ++i)
		{
			partialMovement[i] = ExpressionManager::GetInstance()->getExpressionList()[expression]->getMovement( (TypeMuscle)i ) - totalMovement[i];
			totalMovement[i] = ExpressionManager::GetInstance()->getExpressionList()[expression]->getMovement( (TypeMuscle)i );

			partialMovement[i] /= divisionTime;
			portionMovement[i] = partialMovement[i];
		}
		Render(obj);
	}
	
	animationActive = true;
	++step;
}

void Animation::NextStepAnimation()
{
	++step;
	for (int i=0; i<sizeExpression; ++i)
	{
		partialMovement[i] += portionMovement[i];
	}
}

void Animation::FinalizeAnimation()
{
	for (int i=0; i<sizeExpression; ++i)
	{
		backMovement[i] = totalMovement[i];
	}

	animationActive = false;
	step = 0;
}

bool Animation::IsActiveAnimation( void )
{
	return animationActive;
}

void Animation::Render(Actor* obj)
{
	D3DXVECTOR3* actualMovement = new D3DXVECTOR3 [sizeExpression];

	for (int i=0; i<sizeExpression; ++i)
	{
		actualMovement[i] = backMovement[i] + partialMovement[i];
	}

	if (divisionTime > (float) step)
	{
		ExpressionManager::GetInstance()->ExternalRender(expression,actualMovement, obj);
	}
	
	delete []actualMovement;
}