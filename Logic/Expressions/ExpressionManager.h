#ifndef EXPMANAGER_H
#define	EXPMANAGER_H

#include <vector>
#include "Expression.h"
#include "../Muscles/MuscleManager.h"
#include "../../Models/Actor/Actor.h"
#include "../../Common/ParserXML/XMLParser.h"

#define NEXPRESSIONS 12
//NONE �s l'�ltim element ja que l'array arriba a l'element N-1.
//NONE �s per definir una expressi� quan no hi ha cap expressi� seleccionada.

enum TypeExpression {TRIST, ALEGRE, ENFADAT, SERIOS, SORPRES, A, E, I, O, U, BILABIAL, NEUTRE, NONE_EXPRESSION};

class ExpressionManager : public CXMLParser
{
private:
	
	static ExpressionManager* m_ExpressionManager;

	Expression**	Expressions;
	int				numExpressions;

	void onStartElement	( const std::string &elem, MKeyValue &atts );
	TypeExpression searchExpression ( std::string m );

public:
	
	ExpressionManager();
	~ExpressionManager();

	static ExpressionManager*	GetInstance ();
	void						CleanUp		();
	
	//Posa tots els muscles de l'expressi� sense moviment
	void			resetExpression			( TypeExpression nameExpression );

	//Modifica el moviment d'un muscle que pertany a una expressi� concreta
	void			modifyMuscleExpression	( TypeExpression nameExpression, TypeMuscle nameMuscle, D3DXVECTOR3 movement );
	
	// Mou els muscles de l'expressi� desitjada
	void			RenderExpression		( TypeExpression nameExpression, Actor* obj );
	void			ExternalRender			( TypeExpression nameExpression, D3DXVECTOR3* newMovements, Actor* obj);

	// Carrega el fitxer XML
	void			Load				(std::string xmlFile );

	Expression**	getExpressionList		( void ) { return Expressions; }
	int				getNumExpressions		( void ) { return NEXPRESSIONS; }

};

#endif