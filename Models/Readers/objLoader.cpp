// OBJModel.cpp: Implements the class COBJModel.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//           Wavefront OBJ Loader (C) 2000 Tim C. Schr�der
// -------------------------------------------------------------------
//    tcs_web@gmx.de / tcs_web@hotmail.com / tcs@thereisnofate.net
//                 http://glvelocity.demonews.com
//////////////////////////////////////////////////////////////////////
#include "../../stdafx.h"
#include "objLoader.h"

// OBJ File string indentifiers
#define VERTEX_ID		 "v"
#define TEXCOORD_ID		 "vt"
#define NORMAL_ID		 "vn"
#define FACE_ID			 "f"
#define COMMENT_ID		 "#"
#define MTL_LIB_ID		 "mtllib"
#define USE_MTL_ID		 "usemtl"

// MTL File string indentifiers
#define NEW_MTL_ID		 "newmtl"
#define MTL_TEXTURE_ID   "map_Kd"
#define MTL_AMBIENT_ID	 "Ka"
#define MTL_DIFFUSE_ID	 "Kd"
#define MTL_SPECULAR_ID	 "Ks"
#define MTL_SHININESS_ID "Ns"

// Maximum size of a string that could be read out of the OBJ file
#define MAX_STR_SIZE 1024

// Maximum number of vertices a that a single face can have
#define MAX_VERTICES 256

// Axis constants
const short unsigned int x = 0;
const short unsigned int y = 1;
const short unsigned int z = 2;

//////////////////////////////////////////////////////////////////////
// Construcktion / Destrucktion
//////////////////////////////////////////////////////////////////////

_stdcall COBJModel::COBJModel()
{
	Model.info = NULL;
	Model.pFaces = NULL;
	Model.pMaterials = NULL;
	Model.pNormals = NULL;
	Model.pTexCoords = NULL;
	Model.pVertices = NULL;
}

_stdcall COBJModel::~COBJModel()
{
	glDeleteLists(m_iDisplayList, 1);
	FreeMemory();
}

