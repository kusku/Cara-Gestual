#ifndef MUSCLE_H
#define MUSCLE_H

#include "../../Models/Actor/Actor.h"

class Muscle
{
private:

	unsigned int*	vertexIndex;
	float*			vertexDelta;
	int				size;

	int				searchMuscle	(unsigned int vertex);

public:

	Muscle();
	~Muscle();

	int				addVertex		( unsigned int vertex, float delta );
	void			moveMuscle		( Actor* obj3D, D3DXVECTOR3 vecDir );
	int				alterDelta		( unsigned int vertex, float newDelta );
	int				deleteVertex	( unsigned int vertex );
	void			ClearMuscle		( void );

	unsigned int*	getVertexIndex	( void ) { return vertexIndex; }
	float*			getVertexDelta	( void ) {return vertexDelta; }
	int				getNumVertexs	( void ) { return size; }
};

#endif