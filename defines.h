#ifndef DEFINES_H
#define DEFINES_H

#include "stdafx.h"
#include <math.h>

#define TEXTURES_FOLDER		".\\Data\\Models\\heavytriangles\\maps\\"

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

#define CHECKED_DELETE(x) if(x!=NULL) {delete x; x=NULL;}
#define CHECKED_RELEASE(x) if(x!=NULL) {x->Release(); x=NULL;}

#define DEG2RAD(x) x*D3DX_PI/180.0f
#define RAD2DEG(x) x*180.0f/D3DX_PI

//--------------- GC2: Tipus de Projecci�
#define CAP ' '
#define ORTO 'O'
#define PERSPECT 'P'

//--------------- GC2: Tipus de Polars (per la Visualitzaci� Interactiva)
#define POLARZ 'Z'
#define POLARY 'Y'
#define POLARX 'X'

#define MAX_PATH_TEXTURE 260
#define MAX_NAME_TEXTURE 1024

#endif