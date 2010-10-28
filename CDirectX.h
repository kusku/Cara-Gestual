#ifndef CDIRECTX_H
#define CDIRECTX_H

#include <d3dx9.h>

class CDirectX
{
private:
	static CDirectX* m_CDirectX;

	HWND		m_hWnd;				//Identificador de finestra on s'incrusta el visualitzador 

	LPDIRECT3D9             m_pD3D;
	LPDIRECT3DDEVICE9       m_pD3DDevice;

	bool		m_PaintSolid;		//Indica si pintem els elements sòlids o en wireframe
	bool		m_paintInfoGame;	//Indica si pintem en text2d informació útil
	bool		m_paintInfoInput;	//Indica si pintem en text2d les asociacions [input-accion]
	bool		m_drawAxisGrid;		//Indica si pintem un eix de referència a la posició (0,0,0)

public:

	CDirectX();
	~CDirectX();

	static CDirectX*	GetInstance();
	void				CleanUp();

	HRESULT		InitDX			( HWND hWnd );

	//Prepara la visualizació (obtenínt la matrix View i Projection)
	void		SetupMatrices	( void );
	
	//Prepara les variables de Direct3D per a fer el render de l'escena
	void		BeginRenderDX	( void );

	//Finalitza la composició de l'escena amb Direct3D
	void		EndRenderDX		( void );

};

#endif