bool _stdcall COBJModel::LoadModel(const char szFileName[],unsigned int iDisplayList)
{
	////////////////////////////////////////////////////////////////////////
	// Load a OBJ file and render its data into a display list
	////////////////////////////////////////////////////////////////////////

	//OBJFileInfo OBJInfo;		  // General informations about the model
	OBJFileInfo CurrentIndex;	  // Current array index
	char szString[MAX_STR_SIZE];  // Buffer string for reading the file 
	char szBasePath[_MAX_PATH];	  // Path were all paths in the OBJ start
	int nScanReturn = 0;		  // Return value of fscanf
	float fCoord = 0;			  // Buffer float for reading the file
	int i;						  // Loop variable
	unsigned int iCurMaterial = 0;// Current material

	Model.info = new OBJFileInfo;

	// Get base path
	strcpy_s(szBasePath,_MAX_PATH, szFileName);
	MakePath(szBasePath);

	////////////////////////////////////////////////////////////////////////
	// Open the OBJ file
	////////////////////////////////////////////////////////////////////////
	FILE *hFile;
	errno_t err = fopen_s(&hFile, szFileName, "r");
	
	// Success ?
	if (err != 0)
		return FALSE;

	////////////////////////////////////////////////////////////////////////
	// Allocate space for structures that hold the model data
	////////////////////////////////////////////////////////////////////////
 
	// Which data types are stored in the file ? How many of each type ?
	GetFileInfo(hFile, Model.info, szBasePath);

	// Vertices and faces
	//Vector3D	*pVertices	= new Vector3D[OBJInfo.iVertexCount];
	Model.pVertices = new Vector3D[Model.info->iVertexCount];
	Model.pFaces	= new Face[Model.info->iFaceCount];
	
	// Allocate space for optional model data only if present.
	Model.pNormals	= NULL;
	Model.pTexCoords = NULL;
	Model.pMaterials = NULL;
	if (Model.info->iNormalCount)
		Model.pNormals = new Vector3D[Model.info->iNormalCount];
	if (Model.info->iTexCoordCount)
		Model.pTexCoords = new Vector2D[Model.info->iTexCoordCount];
	if (Model.info->iMaterialCount)
		Model.pMaterials = new Material[Model.info->iMaterialCount];

	// Init structure that holds the current array index
	memset(&CurrentIndex, 0, sizeof(OBJFileInfo));

	////////////////////////////////////////////////////////////////////////
	// Read the file contents
	////////////////////////////////////////////////////////////////////////

	// Start reading the file from the start
	rewind(hFile);
	
	// Quit reading when end of file has been reached
	while (!feof(hFile))
	{
		// Get next string
		ReadNextString(szString, hFile);

		// Next three elements are floats of a vertex
		if (!strncmp(szString, VERTEX_ID, sizeof(VERTEX_ID)))
		{
			// Read three floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f %f",
				&Model.pVertices[CurrentIndex.iVertexCount].fX,
				&Model.pVertices[CurrentIndex.iVertexCount].fY,
				&Model.pVertices[CurrentIndex.iVertexCount].fZ);
			// Next vertex
			CurrentIndex.iVertexCount++;
		}

		// Next two elements are floats of a texture coordinate
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
		{
			// Read two floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f",
				&Model.pTexCoords[CurrentIndex.iTexCoordCount].fX,
				&Model.pTexCoords[CurrentIndex.iTexCoordCount].fY);
			// Next texture coordinate
			CurrentIndex.iTexCoordCount++;
		}

		// Next three elements are floats of a vertex normal
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
		{
			// Read three floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f %f",
				&Model.pNormals[CurrentIndex.iNormalCount].fX,
				&Model.pNormals[CurrentIndex.iNormalCount].fY,
				&Model.pNormals[CurrentIndex.iNormalCount].fZ);
			// Next normal
			CurrentIndex.iNormalCount++;
		}

		// Rest of the line contains face information
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
		{
			// Read the rest of the line (the complete face)
			GetTokenParameter(szString, sizeof(szString) ,hFile);
			// Convert string into a face structure
			ParseFaceString(szString, &Model.pFaces[CurrentIndex.iFaceCount],
				Model.pVertices, Model.pNormals, Model.pTexCoords, iCurMaterial);
			// Next face
			CurrentIndex.iFaceCount++;
		}

		// Process material information only if needed
		if (Model.pMaterials)
		{
			// Rest of the line contains the name of a material
			if (!strncmp(szString, USE_MTL_ID, sizeof(USE_MTL_ID)))
			{
				// Read the rest of the line (the complete material name)
				GetTokenParameter(szString, sizeof(szString), hFile);
				// Are any materials loaded ?
				if (Model.pMaterials)
					// Find material array index for the material name
					for (i=0; i<(int) Model.info->iMaterialCount; i++)
						if (!strncmp(Model.pMaterials[i].szName, szString, sizeof(szString)))
						{
							iCurMaterial = i;
							break;
						}
			}

			// Rest of the line contains the filename of a material library
			if (!strncmp(szString, MTL_LIB_ID, sizeof(MTL_LIB_ID)))
			{
				// Read the rest of the line (the complete filename)
				GetTokenParameter(szString, sizeof(szString), hFile);
				// Append material library filename to the model's base path
				char szLibraryFile[_MAX_PATH];
				strcpy_s(szLibraryFile,_MAX_PATH, szBasePath);
				strcat_s(szLibraryFile,_MAX_PATH, szString);
				// Load the material library
				LoadMaterialLib(szLibraryFile, Model.pMaterials, 
					&CurrentIndex.iMaterialCount, szBasePath);
			}
		}
	}	

	// Close OBJ file
	fclose(hFile);

	////////////////////////////////////////////////////////////////////////
	// Arrange and render the model data
	////////////////////////////////////////////////////////////////////////

	// Sort the faces by material to minimize state changes
	if (Model.pMaterials)
		qsort(Model.pFaces, Model.info->iFaceCount, sizeof(Face), CompareFaceByMaterial);
	
	// Identificar el n�mero de llista donat com a par�metre a la variable local m_iDisplayList
	m_iDisplayList = iDisplayList;
	if (m_iDisplayList>0) glDeleteLists(m_iDisplayList, 1);

	// Render all faces into a display list
	//RenderToDisplayList(Model.pFaces, Model.info->iFaceCount, Model.pMaterials);

	////////////////////////////////////////////////////////////////////////
	// Success
	////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void _stdcall COBJModel::ParseFaceString(char szFaceString[], Face *FaceOut, 
								const Vector3D *pVertices, 
								const Vector3D *pNormals,
								const Vector2D *pTexCoords,
								const unsigned int iMaterialIndex)
{
	////////////////////////////////////////////////////////////////////////
	// Convert face string from the OBJ file into a face structure
	////////////////////////////////////////////////////////////////////////
	
	int i;
	int iVertex = 0, iTextureCoord = 0, iNormal = 0;

	// Pointer to the face string. Will be incremented later to
	// advance to the next triplet in the string.
	char *pFaceString = szFaceString;

	// Save the string positions of all triplets
	int iTripletPos[MAX_VERTICES];
	int iCurTriplet = 0;

	// Init the face structure
	memset(FaceOut, 0, sizeof(Face));

	////////////////////////////////////////////////////////////////////////
	// Get number of vertices in the face
	////////////////////////////////////////////////////////////////////////

	// Loop trough the whole string
	for (i=0; i<(int) strlen(szFaceString); i++)
	{
		// Each triplet is separated by spaces
		if (szFaceString[i] == ' ')
		{
			// One more vertex
			FaceOut->iNumVertices++;
			// Save position of triplet
			iTripletPos[iCurTriplet] = i;
			// Next triplet
			iCurTriplet++;
		}
	}

	// Face has more vertices than spaces that separate them
	FaceOut->iNumVertices++;

	////////////////////////////////////////////////////////////////////////
	// Allocate space for structures that hold the face data
	////////////////////////////////////////////////////////////////////////

	// Vertices
	FaceOut->pVertices		= new Vector3D[FaceOut->iNumVertices];

	// Allocate space for normals and texture coordinates only if present
	if (pNormals)
		FaceOut->pNormals	= new Vector3D[FaceOut->iNumVertices];
	if (pTexCoords)
		FaceOut->pTexCoords = new Vector2D[FaceOut->iNumVertices];
	
	////////////////////////////////////////////////////////////////////////
	// Copy vertex, normal, material and texture data into the structure
	////////////////////////////////////////////////////////////////////////

	// Set material
	FaceOut->iMaterialIndex = iMaterialIndex;

	// Process per-vertex data
	for (i=0; i<(int) FaceOut->iNumVertices; i++)
	{
		// Read one triplet from the face string

		// Are vertices, normals and texture coordinates present ?
		if (pNormals && pTexCoords)
			// Yes
			sscanf_s(pFaceString, "%i/%i/%i", 
				&iVertex, &iTextureCoord, &iNormal);
		else if (pNormals && !pTexCoords)
			// Vertices and normals but no texture coordinates
			sscanf_s(pFaceString, "%i//%i", &iVertex, &iNormal);
		else if (pTexCoords && !pNormals)
			// Vertices and texture coordinates but no normals
			sscanf_s(pFaceString, "%i/%i", &iVertex, &iTextureCoord);
		else
			// Only vertices
			sscanf_s(pFaceString, "%i", &iVertex);

		// Copy vertex into the face. Also check for normals and texture 
		// coordinates and copy them if present.
		memcpy(&FaceOut->pVertices[i], &pVertices[iVertex - 1], 
			sizeof(Vector3D));
		if (pTexCoords)
			memcpy(&FaceOut->pTexCoords[i], 
			&pTexCoords[iTextureCoord - 1], sizeof(Vector2D));
		if (pNormals)
			memcpy(&FaceOut->pNormals[i], 
			&pNormals[iNormal - 1], sizeof(Vector3D));

		// Set string pointer to the next triplet
		pFaceString = &szFaceString[iTripletPos[i]];
	}
}

