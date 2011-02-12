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
}

MuscleManager* MuscleManager::m_MuscleManager = NULL;

MuscleManager* MuscleManager::GetInstance()
{
	if (m_MuscleManager == NULL)
			m_MuscleManager = new MuscleManager();

	return m_MuscleManager;
}

void MuscleManager::CleanUp()
{
	for (short int i = 0; i < NMUSCLES; ++i)
		CHECKED_DELETE(muscles[i]);

	delete [] muscles;

	delete m_MuscleManager;
}

void MuscleManager::addVertexMuscle(TypeMuscle numMuscle, unsigned int vertex, float delta)
{
	muscles[numMuscle]->addVertex(vertex, delta );
}

void MuscleManager::moveAMuscle( TypeMuscle numMuscle, D3DXVECTOR3 vecDir )
{
	muscles[numMuscle]->moveMuscle(vecDir);
}

void MuscleManager::deleteMuscle( TypeMuscle numMuscle )
{
	for (unsigned int i=0; i< (unsigned int)muscles[numMuscle]->getNumVertexs(); ++i)
	{
		muscles[numMuscle]->deleteVertex(i);
	}
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