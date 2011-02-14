#ifndef PARLA_H
#define PARLA_H

#include "../stdafx.h"
#include "Subtitles.h"
#include "../Animacio/Animation.h"
#include "../Logic/Expressions/Expression.h"
#include "../Models/Actor/Actor.h"
#include "../Render/CDirectX.h"
#include <vector>

class CParla
{
private:

	static CParla*	m_Parla;

	Animation*		animacio;
	CSubtitles*		m_Subtitles;

	//char*			text;
	std::vector<std::string> m_Text;
	int				m_FraseActual;

	bool			parlant;
	int				index;
	TypeExpression	lastExpression;

	//Temps d'animaci�
	float			transitionTime;
	float			totalTime;

	TypeExpression		ParseCharacter	( const char c );

public:

	CParla();
	~CParla();

	static CParla*	GetInstance();
	void			CleanUp();

	void		SetVelocity			( float transitionT, float totalT );
	void		SetTextToTalk		( std::string filename );
	
	int			GetNumSubs			() { return m_Text.size(); }
	void		ResetSubs			() { m_FraseActual = -1; }

	void		StartTalk			(Actor* obj);
	void		NextTalk			(Actor* obj);
	void		FinalizeTalk		();
	bool		IsTalking			();
	void		TalkElapsed			(Actor* obj);
	
	void		PrepareFontSubs		( std::string filename );
	void		RenderSubs			( LPDIRECT3DDEVICE9 Device );
	
};

#endif