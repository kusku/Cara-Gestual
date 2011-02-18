#include "../stdafx.h"
#include "Subtitles.h"
#include "../Render/escena.h"
#include "../Models/ModelManager.h"

CSubtitles::CSubtitles()
{
	m_font = NULL;
	red = 0;
	green = 0;
	blue = 0;
	alpha = 255;
	m_Position = D3DXVECTOR3(0.f,0.f,0.f);
	m_Scale = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_Rotation = D3DXVECTOR3(0.f,0.f,0.f);
	subtitle = "Com que no em puc menjar una mandarina, em compro un pressec.";	
}

CSubtitles::~CSubtitles()
{
	CHECKED_RELEASE(m_font);
}

void CSubtitles::CreateFont(std::string filename)
{
	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();
	
	// Create a D3DX font object
	D3DXCreateFont(Device, 100, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
}

void CSubtitles::SetColor(int r, int g, int b, int a)
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
	//Render Subtitles 
	PreRender(Device);
	Render(Device);
	PostRender(Device);
	CDirectX::GetInstance()->EndRenderDX();
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

	Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//Change our rendering target to our created surface.
	Device->SetRenderTarget(0, CDirectX::GetInstance()->GetRenderSurface());
	//Clear it too, with a different color to make sure we're getting it.
	Device->Clear(0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0);
}

void CSubtitles::Render(LPDIRECT3DDEVICE9 Device)
{
	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);   

	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct;
	rct.left=0;
	rct.right=512;
	rct.top=0;
	rct.bottom=512;

	// Draw some text
	m_font->DrawText(NULL, subtitle.c_str(), -1, &rct, 0, fontColor );
}

void CSubtitles::PostRender(LPDIRECT3DDEVICE9 Device)
{
	CDirectX::GetInstance()->EndRenderDX();
	//And we change back to the actual backbuffer
	LPDIRECT3DSURFACE9 MainSurface = CDirectX::GetInstance()->GetMainSurface();
	Device->SetRenderTarget(0, MainSurface);

	Device->BeginScene();

	CUSTOMVERTEXTEXTURA Vertices[]=
	{
		{20.f, 5.f, 0.f, 0xffffffff, 0.f, 0.f},
		{20.f, 5.f, -5.f, 0xffffffff, 0.f, 1.f},
		{20.f, 0.f, 0.f, 0xffffffff, 1.f, 0.f},
		{20.f, 0.f, 0.f, 0xffffffff, 1.f, 0.f},
		{20.f, 5.f, -5.f, 0xffffffff, 0.f, 1.f},
		{20.f, 0.f, -5.f, 0xffffffff, 1.f, 1.f}
	};

	D3DXMATRIX l_MatrixT, l_Matrix;
	D3DXMatrixIdentity(&l_MatrixT);
	D3DXMatrixIdentity(&l_Matrix);
	D3DXMatrixScaling (&l_MatrixT,m_Scale.x, m_Scale.y, m_Scale.z);
	l_Matrix *= l_MatrixT;
	D3DXMatrixRotationYawPitchRoll(&l_MatrixT, DEG2RAD(m_Rotation.y), DEG2RAD(m_Rotation.x), DEG2RAD(m_Rotation.z));
	l_Matrix *= l_MatrixT;
	D3DXMatrixTranslation (&l_MatrixT, m_Position.x, m_Position.y, m_Position.z);
	l_Matrix *= l_MatrixT;
	Device->SetTransform(D3DTS_WORLD, &l_Matrix);

	Device->SetTexture(0, CDirectX::GetInstance()->GetRenderTexture());
	Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &Vertices, sizeof(CUSTOMVERTEXTEXTURA));

	CHECKED_RELEASE(MainSurface);
}