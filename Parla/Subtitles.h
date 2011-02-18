#ifndef CSUBTITLES_H
#define CSUBTITLES_H

#include <vector>
#include <d3dx9.h>

#include "../Render/CDirectX.h"

class CSubtitles
{
private:

	int red, green, blue, alpha;
	D3DXVECTOR3 m_Position, m_Scale, m_Rotation;
	LPD3DXFONT m_font;
	std::string subtitle;

	void	PreRender	( LPDIRECT3DDEVICE9 Device );
	void	Render		( LPDIRECT3DDEVICE9 Device );
	void	PostRender	( LPDIRECT3DDEVICE9 Device );

public:

	CSubtitles();
	~CSubtitles();

	void		CreateFont			( std::string filename );

	void		SetColor			( int r, int g, int b, int a);
	void		SetPosition			( D3DXVECTOR3 pos ) { m_Position = pos; }
	void		SetRotation			( D3DXVECTOR3 rot ) { m_Rotation = rot; }
	void		SetScale			( D3DXVECTOR3 sca ) { m_Scale = sca; }
	void		SetText				( std::string text ) {subtitle = text; }

	void		ParseSubtitles		( void );
	void		RenderSubtitles		( LPDIRECT3DDEVICE9 Device );

};

#endif