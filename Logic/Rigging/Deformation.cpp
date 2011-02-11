#include "../../stdafx.h"
#include "Deformation.h"
#include "../../Render/visualitzacio.h"
#include "../../Models/Actor/Actor.h"
#include "../Expressions/ExpressionManager.h"
#include "../Muscles/MuscleManager.h"


Deformation::Deformation( Actor* obj, EditorManager *editor )
{
	this->ObOBJ = obj;
	this->muscle = NONE_MUSCLE;
	this->editorM = editor;
}

Deformation::~Deformation()
{
	this->ObOBJ = NULL;
	this->editorM = NULL;
}

void Deformation::ButtonDown(float mouseX, float mouseY, D3DXVECTOR3 opv)
{
	D3DXVECTOR3 LP1, LP2;

	if (ObOBJ != NULL)
	{
		GetLine(LP1, LP2, mouseX, mouseY);

		D3DXVECTOR3 dominant = editorM->GetDominantVertex();

		normalPla = opv;
		D3DXVec3Normalize(&normalPla,&normalPla);

		D3DXVECTOR3 HitP(0.0f, 0.0f,0.0f);
		bool coll = CheckLinePlane(normalPla, dominant, LP1, LP2, HitP);
		if (coll)
		{
			vecDir = HitP - dominant;
			editorM->DefineMovement(expression,muscle,vecDir);
			ExpressionManager::GetInstance()->RenderExpression(expression, ObOBJ);
		}
		
	}
}

void Deformation::ButtonMove(float mouseX, float mouseY)
{
	D3DXVECTOR3 LP1, LP2;
	D3DXVECTOR3 HitP(0.0f, 0.0f,0.0f);
	D3DXVECTOR3 dominant = editorM->GetDominantVertex();

	GetLine(LP1, LP2, mouseX, mouseY);

	bool coll = CheckLinePlane(normalPla, dominant, LP1, LP2, HitP);
	if (coll)
	{
		vecDir = HitP - dominant;
		editorM->DefineMovement(expression,muscle,vecDir);
	}
	//EManager->RenderExpression(expression);
}

void Deformation::ButtonUp()
{
	editorM->DefineMovement(expression, muscle, vecDir);
	ExpressionManager::GetInstance()->RenderExpression(expression, ObOBJ);
}

void Deformation::GetLine( D3DXVECTOR3 &L1, D3DXVECTOR3 &L2, float mouseX, float mouseY )
{
	D3DVIEWPORT9 ViewPortMatrix;
	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
	D3DXMATRIX	WorldMatrix;
	D3DXVECTOR3 result;
	D3DXVECTOR3 screenPoint;

	LPDIRECT3DDEVICE9 Device = CDirectX::GetInstance()->GetDevice();

	Device->GetViewport(&ViewPortMatrix);
	Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	Device->GetTransform(D3DTS_PROJECTION, &ProjectionMatrix);

	D3DXMatrixIdentity(&WorldMatrix);

	screenPoint = D3DXVECTOR3(mouseX, ViewPortMatrix.Height - mouseY + 1.0, ViewPortMatrix.MinZ);
	D3DXVec3Unproject( &result, &screenPoint, &ViewPortMatrix, &ProjectionMatrix, &ViewMatrix, &WorldMatrix );
	L1 = D3DXVECTOR3(result.x,  result.y, result.z);

	screenPoint = D3DXVECTOR3(mouseX, mouseY, ViewPortMatrix.MaxZ);
	D3DXVec3Unproject( &result, &screenPoint, &ViewPortMatrix, &ProjectionMatrix, &ViewMatrix, &WorldMatrix );
	L2 = D3DXVECTOR3(result.x, result.y, result.z);

	//double* mvmatrix;
	//double* projmatrix;
	//int* Viewport;
	////int Viewport[4];
	//double dX, dY, dZ, dClickY; // glUnProject uses doubles, but I'm using floats for these 3D vectors

	//Viewport = GetViewportMatrix();
	//mvmatrix = GetModelviewMatrix();
	//projmatrix = GetProjectionMatrix();
	//dClickY = double (Viewport[3] - mouseY); 
	//// OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

	//gluUnProject ((double) mouseX, dClickY, 0.0, mvmatrix, projmatrix, Viewport, &dX, &dY, &dZ);
	//L1 = D3DXVECTOR3( (float) dX, (float) dY, (float) dZ );
	//gluUnProject ((double) mouseX, dClickY, 1.0, mvmatrix, projmatrix, Viewport, &dX, &dY, &dZ);
	//L2 = D3DXVECTOR3( (float) dX, (float) dY, (float) dZ );
}
void Deformation::SetMuscle( TypeMuscle muscle )
{
	this->muscle = muscle;
}

void Deformation::SetExpression( TypeExpression expression )
{
	this->expression = expression;
}

void Deformation::SetObjecte( Actor* Obj )
{
	this->ObOBJ = Obj;
}