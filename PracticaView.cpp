//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn b�sic VS2005 MONOFINESTRA amb interf�cie MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Febrer 2010)
// PracticaView.cpp : implementation of the CPracticaView class
// FUNCIONS:		- Control del bucle principal (OnPaint)
//					- Control teclat (OnKeyDown)
//			- Control mouse interactiu i botons mouse 
//							(OnLButtomDown, OnRButtomDown, OnMouseMove)
//					- Control opcions de men� (On*, OnUpdate*)
//					- Control de color de fons per teclat.
//					- Transformacions Geom�triques Interactives via mouse
//
//

#include "stdafx.h"
#include "MainFrm.h"	// Include per Status Bar
#include "Practica.h"
#include "PracticaDoc.h"
#include "PracticaView.h"
#include "Logic/Muscles/MuscleManager.h"
#include "Logic/Expressions/ExpressionManager.h"
#include "Render/visualitzacio.h"
#include "Logic/Rigging/Selection.h"
#include "Logic/Rigging/EditorManager.h"
#include "Common/Timer/Timer.h"
#include "Render/escena.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <gl\glut.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPracticaView

IMPLEMENT_DYNCREATE(CPracticaView, CView)

BEGIN_MESSAGE_MAP(CPracticaView, CView)
	//{{AFX_MSG_MAP(CPracticaView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//ON_COMMAND(ID_CUB, OnCub)
	//ON_UPDATE_COMMAND_UI(ID_CUB, OnUpdateCub)
	//ON_COMMAND(ID_PERSPECTIVA, OnPerspectiva)
	//ON_UPDATE_COMMAND_UI(ID_PERSPECTIVA, OnUpdatePerspectiva)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateZoom)
	ON_COMMAND(ID_MOBIL, OnMobil)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_ESFERA, OnEsfera)
	//ON_UPDATE_COMMAND_UI(ID_ESFERA, OnUpdateEsfera)
	//ON_COMMAND(ID_TEAPOT, OnTeapot)
	//ON_UPDATE_COMMAND_UI(ID_TEAPOT, OnUpdateTeapot)
	ON_COMMAND(ID_EIXOS, OnEixos)
	ON_UPDATE_COMMAND_UI(ID_EIXOS, OnUpdateEixos)
	ON_COMMAND(ID_FILFERROS, OnFilferros)
	ON_UPDATE_COMMAND_UI(ID_FILFERROS, OnUpdateFilferros)
	ON_COMMAND(ID_SUAU, OnSuau)
	ON_UPDATE_COMMAND_UI(ID_SUAU, OnUpdateSuau)
	ON_COMMAND(ID_I_FIXE, OnIFixe)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_PAN, OnPan)
	ON_UPDATE_COMMAND_UI(ID_PAN, OnUpdatePan)
	ON_COMMAND(ID_TEST, OnTest)
	ON_UPDATE_COMMAND_UI(ID_TEST, OnUpdateTest)
	//ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	/*ON_COMMAND(ID_TRASLACIO, OnTraslacio)
	ON_UPDATE_COMMAND_UI(ID_TRASLACIO, OnUpdateTraslacio)*/
	/*ON_COMMAND(ID_INITRAS, OnInitras)
	ON_COMMAND(ID_ROTACIO, OnRotacio)
	ON_UPDATE_COMMAND_UI(ID_ROTACIO, OnUpdateRotacio)
	ON_COMMAND(ID_INIROT, OnInirot)
	ON_COMMAND(ID_ESCALATGE, OnEscalatge)
	ON_UPDATE_COMMAND_UI(ID_ESCALATGE, OnUpdateEscalatge)*/
	ON_COMMAND(ID_BACK_LINE, OnBackLine)
	ON_UPDATE_COMMAND_UI(ID_BACK_LINE, OnUpdateBackLine)
	ON_COMMAND(ID_ZBuffer, OnZBuffer)
	ON_UPDATE_COMMAND_UI(ID_ZBuffer, OnUpdateZBuffer)
	/*ON_COMMAND(ID_MOBILX, OnMobilx)
	ON_COMMAND(ID_MOBILY, OnMobily)
	ON_UPDATE_COMMAND_UI(ID_MOBILY, OnUpdateMobily)
	ON_UPDATE_COMMAND_UI(ID_MOBILX, OnUpdateMobilx)
	ON_COMMAND(ID_MOBILZ, OnMobilz)
	ON_UPDATE_COMMAND_UI(ID_MOBILZ, OnUpdateMobilz)*/
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_INIPAN, OnInipan)
	/*ON_COMMAND(ID_INIESCAL, OnIniescal)*/
	//ON_COMMAND(ID_TRUCK, OnTruck)
	//ON_UPDATE_COMMAND_UI(ID_TRUCK, OnUpdateTruck)
	ON_COMMAND(ID_NAVEGA, OnNavega)
	ON_UPDATE_COMMAND_UI(ID_NAVEGA, OnUpdateNavega)
	ON_COMMAND(ID_ININAV, OnIninav)
	//ON_COMMAND(ID_OBJ3DS, OnObj3ds)
	//ON_UPDATE_COMMAND_UI(ID_OBJ3DS, OnUpdateObj3ds)
	//ON_COMMAND(ID_FULLSCREEN, OnFullscreen)
	//ON_UPDATE_COMMAND_UI(ID_FULLSCREEN, OnUpdateFullscreen)
	ON_UPDATE_COMMAND_UI(ID_MOBIL, OnUpdateMobil)
	ON_UPDATE_COMMAND_UI(ID_I_FIXE, OnUpdateIFixe)
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN_OBJ, &CPracticaView::OnFileOpenObj)
	ON_COMMAND(ID_FILE_OPEN_3DS, &CPracticaView::OnFileOpen3ds)
	//ON_COMMAND(ID_OBJOBJ, &CPracticaView::OnObjobj)
	//ON_UPDATE_COMMAND_UI(ID_OBJOBJ, &CPracticaView::OnUpdateObjobj)
	ON_COMMAND(ID_POLAR_Z, &CPracticaView::OnPolarZ)
	ON_UPDATE_COMMAND_UI(ID_POLAR_Z, &CPracticaView::OnUpdatePolarZ)
	ON_COMMAND(ID_POLAR_Y, &CPracticaView::OnPolarY)
	ON_UPDATE_COMMAND_UI(ID_POLAR_Y, &CPracticaView::OnUpdatePolarY)
	ON_COMMAND(ID_POLAR_X, &CPracticaView::OnPolarX)
	ON_UPDATE_COMMAND_UI(ID_POLAR_X, &CPracticaView::OnUpdatePolarX)

	//Vinculaci� de funcions de Muscles i Expressions amb el men� gr�fic de l'aplicaci�.
	ON_COMMAND(ID_AUTOMATIC, &CPracticaView::OnCarregaAutomatica)
	ON_COMMAND(ID_IMPORT_MUSCLES, &CPracticaView::OnImportMuscles)
	ON_COMMAND(ID_EXPORT_MUSCLES, &CPracticaView::OnExportMuscles)
	ON_COMMAND(ID_IMPORT_EXPRESSIONS, &CPracticaView::OnImportExpressions)
	ON_COMMAND(ID_EXPORT_EXPRESSIONS, &CPracticaView::OnExportExpressions)
	ON_COMMAND(ID_MUSCLE_EDIT, &CPracticaView::OnMuscleEdit)
	ON_UPDATE_COMMAND_UI(ID_MUSCLE_EDIT, &CPracticaView::OnUpdateMuscleEdit)
	ON_COMMAND(ID_EXPRESSION_EDIT, &CPracticaView::OnExpressionEdit)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSION_EDIT, &CPracticaView::OnUpdateExpressionEdit)


	ON_COMMAND(ID_MCELLES_DRETA, &CPracticaView::OnMCellesDreta)
	ON_UPDATE_COMMAND_UI(ID_MCELLES_DRETA, &CPracticaView::OnUpdateMCellesDreta)
	ON_COMMAND(ID_MCELLES_ESQUERRE, &CPracticaView::OnMCellesEsquerra)
	ON_UPDATE_COMMAND_UI(ID_MCELLES_ESQUERRE, &CPracticaView::OnUpdateMCellesEsquerra)
	ON_COMMAND(ID_MBOCA_DRETA, &CPracticaView::OnMBocaDreta)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_DRETA, &CPracticaView::OnUpdateMBocaDreta)
	ON_COMMAND(ID_MBOCA_ESQUERRE, &CPracticaView::OnMBocaEsquerre)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_ESQUERRE, &CPracticaView::OnUpdateMBocaEsquerre)
	ON_COMMAND(ID_MBOCA_INFERIOR, &CPracticaView::OnMBocaInferior)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_INFERIOR, &CPracticaView::OnUpdateMBocaInferior)
	ON_COMMAND(ID_MPARPELLES_DRETA, &CPracticaView::OnMParpellesDreta)
	ON_UPDATE_COMMAND_UI(ID_MPARPELLES_DRETA, &CPracticaView::OnUpdateMParpellesDreta)
	ON_COMMAND(ID_MPARPELLES_ESQUERRA, &CPracticaView::OnMParpellesEsquerra)
	ON_UPDATE_COMMAND_UI(ID_MPARPELLES_ESQUERRA, &CPracticaView::OnUpdateMParpellesEsquerra)
	ON_COMMAND(ID_MGALTES_DRETA, &CPracticaView::OnMGaltesDreta)
	ON_UPDATE_COMMAND_UI(ID_MGALTES_DRETA, &CPracticaView::OnUpdateMGaltesDreta)
	ON_COMMAND(ID_MGALTES_ESQUERRA, &CPracticaView::OnMGaltesEsquerra)
	ON_UPDATE_COMMAND_UI(ID_MGALTES_ESQUERRA, &CPracticaView::OnUpdateMGaltesEsquerra)
	ON_COMMAND(ID_MBOCA_SUPERIOR, &CPracticaView::OnMBocaSuperior)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_SUPERIOR, &CPracticaView::OnUpdateMBocaSuperior)
	ON_COMMAND(ID_MBOCA_LATERALESQUERRE, &CPracticaView::OnMBocaLateralE)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_LATERALESQUERRE, &CPracticaView::OnUpdateMBocaLateralE)
	ON_COMMAND(ID_MBOCA_LATERALDRET, &CPracticaView::OnMBocaLateralD)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_LATERALDRET, &CPracticaView::OnUpdateMBocaLateralD)
	ON_COMMAND(ID_MBOCA_DENTDALT, &CPracticaView::OnMBocaDentDalt)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_DENTDALT, &CPracticaView::OnUpdateMBocaDentDalt)
	ON_COMMAND(ID_MBOCA_DENTBAIX, &CPracticaView::OnMBocaDentBaix)
	ON_UPDATE_COMMAND_UI(ID_MBOCA_DENTBAIX, &CPracticaView::OnUpdateMBocaDentBaix)

	ON_COMMAND(ID_EXPRESSIONS_TRIST, &CPracticaView::OnExpTrist)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSIONS_TRIST, &CPracticaView::OnUpdateExpTrist)
	ON_COMMAND(ID_EXPRESSIONS_ALEGRE, &CPracticaView::OnExpAlegre)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSIONS_ALEGRE, &CPracticaView::OnUpdateExpAlegre)
	ON_COMMAND(ID_EXPRESSIONS_ENFADAT, &CPracticaView::OnExpEnfadat)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSIONS_ENFADAT, &CPracticaView::OnUpdateExpEnfadat)
	ON_COMMAND(ID_EXPRESSIONS_SERIOS, &CPracticaView::OnExpSerios)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSIONS_SERIOS, &CPracticaView::OnUpdateExpSerios)
	ON_COMMAND(ID_EXPRESSIONS_SORPRES, &CPracticaView::OnExpSorpres)
	ON_UPDATE_COMMAND_UI(ID_EXPRESSIONS_SORPRES, &CPracticaView::OnUpdateExpSorpres)

	ON_COMMAND(ID_LLETRES_A, &CPracticaView::OnLletresA)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_A, &CPracticaView::OnUpdateLletresA)
	ON_COMMAND(ID_LLETRES_E, &CPracticaView::OnLletresE)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_E, &CPracticaView::OnUpdateLletresE)
	ON_COMMAND(ID_LLETRES_I, &CPracticaView::OnLletresI)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_I, &CPracticaView::OnUpdateLletresI)
	ON_COMMAND(ID_LLETRES_O, &CPracticaView::OnLletresO)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_O, &CPracticaView::OnUpdateLletresO)
	ON_COMMAND(ID_LLETRES_U, &CPracticaView::OnLletresU)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_U, &CPracticaView::OnUpdateLletresU)
	ON_COMMAND(ID_LLETRES_BILABIAL, &CPracticaView::OnLletresBilabial)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_BILABIAL, &CPracticaView::OnUpdateLletresBilabial)
	ON_COMMAND(ID_LLETRES_NEUTRE, &CPracticaView::OnLletresNeutre)
	ON_UPDATE_COMMAND_UI(ID_LLETRES_NEUTRE, &CPracticaView::OnUpdateLletresNeutre)

	//Vinculaci� de men� amb les opcions d'animaci�
	ON_COMMAND(ID_ANIMACIO, &CPracticaView::OnAnimacio)
	ON_UPDATE_COMMAND_UI(ID_ANIMACIO, &CPracticaView::OnUpdateAnimacio)
	ON_COMMAND(ID_FASTV, &CPracticaView::OnVFast)
	ON_UPDATE_COMMAND_UI(ID_FASTV, &CPracticaView::OnUpdateVFast)
	ON_COMMAND(ID_FAST, &CPracticaView::OnFast)
	ON_UPDATE_COMMAND_UI(ID_FAST, &CPracticaView::OnUpdateFast)
	ON_COMMAND(ID_NORMAL, &CPracticaView::OnNormal)
	ON_UPDATE_COMMAND_UI(ID_NORMAL, &CPracticaView::OnUpdateNormal)
	ON_COMMAND(ID_SLOW, &CPracticaView::OnSlow)
	ON_UPDATE_COMMAND_UI(ID_SLOW, &CPracticaView::OnUpdateSlow)
	ON_COMMAND(ID_VSLOW, &CPracticaView::OnVSlow)
	ON_UPDATE_COMMAND_UI(ID_VSLOW, &CPracticaView::OnUpdateVSlow)
	ON_COMMAND(ID_PARLA,&CPracticaView::OnParla)
	ON_COMMAND(ID_PARLABUCLE,&CPracticaView::OnParlaBucle)
	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPracticaView construction/destruction

