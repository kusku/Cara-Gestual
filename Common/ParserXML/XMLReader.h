#ifndef XMLREADER_H
#define XMLREADER_H
#include "../../Logic/Expressions/ExpressionManager.h"
#include "../../Logic/Muscles/MuscleManager.h"
#include "../../Logic/Rigging/EditorManager.h"
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <string>

class XMLReader {
public:
	XMLReader(char* path, ExpressionManager* EManager, MuscleManager* MManager, EditorManager* EditorM);
	~XMLReader();
	void	Read();

private:
	D3DXVECTOR3			punt;
	ExpressionManager*	EManager;
	MuscleManager*		MManager;
	EditorManager*		EditorM;
	FILE				*fitxer;
	
	char				num		[4];
	char				c1;


	void				ReadAtribut();
	void				ReadWord();
	void				ReadChar();
	TypeMuscle			ConvertirNomMuscles();
	TypeExpression		ConvertirNomExpressions();
	void				GuardarNums();
	float				ConvertirNum();
	void				ReadExpressions();
	void				ReadMuscles();
	void				ReadActors();
};

#endif