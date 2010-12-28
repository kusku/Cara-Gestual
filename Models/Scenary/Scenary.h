#ifndef SCENARY_H
#define SCENARY_H

#include <vector>

class Scenary
{
private:
	
	std::string m_Path;
	std::string m_Name;

	D3DXVECTOR3 m_Position;

public:

	Scenary( const std::string name = "", const std::string path = "" );
	~Scenary();

	void	Render		( );

	void	SetPath		( std::string path ) { m_Path = path; };
	void	SetName		( std::string name ) { m_Name = name; };
	void	SetPosition	( D3DXVECTOR3 pos ) { m_Position = pos; };
};

#endif


