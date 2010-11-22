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

	D3DXVECTOR3 l_Eye;
	D3DXVECTOR3 l_LookAt;
	D3DXVECTOR3 l_VUP;

	bool		m_PaintSolid;		//Indica si pintem els elements sòlids o en wireframe
	bool		m_paintInfoGame;	//Indica si pintem en text2d informació útil
	bool		m_paintInfoInput;	//Indica si pintem en text2d les asociacions [input-accion]
	bool		m_ZBuffer;
	bool		m_CullingFace;

	void		DrawLine (const D3DXVECTOR3 &PosA, const D3DXVECTOR3 &PosB, DWORD Color=0xffffffff);

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

	//Render eixos
	void		RenderAxis		( float size );
	void		RenderSphere	( float Radius, DWORD Color=0x00000000, int Aristas=10 );
	void		RenderSquare	( D3DXVECTOR2 TopLeft, D3DXVECTOR2 BottomRight );

	void		PointLight		( D3DXVECTOR3 position, D3DXVECTOR3 direction );

	void		SetEye			( D3DXVECTOR3 eye ){ l_Eye = eye; } ;
	void		SetLookAt		( D3DXVECTOR3 lookAt ) { l_LookAt = lookAt; };
	void		SetUpVector		( D3DXVECTOR3 upVector ) { l_VUP = upVector; } ;
	void		SetZBuffer		( bool active ) { m_ZBuffer = active; };
	void		SetPaintSolid	( bool solid ) { m_PaintSolid = solid; };
	void		SetCullingFace	( bool cull ) { m_CullingFace = cull; };

	LPDIRECT3DDEVICE9	GetDevice			() { return m_pD3DDevice; };
};

#endif