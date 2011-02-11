#include "../../stdafx.h"
#include "ExpressionManager.h"
#include "../Muscles/MuscleManager.h"
#include "Expression.h"
#include "../../Render/CDirectX.h"

ExpressionManager::ExpressionManager()
{
	Expressions = (Expression**) malloc (NEXPRESSIONS*sizeof(Expression));
	for (int i=0; i<NEXPRESSIONS; ++i)
	{
		Expressions[i] = new Expression();
	}
}

ExpressionManager::~ExpressionManager()
{
}

ExpressionManager* ExpressionManager::m_ExpressionManager = NULL;

ExpressionManager* ExpressionManager::GetInstance()
{
	if (m_ExpressionManager == NULL)
		m_ExpressionManager = new ExpressionManager();

	return m_ExpressionManager;
}

void ExpressionManager::CleanUp()
{
	for (short int i = 0; i < NEXPRESSIONS; ++i)
		CHECKED_DELETE(Expressions[i]);

	delete [] Expressions;

	delete m_ExpressionManager;
}

void ExpressionManager::resetExpression( TypeExpression nameExpression )
{
	if (nameExpression != NONE_EXPRESSION)
		Expressions[nameExpression]->resetMuscles();	
}

void ExpressionManager::modifyMuscleExpression(TypeExpression nameExpression, TypeMuscle nameMuscle, D3DXVECTOR3 movement)
{
	if (nameExpression != NONE_EXPRESSION && nameMuscle != NONE_MUSCLE)
		Expressions[nameExpression]->modifyMuscle(nameMuscle, movement);
}

void ExpressionManager::RenderExpression( TypeExpression nameExpression, Actor* obj  )
{
	if (nameExpression != NONE_EXPRESSION)
	{
		Expressions[nameExpression]->RenderExpression();
		obj->LoadVertexsBuffers(CDirectX::GetInstance()->GetDevice());
	}
}

void ExpressionManager::ExternalRender(TypeExpression nameExpression, D3DXVECTOR3 *newMovements, Actor* obj)
{
	if (nameExpression != NONE_EXPRESSION)
	{
		Expressions[nameExpression]->ExternalRender(newMovements);
		obj->LoadVertexsBuffers(CDirectX::GetInstance()->GetDevice());
	}
}

void ExpressionManager::Load(std::string xmlFile)
{
	//Llegeix el fitxer XML
	xmlParseFile(xmlFile);
}

void ExpressionManager::onStartElement(const std::string &elem, MKeyValue &atts)
{
	if (searchExpression(elem) != NONE_EXPRESSION)
	{
		std::string s_muscle = atts["muscle"];
		std::string s_posX = atts["posX"];
		std::string s_posY = atts["posY"];
		std::string s_posZ = atts["posZ"];

		D3DXVECTOR3 vector;
		sscanf_s(s_posX.c_str(), "%f", &vector.x);
		sscanf_s(s_posY.c_str(), "%f", &vector.y);
		sscanf_s(s_posZ.c_str(), "%f", &vector.z);

		modifyMuscleExpression(searchExpression(elem),MuscleManager::GetInstance()->searchMuscle(s_muscle),vector);
	}
}

TypeExpression ExpressionManager::searchExpression(std::string m)
{
	if (m == "TRIST") return TRIST;
	if (m == "ALEGRE") return ALEGRE;
	if (m == "ENFADAT") return ENFADAT;
	if (m == "SERIOS") return SERIOS;
	if (m == "SORPRES") return SORPRES;
	if (m == "A") return A;
	if (m == "E") return E;
	if (m == "I") return I;
	if (m == "O") return O;
	if (m == "U") return U;
	if (m == "BILABIAL") return BILABIAL;
	if (m == "NEUTRE") return NEUTRE;
	return NONE_EXPRESSION;
}