CPracticaView::CPracticaView()
{
// TODO: add construction code here
	
//------ GC2: Inicialitzaci� de les variables globals de CPracticaView

// GC2: Variables de control per Men� Veure->Pantalla Completa 
	fullscreen=false;

// GC2: Variables de control per Men� Vista: canvi PV interactiu, Zoom i dibuixar eixos 
	mobil=true;				zzoom=true;		pan=false;		navega=false;	eixos=true;

// GC2: Variables opci� Vista->Pan
	fact_pan=1;
	tr_cpv = D3DXVECTOR3(0.f, 0.f, 0.f);

// GC2: Variables de control de l'opci� Vista->Navega?
	n.x=0.0;		n.y=0.0;		n.z=0.0;
	opv.x=10.0;		opv.y=0.0;		opv.z=0.0;
	angleZ=0.0;

// GC2: Variables de control per les opcions de men� Projecci�, Objecte
	projeccio=PERSPECT;		

// GC2: Variables de control del men� Transforma
	transf=false;	trasl=false;	rota=false;		escal=false;
	fact_Tras=1;	fact_Rota=90;
	VTras.x=0.0;	VTras.y=0.0;	VTras.z=0;
	VRota.x=0;		VRota.y=0;		VRota.z=0;
	VScal.x=1;		VScal.y=1;		VScal.z=1;
	transX=false;	transY=false;	transZ=false;

// GC2: Variables de control per les opcions de men� Ocultacions
	oculta=true;			test_vis=false;			back_line=false;

// GC2: Variables de control del men� Iluminaci�		
	filferros=false;		ifixe=true;	textura=false;	t_textura=CAP;

// GC2: Variables de control dels botons de mouse
	m_PosEAvall=(0,0);		m_PosDAvall=(0,0);
	m_ButoEAvall=false;		m_ButoDAvall=false;

// GC2: Variables que controlen par�metres visualitzaci�: Mides finestra Windows i PV
	w=0;			h=0;					// Mides finestra
	R=15;		angleh=0;		anglev=0;	// PV en esf�riques
	Vis_Polar=POLARZ;


// GC2: Objecte OBJ:
	ObOBJ = NULL;

// GC2: Variables del Timer
	t=0;			anima=false;

// GC2: Variables dels Muscles
	editMuscle = false;
	selectedMuscle = NONE_MUSCLE;

// GC2: Variables de les Expressions
	editExpression = false;
	selectedExpression = NONE_EXPRESSION;

// GC2: Altres variables
	nom="";			// Nom del fitxer

// GC2: Tecla Control per a la sel�lecci�
	TeclaControl = false;
	TeclaTab = false;
	zBuffer = false;

// GC2: Inicialitzaci� de les llibreries DevIL per a la c�rrega de textures i fitxers .3DS
	ilInit();					// Inicialitzar llibreria IL
	iluInit();					// Inicialitzar llibreria ILU
	ilutRenderer(ILUT_DIRECT3D9);	// Inicialitzar llibreria ILUT per a DirectX

	// Variables d'animaci�
	animacio = false;
	temporitzador = 0.5;
	acumulativeTime = 0.f;
	subtitles = false;
	tempsParla = 0.02f; //Temps de temporitzador Normal
	tParlaBucle = 10.0f;

	select = NULL;
	deform = NULL;
	editor = NULL;
}

CPracticaView::~CPracticaView()
{
	// Eliminar estructures din�miques
	CHECKED_DELETE(editor);
	CHECKED_DELETE(deform);
	CHECKED_DELETE(select);
	ObOBJ = NULL;

	MuscleManager::GetInstance()->CleanUp();
	ExpressionManager::GetInstance()->CleanUp();
	Animation::GetInstance()->CleanUp();
	CParla::GetInstance()->CleanUp();
	ModelManager::GetInstance()->CleanUp();
	Timer::GetInstance()->CleanUp();
	CDirectX::GetInstance()->CleanUp();

	_CrtDumpMemoryLeaks();
}

BOOL CPracticaView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: Modify the Window class or styles here by modifying
//  the CREATESTRUCT cs

// ATENCI�:MODIFICACI� OPENGL
// Afegim els estils de finestra que necessita OpenGl abans 
//	  de crear la finestra
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPracticaView drawing

