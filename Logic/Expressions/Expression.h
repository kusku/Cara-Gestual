#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../Muscles/MuscleManager.h"

class Expression
{
private:
	
	D3DXVECTOR3*	movements;

public:

	Expression();
	~Expression();

	void		resetMuscles		( void );
	void		modifyMuscle		( TypeMuscle muscle, D3DXVECTOR3 movement );
	D3DXVECTOR3		getMovement			( TypeMuscle muscle );

	// Mou tots els muscles per a generar l'expressió
	void		RenderExpression	( void );
	void		ExternalRender		( D3DXVECTOR3* newMovements );

};

#endif
