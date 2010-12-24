#ifndef DEFORMATION_H
#define DEFORMATION_H

#include "../../Models/Actor/Actor.h"
#include "../Expressions/ExpressionManager.h"
#include "../Muscles/MuscleManager.h"
#include "EditorManager.h"

class Deformation
{
private:
	Actor* ObOBJ;
	ExpressionManager* EManager;
	EditorManager* editorM;
	TypeMuscle muscle;
	TypeExpression expression;
	D3DXVECTOR3 pla[4];
	D3DXVECTOR3 vecDir;
	D3DXVECTOR3 normalPla;

	void		GetLine				( D3DXVECTOR3 &L1, D3DXVECTOR3 &L2, float mouseX, float mouseY );

public:
	Deformation( ExpressionManager* manager, Actor* obj, EditorManager *editor );
	~Deformation();

	void	ButtonDown			( float mouseX, float mouseY, D3DXVECTOR3 opv );
	void	ButtonMove			( float mouseX, float mouseY );
	void	ButtonUp			( void );

	void	SetMuscle			( TypeMuscle muscle );
	void	SetExpression		( TypeExpression expression );
	void	SetObjecte			( Actor* Obj );

	

};

#endif