void CPracticaView::OnDraw(CDC* pDC)
{
	CPracticaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CPracticaView printing

BOOL CPracticaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPracticaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPracticaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPracticaView diagnostics

#ifdef _DEBUG
void CPracticaView::AssertValid() const
{
	CView::AssertValid();
}

void CPracticaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPracticaDoc* CPracticaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPracticaDoc)));
	return (CPracticaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPracticaView message handlers

int CPracticaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{	
	if(CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDirectX::GetInstance()->InitDX(lpCreateStruct->hwndParent);

	//Carrega el model amb els muscles i expressions
	OnCarregaAutomatica();

	return true;
}

void CPracticaView::OnDestroy() 
{
	CDirectX::GetInstance()->EndRenderDX();
	CDirectX::GetInstance()->PresentDX();
	CView::OnDestroy();
}

void CPracticaView::OnPaint() 
{
	Perspectiva(anglev,angleh,R,Vis_Polar,pan,tr_cpv,transf,
				VScal,VTras,VRota,oculta,test_vis,back_line,filferros,textura,ifixe,eixos, editor,
				editMuscle, subtitles);
}

void CPracticaView::OnSize(UINT nType, int cx, int cy) 
{

// ATENCI�:MODIFICACI� OPENGL
// Establim les mides de la finestra actual
	w=cx;
	h=cy;

	CView::OnSize(nType, cx, cy);	
}

BOOL CPracticaView::OnEraseBkgnd(CDC* pDC) 
{
// TODO: Add your message handler code here and/or call default
		return FALSE;
}



/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */


void CPracticaView::OnLButtonDown(UINT nFlags, CPoint point) 
{
// TODO: Add your message handler code here and/or call default
	
// Detectem en quina posici� s'ha pitjat el bot� esquerra del
//  mouse i ho guardem a la variable m_PosEAvall
	m_ButoEAvall = true;
	m_PosEAvall = point;
	
	/////////////////////////////////////////////////////////
	/////// wx, wy, wz s�n les coordenades m�n			/////	
	/////////////////////////////////////////////////////////
	
	if (editExpression && ObOBJ != NULL)
	{
		if (deform == NULL)
			deform = new Deformation(ObOBJ,editor);

		deform->SetExpression(selectedExpression);
		deform->SetMuscle(selectedMuscle);
		deform->SetObjecte(ObOBJ);
		// Pas de coordenades esferiques a mon
		float x,y,z;
		if (this->Vis_Polar==POLARZ) {
			x=R*cos(anglev)*cos(angleh);
			y=R*sin(anglev)*cos(angleh);
			z=R*sin(angleh);
		}
		else if (this->Vis_Polar==POLARY) {
			x=R*sin(anglev)*cos(angleh);
			y=R*sin(angleh);
			z=R*cos(anglev)*cos(angleh);
		}
		else {
			x=R*sin(angleh);
			y=R*cos(anglev)*cos(angleh);
			z=R*sin(anglev)*cos(angleh);
		}
		deform->ButtonDown(point.x, point.y, D3DXVECTOR3(x, y, z));

	}
	else if (editMuscle && ObOBJ != NULL)
	{
		if (select == NULL)
			select = new Selection(ObOBJ,editor);

		select->SetObj(ObOBJ);
		select->SetZBufferTriangles(D3DXVECTOR3(opv.x,opv.y,opv.z));
		if(TeclaControl)
		{
			select->ButtonDown(point.x, point.y, 1);
			select->Render();
		}
		else if (TeclaTab)
		{
			select->ButtonDown(point.x, point.y, 2);
			select->Render();
		}
	}

	Invalidate();

	//////////////////////////////////////////////////////////


	CView::OnLButtonDown(nFlags, point);
}


void CPracticaView::OnLButtonUp(UINT nFlags, CPoint point) 
{
// TODO: Add your message handler code here and/or call default
	m_ButoEAvall = false;

	if (editExpression && deform != NULL)
	{
		deform->ButtonUp();
	}
	else if ((select != NULL && TeclaControl) ||(select != NULL && TeclaTab))
	{
		//select->SetZBufferTriangles(D3DXVECTOR3(opv.x,opv.y,opv.z));
		select->ButtonUp();
		select->NoRender();
	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

void CPracticaView::OnRButtonDown(UINT nFlags, CPoint point) 
{
// TODO: Add your message handler code here and/or call default
	m_ButoDAvall = true;
	m_PosDAvall = point;

	//Sel�lecci� del punt domimant del muscle del model
	if (editMuscle && ObOBJ != NULL)
	{
		if (select == NULL)
			select = new Selection(ObOBJ,editor);

		select->SetObj(ObOBJ);
		select->SetZBufferTriangles(D3DXVECTOR3(opv.x,opv.y,opv.z));
		if (TeclaControl)
			select->ButtonRDown(point.x,point.y);
	}
	
	CView::OnRButtonDown(nFlags, point);
}

void CPracticaView::OnRButtonUp(UINT nFlags, CPoint point) 
{
// TODO: Add your message handler code here and/or call default
	m_ButoDAvall = false;
	
	CView::OnRButtonUp(nFlags, point);
}


// OnMouseWheel: Funci� que es crida quan es toca el rodet del mouse
BOOL CPracticaView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	float modul=0;
	D3DXVECTOR3 vdir=D3DXVECTOR3(0.f,0.f,0.f);

// Funci� de zoom quan est� activada la funci� pan o les T. Geom�triques
	if ((zzoom) || (transX) || (transY) || (transZ)) 
		{	R=R+zDelta;
			if (R<1) R=1;
			Invalidate();
	}
	else if (navega)
		{	vdir.x=n.x-opv.x;
			vdir.y=n.y-opv.y;
			vdir.z=n.z-opv.z;
			modul=sqrt(vdir.x*vdir.x+vdir.y*vdir.y+vdir.z*vdir.z);
			vdir.x=vdir.x/modul;
			vdir.y=vdir.y/modul;
			vdir.z=vdir.z/modul;
			opv.x+=(zDelta/4)*vdir.x;
			opv.y+=(zDelta/4)*vdir.y;
			n.x+=(zDelta/4)*vdir.x;
			n.y+=(zDelta/4)*vdir.y;
			Invalidate();
		}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


// OnMouseMove: Funci� que es crida quan es mou el mouse. La utilitzem per la 
//				  Visualitzaci� Interactiva amb les tecles del mouse apretades per 
//				  modificar els par�metres de P.V. (R,angleh,anglev) segons els 
//				  moviments del mouse.
//      PARAMETRES: - nFlags: Flags que controlen si el bot� es apretat o no.
//					- point: Estructura (x,y) que d�na la posici� del mouse 
//							 (coord. pantalla) quan el bot� s'ha apretat.
void CPracticaView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (deform != NULL && editExpression && m_ButoEAvall)
	{
		deform->ButtonMove(point.x, point.y);
	}
	else if ((select != NULL && TeclaControl) ||(select != NULL && TeclaTab) || (select != NULL && zBuffer))
	{
		select->ButtonMove(point.x, point.y);
		select->Render();
		Invalidate();
	}
	else
		{
		float modul=0;
		D3DXVECTOR3 vdir=D3DXVECTOR3(0.f,0.f,0.f);

		if (m_ButoEAvall && mobil && projeccio!=CAP)
		{
			
	// Determinaci� dels angles (en graus) segons l'increment
	//  horitzontal i vertical de la posici� del mouse
			CSize gir = m_PosEAvall - point;
			m_PosEAvall = point;
			angleh = angleh-gir.cx/2;
			anglev= anglev+gir.cy/2 ;
			
	// Control per evitar el creixement desmesurat dels angles
			if(anglev>=360)	anglev=anglev-360;
			if(anglev<0)	anglev=anglev+360;
			if(angleh>=360)	angleh=angleh-360;
			if(angleh<0)	angleh=angleh+360;
			InvalidateRect(NULL, false);	
		}
		else if (m_ButoEAvall && navega && (projeccio!=CAP && projeccio!=ORTO)) // Opci� Navegaci�
				{
	// Canviar orientaci� en opci� de Navegaci�
				CSize girn = m_PosEAvall - point;
				angleZ=girn.cx/2.0;
	// Control per evitar el creixement desmesurat dels angles
				if(angleZ>=360) angleZ=angleZ-360;
				if(angleZ<0)	angleZ=angleZ+360;
	//			if(angleZ>=360)	angleZ=angleZ-360;
	//			if(angleZ<0)	angleZ=angleZ+360;

				n.x=n.x-opv.x;
				n.y=n.y-opv.y;
				n.x=n.x*cos(angleZ*D3DX_PI/180)-n.y*sin(angleZ*D3DX_PI/180);
				n.y=n.x*sin(angleZ*D3DX_PI/180)+n.y*cos(angleZ*D3DX_PI/180);
				n.x=n.x+opv.x;
				n.y=n.y+opv.y;

				m_PosEAvall = point;
				InvalidateRect(NULL,false);
				}

	// Transformaci� Geom�trica interactiva pels eixos X,Y boto esquerra del mouse
				else {	bool transE = transX || transY;
						if (m_ButoEAvall && transE && transf)
						{
	// Calcular increment
							CSize girT = m_PosEAvall - point;
							if (transX)
								{	long int incrT=girT.cx;
									if (trasl)
										{ VTras.x+=incrT*fact_Tras;
										  if (VTras.x<-100000) VTras.x=100000;
										  if (VTras.x>100000) VTras.x=100000;
										}
										else if (rota)
												{	VRota.x+=incrT*fact_Rota;
													while (VRota.x>=360) VRota.x-=360;
													while (VRota.x<0) VRota.x+=360;
												}
												else if (escal)
													{ if (incrT<0) incrT=-1/incrT;
													  VScal.x=VScal.x*incrT;
													  if(VScal.x<0.25) VScal.x=0.25;
													  if(VScal.x>8192) VScal.x=8192;
													}
								}
							if (transY)
								{	long int incrT=girT.cy;
									if (trasl)
										{	VTras.y+=incrT*fact_Tras;
											if (VTras.y<-100000) VTras.y=100000;
											if (VTras.y>100000) VTras.y=100000;
										}
										else if (rota)
											{	VRota.y+=incrT*fact_Rota;
												while (VRota.y>=360) VRota.y-=360;
												while (VRota.y<0) VRota.y+=360;
											}
											else if (escal)
											{	if (incrT<=0) {	if (incrT>=-2) incrT=-2;
																incrT=1/Log2(-incrT);}
													else incrT=Log2(incrT);
												VScal.y=VScal.y*incrT;
												if(VScal.y<0.25) VScal.y=0.25;
												if(VScal.y>8192) VScal.y=8192;
												}
								}
						m_PosEAvall = point;
						InvalidateRect(NULL,false);
						}
					}

	// Determinaci� del despla�ament del pan segons l'increment
	//   vertical de la posici� del mouse (tecla dreta apretada)
			if (m_ButoDAvall && pan && (projeccio!=CAP && projeccio!=ORTO))
			{	CSize zoomincr=m_PosDAvall - point;
				long int incrx=zoomincr.cx;
				long int incry=zoomincr.cy;

				// Despla�ament pan vertical
				tr_cpv.y-=incry*fact_pan;
				if(tr_cpv.y>100000) tr_cpv.y=100000;
				else if(tr_cpv.y<-100000) tr_cpv.y=-100000;

				// Despla�ament pan horitzontal
				tr_cpv.x+=incrx*fact_pan;
				if(tr_cpv.x>100000) tr_cpv.x=100000;
				else if(tr_cpv.x<-100000) tr_cpv.x=-100000;

				m_PosDAvall=point;
				InvalidateRect(NULL,false);
			}
	// Determinaci� del par�metre R segons l'increment
	//   vertical de la posici� del mouse (tecla dreta apretada)
			else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
				{
					CSize zoomincr=m_PosDAvall - point;
					long int incr=zoomincr.cy/1.0;

	//				zoom=zoom+incr;
					R=R+incr;
					if (R<1) R=1;
					m_PosDAvall=point;
					InvalidateRect(NULL,false);
				}
				else if (m_ButoDAvall && navega && (projeccio!=CAP && projeccio!=ORTO))
					{
	// Avan�ar en opci� de Navegaci�
					if (m_PosDAvall!=point) 
						{
						CSize zoomincr=m_PosDAvall - point;

						float incr=zoomincr.cy/2;
	//					long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
						vdir.x=n.x-opv.x;
						vdir.y=n.y-opv.y;
						vdir.z=n.z-opv.z;
						modul=sqrt(vdir.x*vdir.x+vdir.y*vdir.y+vdir.z*vdir.z);
						vdir.x=vdir.x/modul;
						vdir.y=vdir.y/modul;
						vdir.x=vdir.x/modul;
						opv.x+=incr*vdir.x;
						opv.y+=incr*vdir.y;
						n.x+=incr*vdir.x;
						n.y+=incr*vdir.y;
						m_PosDAvall=point;
						InvalidateRect(NULL,false);
						}
					}

	// Transformaci� Geom�trica interactiva per l'eix Z amb boto dret del mouse
					else if (m_ButoDAvall && transZ && transf)
							{
	// Calcular increment
								CSize girT = m_PosDAvall - point;
								long int incrT=girT.cy;
								if (trasl)
									{ VTras.z+=incrT*fact_Tras;
									  if (VTras.z<-100000) VTras.z=100000;
									  if (VTras.z>100000) VTras.z=100000;
									}
									else if (rota)
											{	incrT=girT.cx;
												VRota.z+=incrT*fact_Rota;
												while (VRota.z>=360) VRota.z-=360;
												while (VRota.z<0) VRota.z+=360;
											}
										else if (escal)
											{	if (incrT<=0) {	if (incrT>=-2) incrT=-2;
																incrT=1/Log2(-incrT);}
													else incrT=Log2(incrT);
												VScal.z=VScal.z*incrT;
												if(VScal.z<0.25) VScal.z=0.25;
												if(VScal.z>8192) VScal.z=8192;
											}
		
								m_PosDAvall = point;
								InvalidateRect(NULL,false);
							}

		CView::OnMouseMove(nFlags, point);
//	 Do not call CView::OnPaint() for painting messages
	 }
}

/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL TECLAT                              */
/* ------------------------------------------------------------------------- */

// OnKeyDown: Funci� de tractament de teclat (funci� que es crida quan es prem una tecla)
//   PAR�METRES:
//    - nChar: Codi del caracter seleccionat
//    - nRepCnt: Nombre de vegades que s'ha apretat la tecla (acceleraci�)
//    - nFlags: Flags d'interrupci� activats.
void CPracticaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

const float incr=0.025f;
float modul=0;
D3DXVECTOR3 vdir=D3DXVECTOR3(0.f,0.f,0.f);

	if ((!pan) && (!transf) && (!navega)) {

		if (nChar==VK_CONTROL && !TeclaControl)
		{
			TeclaControl = true;
			TeclaTab = false;
		}
		else
			TeclaControl = false;
		if (nChar==VK_TAB && !TeclaTab)
		{
			TeclaTab = true;
			TeclaControl = false;
		}
		else
			TeclaTab = false;

	}else {	
			if (transf)
				{ if (rota)
// Modificar vector de rotaci� per teclat.
					{switch(nChar)
						{
						// Tecla cursor amunt ('8')
						case 104:
							VRota.x+=nRepCnt*fact_Rota;
							if(VRota.x>=360) VRota.x-=360;
							break;

						// Tecla cursor avall ('2')
						case 98:
							VRota.x-=nRepCnt*fact_Rota;
							if(VRota.x<1) VRota.x+=360;
							break;

						// Tecla cursor esquerra ('4')
						case 100:
							VRota.y-=nRepCnt*fact_Rota;
							if(VRota.y<1) VRota.y+=360;
							break;
				
						// Tecla cursor dret ('6')
						case 102:
							VRota.y+=nRepCnt*fact_Rota;
							if(VRota.y>=360) VRota.y-=360;
							break;

						// Tecla HOME ('7')
						case 103:
							VRota.z+=nRepCnt*fact_Rota;
							if(VRota.z>=360) VRota.z-=360;
							break;
				
						// Tecla END ('1')
						case 97:
							VRota.z-=nRepCnt*fact_Rota;
							if(VRota.z<1) VRota.z+=360;
							break;

						// Tecla PgUp ('9')
						case 105:
							fact_Rota/=2;
							if (fact_Rota<1) fact_Rota=1;
							break;
				
						// Tecla PgDown ('3')
						case 99:
							fact_Rota*=2;
							if(fact_Rota>90) fact_Rota=90;
							break;

// Modificar vector d'Escalatge per teclat (actiu amb Rotaci�)
						// Tecla '+' (augmentar escalat)
						case 107:
							VScal.x=VScal.x*2;
							if(VScal.x>8192) VScal.x=8192;
							VScal.y=VScal.y*2;
							if(VScal.y>8192) VScal.y=8192;
							VScal.z=VScal.z*2;
							if(VScal.z>8192) VScal.z=8192;
							break;

						// Tecla '-' (disminuir escalat)
						case 109:
							VScal.x=VScal.x/2;
							if(VScal.x<0.25) VScal.x=0.25;
							VScal.y=VScal.y/2;
							if(VScal.y<0.25) VScal.y=0.25;
							VScal.z=VScal.z/2;
							if(VScal.z<0.25) VScal.z=0.25;
							break;
							
						// Tecla Espaiador
						case VK_SPACE:
							rota=!rota;
							trasl=!trasl;
							break;
//						default:		
//								break;
						}
					}
					else if (trasl)
						{switch(nChar)
							{
// Modificar vector de Traslaci� per teclat.
							// Tecla cursor amunt ('8')
							case 104:
								VTras.x-=nRepCnt*fact_Tras;
								if(VTras.x<-100000) VTras.x=100000;
								break;

							// Tecla cursor avall ('2')
							case 98:
								VTras.x+=nRepCnt*fact_Tras;
								if(VTras.x>10000) VTras.x=100000;
								break;

							// Tecla cursor esquerra ('4')
							case 100:
								VTras.y-=nRepCnt*fact_Tras;
								if(VTras.y<-100000) VTras.y=-100000;
								break;
				
							// Tecla cursor dret ('6')
							case 102:
								VTras.y+=nRepCnt*fact_Tras;
								if(VTras.y>100000) VTras.y=100000;
								break;
	
							// Tecla HOME ('7')
							case 103:
								VTras.z+=nRepCnt*fact_Tras;
								if(VTras.z>100000) VTras.z=100000;
								break;
					
							// Tecla END ('1')
							case 97:
								VTras.z-=nRepCnt*fact_Tras;
								if(VTras.z<-100000) VTras.z=-100000;
								break;
	
							// Tecla PgUp ('9')
							case 105:
								fact_Tras/=2;
								if (fact_Tras<1) fact_Tras=1;
								break;
					
							// Tecla PgDown ('3')
							case 99:
								fact_Tras*=2;
								if(fact_Tras>100000) fact_Tras=100000;
								break;

// Modificar vector d'Escalatge per teclat (actiu amb Traslaci�)
							// Tecla '+' (augmentar escalat)
							case 107:
								VScal.x=VScal.x*2;
								if(VScal.x>8192) VScal.x=8192;
								VScal.y=VScal.y*2;
								if(VScal.y>8192) VScal.y=8192;
								VScal.z=VScal.z*2;
								if(VScal.z>8192) VScal.z=8192;
								break;

							// Tecla '-' (disminuir escalat)
							case 109:
								VScal.x=VScal.x/2;
								if(VScal.x<0.25) VScal.x=0.25;
								VScal.y=VScal.y/2;
								if(VScal.y<0.25) VScal.y=0.25;
								VScal.z=VScal.z/2;
								if(VScal.z<0.25) VScal.z=0.25;	
								break;

							// Tecla Espaiador
							case VK_SPACE:
								rota=!rota;
								trasl=!trasl;
								break;	
//							default:		
//								break;
							}
						}
					else if (escal)
							{/*3*/switch(nChar)
								{/*4*/
// Modificar vector d'Escalatge per teclat (actiu amb Escalat �nicament)
								// Tecla '+' (augmentar tot l'escalat)
								case 107:
									VScal.x=VScal.x*2;
									if(VScal.x>8192) VScal.x=8192;
									VScal.y=VScal.y*2;
									if(VScal.y>8192) VScal.y=8192;
									VScal.z=VScal.z*2;
									if(VScal.z>8192) VScal.z=8192;
									break;

								// Tecla '-' (disminuir tot l'escalat)
								case 109:
									VScal.x=VScal.x/2;
									if(VScal.x<0.25) VScal.x=0.25;
									VScal.y=VScal.y/2;
									if(VScal.y<0.25) VScal.y=0.25;
									VScal.z=VScal.z/2;
									if(VScal.z<0.25) VScal.z=0.25;	
									break;
								// Tecla cursor amunt ('8')
								case 104:
									VScal.x=VScal.x*2;
									if(VScal.x>8192) VScal.x=8192;
									break;

								// Tecla cursor avall ('2')
								case 98:
									VScal.x=VScal.x/2;
									if(VScal.x<0.25) VScal.x=0.25;
									break;

								// Tecla cursor esquerra ('4')
								case 100:
									VScal.y=VScal.y/2;
									if(VScal.y<0.25) VScal.y=0.25;
									break;
						
								// Tecla cursor dret ('6')
								case 102:
									VScal.y=VScal.y*2;
									if(VScal.y>8192) VScal.y=8192;
									break;

								// Tecla HOME ('7')
								case 103:
									VScal.z=VScal.z*2;
									if(VScal.z>8192) VScal.z=8192;
									break;
				
								// Tecla END ('1')
								case 97:
									VScal.z=VScal.z/2;
									if(VScal.z<0.25) VScal.z=0.25;	
									break;	
	
//								default:		
//									break;
							/*4*/}
						/*3*/}
						}
//				}
				if (pan) {
					switch(nChar)
						{
							// Tecla cursor amunt
							case VK_UP:
								tr_cpv.y-=nRepCnt*fact_pan;
								if(tr_cpv.y<-100000) tr_cpv.y=100000;
								break;

							// Tecla cursor avall
							case VK_DOWN:
								tr_cpv.y+=nRepCnt*fact_pan;
								if(tr_cpv.y>100000) tr_cpv.y=100000;
								break;

							// Tecla cursor esquerra
							case VK_LEFT:
								tr_cpv.x+=nRepCnt*fact_pan;
								if(tr_cpv.x>100000) tr_cpv.x=100000;
								break;
					
							// Tecla cursor dret
							case VK_RIGHT:
								tr_cpv.x-=nRepCnt*fact_pan;
								if(tr_cpv.x<-100000) tr_cpv.x=100000;
								break;

							// Tecla PgUp
							case VK_PRIOR:
								fact_pan/=2;
								if(fact_pan<0.125) fact_pan=0.125;
								break;
					
							// Tecla PgDown
							case VK_NEXT:
								fact_pan*=2;
								if(fact_pan>2048) fact_pan=2048;
								break;

							default:		
								break;
						}
					}
					else if (navega)
// Controls de moviment de navegaci�
							{ vdir.x=n.x-opv.x;
							  vdir.y=n.y-opv.y;
							  vdir.z=n.z-opv.z;
							  modul=sqrt(vdir.x*vdir.x+vdir.y*vdir.y+vdir.z*vdir.z);
							  vdir.x=vdir.x/modul;
							  vdir.y=vdir.y/modul;
							  vdir.z=vdir.z/modul;
							  switch(nChar)
								{
								// Tecla cursor amunt
								case VK_UP:
									opv.x+=nRepCnt*fact_pan*vdir.x;
									opv.y+=nRepCnt*fact_pan*vdir.y;
									n.x+=nRepCnt*fact_pan*vdir.x;
									n.y+=nRepCnt*fact_pan*vdir.y;
									break;

								// Tecla cursor avall
								case VK_DOWN:
									opv.x-=nRepCnt*fact_pan*vdir.x;
									opv.y-=nRepCnt*fact_pan*vdir.y;
									n.x-=nRepCnt*fact_pan*vdir.x;
									n.y-=nRepCnt*fact_pan*vdir.y;
									break;

								// Tecla cursor esquerra
								case VK_LEFT:
									angleZ=+nRepCnt*fact_pan;
									n.x=n.x-opv.x;
									n.y=n.y-opv.y;
									n.x=n.x*cos(angleZ*D3DX_PI/180)-n.y*sin(angleZ*D3DX_PI/180);
									n.y=n.x*sin(angleZ*D3DX_PI/180)+n.y*cos(angleZ*D3DX_PI/180);
									n.x=n.x+opv.x;
									n.y=n.y+opv.y;
									break;
						
								// Tecla cursor dret
								case VK_RIGHT:
									angleZ=360-nRepCnt*fact_pan;
									n.x=n.x-opv.x;
									n.y=n.y-opv.y;
									n.x=n.x*cos(angleZ*D3DX_PI/180)-n.y*sin(angleZ*D3DX_PI/180);
									n.y=n.x*sin(angleZ*D3DX_PI/180)+n.y*cos(angleZ*D3DX_PI/180);
									n.x=n.x+opv.x;
									n.y=n.y+opv.y;
									break;

								// Tecla Inicio
								case VK_HOME:
									opv.z+=nRepCnt*fact_pan;
									n.z+=nRepCnt*fact_pan;
									break;
				
								// Tecla Fin
								case VK_END:
									opv.z-=nRepCnt*fact_pan;
									n.z-=nRepCnt*fact_pan;
									break;

								// Tecla PgUp
								case VK_PRIOR:
									fact_pan/=2;
									if(fact_pan<1) fact_pan=1;
									break;
				
								// Tecla PgDown
								case VK_NEXT:
									fact_pan*=2;
									if(fact_pan>2048) fact_pan=2048;
									break;

								default:		
									break;
								}
							}

				}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();	
}

/* ------------------------------------------------------------------------- */
/*   RECURSOS DE MENU (persianes) DE L'APLICACIO:                            */
/*					1. ARXIUS												 */
/*					2. VEURE: ToolBar, StatusBar, Pantalla Completa			 */
/*					3. VISTA: Punt de Vista, Zoom, Pan Navega i Eixos	     */
/*					4. PROJECCI�                                             */
/*					5. OBJECTE					                             */
/*					6. TRANSFORMA											 */
/*					7. OCULTACIONS											 */
/*					8. IL.LUMINACI�											 */
/*					9. TIMER (ANIMACI�)									     */
/*					10. MUSCLES												 */
/*					11. EXPRESSIONS											 */
/*					12. ANIMACI�											 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*					1. ARXIUS 												 */
/* ------------------------------------------------------------------------- */

// Obrir fitxer
//void CPracticaView::OnFileOpen() 
//{
//
////  Obrir di�leg de lectura de fitxer (fitxers (*.MNT)
//	CFileDialog openMunt (TRUE, NULL, NULL,
//		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
//		_T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;
//
//	if (openMunt.DoModal() != IDOK)
//		return;                 // stay with old data file
//    else
//	   {nom=openMunt.GetPathName();}
//
//// Variable de tipus CString 'nom' cont� el nom del fitxer seleccionat
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();		
//}

// Obrir fitxer en format gr�fic 3DS
void CPracticaView::OnFileOpen3ds()
{
	if(ObOBJ!=NULL) delete ObOBJ;

	// Obrir di�leg de lectura de fitxer
		CFileDialog openOBJ (TRUE, NULL, NULL,
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
			_T("3DS Files(*.3ds)|*.3ds|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

			if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
			else nom=openOBJ.GetPathName();
	
	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;

	/* i carreguem */	
	ModelManager::GetInstance()->Load(std::string(nomfitx));
	ObOBJ = ModelManager::GetInstance()->GetActor();
	//ObOBJ = new Actor(nomfitx,TIPUS_3DS);
	editor = new EditorManager();

	//Resetejar la mem�ria de Selection
	if (select != NULL)
		select->SetFlagsTriangles();

	//Borrem els muscles i expressions que hi pugui haver
	int expressions = ExpressionManager::GetInstance()->getNumExpressions();
		for (int i=0; i<expressions; ++i)
		{
			ExpressionManager::GetInstance()->resetExpression((TypeExpression) i);
		}
		int muscles = MuscleManager::GetInstance()->getNumMuscles();
		for (int i=0; i<muscles;++i)
		{
			MuscleManager::GetInstance()->ClearMuscle((TypeMuscle) i);
		}
		editor->ClearVertexs();

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}


// OnFileOpenObj: Obrir fitxer en format gr�fic OBJ
void CPracticaView::OnFileOpenObj()
{
	if(ObOBJ!=NULL) delete ObOBJ;

	// Obrir di�leg de lectura de fitxer
		CFileDialog openOBJ (TRUE, NULL, NULL,
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
			_T("OBJ Files(*.obj)|*.obj|All Files (*.*)|*.*||"));;

			if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
			else nom=openOBJ.GetPathName();

	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;

	/* i carreguem */	
	ModelManager::GetInstance()->Load(std::string(nomfitx));
	ObOBJ = ModelManager::GetInstance()->GetActor();
	//ObOBJ = new Actor(nomfitx,TIPUS_OBJ);
	editor = new EditorManager();

	//Resetejar la mem�ria de Selection
	if (select != NULL)
		select->SetFlagsTriangles();
	//Borrem els muscles i expressions que hi pugui haver
		int expressions = ExpressionManager::GetInstance()->getNumExpressions();
		for (int i=0; i<expressions; ++i)
		{
			ExpressionManager::GetInstance()->resetExpression((TypeExpression) i);
		}
		int muscles = MuscleManager::GetInstance()->getNumMuscles();
		for (int i=0; i<muscles;++i)
		{
			MuscleManager::GetInstance()->ClearMuscle((TypeMuscle) i);
		}
		editor->ClearVertexs();

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();	
}

void CPracticaView::OnCarregaAutomatica()
{
	ModelManager::GetInstance()->Load(std::string(ACTORS_XML));
	ModelManager::GetInstance()->Load(std::string(SCENE_XML),true);
	CParla::GetInstance()->PrepareFontSubs(std::string(FONT_XML));
	CParla::GetInstance()->LoadXML(std::string(SCENE_XML));
	CParla::GetInstance()->SetTextToTalk(std::string(SUBTITLES));

	ObOBJ = ModelManager::GetInstance()->GetActor();
	std::string m_musclePath = ModelManager::GetInstance()->GetMusclePath();
	std::string m_expressionPath = ModelManager::GetInstance()->GetExpressionPath();

	if (ObOBJ != NULL)
	{
		editor = new EditorManager();

		// Eliminaci� de la mem�ria en Selection, Expressions i Muscles
		{
			//Reseteja la mem�ria de Selection
			if (select != NULL)
				select->SetFlagsTriangles();

			//Borrem els muscles i expressions que hi pugui haver
			int expressions = ExpressionManager::GetInstance()->getNumExpressions();
			for (int i=0; i<expressions; ++i)
			{
				ExpressionManager::GetInstance()->resetExpression((TypeExpression) i);
			}
			int muscles = MuscleManager::GetInstance()->getNumMuscles();
			for (int i=0; i<muscles;++i)
			{
				MuscleManager::GetInstance()->ClearMuscle((TypeMuscle) i);
			}
			editor->ClearVertexs();
		}
		
		MuscleManager::GetInstance()->Load(m_musclePath);
		ExpressionManager::GetInstance()->Load(m_expressionPath);
	}

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();	
}

// Escriure fitxer
//void CPracticaView::OnFileSaveAs() 
//{
//
////  Obrir di�leg d'escriptura de fitxer (fitxers *.MNT)
//	CFileDialog openMunt (FALSE, NULL, NULL,
//		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
//		_T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;
//
//	if (openMunt.DoModal() != IDOK)
//		return;                 // stay with old data file
//    else
//	   {nom=openMunt.GetPathName();}
//
//// Variable de tipus CString 'nom' cont� el nom del fitxer seleccionat
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}

/* ------------------------------------------------------------------------- */
/*					2. VEURE: ToolBar), StatusBar, Pantalla Completa         */
/* ------------------------------------------------------------------------- */

// PANTALLA COMPLETA: opci� booleana
//void CPracticaView::OnFullscreen() 
//{
//// TODO: Add your command handler code here
//	fullscreen=!fullscreen;
//
//}

void CPracticaView::OnUpdateFullscreen(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (fullscreen)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);
}

/* ------------------------------------------------------------------------- */
/*					3. VISTA: Punt de Vista, Zoom, Pan i Eixos				 */
/* ------------------------------------------------------------------------- */

// Punt de Vista Interactiu (opci� booleana)
void CPracticaView::OnMobil() 
{
// TODO: Add your command handler code here
	if (projeccio==PERSPECT) mobil=!mobil;
// Desactivaci� de Transformacions Geom�triques via mouse 
//	si Visualitzaci� Interactiva activada	
	if (mobil)	{	transX=false;		
					transY=false; 		
					transZ=false;
				}
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateMobil(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (mobil)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);
}

// Zoom Interactiu (opci� booleana)
void CPracticaView::OnZoom() 
{
// TODO: Add your command handler code here
	if (projeccio==PERSPECT) zzoom=!zzoom;
// Desactivaci� de Transformacions Geom�triques via mouse 
//	si Zoom activat
	if (zzoom)	{	transX=false;
					transY=false;
					transZ=false;
				}
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateZoom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (zzoom)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);
}


// VISTA: Polars Eix Z cap amunt per a Visialitzaci� Interactiva
void CPracticaView::OnPolarZ()
{
// TODO: Add your command handler code here
	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARZ;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();

}

void CPracticaView::OnUpdatePolarZ(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
	if (Vis_Polar==POLARZ)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);

}

