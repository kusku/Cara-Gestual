#ifndef CSUBTITLES_H
#define CSUBTITLES_H

class CSubtitles
{
private:

	float red, green, blue, alpha;
	D3DXVECTOR2 position;
	char* subtitle;

public:

	CSubtitles();
	~CSubtitles();

	void		SetColor			( float r, float g, float b, float a );
	void		SetPosition			( D3DXVECTOR2 pos ) { position = pos; };
	void		SetText				( char* text ) {subtitle = text; };

	void		ParseSubtitles		( void );
	void		RenderSubtitles		( void );

};

#endif