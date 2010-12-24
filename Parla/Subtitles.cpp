#include "../stdafx.h"
#include "Subtitles.h"
#include "../Render/escena.h"

CSubtitles::CSubtitles()
{
	red = 1.f;
	green = 1.f;
	blue = 1.f;
	alpha = 1.f;
	position = D3DXVECTOR2(100.0f, 100.0f);
	subtitle = "Com que no em puc menjar una mandarina, em compro un pressec.";
}

CSubtitles::~CSubtitles()
{
}

void CSubtitles::SetColor(float r, float g, float b, float a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

void CSubtitles::ParseSubtitles()
{
	//Controla el temps entre cada letra
	//Anar llegint cada lletra del subtítol
	//Cridar a l'animació (expressió) corresponent
}

void CSubtitles::RenderSubtitles()
{
	char* frase = subtitle;

	//SetProjection2D();
	//glColor4f(red,green,blue,alpha); 
	//glPushMatrix();
	//	glRasterPos2f(posX,posY);
	//	while (*frase)
	//	{
	//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *frase); 
	//		++frase;
	//	}
	//	
	//glPopMatrix();
	//SetProjection3D();
}