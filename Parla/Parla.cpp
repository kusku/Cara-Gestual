#include "../stdafx.h"
#include "Parla.h"
#include "Subtitles.h"
#include "../PracticaView.h"
#include "../Common/Timer/Timer.h"
#include <iostream>
#include <fstream>

CParla::CParla()
{
	parlant = false;
	index = 0;
	transitionTime = 1.f;
	totalTime = 0.01f;
	lastExpression = NONE_EXPRESSION;

	m_Subtitles = new CSubtitles();
}

CParla::~CParla()
{
}

CParla* CParla::m_Parla = NULL;

CParla* CParla::GetInstance()
{
	if (m_Parla == NULL)
		m_Parla = new CParla();

	return m_Parla;
}

void CParla::CleanUp()
{
	CHECKED_DELETE(m_Subtitles);

	delete m_Parla;
}

void CParla::SetTextToTalk(std::string filename)
{
	char textLlegit[1024];
	filebuf fb;
	
	fb.open(filename.c_str(),ios::in);
	istream is(&fb);
	is.getline(textLlegit,1024);
	text = std::string(textLlegit);
	m_Subtitles->SetText(text);
	
	fb.close();


	//FILE *fitxer;
	//errno_t err;
	//char textLlegit[100];

	//if( (err  = fopen_s(&fitxer,filename.c_str(),"r")) ==0 )
	//{
	//	fscanf_s(fitxer,"%s", textLlegit, _countof(textLlegit));	
	//	

	//	m_Subtitles->SetText(std::string(textLlegit));
	//	text = textLlegit;
	//}
	//fclose(fitxer);
}

void CParla::SetVelocity(float transitionT, float totalT)
{
	transitionTime = transitionT;
	totalTime = totalT;
}

void CParla::StartTalk(Actor* obj)
{
	TypeExpression expressio;
	index = 0;

	lastExpression = NONE_EXPRESSION;
	parlant = true;

	//Posa una expressió inicial
	Animation::GetInstance()->SetTime(transitionTime, totalTime);
	Animation::GetInstance()->StartAnimation(NEUTRE, obj);
	Animation::GetInstance()->FinalizeAnimation();

	if (text.empty() != true)
	{	
		do
		{
			expressio = ParseCharacter(text[index]);
			++index;
		}while (expressio == NONE_EXPRESSION && text[index] != NULL);

		if (text[index] != NULL)
		{
			Animation::GetInstance()->SetTime(transitionTime, totalTime);
			Animation::GetInstance()->StartAnimation(expressio, obj);
			lastExpression = expressio;
		}
	}
}

void CParla::NextTalk(Actor* obj)
{
	TypeExpression expressio;

	parlant = true;
	if (text[index] != NULL)
	{	
		do
		{
			expressio = ParseCharacter(text[index]);
			++index;
		}while (expressio == NONE_EXPRESSION && text[index] != NULL);

		if (text[index] != NULL)
		{
			if (lastExpression == expressio)
			{
				Animation::GetInstance()->SetTime(transitionTime, totalTime);
				Animation::GetInstance()->StartAnimation(NEUTRE, obj);	//Si es repeteix una mateix lletra, es posa entremig la neutre.
				lastExpression = NONE_EXPRESSION;
				--index;
			}
			else
			{
				Animation::GetInstance()->SetTime(transitionTime, totalTime);
				Animation::GetInstance()->StartAnimation(expressio, obj);
				lastExpression = expressio;
			}
		}
		else
		{
			if (text[index] == NULL && text[index -1] != NULL)
			{
				Animation::GetInstance()->SetTime(transitionTime, totalTime);
				Animation::GetInstance()->StartAnimation(NEUTRE, obj);
			}
			else
			{
				FinalizeTalk();
			}
		}
	}
	else
	{
		FinalizeTalk();
	}
}

void CParla::FinalizeTalk()
{
	index = 0;
	parlant = false;
}

TypeExpression CParla::ParseCharacter(const char c)
{
	if (strncmp(&c,"a",1)==0 || strncmp(&c,"A",1)==0)
	{
		return A;
	} 
	else if (strncmp(&c,"e",1)==0 || strncmp(&c,"E",1)==0)
	{
		return E;
	}
	else if (strncmp(&c,"i",1)==0 || strncmp(&c,"I",1)==0)
	{
		return I;
	}
	else if (strncmp(&c,"o",1)==0 || strncmp(&c,"O",1)==0)
	{
		return O;
	}
	else if (strncmp(&c,"u",1)==0 || strncmp(&c,"U",1)==0)
	{
		return U;
	}
	else if (strncmp(&c,"m",1)==0 || strncmp(&c,"M",1)==0 || 
			 strncmp(&c,"b",1)==0 || strncmp(&c,"B",1)==0 ||
			 strncmp(&c,"p",1)==0 || strncmp(&c,"P",1)==0 ||
			 strncmp(&c,"v",1)==0 || strncmp(&c,"V",1)==0 )
	{
		return BILABIAL;
	}
	else if (strncmp(&c,"t",1)==0 || strncmp(&c,"T",1)==0)
	{
		return NEUTRE;
	}
	else
	{
		return NONE_EXPRESSION;
	}
}

bool CParla::IsTalking()
{
	return parlant;
}

void CParla::TalkElapsed(Actor* obj)
{
	TypeExpression expressio;
	index = 0;
	float time;
	float StopTime;

	parlant = true;
	if (text.empty() != true)
	{	
		do
		{
			time = 0.f;
			StopTime = 0.f;
			Timer::GetInstance()->ResetTimer();
			do
			{
				expressio = ParseCharacter(text[index]);
				++index;
				StopTime += 0.2f;
			}while (expressio == NONE_EXPRESSION && text[index] != NULL);

			if (text[index] != NULL)
			{
				if (StopTime > 0.2f)
					Animation::GetInstance()->SetTime(transitionTime, totalTime*2);
				else
					Animation::GetInstance()->SetTime(transitionTime, totalTime);

				Animation::GetInstance()->StartAnimation(expressio, obj);
			}
			while (time < StopTime)
			{
				time += Timer::GetInstance()->GetElapsedTime();
			}
			
		}while (text[index] != NULL);
	}
	FinalizeTalk();
}

void CParla::RenderSubs(LPDIRECT3DDEVICE9 Device)
{
	m_Subtitles->RenderSubtitles(Device);
}

void CParla::PrepareFontSubs(std::string filename)
{
	m_Subtitles->CreateFont(filename);
}