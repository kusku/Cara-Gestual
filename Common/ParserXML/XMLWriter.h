#ifndef XMLWRITER_H
#define XMLWRITER_H
#include "../../Logic/Expressions/ExpressionManager.h"
#include "../../Logic/Muscles/MuscleManager.h"
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <string>

class XMLWriter {
public:
	XMLWriter(char* path, int i);
	~XMLWriter();
	void	Guardar( void );

private:
	FILE				*fitxer;
	int					x;

	TypeExpression ConvertirTypeExpression(int i);
	TypeMuscle ConvertirTypeMuscle(int i);

	void				GuardarMuscles		( void );
	void				GuardarExpresions	( void );	
	char*				ConvertirMuscle		( int i );
	char*				ConvertirExpression	( int i );
};

#endif