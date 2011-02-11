#ifndef EDITOR_H
#define EDITOR_H

#include "../Muscles/MuscleManager.h"
#include "../Expressions/ExpressionManager.h"

class EditorManager
{
private:

	bool				*VertexList;
	float				*DeltaList;
	int					CurrentVertex;
	int					DominantVertex;
	TypeMuscle			CurrentMuscle;
	Actor*			objecte;

	void		CalculateDelta			( );
	D3DXVECTOR3	PuntMesProximTriangle	( D3DXVECTOR3 colisio, D3DXVECTOR3* triangle );

public:

	EditorManager(Actor* objecte);
	~EditorManager();
	
	// Roger
	// Metodes relacionats amb els muscles
	void		AddVertex				( D3DXVECTOR3 vertex );
	void		DeleteVertex			( D3DXVECTOR3 vertex );
	void		SetMuscle				( TypeMuscle m );
	void		SaveMuscle				( void );
	void		AddVertexFromTriangle	( D3DXVECTOR3 colisio, D3DXVECTOR3* triangle );
	void		DeleteVertexFromTriangle( D3DXVECTOR3 colisio, D3DXVECTOR3* triangle );
	void		SetDominantVertex		( D3DXVECTOR3 colisio, D3DXVECTOR3* triangle );
	void		ClearVertexs			( void );
	D3DXVECTOR3*	GetPointList			( int* tamany, int* dominant );
	D3DXVECTOR3	GetDominantVertex		( void );

	// Metodes relacionats amb les expresions
	void		DefineMovement	( TypeExpression expression, TypeMuscle muscle, D3DXVECTOR3 desplaçament );
};

#endif
