#include "../stdafx.h"
#include "ModelManager.h"
#include "../Render/CDirectX.h"

ModelManager::ModelManager()
{
	m_IsReadingScene = false;
	m_Actor = NULL;
}

ModelManager::~ModelManager()
{
	m_Actor = NULL;
}

void ModelManager::Render(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3 pan )
{
	if (m_Actor != NULL)
		m_Actor->Render(Device, pan);

	for(TScenaryIterator it = m_Scenary.begin(); it != m_Scenary.end(); ++it)
	{
		(*it->second).Render(Device);	
	}
}

void ModelManager::Load(std::string xmlFile, bool IsScene)
{
	//Llegeix el fitxer XML
	m_IsReadingScene = IsScene;
	xmlParseFile(xmlFile);
}

void ModelManager::onStartElement(const std::string &elem, MKeyValue &atts)
{
	if (!m_IsReadingScene)
	{
		if ( elem == "ACTOR" )
		{
			std::string s_path = atts["path"];
			m_MusclePath = atts["muscles"];
			m_ExpressionPath = atts["expressions"];

			if (s_path.find(".obj") != -1 || s_path.find(".OBJ") != -1)
			{
				m_Actor = new Actor((char*)s_path.c_str(),TIPUS_OBJ);
			}
			else if (s_path.find(".3ds") != -1 || s_path.find(".3DS") != -1)
			{
				m_Actor = new Actor((char*)s_path.c_str(),TIPUS_3DS);
			}
			else if (s_path.find(".x") != -1 || s_path.find(".X") != -1)
			{
				m_Actor = new Actor((char*)s_path.c_str(),TIPUS_X);
			}
		}

		if ( elem == "SCENARY" )
		{
			std::string s_name = atts["name"];
			if (m_Scenary[s_name] == NULL)
			{
				std::string s_path = atts["path"];
				m_Scenary[s_name] = new Scenary(s_name, s_path);
			}
		}
	} else {
		if ( elem == "ACTOR" )
		{
			if (m_Actor != NULL)
			{
				std::string posX = atts["posX"];
				std::string posY = atts["posY"];
				std::string posZ = atts["posZ"];

				D3DXVECTOR3 position;
				sscanf_s(posX.c_str(), "%f", &position.x);
				sscanf_s(posY.c_str(), "%f", &position.y);
				sscanf_s(posZ.c_str(), "%f", &position.z);

				m_Actor->SetPosition(position);
			}
		}

		if ( elem == "SCENARY" )
		{
			std::string name = atts["name"];
			if (m_Scenary[name] != NULL)
			{
				std::string posX = atts["posX"];
				std::string posY = atts["posY"];
				std::string posZ = atts["posZ"];

				D3DXVECTOR3 position;
				sscanf_s(posX.c_str(), "%f", &position.x);
				sscanf_s(posY.c_str(), "%f", &position.y);
				sscanf_s(posZ.c_str(), "%f", &position.z);

				m_Scenary[name]->SetPosition(position);
			}
		}
	}
}

