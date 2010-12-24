#ifndef SELECTION_H
#define SELECTION_H

#include "../../Models/Actor/Actor.h"
#include "EditorManager.h"
#include "intersection.h"
#include "../../defines.h"

enum SelectionModes{ SELECT_ADD, SELECT_SUB };
enum TriFlags{ TF_SELECTED = 1, NTF_SELECTED = 2, TF_BACKFACING = 3, NTF_BACKFACING = 4 };

class Selection
{
private:
	float nStartX, nStartY, nEndX, nEndY;
	bool buttonState, dominantSelect;
	D3DXVECTOR3 m_vLineP[2];
	Actor* ObOBJ;
	EditorManager* editorM;

	int *m_pTriFlags;
	int *m_pTriBackFacing;
	int m_nSelMode;

	/* M�todes de selecci� */
	int			FrustumSelect		( D3DXVECTOR3 Normals[4], D3DXVECTOR3 Points[8] );
	int			LineSelect			( const D3DXVECTOR3 &LP1, const D3DXVECTOR3 &LP2 );
	void		SelectTriangle		( int nTri );

	/* M�todes de Set i Get */
	void		SetSelectionMode	( int nMode );
	void		GetFrustum			( D3DXVECTOR3 Normals[4], D3DXVECTOR3 P[8] );
	void		GetLine				( D3DXVECTOR3 &L1, D3DXVECTOR3 &L2, float mouseX, float mouseY );

public:
	Selection(Actor* obj, EditorManager* editor);
	~Selection();

	/* M�todes de pulsacions del ratol� */

	void	ButtonDown			( float mouseX, float mouseY, int flag );
	void	ButtonMove			( float mouseX, float mouseY );
	void	ButtonUp			( void );
	void	ButtonRDown			( float mouseX, float mouseY );

	/* M�todes de Render */
	void	Render				( void );
	void	NoRender			( void );

	/* M�todes de Set */
	void	SetObj				( Actor* obj );
	void	SetZBufferTriangles	( D3DXVECTOR3 camera );
	void	SetFlagsTriangles	( void );

	bool	IsTriangleSelected	( int nTri );
};

#endif