#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <vector>
#include <map>
#include <string>
#include "Actor/Actor.h"
#include "Scenary/Scenary.h"
#include "../Common/ParserXML/XMLParser.h"

class ModelManager : public CXMLParser
{

private:
	
	bool m_IsReadingScene; //Identifica si s'està llegint una escena o models en XML

	Actor* m_Actor;

	typedef std::map<std::string, Scenary*>	TScenary;
	typedef TScenary::iterator	TScenaryIterator;
	TScenary m_Scenary;

	std::string m_MusclePath;
	std::string m_ExpressionPath;

	D3DXVECTOR3 m_Pan;

	void		onStartElement	( const std::string &elem, MKeyValue &atts );

public:

	ModelManager();
	~ModelManager();

	// Carrega els XML dels models
	void		Load		(std::string xmlFile, bool IsScene = false );
	void		Render		(LPDIRECT3DDEVICE9 Device);

	Actor*		GetActor			() {return m_Actor; };
	std::string GetMusclePath		() {return m_MusclePath; };
	std::string GetExpressionPath	() {return m_ExpressionPath; };
};

#endif