bool _stdcall COBJModel::LoadMaterialLib(const char szFileName[], 
								Material *pMaterials, 
								unsigned int *iCurMaterialIndex,
								char szBasePath[])
{
	////////////////////////////////////////////////////////////////////////
	// Loads a material library file (.mtl)
	////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE];	// Buffer used while reading the file
	bool bFirstMaterial = TRUE;		// Only increase index after first 
									// material has been set

	////////////////////////////////////////////////////////////////////////
	// Open library file
	////////////////////////////////////////////////////////////////////////
	FILE *hFile;
	errno_t err = fopen_s(&hFile, szFileName, "r");

	// Success ?
	if (err != 0)
		return FALSE;

	////////////////////////////////////////////////////////////////////////
	// Read all material definitions
	////////////////////////////////////////////////////////////////////////

	// Quit reading when end of file has been reached
	while (!feof(hFile))
	{
		// Get next string
		ReadNextString(szString, hFile);

		// Is it a "new material" identifier ?
		if (!strncmp(szString, NEW_MTL_ID, sizeof(NEW_MTL_ID)))
		{
			// Only increase index after first material has been set
			if (bFirstMaterial == TRUE)
				// First material has been set
				bFirstMaterial = FALSE;
			else
				// Use next index
				(*iCurMaterialIndex)++;
			// Read material name
			GetTokenParameter(szString, sizeof(szString), hFile);
			// Store material name in the structure
			strcpy_s(pMaterials[*iCurMaterialIndex].szName,1024, szString);
		}

		// Ambient material properties
		if (!strncmp(szString, MTL_AMBIENT_ID, sizeof(MTL_AMBIENT_ID)))
		{
			// Read into current material
			fscanf_s(hFile, "%f %f %f",
				&pMaterials[*iCurMaterialIndex].fAmbient[0],
				&pMaterials[*iCurMaterialIndex].fAmbient[1],
				&pMaterials[*iCurMaterialIndex].fAmbient[2]);
		}

		// Diffuse material properties
		if (!strncmp(szString, MTL_DIFFUSE_ID, sizeof(MTL_DIFFUSE_ID)))
		{
			// Read into current material
			fscanf_s(hFile, "%f %f %f",
				&pMaterials[*iCurMaterialIndex].fDiffuse[0],
				&pMaterials[*iCurMaterialIndex].fDiffuse[1],
				&pMaterials[*iCurMaterialIndex].fDiffuse[2]);
		}

		// Specular material properties
		if (!strncmp(szString, MTL_SPECULAR_ID, sizeof(MTL_SPECULAR_ID)))
		{
			// Read into current material
			fscanf_s(hFile, "%f %f %f",
				&pMaterials[*iCurMaterialIndex].fSpecular[0],
				&pMaterials[*iCurMaterialIndex].fSpecular[1],
				&pMaterials[*iCurMaterialIndex].fSpecular[2]);
		}

		// Texture map name
		if (!strncmp(szString, MTL_TEXTURE_ID, sizeof(MTL_TEXTURE_ID)))
		{
			// Read texture filename
			GetTokenParameter(szString, sizeof(szString), hFile);
			// Append material library filename to the model's base path
			char szTextureFile[_MAX_PATH];
			strcpy_s(szTextureFile,_MAX_PATH, szBasePath);
			strcat_s(szTextureFile,_MAX_PATH, szString);
			// Store texture filename in the structure
			strcpy_s(pMaterials[*iCurMaterialIndex].szTexture,_MAX_PATH, szTextureFile);
			// Load texture and store its ID in the structure
			pMaterials[*iCurMaterialIndex].iTextureID = LoadTexture(szTextureFile);
		}

		// Shininess
		if (!strncmp(szString, MTL_SHININESS_ID, sizeof(MTL_SHININESS_ID)))
		{
			// Read into current material
			fscanf_s(hFile, "%f",
				&pMaterials[*iCurMaterialIndex].fShininess);
			// OBJ files use a shininess from 0 to 1000; Scale for OpenGL
			pMaterials[*iCurMaterialIndex].fShininess /= 1000.0f;
			pMaterials[*iCurMaterialIndex].fShininess *= 128.0f;
		}
	}

	fclose(hFile);

	// Increment index cause LoadMaterialLib() assumes that the passed
	// index is always empty
	(*iCurMaterialIndex)++;

	return TRUE;
}

