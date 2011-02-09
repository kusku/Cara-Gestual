#include "../stdafx.h"
#include "Subtitles.h"
#include "../Render/escena.h"
#include "../Models/ModelManager.h"

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

void CSubtitles::RenderSubtitles(LPDIRECT3DDEVICE9 Device)
{
	char* frase = subtitle;
	CDirectX* l_DX = CDirectX::GetInstance();

		// Create a D3DX font object
	D3DXCreateFont( Device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );

	PreRender(Device);
	l_DX->BeginRenderDX();
	l_DX->SetupMatrices();
	Render(Device);
	PostRender(Device);
	l_DX->EndRenderDX();
	if(::GetAsyncKeyState(VK_F8)&0x8000)
	{
		HRESULT l_HR=D3DXSaveTextureToFile( "c:/textura.bmp", D3DXIFF_BMP,  CDirectX::GetInstance()->GetRenderTexture(), NULL);
		l_HR=l_HR;
	}
}

void CSubtitles::PreRender(LPDIRECT3DDEVICE9 Device)
{
	if(CDirectX::GetInstance()->GetRenderTexture()==NULL)
		CDirectX::GetInstance()->CreateTexDX(512,512);
	Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);
	//Change our rendering target to our created surface.
	LPDIRECT3DSURFACE9 RenderSurface = CDirectX::GetInstance()->GetRenderSurface();
	Device->SetRenderTarget(0, RenderSurface);
	//Clear it too, with a different color to make sure we're getting it.
	Device->Clear(0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0);
}

void CSubtitles::Render(LPDIRECT3DDEVICE9 Device)
{
	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,255,255,255);   

	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct;
	rct.left=0;
	rct.right=512;
	rct.top=0;
	rct.bottom=512;

	// Draw some text
	m_font->DrawText(NULL, "Hello World", -1, &rct, 0, fontColor );
}

void CSubtitles::PostRender(LPDIRECT3DDEVICE9 Device)
{
	Device->EndScene();
	//And we change back to the actual backbuffer
	LPDIRECT3DSURFACE9 MainSurface = CDirectX::GetInstance()->GetMainSurface();
	Device->SetRenderTarget(0, MainSurface);
}