// VISTA: Polars Eix Y cap amunt per a Visialitzaci� Interactiva
void CPracticaView::OnPolarY()
{
// TODO: Add your command handler code here
	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARY;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();

}

void CPracticaView::OnUpdatePolarY(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
	if (Vis_Polar==POLARY)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);

}


// VISTA: Polars Eix X cap amunt per a Visialitzaci� Interactiva
void CPracticaView::OnPolarX()
{
// TODO: Add your command handler code here
	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARX;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();

}

void CPracticaView::OnUpdatePolarX(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
	if (Vis_Polar==POLARX)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);

}


// PAN: Despla�ament del Centre de l'esfera de visualitzaci� (opci� booleana)
void CPracticaView::OnPan() 
{
// TODO: Add your command handler code here
	if (projeccio==PERSPECT) pan=!pan;
// Desactivaci� de Transformacions Geom�triques via mouse i navega si pan activat
	if (pan)	{	mobil=true;		zzoom=true;
					transX=false;	transY=false;	transZ=false;
					navega=false;
				}
	
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();		
}

void CPracticaView::OnUpdatePan(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (pan)
		pCmdUI->SetCheck(1);
	else
        pCmdUI->SetCheck(0);
}


// VISTA: Origen (inicialitzar) Pan
void CPracticaView::OnInipan() 
{
// TODO: Add your command handler code here
	if (pan) {	fact_pan=1;
				tr_cpv = D3DXVECTOR3(0.f, 0.f, 0.f);
				}
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();	
}


