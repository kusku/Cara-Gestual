#include "../stdafx.h"
#include "ModelManager.h"


ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

void ModelManager::Load(std::string xmlFile)
{
	//Llegeix el fitxer XML
	xmlParseFile(xmlFile);
}

void ModelManager::onStartElement(const std::string &elem, MKeyValue &atts)
{
	if ( elem == "Actor" )
	{
		std::string s_path = atts["path"];
		m_MusclePath = atts["muscles"];
		m_ExpressionPath = atts["expressions"];

		if (s_path.find(".obj") || s_path.find(".OBJ"))
		{
			m_Actor = new Actor((char*)s_path.c_str(),TIPUS_OBJ);
		}
		else if (s_path.find(".3ds") || s_path.find(".3DS"))
		{
			m_Actor = new Actor((char*)s_path.c_str(),TIPUS_3DS);
		}
	}

	if ( elem == "Scenary" )
	{
		//TODO: Implementar el llegir un model de tipus escenari
	}
}

