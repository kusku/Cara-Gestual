#include "../../stdafx.h"
#include "Muscle.h"
#include "MuscleManager.h"

MuscleManager::MuscleManager()
{
	muscles = (Muscle**) malloc (NMUSCLES*sizeof(Muscle));
	for(int i=0; i<NMUSCLES; ++i)
	{
		muscles[i] = new Muscle();
	}

}
MuscleManager::~MuscleManager()
{
	delete [] muscles;

}

void MuscleManager::addVertexMuscle(TypeMuscle numMuscle, unsigned int vertex, float delta)
{
	muscles[numMuscle]->addVertex(vertex, delta );
}

void MuscleManager::moveAMuscle( TypeMuscle numMuscle, D3DXVECTOR3 vecDir )
{
	muscles[numMuscle]->moveMuscle(obj3D,vecDir);
}

void MuscleManager::deleteMuscle( TypeMuscle numMuscle )
{
	for (unsigned int i=0; i< (unsigned int)muscles[numMuscle]->getNumVertexs(); ++i)
	{
		muscles[numMuscle]->deleteVertex(i);
	}
}

Muscle** MuscleManager::getMuscleList(void)
{
	return muscles;
}

int MuscleManager::getNumMuscles ()
{
	return NMUSCLES;
}

void MuscleManager::SetModel( Actor* ObOBJ )
{
	this->obj3D = ObOBJ;
}

void MuscleManager::ClearMuscle( TypeMuscle numMuscle )
{
	muscles[numMuscle]->ClearMuscle();
}

void MuscleManager::Load(std::string xmlFile)
{
	//Llegeix el fitxer XML
	xmlParseFile(xmlFile);
}

void MuscleManager::onStartElement(const std::string &elem, MKeyValue &atts)
{
	if (searchMuscle(elem) != NONE_MUSCLE)
	{
		std::string s_vertex = atts["vertex"];
		std::string s_delta = atts["delta"];

		int n_vertex;
		float n_delta;

		sscanf_s(s_vertex.c_str(),"%d",&n_vertex);
		sscanf_s(s_delta.c_str(),"%f",&n_delta);
		addVertexMuscle(searchMuscle(elem),n_vertex, n_delta);
	}
}

TypeMuscle MuscleManager::searchMuscle( std::string m )
{
	if (m == "ECELLA") return ECELLA;
	if (m == "DCELLA") return DCELLA;
	if (m == "INFBOCA") return INFBOCA;
	if (m == "EBOCA") return EBOCA;
	if (m == "DBOCA") return DBOCA;
	if (m == "EPARPELLA") return EPARPELLA;
	if (m == "DPARPELLA") return DPARPELLA;
	if (m == "EGALTA") return EGALTA;
	if (m == "DGALTA") return DGALTA;
	if (m == "SUPBOCA") return SUPBOCA;
	if (m == "LATEBOCA") return LATEBOCA;
	if (m == "LATDBOCA") return LATDBOCA;
	if (m == "DENTDALT") return DENTDALT;
	if (m == "DENTBAIX") return DENTBAIX;
	return NONE_MUSCLE;
}