// VISTA: Opci� Navega (opci� booleana)
void CPracticaView::OnNavega() 
{
// TODO: Add your command handler code here
	if (projeccio==PERSPECT) navega=!navega;
// Desactivaci� de zoom, mobil, Transformacions Geom�triques via mouse i pan 
//		si navega activat
	if (navega){	mobil=false;	zzoom=false;
					pan=false;
					transX=false;	transY=false;	transZ=false;
				}
	else {mobil=true;
		  zzoom=true;}

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();	
}

void CPracticaView::OnUpdateNavega(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if(navega)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}


// VISTA: Origen Navegaci�
void CPracticaView::OnIninav() 
{
// TODO: Add your command handler code here
	if (navega) {	n.x=0.0;		n.y=0.0;		n.z=0.0;
					opv.x=10.0;		opv.y=0.0;		opv.z=0.0;
					angleZ=0.0;
				}
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}


// VISTA: Dibuixar els eixos (opci� booleana)
void CPracticaView::OnEixos() 
{
// TODO: Add your command handler code here
	eixos=!eixos;
// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateEixos(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if(eixos)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

/* ------------------------------------------------------------------------- */
/*					4. PROJECCI�                                             */
/* ------------------------------------------------------------------------- */

// PROJECCI�: Perspectiva
//void CPracticaView::OnPerspectiva() 
//{
//	//Fixat en OnPaint() que dibuixi en Perspectiva
//// TODO: Add your command handler code here
//	projeccio=PERSPECT;
//	mobil=true;			zzoom=true;
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();	
//}
//
//void CPracticaView::OnUpdatePerspectiva(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if (projeccio==PERSPECT)
//		pCmdUI->SetCheck(1);
//	else
//        pCmdUI->SetCheck(0);	
//}
/* ------------------------------------------------------------------------- */
/*					5. OBJECTE					                             */
/* ------------------------------------------------------------------------- */

// OBJECTE Cub
//void CPracticaView::OnCub() 
//{
//// TODO: Add your command handler code here
//	objecte=CUB;
//
//// 	----GC2: Modificar R per centrar Vista amb mida de l'objecte
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();	
//}

//void CPracticaView::OnUpdateCub(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(objecte==CUB)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


// OBJECTE Esfera
//void CPracticaView::OnEsfera() 
//{
//// TODO: Add your command handler code here
//	objecte=ESFERA;
//
//// 	----GC2: Modificar R per centrar Vista amb mida de l'objecte
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();	
//}

//void CPracticaView::OnUpdateEsfera(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(objecte==ESFERA)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


// OBJECTE Tetera
//void CPracticaView::OnTeapot() 
//{
//// TODO: Add your command handler code here
//	objecte=TETERA;
//
//// 	----GC2: Modificar R per centrar Vista amb mida de l'objecte
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}

