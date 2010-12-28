#include "../../stdafx.h"
#include "Scenary.h"

Scenary::Scenary(const std::string name, const std::string path)
{
	m_Name = name;
	m_Path = path;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Scenary::~Scenary()
{
}

void Scenary::Render()
{
}