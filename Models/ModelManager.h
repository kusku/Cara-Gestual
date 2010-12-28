#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <vector>
#include <map>
#include "Actor/Actor.h"
#include "Scenary/Scenary.h"
#include "../../include/expat/XMLParser.h"

class ModelManager : public CXMLParser
{

private:
	
	Actor*		m_Actor;
	std::map<std::string, Scenary*> m_Scenary;

	std::string m_MusclePath;
	std::string m_ExpressionPath;

	void		onStartElement	( const std::string &elem, MKeyValue &atts );

public:

	ModelManager();
	~ModelManager();

	// Carrega els XML dels models
	void		Load		(std::string xmlFile );
	void		Render		();

	Actor*		GetActor			() {return m_Actor; };
	std::string GetMusclePath		() {return m_MusclePath; };
	std::string GetExpressionPath	() {return m_ExpressionPath; };
};

#endif