#ifndef DEFINES_H
#define DEFINES_H

#include "stdafx.h"
#include <math.h>

#define ACTORS_XML		".\\Data\\XML\\actors.xml"
#define SCENE_XML		".\\Data\\XML\\scene.xml"
#define FONT_XML		".\\Data\\XML\\font.xml"
#define SUBTITLES		".\\Data\\Subtitles\\subtitles.txt"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEXTEXTURA (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)
#define D3DFVF_VERTEX_TEXTURED (D3DFVF_XYZ|D3DFVF_TEX2)

struct CUSTOMVERTEX
{
    float x, y, z;
    DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};
struct CUSTOMVERTEXTEXTURA
{
    float x, y, z;
	DWORD color;
    float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEXTEXTURA;
	}
};

struct VERTEX_TEXTURED
{
	float x, y, z;
    float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_VERTEX_TEXTURED;
	}
};

struct D3DXVECTOR3_LessThan
{
	bool operator()(D3DXVECTOR3 lhs, D3DXVECTOR3 rhs) const
	{
		return lhs.x < rhs.x ||
		(lhs.x==rhs.x && lhs.y<rhs.y) ||
		(lhs.x==rhs.x && lhs.y==rhs.y && lhs.z<rhs.z);
	}
};


#define CHECKED_DELETE(x) if(x!=NULL) {delete x; x=NULL;}
#define CHECKED_RELEASE(x) if(x!=NULL) {x->Release(); x=NULL;}

#define DEG2RAD(x) x*D3DX_PI/180.0f
#define RAD2DEG(x) x*180.0f/D3DX_PI

//--------------- GC2: Tipus de Projecció
#define CAP ' '
#define ORTO 'O'
#define PERSPECT 'P'

//--------------- GC2: Tipus de Polars (per la Visualització Interactiva)
#define POLARZ 'Z'
#define POLARY 'Y'
#define POLARX 'X'

#define MAX_PATH_TEXTURE 260
#define MAX_NAME_TEXTURE 1024

#endif