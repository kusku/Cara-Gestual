#include "../../stdafx.h"
#include "EditorManager.h"
#include "../Muscles/MuscleManager.h"
#include "../Expressions/ExpressionManager.h"
#include "../../Models/ModelManager.h"
#include "../../Models/Actor/Actor.h"
#include "intersection.h"

EditorManager::EditorManager()
{
	CurrentVertex = 0;
	CurrentMuscle = NONE_MUSCLE;
	objecte = ModelManager::GetInstance()->GetActor();
	int nVertex = objecte->GetNumVertexs();
	VertexList = new bool[nVertex];
	DeltaList = new float[nVertex];
	for ( int i = 0; i < nVertex; ++i ) {
		VertexList[i] = false;
		DeltaList[i] = 0.0;
	}
}

EditorManager::~EditorManager()
{
	delete [] VertexList;
	delete [] DeltaList;
	objecte = NULL;
}

//Afegeix un vèrtex al muscle definit
void EditorManager::AddVertex(D3DXVECTOR3 vertex)
{
	int v = this->objecte->buscarPunt(vertex);
	if(!this->VertexList[v]) {
		this->VertexList[v] = true;
		++CurrentVertex;
		if (CurrentVertex == 1)
			this->DominantVertex = v;
	}
}

D3DXVECTOR3* EditorManager::GetPointList(int* tamany, int* dominant)
{
	*tamany = this->CurrentVertex;
	int i = -1, nVertex;
	nVertex = *tamany;
	D3DXVECTOR3 *llista = new D3DXVECTOR3[*tamany];
	while (nVertex) {
		if(this->VertexList[++i]) {
			if (i == this->DominantVertex) {
				*dominant = *tamany - nVertex;
			}
			llista[*tamany - nVertex] = objecte->GetPoint(i) + objecte->GetMovement(i);
			--nVertex;
		}
	}
	return llista;
}

//Calcula i assigna les deltes del muscle concret
void EditorManager::CalculateDelta()
{
	int numVertex;
	numVertex = objecte->GetNumVertexs();
	for (int i = 0; i < numVertex; i++) {
		if (VertexList[i]) {
			DeltaList[i] = (float) 1/(1 + (float)Distance(objecte->GetPoint(i),objecte->GetPoint(DominantVertex)));
		}
	}
}

//Elimina un vèrtex del muscle definit
void EditorManager::DeleteVertex(D3DXVECTOR3 vertex)
{
	int v = objecte->buscarPunt(vertex);
	if(VertexList[v]) {
		VertexList[v] = false;
		--CurrentVertex;
		if (v == DominantVertex)
			for (DominantVertex = 0; !VertexList[DominantVertex]; ++DominantVertex);
	}
}

//Defineix el moviment del muscle per a una expressió
void EditorManager::DefineMovement(TypeExpression expression, TypeMuscle muscle, D3DXVECTOR3 desplaçament)
{
	ExpressionManager::GetInstance()->modifyMuscleExpression(expression,muscle,desplaçament);
}

void EditorManager::AddVertexFromTriangle(D3DXVECTOR3 colisio, D3DXVECTOR3* triangle)
{
	D3DXVECTOR3 puntFinal;
	puntFinal = PuntMesProximTriangle(colisio,triangle);
	AddVertex(puntFinal);
}

void EditorManager::DeleteVertexFromTriangle(D3DXVECTOR3 colisio, D3DXVECTOR3* triangle)
{
	D3DXVECTOR3 puntFinal;
	puntFinal = PuntMesProximTriangle(colisio,triangle);
	DeleteVertex(puntFinal);
}

void EditorManager::SetMuscle(TypeMuscle muscle)
{
	int i = 0,index, maxVertex;
	unsigned int *llistatVertex = NULL;
	float *llistatDelta = NULL;
	Muscle *m;

	maxVertex = objecte->GetNumVertexs();
	if ( CurrentMuscle != NONE_MUSCLE) {
		this->CalculateDelta();
		while (CurrentVertex) {
			if (VertexList[i]) {
				// Afegir el vertex al muscle
				MuscleManager::GetInstance()->addVertexMuscle(CurrentMuscle, (unsigned int) i, DeltaList[i]);
				// Eliminar el vertex ja guardat
				VertexList[i] = false;
				--CurrentVertex;
			}
			++i;
		}
	}
	// Canviar el muscle que s'està editant
	this->CurrentMuscle = muscle;
	// Carregar les dades del nou muscle
	m = MuscleManager::GetInstance()->getMuscleList()[muscle];
	llistatVertex = m->getVertexIndex();
	llistatDelta = m->getVertexDelta();
	CurrentVertex = m->getNumVertexs();
	float maxDelta = 0.0;
	for (i = 0; i < CurrentVertex; i++) {
		index = llistatVertex[i];
		this->VertexList[index] = true;
		this->DeltaList[index] = llistatDelta[i];
		if (this->DeltaList[index] > maxDelta) {
			this->DominantVertex = index;
			maxDelta = this->DeltaList[index];
		}
	}
}

void EditorManager::SaveMuscle()
{
	int i = 0, maxVertex;
	int CurrentTemp = CurrentVertex;

	maxVertex = objecte->GetNumVertexs();
	if ( CurrentMuscle != NONE_MUSCLE) {
		this->CalculateDelta();
		while (CurrentTemp) {
			if (VertexList[i]) {
				// Afegir el vertex al muscle
				MuscleManager::GetInstance()->addVertexMuscle(CurrentMuscle, (unsigned int) i, DeltaList[i]);
				--CurrentTemp;
			}
			++i;
		}
	}
}

D3DXVECTOR3 EditorManager::PuntMesProximTriangle(D3DXVECTOR3 colisio, D3DXVECTOR3* triangle)
{
	D3DXVECTOR3 puntFinal = triangle[0];
	float distanciaMinima, distanciaActual;
	distanciaMinima = (float)Distance(colisio, puntFinal);
	for (int i = 1; i < 3; i++) {
		distanciaActual = (float)Distance(colisio, triangle[i]);
		if (distanciaActual < distanciaMinima) {
			puntFinal = triangle[i];
			distanciaMinima = distanciaActual;
		}
	}
	return puntFinal;
}
void EditorManager::SetDominantVertex( D3DXVECTOR3 colisio, D3DXVECTOR3* triangle )
{
	D3DXVECTOR3 puntFinal;

	puntFinal = this->PuntMesProximTriangle(colisio,triangle);
	this->AddVertex(puntFinal);
	DominantVertex = objecte->buscarPunt(puntFinal);
}
D3DXVECTOR3 EditorManager::GetDominantVertex( void )
{
	return objecte->GetPoint(DominantVertex);
}

void EditorManager::ClearVertexs ( void )
{
	CurrentVertex = 0;
	CurrentMuscle = NONE_MUSCLE;

	int nVertex = objecte->GetNumVertexs();
	for ( int i = 0; i < nVertex; ++i ) {
		VertexList[i] = false;
		DeltaList[i] = 0.0;
	}
}
