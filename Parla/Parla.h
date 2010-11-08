#ifndef PARLA_H
#define PARLA_H

#include "../stdafx.h"
#include "Subtitles.h"
#include "../Animacio/Animation.h"
#include "../Expresions/Expression.h"
#include "../lectorsModels/Objecte3D.h"

class CParla
{
private:

	CSubtitles*		subtitols;
	Animation*		animacio;

	char*			text;
	bool			parlant;
	int				index;
	TypeExpression	lastExpression;

	//Temps d'animació
	float			transitionTime;
	float			totalTime;

	TypeExpression		ParseCharacter	( const char c );

public:

	CParla( Animation* an );
	~CParla();

	void		SetVelocity			( float transitionT, float totalT );
	void		SetTextToTalk		( char* text );
	void		StartTalk			(Objecte3D* obj);
	void		NextTalk			(Objecte3D* obj);
	void		FinalizeTalk		();
	bool		IsTalking			();
	void		TalkElapsed			(Objecte3D* obj);
	
};

#endif