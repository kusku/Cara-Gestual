#ifndef PARLA_H
#define PARLA_H

#include "../stdafx.h"
#include "Subtitles.h"
#include "../Animacio/Animation.h"
#include "../Logic/Expressions/Expression.h"
#include "../Models/Actor/Actor.h"

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
	void		StartTalk			(Actor* obj);
	void		NextTalk			(Actor* obj);
	void		FinalizeTalk		();
	bool		IsTalking			();
	void		TalkElapsed			(Actor* obj);
	
};

#endif