#include "../../stdafx.h"
#include "XMLWriter.h"

XMLWriter::XMLWriter(char* path, ExpressionManager* EManager, MuscleManager* MManager, int x) {
	errno_t err;

	if( (err  = fopen_s(&fitxer,path,"w")) !=0 )
		printf( "The file %s was not opened\n",path );
	else
		printf( "The file %s was opened\n",path );

	this->EManager = EManager;
	this->MManager = MManager;
	this->x = x;
}

XMLWriter::~XMLWriter() {
	fclose (fitxer);
}

void XMLWriter::Guardar()	{

	if(x==1)
		GuardarMuscles();
	else
		GuardarExpresions();

}

void XMLWriter::GuardarMuscles()	{

	char* nomMuscle;

	fprintf_s(fitxer,"<muscles>\n");
	for (int i = 0; i < MManager->getNumMuscles(); i++)
	{
		nomMuscle = ConvertirMuscle(i);
		for (int j=0; j < MManager->getMuscleList()[i]->getNumVertexs(); j++)
			fprintf_s(fitxer,"\t\t<%s vertex = \"%d\" delta = \"%f\"/>\n",nomMuscle, MManager->getMuscleList()[i]->getVertexIndex()[j], MManager->getMuscleList()[i]->getVertexDelta()[j]);		

		fprintf_s(fitxer,"\n");
	}
	fprintf_s(fitxer,"</muscles>");
}

void XMLWriter::GuardarExpresions()	{
	char *nomExpressio,*nomMuscle;
	TypeMuscle t;
	
	fprintf_s(fitxer, "<expressions>\n");
	for ( int i=0; i < EManager->getNumExpressions(); i++ )
	{
		nomExpressio = ConvertirExpression(i);
		for ( int j=0; j < MManager->getNumMuscles(); j++ )
		{
			nomMuscle = ConvertirMuscle(j);
			t = ConvertirTypeMuscle(j);
			fprintf_s(fitxer, "\t\t<%s muscle = \"%s\" posX = \"%f\" posY = \"%f\" posZ = \"%f\"/>\n",nomExpressio, nomMuscle, EManager->getExpressionList()[i]->getMovement(t).x, EManager->getExpressionList()[i]->getMovement(t).y, EManager->getExpressionList()[i]->getMovement(t).z);
		}
		fprintf_s(fitxer,"\n");
	}
	fprintf_s(fitxer, "</expressions>");
}

char* XMLWriter::ConvertirMuscle(int i)	{		// TypeMuscle(0) retorna 0 i no pas ECELLA

	if		(i==0)	return "ECELLA";
	else if (i==1)	return "DCELLA";
	else if (i==2)	return "INFBOCA";
	else if (i==3)	return "EBOCA";
	else if (i==4)	return "DBOCA";
	else if (i==5)	return "EPARPELLA";
	else if (i==6)	return "DPARPELLA";
	else if (i==7)	return "EGALTA";
	else if (i==8)	return "DGALTA";
	else if (i==9)	return "SUPBOCA";
	else if (i==10)	return "LATEBOCA";
	else if (i==11)	return "LATDBOCA";
	else if (i==12)	return "DENTDALT";
	else if (i==13)	return "DENTBAIX";
	return "NONE_MUSCLE";
}

TypeMuscle XMLWriter::ConvertirTypeMuscle(int i)	{		// TypeMuscle(0) retorna 0 i no pas ECELLA

	if		(i==0)	return ECELLA;
	else if (i==1)	return DCELLA;
	else if (i==2)	return INFBOCA;
	else if (i==3)	return EBOCA;
	else if (i==4)	return DBOCA;
	else if (i==5)	return EPARPELLA;
	else if (i==6)	return DPARPELLA;
	else if (i==7)	return EGALTA;
	else if (i==8)	return DGALTA;
	else if (i==9)	return SUPBOCA;
	else if (i==10)	return LATEBOCA;
	else if (i==11)	return LATDBOCA;
	else if (i==12)	return DENTDALT;
	else if (i==13)	return DENTBAIX;
	return NONE_MUSCLE;
}

char* XMLWriter::ConvertirExpression(int i)	
{		
	switch (i)
	{
	case 0:
		{
			return "TRIST";
			break;
		}
	case 1:
		{
			return "ALEGRE";
			break;
		}
	case 2:
		{
			return "ENFADAT";
			break;
		}
	case 3:
		{
			return "SERIOS";
			break;
		}
	case 4:
		{
			return "SORPRES";
			break;
		}
	case 5:
		{
			return "A";
			break;
		}
	case 6:
		{
			return "E";
			break;
		}
	case 7:
		{
			return "I";
			break;
		}
	case 8:
		{
			return "O";
			break;
		}
	case 9:
		{
			return "U";
			break;
		}
	case 10:
		{
			return "BILABIAL";
			break;
		}
	case 11:
		{
			return "NEUTRE";
			break;
		}
	default:
		{
			return "NONE_EXPRESSION";
			break;
		}
	}
}

TypeExpression XMLWriter::ConvertirTypeExpression(int i)	{		

	switch (i)
	{
	case 0:
		{
			return TRIST;
			break;
		}
	case 1:
		{
			return ALEGRE;
			break;
		}
	case 2:
		{
			return ENFADAT;
			break;
		}
	case 3:
		{
			return SERIOS;
			break;
		}
	case 4:
		{
			return SORPRES;
			break;
		}
	case 5:
		{
			return A;
			break;
		}
	case 6:
		{
			return E;
			break;
		}
	case 7:
		{
			return I;
			break;
		}
	case 8:
		{
			return O;
			break;
		}
	case 9:
		{
			return U;
			break;
		}
	case 10:
		{
			return BILABIAL;
			break;
		}
	case 11:
		{
			return NEUTRE;
			break;
		}
	default:
		{
			return NONE_EXPRESSION;
			break;
		}
	}

}
