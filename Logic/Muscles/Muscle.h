#ifndef MUSCLE_H
#define MUSCLE_H

#include "../../SPoint3D.h"
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
	void			moveMuscle		( Actor* obj3D, SPoint3D vecDir );
	int				alterDelta		( unsigned int vertex, float newDelta );
	int				deleteVertex	( unsigned int vertex );
	void			ClearMuscle		( void );
	void			RenderMuscle	( Actor* obj3D );

	unsigned int*	getVertexIndex	( void );
	float*			getVertexDelta	( void );
	int				getNumVertexs	( void );
};

#endif