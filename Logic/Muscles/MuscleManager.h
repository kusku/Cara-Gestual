#ifndef MUSCLEMANAGER_H
#define MUSCLEMANAGER_H

#include <vector>
#include "../../Models/Actor/Actor.h"
#include "Muscle.h"
#include "../../../include/expat/XMLParser.h"

#define NMUSCLES 14
//NONE és l'últim element ja que l'array arriba a l'element N-1.
//NONE és per definir un muscle quan no hi ha cap muscle seleccionada.
enum TypeMuscle {ECELLA, DCELLA, INFBOCA, EBOCA, DBOCA, EPARPELLA, DPARPELLA, EGALTA, DGALTA, SUPBOCA, LATEBOCA, LATDBOCA, DENTDALT, DENTBAIX, NONE_MUSCLE};

class MuscleManager : public CXMLParser
{
private:

	Muscle**	muscles;
	Actor* obj3D;

	void onStartElement	( const std::string &elem, MKeyValue &atts );

public:

	MuscleManager();
	~MuscleManager();

	void			addVertexMuscle		( TypeMuscle numMuscle, unsigned int vertex, float delta );
	void			moveAMuscle			( TypeMuscle numMuscle, D3DXVECTOR3 vecDir );
	void			deleteMuscle		( TypeMuscle numMuscle );
	void			ClearMuscle			( TypeMuscle numMuscle );
	void			SetModel			( Actor* ObOBJ );
	void			Load				(std::string xmlFile );

	TypeMuscle		searchMuscle		( std::string m );
	
	Muscle**		getMuscleList		( void );
	int				getNumMuscles		( void );
	
};

#endif