//void CPracticaView::OnUpdateTeapot(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(objecte==TETERA)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


// OBJECTE Truck
//void CPracticaView::OnTruck() 
//{
//// TODO: Add your command handler code here
//	objecte=TRUCK;
//
//// Activaci� el contexte OpenGL
//    wglMakeCurrent(m_hDC,m_hRC);
//// Inicialitza vector imatges textura pel Truck.
//	Init_Textures();
//// Desactivaci� contexte OpenGL
//	wglMakeCurrent(m_hDC,NULL);
//
//// 	----GC2: Modificar R per centrar Vista amb mida de l'objecte
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}

//void CPracticaView::OnUpdateTruck(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(objecte==TRUCK)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


// OBJECTE 3DS
//void CPracticaView::OnObj3ds() 
//{
//// TODO: Add your command handler code here
//	if(Ob3DS!=NULL) delete Ob3DS;
//
//	objecte=OBJ3DS;
//
//// Obrir di�leg de lectura de fitxer
//		CFileDialog open3DS (TRUE, NULL, NULL,
//			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
//			_T("3DS Files(*.3ds)|*.3ds|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;
//
//			if (open3DS.DoModal() != IDOK)	return;  // stay with old data file
//			else nom=open3DS.GetPathName();
//	
//// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;
//
//	/* i carreguem */	
//	wglMakeCurrent(m_hDC,m_hRC);	// Activem contexte OpenGL
//	Ob3DS = new Obj_3DS;
//	Ob3DS->EliminarMemoria();
//	Ob3DS->Inicialitzar();
//	Ob3DS->Carregar3DS(nomfitx);
//
//// objecte nou;index DisplayList n�2; amb textures
//	Ob3DS->Dibuixa3DS(false,ActorS);
//	wglMakeCurrent(m_hDC,NULL);	// Desactivem contexte OpenGL
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}


//void CPracticaView::OnUpdateObj3ds(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if (objecte==OBJ3DS)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// OBJECTE OBJ
//void CPracticaView::OnObjobj()
//{
//// TODO: Add your command handler code here
//	if(ObOBJ!=NULL) delete ObOBJ;
//
//	objecte=OBJOBJ;
//
//// Obrir di�leg de lectura de fitxer
//		CFileDialog openOBJ (TRUE, NULL, NULL,
//			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
//			_T("OBJ Files(*.obj)|*.obj|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;
//
//			if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
//			else nom=openOBJ.GetPathName();
//	
//// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;
//
//	/* i carreguem */	
//	wglMakeCurrent(m_hDC,m_hRC);	// Activem contexte OpenGL
//	ObOBJ = new Actor(nomfitx,TIPUS_OBJ);
//	ObOBJ->Dibuixar(OBJECTEOBJ);
//	wglMakeCurrent(m_hDC,NULL);	// Desactivem contexte OpenGL
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateObjobj(CCmdUI *pCmdUI)
//{
//// TODO: Add your command update UI handler code here
//	if (objecte==OBJOBJ)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


/* ------------------------------------------------------------------------- */
/*					6. TRANSFORMA											 */
/* ------------------------------------------------------------------------- */

