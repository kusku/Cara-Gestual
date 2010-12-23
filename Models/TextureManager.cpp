#include "../stdafx.h"
#include "TextureManager.h"
#include <assert.h>
#include "../defines.h"

void GetFilename(const char *path, std::string *filename)
{
	char *pnt=NULL;
	for(int b=(int)strlen(path);b>0;--b)
	{
		if(path[b]=='\\')
		{
			pnt=(char*)&(path[b+1]);
			b=-1;
		}
	}
	if(pnt==NULL)
		pnt=(char*)&(path[0]);
	*filename=pnt;
}

CTextureManager * CTextureManager::m_TextureManager=NULL;

CTextureManager * CTextureManager::GetInstance()
{
	if(m_TextureManager==NULL)
		m_TextureManager=new CTextureManager;
	assert(m_TextureManager!=NULL);
	return m_TextureManager;
}

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

void CTextureManager::CleanUp()
{
	for(TTextureIterator it=m_TextureMap.begin();it!=m_TextureMap.end();++it)
	{
		CHECKED_RELEASE(it->second);
	}
	m_TextureMap.clear();
	CHECKED_DELETE(m_TextureManager);
}

LPDIRECT3DTEXTURE9 CTextureManager::LoadTexture(const std::string &filename, LPDIRECT3DDEVICE9 Device)
{
	LPDIRECT3DTEXTURE9 l_Texture=NULL;
	if(m_TextureMap.find(filename)!=m_TextureMap.end())
		l_Texture=m_TextureMap[filename];
	else
	{
		D3DXCreateTextureFromFile(Device,filename.c_str(),&l_Texture);
		m_TextureMap[filename]=l_Texture;
	}
	assert(l_Texture!=NULL);
	return l_Texture;
}