void _stdcall COBJModel::RenderToDisplayList(const Face *pFaces, 
									const unsigned int iFaceCount,
									const Material *pMaterials)
{
	////////////////////////////////////////////////////////////////////////
	// Render a list of faces into a display list
	////////////////////////////////////////////////////////////////////////

	int i, j;
	float fNormal[3];
	int iPreviousMaterial = -1;
	
	// Generate & save display list index
	

	// Render model into the display list
	glNewList(m_iDisplayList, GL_COMPILE);

		// Save texture bit to recover from the various texture state changes
		glPushAttrib(GL_TEXTURE_BIT);

			// Activate automatic texture coord generation if no coords loaded
			if (!pFaces[0].pTexCoords)
				GenTexCoords();
		
			// Use default material if no materials are loaded
			if (!pMaterials)
				UseMaterial(NULL);

			// Process all faces
			for (i=0; i<(int) iFaceCount; i++)
			{
				// Any materials loaded ?
				
				if (pMaterials)
					
					// Set material (if it differs from the previous one)
					if (iPreviousMaterial != (int) pFaces[i].iMaterialIndex)
					{
						iPreviousMaterial = pFaces[i].iMaterialIndex;
						UseMaterial(&pMaterials[pFaces[i].iMaterialIndex]);
					}

				// Set primitive of the current face
				switch (pFaces[i].iNumVertices)
				{
					case 3:
						glBegin(GL_TRIANGLES);
						break;
					case 4:
						glBegin(GL_QUADS);
						break;
					default:
						glBegin(GL_POLYGON);
				}
							
				// Calculate and set face normal if no vertex normals are specified
				if (!pFaces[i].pNormals)
				{
					GetFaceNormal(fNormal, &pFaces[i]);
					glNormal3fv(fNormal);
				}

				// Process all vertices
				for (j=0; j<(int) pFaces[i].iNumVertices; j++)
				{
					// Set vertex normal (if vertex normals are specified)
					if (pFaces[i].pNormals)
						glNormal3f(pFaces[i].pNormals[j].fX, 
							pFaces[i].pNormals[j].fY, pFaces[i].pNormals[j].fZ);

					// Set texture coordinates (if any specified)
					if (pFaces[i].pTexCoords)
						glTexCoord2f(pFaces[i].pTexCoords[j].fX, 
							pFaces[i].pTexCoords[j].fY);

					// Set vertex
					glVertex3f(pFaces[i].pVertices[j].fX, 
						pFaces[i].pVertices[j].fY, pFaces[i].pVertices[j].fZ);
				}

				glEnd();	
			}

		glPopAttrib();

	glEndList();
}