//// TRANSFORMA: TRASLACI�
//void CPracticaView::OnTraslacio() 
//{
//// TODO: Add your command handler code here	
//	trasl=!trasl;
//	rota=false;
//	if (trasl) escal=true;
//	transf=trasl || rota || escal;
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateTraslacio(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(trasl)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// Reinicialitza a l'origen la Traslaci�
//void CPracticaView::OnInitras() 
//{
//// TODO: Add your command handler code here	
//	if (trasl) 
//		{	fact_Tras=1;
//			VTras.x=0.0;	VTras.y=0.0;	VTras.z=0;	
//		}		
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//// TRANSFORMA: ROTACI�
//void CPracticaView::OnRotacio() 
//{
//// TODO: Add your command handler code here
//	rota=!rota;
//	trasl=false;
//	if (rota) escal=true;
//	transf=trasl || rota || escal;
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateRotacio(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(rota)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// Reinicialitza a l'origen la Rotaci�
//void CPracticaView::OnInirot() 
//{
//// TODO: Add your command handler code here
//	if (rota) 
//		{	fact_Rota=90;
//			VRota.x=0;		VRota.y=0;		VRota.z=0;
//		}
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//
//// TRANSFORMA: ESCALATGE
//void CPracticaView::OnEscalatge() 
//{
//// TODO: Add your command handler code here
//	if ((!rota) && (!trasl)) escal=!escal;
//	transf=trasl || rota || escal;
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateEscalatge(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if(escal)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// Reinicialitza a l'origen l'Escalatge
//void CPracticaView::OnIniescal() 
//{
//// TODO: Add your command handler code here
//	if (escal) {VScal.x=1;	VScal.y=1;	VScal.z=1;}
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//// TRANSFORMA: Transformacions Geom�triques per l'eix X movent el mouse (booleana)
//void CPracticaView::OnMobilx() 
//{
//// TODO: Add your command handler code here
//	if (transf) 
//		{	transX=!transX;
//			if (transX) {	mobil=false;			zzoom=false;
//							pan=false;				navega=false;
//						}
//				else if ((!transY) && (!transZ)){	mobil=true;
//													zzoom=true;
//												}	
//		}
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateMobilx(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if (transX)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// TRANSFORMA: Transformacions Geom�triques per l'eix Y movent el mouse (booleana)
//void CPracticaView::OnMobily() 
//{
//// TODO: Add your command handler code here
//	if (transf) 
//		{	transY=!transY;
//			if (transY) {	mobil=false;			zzoom=false;
//							pan=false;				navega=false;
//						}
//				else if ((!transX) && (!transZ)){	mobil=true;
//													zzoom=true;
//												}
//		}
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateMobily(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if (transY)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}
//
//// TRANSFORMA: Transformacions Geom�triques per l'eix X movent el mouse (booleana)
//void CPracticaView::OnMobilz() 
//{
//// TODO: Add your command handler code here
//	if (transf) 
//		{	transZ=!transZ;
//			if (transZ) {	mobil=false;			zzoom=false;
//							pan=false;				navega=false;
//						}
//				else if ((!transX) && (!transY)) {	mobil=true;
//													zzoom=true;
//												}	
//		}
//
//// Crida a OnPaint() per redibuixar l'escena
//	Invalidate();
//}
//
//void CPracticaView::OnUpdateMobilz(CCmdUI* pCmdUI) 
//{
//// TODO: Add your command update UI handler code here
//	if (transZ)
//		 pCmdUI->SetCheck(1);
//	else
//		 pCmdUI->SetCheck(0);
//}


/* ------------------------------------------------------------------------- */
/*					7. OCULTACIONS											 */
/* ------------------------------------------------------------------------- */

// OCULTACIONS: Test de Visibilitat (BACK FACE CULLING) (opci� booleana)
void CPracticaView::OnTest() 
{
// TODO: Add your command handler code here
	test_vis=!test_vis;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateTest(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (test_vis)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

// OCULTACIONS: Algorisme d'ocultacions Z-buffer (opci� booleana)
void CPracticaView::OnZBuffer() 
{
// TODO: Add your command handler code here
	oculta=!oculta;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateZBuffer(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (oculta)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

// OCULTACIONS: Dibuix cares darrera com a l�nies (opci� booleana)
void CPracticaView::OnBackLine() 
{
// TODO: Add your command handler code here
	back_line=!back_line;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateBackLine(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if (back_line)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					8. IL.LUMINACI�											 */
/* ------------------------------------------------------------------------- */

// IL.LUMINACI� Font de llum fixe? (opci� booleana).
void CPracticaView::OnIFixe() 
{
// TODO: Add your command handler code here
	ifixe=!ifixe;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateIFixe(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if(ifixe)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}


// IL.LUMINACI� Filferros
void CPracticaView::OnFilferros() 
{
// TODO: Add your command handler code here
	filferros = true;
	test_vis=false;

// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateFilferros(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if(filferros)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

// IL.LUMINACI� Suau
void CPracticaView::OnSuau() 
{
// TODO: Add your command handler code here
	filferros=false;
	test_vis=false;

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateSuau(CCmdUI* pCmdUI) 
{
// TODO: Add your command update UI handler code here
	if(!filferros)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					9. TIMER (ANIMACI�)										 */
/* ------------------------------------------------------------------------- */

// Rutina d'interrupci� pel crono
void CPracticaView::OnTimer(UINT nIDEvent) 
{
// TODO: Add your message handler code here and/or call default
	if (!subtitles)
	{
		if (this->animacio && !editExpression)	
		{
			if (acumulativeTime < temporitzador)
			{
				anima = true;
				Animation::GetInstance()->NextStepAnimation();
				Animation::GetInstance()->Render(ObOBJ);
				acumulativeTime += 0.004;
			}
			else
			{
				Animation::GetInstance()->FinalizeAnimation();
				KillTimer(WM_TIMER);
				acumulativeTime = 0.f;
				anima = false;
				subtitles = false;
			}
			// Crida a OnPaint() per redibuixar l'escena
			Invalidate();
		}
	}
	else
	{
		if (CParla::GetInstance()->IsTalking())
		{
			if (acumulativeTime < tempsParla)
			{
				anima = true;
				Animation::GetInstance()->NextStepAnimation();
				Animation::GetInstance()->Render(ObOBJ);
				acumulativeTime += 0.004;
			}
			else
			{
				Animation::GetInstance()->FinalizeAnimation();
				acumulativeTime = 0.f;
				CParla::GetInstance()->NextTalk(ObOBJ);
			}
		}
		else
		{
				KillTimer(WM_TIMER);
				acumulativeTime = 0.f;
				Animation::GetInstance()->FinalizeAnimation();
				anima = false;
		}
		Invalidate();
	}

	CView::OnTimer(nIDEvent);
}


// ---------------- GC2: Funcions locals a PracticaView.cpp

// Log2: C�lcul del log base 2 de num
int CPracticaView::Log2(int num)
{
	int tlog;

	if (num>=8192) tlog=13;
	else if (num>=4096) tlog=12;
	else if (num>=2048) tlog=11;
	else if (num>=1024) tlog=10;
	else if (num>=512) tlog=9;
	else if (num>=256) tlog=8;
	else if (num>=128) tlog=7;
	else if (num>=64) tlog=6;
	else if (num>=32) tlog=5;
	else if (num>=16) tlog=4;
	else if (num>=8) tlog=3;
	else if (num>=4) tlog=2;
	else if (num>=2) tlog=1;
	else tlog=0;

	return tlog;
}
/* ------------------------------------------------------------------------- */
/*					10. MUSCLES												 */
/* ------------------------------------------------------------------------- */
void CPracticaView::OnImportMuscles()
{
	// Obrir di�leg de lectura de fitxer
	CFileDialog openXML (TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
		_T("XML Files(*.xml)|*.xml|All Files (*.*)|*.*||"));;

	if (openXML.DoModal() != IDOK)	return;  // stay with old data file
	else nom=openXML.GetPathName();

	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;
	std::string path = nomfitx;
	MuscleManager::GetInstance()->Load(path);
	
	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnExportMuscles()
{
		//  Obrir di�leg d'escriptura de fitxer (fitxers *.MNT)
	CFileDialog openExportXML (FALSE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
		_T("XML Files(*.xml)|*.xml|All Files (*.*)|*.*||"));;

	if (openExportXML.DoModal() != IDOK)
		return;                 // stay with old data file
    else
	   {nom=openExportXML.GetPathName();}

	nom = nom + CString(".xml");
	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;

	// La variable nomfitx cont� tot el path del fitxer.
	if(ObOBJ != NULL)
	{
		XMLWriter* escriptura = new XMLWriter(nomfitx,1);
		escriptura->Guardar();
		delete escriptura;
	}


	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnMuscleEdit()
{
	editMuscle = !editMuscle;
	if (!editMuscle)
	{
		ChangeMuscleState(NONE_MUSCLE);
		SetRenderMuscle(selectedMuscle);
		//TODO Cridar funci� de EditorManager per tal de guardar les dades a Muscles
		editor->SaveMuscle();
		TeclaControl = false;
		TeclaTab = false;
	}

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateMuscleEdit(CCmdUI *pCmdUI)
{
	if(editMuscle)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::SwitchMuscle(TypeMuscle m)
{
	if (selectedMuscle != m)
	{
		if (ObOBJ != NULL)
			editor->SetMuscle(m);

		if (editExpression)
		{
			ChangeMuscleState(m);
			//TODO Aqu� hi va quan es defineixen els vectors directors
			SetRenderMuscle(selectedMuscle);
			if (deform != NULL)
			{
				deform->SetExpression(selectedExpression);
				deform->SetMuscle(selectedMuscle);
			}
		}
		else if (editMuscle)
		{
			ChangeMuscleState(m);
			SetRenderMuscle(selectedMuscle);
			//TODO Aqu� hi va el codi quan es defineixen els muscles
			/*if (ObOBJ != NULL)
				editor->SetMuscle(DCELLA);*/
		
		}
		else
		{
			ChangeMuscleState(m);
			//TODO Aqu� hi va el codi quan es visualitza el muscle sense editar
			SetRenderMuscle(selectedMuscle);
		}
	}
	else
	{
		selectedMuscle = NONE_MUSCLE;
		SetRenderMuscle(selectedMuscle);
	}

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnMCellesDreta()
{
	SwitchMuscle(DCELLA);
}

void CPracticaView::OnUpdateMCellesDreta(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DCELLA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMCellesEsquerra()
{
	SwitchMuscle(ECELLA);
}

void CPracticaView::OnUpdateMCellesEsquerra(CCmdUI *pCmdUI)
{
	if(selectedMuscle == ECELLA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaDreta()
{
	SwitchMuscle(DBOCA);
}

void CPracticaView::OnUpdateMBocaDreta(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaEsquerre()
{
	SwitchMuscle(EBOCA);
}

void CPracticaView::OnUpdateMBocaEsquerre(CCmdUI *pCmdUI)
{
	if(selectedMuscle == EBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaInferior()
{
	SwitchMuscle(INFBOCA);
}

void CPracticaView::OnUpdateMBocaInferior(CCmdUI *pCmdUI)
{
	if(selectedMuscle == INFBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMParpellesDreta()
{
	SwitchMuscle(DPARPELLA);
}

void CPracticaView::OnUpdateMParpellesDreta(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DPARPELLA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMParpellesEsquerra()
{
	SwitchMuscle(EPARPELLA);
}

void CPracticaView::OnUpdateMParpellesEsquerra(CCmdUI *pCmdUI)
{
	if(selectedMuscle == EPARPELLA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMGaltesDreta()
{
	SwitchMuscle(DGALTA);
}

void CPracticaView::OnUpdateMGaltesDreta(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DGALTA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMGaltesEsquerra()
{
	SwitchMuscle(EGALTA);
}

void CPracticaView::OnUpdateMGaltesEsquerra(CCmdUI *pCmdUI)
{
	if(selectedMuscle == EGALTA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaSuperior()
{
	SwitchMuscle(SUPBOCA);
}

void CPracticaView::OnUpdateMBocaSuperior(CCmdUI *pCmdUI)
{
	if(selectedMuscle == SUPBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaLateralE()
{
	SwitchMuscle(LATEBOCA);
}

void CPracticaView::OnUpdateMBocaLateralE(CCmdUI *pCmdUI)
{
	if(selectedMuscle == LATEBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaLateralD()
{
	SwitchMuscle(LATDBOCA);
}

void CPracticaView::OnUpdateMBocaLateralD(CCmdUI *pCmdUI)
{
	if(selectedMuscle == LATDBOCA)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaDentDalt()
{
	SwitchMuscle(DENTDALT);
}

void CPracticaView::OnUpdateMBocaDentDalt(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DENTDALT)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnMBocaDentBaix()
{
	SwitchMuscle(DENTBAIX);
}

void CPracticaView::OnUpdateMBocaDentBaix(CCmdUI *pCmdUI)
{
	if(selectedMuscle == DENTBAIX)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::ChangeMuscleState ( TypeMuscle muscle )
{
	//this->SwitchMuscle(selectedMuscle);
	selectedMuscle = muscle;
}
/* ------------------------------------------------------------------------- */
/*					11. EXPRESSIONS											 */
/* ------------------------------------------------------------------------- */
void CPracticaView::OnImportExpressions()
{
	// Obrir di�leg de lectura de fitxer
	CFileDialog openXML (TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
		_T("XML Files(*.xml)|*.xml|All Files (*.*)|*.*||"));;

	if (openXML.DoModal() != IDOK)	return;  // stay with old data file
	else nom=openXML.GetPathName();

	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;

	//Cridar al parsejador de fitxers XML per carregar les expressions.
	// La variable nomfitx cont� tot el path del fitxer.
	std::string  path = nomfitx;
	ExpressionManager::GetInstance()->Load(path);

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnExportExpressions()
{
	//  Obrir di�leg d'escriptura de fitxer (fitxers *.MNT)
	CFileDialog openExportXML (FALSE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
		_T("XML Files(*.xml)|*.xml|All Files (*.*)|*.*||"));;

	if (openExportXML.DoModal() != IDOK)
		return;                 // stay with old data file
    else
	   {nom=openExportXML.GetPathName();}

	nom = nom + CString(".xml");
	// Conversi� de la variable CString nom a la variable char *nomfitx, compatible amb la funci� carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;

	// La variable nomfitx cont� tot el path del fitxer.
	if (ObOBJ != NULL)
	{
		XMLWriter* escriptura = new XMLWriter(nomfitx,0);
		escriptura->Guardar();
		delete escriptura;
	}


	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnExpressionEdit()
{
	if (editMuscle)
		editor->SaveMuscle();

	editExpression = !editExpression;


	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnUpdateExpressionEdit(CCmdUI *pCmdUI)
{
	if(editExpression)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::SwitchExpression(TypeExpression e)
{
	if (ObOBJ != NULL && !animacio)
		this->ObOBJ->resetMoviments();

	if (!anima)
	{
		if (selectedExpression != e)
		{
			if (editExpression)
				ChangeExpressionState(e);
			else
			{
				ChangeExpressionState(e);
				if (!this->animacio)
					ExpressionManager::GetInstance()->RenderExpression(selectedExpression,ObOBJ);
				else
					SetAndStartAnimation(selectedExpression);
			}
		}
		else
		{
			if (!this->animacio)
				selectedExpression = NONE_EXPRESSION;
		}
	}

	// Crida a OnPaint() per redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnExpTrist()
{
	SwitchExpression(TRIST);
}
void CPracticaView::OnUpdateExpTrist(CCmdUI *pCmdUI)
{
	if(selectedExpression == TRIST)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnExpAlegre()
{
	SwitchExpression(ALEGRE);
}

void CPracticaView::OnUpdateExpAlegre(CCmdUI *pCmdUI)
{
	if(selectedExpression == ALEGRE)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnExpEnfadat()
{
	SwitchExpression(ENFADAT);
}

void CPracticaView::OnUpdateExpEnfadat(CCmdUI *pCmdUI)
{
	if(selectedExpression == ENFADAT)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnExpSerios()
{
	SwitchExpression(SERIOS);
}

void CPracticaView::OnUpdateExpSerios(CCmdUI *pCmdUI)
{
	if(selectedExpression == SERIOS)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnExpSorpres()
{
	SwitchExpression(SORPRES);
}

void CPracticaView::OnUpdateExpSorpres(CCmdUI *pCmdUI)
{
	if(selectedExpression == SORPRES)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresA()
{
	SwitchExpression(A);
}

void CPracticaView::OnUpdateLletresA(CCmdUI *pCmdUI)
{
	if(selectedExpression == A)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresE()
{
	SwitchExpression(E);
}

void CPracticaView::OnUpdateLletresE(CCmdUI *pCmdUI)
{
	if(selectedExpression == E)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresI()
{
	SwitchExpression(I);
}

void CPracticaView::OnUpdateLletresI(CCmdUI *pCmdUI)
{
	if(selectedExpression == I)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresO()
{
	SwitchExpression(O);
}

void CPracticaView::OnUpdateLletresO(CCmdUI *pCmdUI)
{
	if(selectedExpression == O)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresU()
{
	SwitchExpression(U);
}

void CPracticaView::OnUpdateLletresU(CCmdUI *pCmdUI)
{
	if(selectedExpression == U)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresBilabial()
{
	SwitchExpression(BILABIAL);
}

void CPracticaView::OnUpdateLletresBilabial(CCmdUI *pCmdUI)
{
	if(selectedExpression == BILABIAL)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}

void CPracticaView::OnLletresNeutre()
{
	SwitchExpression(NEUTRE);
}

void CPracticaView::OnUpdateLletresNeutre(CCmdUI *pCmdUI)
{
	if(selectedExpression == NEUTRE)
		 pCmdUI->SetCheck(1);
	else
		 pCmdUI->SetCheck(0);
}


void CPracticaView::ChangeExpressionState ( TypeExpression expression )
{
	//this->SwitchExpression(selectedExpression);
	selectedExpression = expression;
}

void CPracticaView::SetAndStartAnimation( TypeExpression expression )
{
	Animation::GetInstance()->SetTime(4, temporitzador);
	Animation::GetInstance()->StartAnimation(expression, ObOBJ);
	SetTimer(WM_TIMER,4,NULL);	
}

/* ------------------------------------------------------------------------- */
/*					12. ANIMACI�											 */
/* ------------------------------------------------------------------------- */
void CPracticaView::OnAnimacio()
{
	animacio = !animacio;
}

void CPracticaView::OnUpdateAnimacio(CCmdUI *pCmdUI)
{
	if (animacio)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnVFast()
{
	temporitzador = 0.05f;
	tempsParla = 0.005f;
	CParla::GetInstance()->SetVelocity(1.f, 0.005f);
}

void CPracticaView::OnUpdateVFast(CCmdUI *pCmdUI)
{
	if (temporitzador == 0.05f)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnFast()
{
	temporitzador = 0.1f;
	tempsParla = 0.01f;
	CParla::GetInstance()->SetVelocity(1.f, 0.007f);
}

void CPracticaView::OnUpdateFast(CCmdUI *pCmdUI)
{
	if (temporitzador == 0.1f)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnNormal()
{
	temporitzador = 0.5f;
	tempsParla = 0.02f;
	tParlaBucle = 10.0f;
	CParla::GetInstance()->SetVelocity(1.f, 0.01f);
}

void CPracticaView::OnUpdateNormal(CCmdUI *pCmdUI)
{
	if (temporitzador == 0.5f)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnSlow()
{
	temporitzador = 1.f;
	tempsParla = 0.1f;
	CParla::GetInstance()->SetVelocity(1.f, 0.03f);
}

void CPracticaView::OnUpdateSlow(CCmdUI *pCmdUI)
{
	if (temporitzador == 1.f)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnVSlow()
{
	temporitzador = 3.f;
	tempsParla = 0.3f;
	CParla::GetInstance()->SetVelocity(1.f, 0.09f);
}

void CPracticaView::OnUpdateVSlow(CCmdUI *pCmdUI)
{
	if (temporitzador == 3.f)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPracticaView::OnParla()
{
	subtitles = true;
	SetTimer(WM_TIMER,4,NULL);
	CParla::GetInstance()->StartTalk(ObOBJ);

	// Crida el OnPaint() per a redibuixar l'escena
	Invalidate();
}

void CPracticaView::OnParlaBucle()
{
	//Fa parlar el personatge SENSE fer �s del Timer.
	int numSubs = CParla::GetInstance()->GetNumSubs();
	int actSub = 0;

	while ( numSubs > actSub )
	{
		subtitles = true;
		/*parla->SetVelocity(8.f, 0.1f);*/
		CParla::GetInstance()->SetVelocity(11.f, 0.04f);
		CParla::GetInstance()->StartTalk(ObOBJ);

		acumulativeTime = 0.f;
		

		while ( CParla::GetInstance()->IsTalking() )
		{
			if (acumulativeTime < 0.3f)
			{
				Timer::GetInstance()->ResetTimer();
				anima = true;
				Animation::GetInstance()->NextStepAnimation();
				Animation::GetInstance()->Render(ObOBJ);
				acumulativeTime += Timer::GetInstance()->GetElapsedTime();
			}
			else
			{
				Animation::GetInstance()->StopAnimation();
				acumulativeTime = 0.f;
				CParla::GetInstance()->NextTalk(ObOBJ);
			}

			OnPaint();
		}

		acumulativeTime = 0.f;
		Animation::GetInstance()->StopAnimation();
		++ actSub;
	}
	Animation::GetInstance()->FinalizeAnimation();
	CParla::GetInstance()->ResetSubs();
	anima = false;
	subtitles = false;

	OnPaint();
}
