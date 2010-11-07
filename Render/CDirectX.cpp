#include "../stdafx.h"
#include "CDirectX.h"
#include "../defines.h"
#include <assert.h>


CDirectX::CDirectX()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_PaintSolid = true;
	m_hWnd = NULL;
	m_paintInfoGame = true;
	m_paintInfoInput = false;
	m_ZBuffer = true;
	m_CullingFace = true;

	l_Eye = D3DXVECTOR3(0.0f,5.0f,-5.0f);
	l_LookAt = D3DXVECTOR3(0.0f,0.0f,0.0f);
	l_VUP = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

CDirectX::~CDirectX()
{
	CHECKED_RELEASE(m_pD3D);
	CHECKED_RELEASE(m_pD3DDevice);
}

CDirectX* CDirectX::m_CDirectX = NULL;

CDirectX* CDirectX::GetInstance()
{
	if (m_CDirectX == NULL)
		m_CDirectX = new CDirectX();

	return m_CDirectX;
}

HRESULT CDirectX::InitDX ( HWND hWnd )
{
// Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags                  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pD3DDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	
	//Inicializamos los objetos de DirectInput (Keyboard y Mouse)
	
    return S_OK;
}

void CDirectX::SetupMatrices()
{
	//TODO: S'ha de canviar ja que es pot moure la càmera amb el ratolí

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
	//D3DXVECTOR3 l_Eye(0.0f,5.0f,-5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);

	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
	D3DXMatrixPerspectiveFovLH( &m_matProject, 
							   45.0f * D3DX_PI / 180.0f, 
                               1.0f, 
							   1.0f, 
							   1000.0f );

	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}

void CDirectX::BeginRenderDX()
{
	#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	#else // Clear the backbuffer to a black color in a Release mode
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	#endif


    // Begin the scene
    HRESULT hr=m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_ZBuffer)
	{
		m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
		m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_FALSE);
		m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	}
	
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if(m_PaintSolid)
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
	else
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );

	if (m_CullingFace)
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	else
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
}

void CDirectX::EndRenderDX()
{
	m_pD3DDevice->EndScene();
    // Present the backbuffer contents to the display
    m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CDirectX::RenderAxis(float size)
{
	DrawLine(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(size,0.0f,0.0f),0xffff0000);
	DrawLine(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-size,0.0f),0xff00ff00);
	DrawLine(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,size),0xff0000ff);
}

void CDirectX::DrawLine(const D3DXVECTOR3 &PosA, const D3DXVECTOR3 &PosB, DWORD Color)
{
	CUSTOMVERTEX v[2] =
	{
		{PosA.x, PosA.y, PosA.z, Color},
		{PosB.x, PosB.y, PosB.z, Color},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CDirectX::PointLight( D3DXVECTOR3 position, D3DXVECTOR3 direction )
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_SPOT;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	   
	  // Spot lights have direction and a position
	light.Position = position;
	light.Direction = direction;

	// Tell the device about the light and turn it on
	light.Range=1.0f;
	light.Theta=0.5f;
	light.Phi=1.0f;
	light.Falloff=1.0f;
	light.Attenuation0= 1.0f;

	m_pD3DDevice->SetLight( 1, &light );
	m_pD3DDevice->LightEnable( 1, TRUE );
}