void _stdcall COBJModel::UseMaterial(const Material *pMaterial)
{
	////////////////////////////////////////////////////////////////////////
	// Make a given material the current one
	////////////////////////////////////////////////////////////////////////

	glColor3f(1.0,1.0,1.0);
	// Look for the presence of a texture and activate texturing if succeed
	if (pMaterial!=NULL)
	{
		if (pMaterial->iTextureID)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, pMaterial->iTextureID);
		}
		else
			glDisable(GL_TEXTURE_2D);
		
	}
	else
		glDisable(GL_TEXTURE_2D);
}

void _stdcall COBJModel::GenTexCoords()
{
	////////////////////////////////////////////////////////////////////////
	// Set up the automatic texture coord generation
	////////////////////////////////////////////////////////////////////////

	float fS[4] = { -0.1f, 0.0f, 0.0f, 0.0f };
	float fT[4] = { 0.0f, 0.0f, -0.1f, 0.0f };

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexGenfv(GL_S, GL_OBJECT_PLANE, fS);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, fT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}

void _stdcall COBJModel::GetFaceNormal(float fNormalOut[], const Face *pFace)
{
	////////////////////////////////////////////////////////////////////////
	// Calculate normal for a given face
	////////////////////////////////////////////////////////////////////////

	// Only faces with at least 3 vertices can have a normal
	if (pFace->iNumVertices < 3)
	{
		// Clear the normal and exit
		memset(fNormalOut, 0, sizeof(fNormalOut));
		return;
	}

	// Two vectors
	float v1[3], v2[3];

	// Calculate two vectors from the three points
	v1[x] = pFace->pVertices[0].fX - pFace->pVertices[1].fX;
	v1[y] = pFace->pVertices[0].fY - pFace->pVertices[1].fY;
	v1[z] = pFace->pVertices[0].fZ - pFace->pVertices[1].fZ;

	v2[x] = pFace->pVertices[1].fX - pFace->pVertices[2].fX;
	v2[y] = pFace->pVertices[1].fY - pFace->pVertices[2].fY;
	v2[z] = pFace->pVertices[1].fZ - pFace->pVertices[2].fZ;

	// Take the cross product of the two vectors to get the normal vector
	fNormalOut[x] = v1[y] * v2[z] - v1[z] * v2[y];
	fNormalOut[y] = v1[z] * v2[x] - v1[x] * v2[z];
	fNormalOut[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Calculate the length of the vector		
	float length = (float) sqrt(( fNormalOut[x] * fNormalOut[x]) + 
						        ( fNormalOut[y] * fNormalOut[y]) +
						        ( fNormalOut[z] * fNormalOut[z]) );

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	fNormalOut[x] /= length;
	fNormalOut[y] /= length;
	fNormalOut[z] /= length;
}

void _stdcall COBJModel::GetFileInfo(FILE *hStream, OBJFileInfo *Info, 
							const char szConstBasePath[])
{
	////////////////////////////////////////////////////////////////////////
	// Read the count of all important identifiers out of the given stream
	////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE]; // Buffer for reading the file
	char szBasePath[_MAX_PATH];	 // Needed to append a filename to the base path
	
	// Init structure
	memset(Info, 0, sizeof(OBJFileInfo));

	// Rollback the stream
	rewind(hStream);

	// Quit reading when end of file has been reached
	while (!feof(hStream))
	{
		// Get next string
		ReadNextString(szString, hStream);

		// Vertex ?
		if (!strncmp(szString, VERTEX_ID, sizeof(VERTEX_ID)))
			Info->iVertexCount++;	
		// Texture coordinate ?
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
			Info->iTexCoordCount++;
		// Vertex normal ?
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
			Info->iNormalCount++;		
		// Face ?
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
			Info->iFaceCount++;

		// Material library definition ?
		if (!strncmp(szString, MTL_LIB_ID, sizeof(MTL_LIB_ID)))
		{
			// Read the filename of the library
			GetTokenParameter(szString, sizeof(szString), hStream);
			// Copy the model's base path into a none-constant string
			strcpy_s(szBasePath,_MAX_PATH, szConstBasePath);
			// Append material library filename to the model's base path
			strcat_s(szBasePath,_MAX_PATH, szString);
			// Open the library file
			FILE *hMaterialLib;
			errno_t err = fopen_s(&hMaterialLib, szBasePath, "r");
			// Success ?
			if (err == 0)
			{
				// Quit reading when end of file has been reached
				while (!feof(hMaterialLib))
				{
					// Read next string
					fscanf_s(hMaterialLib, "%s" ,szString, _countof(szString));
					// Is it a "new material" identifier ?
					if (!strncmp(szString, NEW_MTL_ID, sizeof(NEW_MTL_ID)))
						// One more material defined
						Info->iMaterialCount++;		
				}
				// Close material library
				fclose(hMaterialLib);
			}
		}
			
		// Clear string two avoid counting something twice
		memset(szString, '\0', sizeof(szString));
	}
}

void _stdcall COBJModel::ReadNextString(char szString[], FILE *hStream)
{
	////////////////////////////////////////////////////////////////////////
	// Read the next string that isn't a comment
	////////////////////////////////////////////////////////////////////////

	bool bSkipLine = FALSE;	// Skip the current line ?
	int nScanReturn = 0;	// Return value of fscanf

	// Skip all strings that contain comments
	do
	{
		// Read new string
		nScanReturn = fscanf(hStream, "%s", szString);
		// Is rest of the line a comment ?
		if (!strncmp(szString, COMMENT_ID, sizeof(COMMENT_ID)))
		{
			// Skip the rest of the line
			fgets(szString, sizeof(szString), hStream);
			bSkipLine = TRUE;
		}
		else bSkipLine = FALSE;
	} while (bSkipLine == TRUE);
}

void _stdcall COBJModel::MakePath(char szFileAndPath[])
{
	////////////////////////////////////////////////////////////////////////
	// Rips the filenames out of a path and adds a slash (if needed)
	////////////////////////////////////////////////////////////////////////
	
	// Get string length
	int iNumChars = (int)strlen(szFileAndPath);

	// Loop from the last to the first char
	for (int iCurChar=iNumChars-1; iCurChar>=0; iCurChar--)
	{
		// If the current char is a slash / backslash
		if (szFileAndPath[iCurChar] == char('\\') ||
			szFileAndPath[iCurChar] == char('/'))
		{
			// Terminate the the string behind the slash / backslash
			szFileAndPath[iCurChar + 1] = char('\0');
			return;
		}
	}

	// No slash there, set string length to zero
	szFileAndPath[0] = char('\0');
}

void _stdcall COBJModel::GetTokenParameter(char szString[], 
								  const unsigned int iStrSize, FILE *hFile)
{
	////////////////////////////////////////////////////////////////////////
	// Read the parameter of a token, remove space and newline character
	////////////////////////////////////////////////////////////////////////

	// Read the parameter after the token
	fgets(szString, iStrSize, hFile);

	// Remove space before the token			
	strcpy_s(szString,iStrSize, &szString[1]);

	// Remove newline character after the token
	szString[strlen(szString) - 1] = char('\0');
}

static int CompareFaceByMaterial(const void *Arg1, const void *Arg2)
{
	////////////////////////////////////////////////////////////////////////
	// Callback function for comparing two faces with qsort
	////////////////////////////////////////////////////////////////////////

	// Cast the void pointers to faces
	Face *Face1 = (Face*) Arg1;
	Face *Face2 = (Face*) Arg2;

	// Greater
	if (Face1->iMaterialIndex > Face2->iMaterialIndex)
		return 1;

	// Less
	if (Face1->iMaterialIndex < Face2->iMaterialIndex)
		return -1;

	// Equal
	return 0;
}

int COBJModel::LoadTexture(const char szFileName[_MAX_PATH])
{
	////////////////////////////////////////////////////////////////////////
	// Load a texture and return its ID
	////////////////////////////////////////////////////////////////////////

	FILE *file=NULL;
	int errno;
	unsigned int iTexture = 0;

	
	// Open the image file for reading
	errno=fopen_s(&file,szFileName,"r");			// Funci� Visual 2005
	
	// If the file is empty (or non existent) print an error and return false
	// if (file == NULL)
	if (errno!=0) 
		return false ;

	// Close the image file
	 fclose(file);

	// ilLoadImage: Funci� que llegeix la imatge del fitxer filename
	//				si �s compatible amb els formats DevIL/OpenIL (BMP,JPG,GIF,TIF,TGA,etc.)
	//				i defineix la imatge com a textura retornant l'identificador 
	//				de textura.
	// GetBuffer: Funci� de converi� d'una variable CString -> char *
	iTexture = ilLoadImage((char *) szFileName);

	// If execution arrives here it means that all went well. Return true
	return iTexture;
}

  
//Create Object Instance-Access Function
OBJLOADER_CLASS_DECL COBJModel* _stdcall InitObject()
{
  return new COBJModel; //Alloc TMyObject instance
}

//Release Object Instance-Access Function
OBJLOADER_CLASS_DECL void _stdcall UnInitObject(COBJModel * obj) 
{
  delete obj; //Release TMyObject instance
}

void COBJModel::FreeMemory ( void )
{
	if ( COBJModel::ExistModelInMemory() )
	{
		// Alliberem la mem�ria de l'estructura
		delete [] Model.pVertices;	
		delete [] Model.pNormals;
		delete [] Model.pTexCoords;
		delete [] Model.pMaterials;

		Model.pVertices = NULL;
		Model.pNormals = NULL;
		Model.pTexCoords = NULL;
		Model.pMaterials = NULL;

		for (unsigned int i = 0; i < Model.info->iFaceCount; i++ )
		{
			delete [] Model.pFaces[i].pNormals;
			delete [] Model.pFaces[i].pTexCoords;
			delete [] Model.pFaces[i].pVertices;

			Model.pFaces[i].pNormals = NULL;
			Model.pFaces[i].pTexCoords = NULL;
			Model.pFaces[i].pVertices = NULL;
		}
		delete [] Model.pFaces;

		delete Model.info;
		Model.info = NULL;
	}
}

ModelOBJ  COBJModel::GetModel ( void )
{
	return Model;
}
unsigned int COBJModel::GetNumVertices ( void )
{
	return Model.info->iVertexCount;
}
unsigned int COBJModel::GetNumFaces ( void )
{
	return Model.info->iFaceCount;
}
unsigned int COBJModel::GetNumTexCoords	( void )
{
	return Model.info->iTexCoordCount;
}
unsigned int COBJModel::GetNumNormals ( void )
{
	return Model.info->iNormalCount;
}
unsigned int COBJModel::GetNumMaterials ( void )
{
	return Model.info->iMaterialCount;
}
bool COBJModel::ExistModelInMemory( void )
{
	if ( Model.info != NULL && Model.pVertices != NULL && Model.pFaces != NULL )
		return true;
	else
		return false;
}