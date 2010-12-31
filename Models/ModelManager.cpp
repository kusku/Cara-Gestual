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
				std::string pos = atts["position"];
				std::string sca = atts["scale"];
				std::string rot = atts["rotation"];

				D3DXVECTOR3 position, rotation, scale;
				sscanf_s(pos.c_str(), "%f %f %f", &position.x, &position.y, &position.z);
				sscanf_s(sca.c_str(), "%f %f %f", &scale.x, &scale.y, &scale.z);
				sscanf_s(rot.c_str(), "%f %f %f", &rotation.x, &rotation.y, &rotation.z);

				m_Actor->SetPosition(position);
				m_Actor->SetRotation(rotation);
				m_Actor->SetScale(scale);
			}
		}

		if ( elem == "SCENARY" )
		{
			std::string name = atts["name"];
			if (m_Scenary[name] != NULL)
			{
				std::string pos = atts["position"];
				std::string sca = atts["scale"];
				std::string rot = atts["rotation"];

				D3DXVECTOR3 position, rotation, scale;
				sscanf_s(pos.c_str(), "%f %f %f", &position.x, &position.y, &position.z);
				sscanf_s(sca.c_str(), "%f %f %f", &scale.x, &scale.y, &scale.z);
				sscanf_s(rot.c_str(), "%f %f %f", &rotation.x, &rotation.y, &rotation.z);

				m_Scenary[name]->SetPosition(position);
				m_Scenary[name]->SetRotation(rotation);
				m_Scenary[name]->SetScale(scale);
			}
		}
	}
}

