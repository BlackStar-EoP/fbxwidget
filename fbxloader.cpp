/*
MIT License

Copyright (c) 2019 BlackStar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "fbxloader.h"
#include "fbxmemorystream.h"

#pragma comment(lib, "libfbxsdk.lib")

FbxLoader::FbxLoader(uint8_t* data, size_t data_size)
{
	// create a SdkManager
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	m_scene = FbxScene::Create(lSdkManager, "");
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	FbxMemoryStream memory_stream(lSdkManager, data, data_size);

	void* streamData = NULL;
	// initialize the importer with a stream
	if (!lImporter->Initialize(&memory_stream, NULL, -1, ios))
		printf("error1");
	//return -1;
// import the scene.
	if (!lImporter->Import(m_scene))
		printf("error1");
	//return -1;
// destroy the importer.
	lImporter->Destroy();

	//return 0;

	//fbxsdk::FbxGeometryConverter converter(lSdkManager);
	//converter.Triangulate(m_scene, true);

	///// export test
	////// create an exporter.
	//FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

	//// initialize the exporter by providing a filename and the IOSettings to use
	//lExporter->Initialize("teapot_triangulated.fbx", -1, ios);

	//// export the scene.
	//lExporter->Export(m_scene);

	//// destroy the exporter
	//lExporter->Destroy();


}

FbxLoader::~FbxLoader()
{
	if (m_scene != nullptr)
		m_scene->Destroy();
}

FbxScene* FbxLoader::scene() const
{
	return m_scene;
}

	/* 
	from engine:
	TriangleMeshData* data = new TriangleMeshData(gi, MeshParameters::STATIC_DATA);
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	lSdkManager = FbxManager::Create();
	if (!lSdkManager)
	{
	FBXSDK_printf("Error: Unable to create FBX Manager!\n");
	exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", lSdkManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	lSdkManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	lScene = FbxScene::Create(lSdkManager, "My Scene");
	if (!lScene)
	{
	FBXSDK_printf("Error: Unable to create FBX scene!\n");
	exit(1);
	}
	// Load the scene.

	// The example can take a FBX file as an argument.
	FbxString lFilePath(filename.c_str());

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(lFilePath, -1, lSdkManager->GetIOSettings());
	//lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
	FbxString error = lImporter->GetStatus().GetErrorString();
	FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
	FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

	if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
	{
	//FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
	//FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
	}

	return false;
	}
	// Import the scene.
	bool lStatus = lImporter->Import(lScene);
	
	
	*/

/*

TriangleMeshData* FbxLoader::load_fbx(OpenGLGI& gi, const std::string& filename)
{
	TriangleMeshData* data = new TriangleMeshData(gi, MeshParameters::STATIC_DATA);
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	lSdkManager = FbxManager::Create();
	if (!lSdkManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", lSdkManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	lSdkManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	lScene = FbxScene::Create(lSdkManager, "My Scene");
	if (!lScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
	// Load the scene.

	// The example can take a FBX file as an argument.
	FbxString lFilePath(filename.c_str());

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(lFilePath, -1, lSdkManager->GetIOSettings());
	//lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			//FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			//FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}
	// Import the scene.
	bool lStatus = lImporter->Import(lScene);
	if (lStatus)
	{
		int objects = lScene->GetRootNode()->GetChildCount();
		for (uint32_t i = 0; i < 1; ++i)
		{
			FbxNode* node = lScene->GetRootNode()->GetChild(0);
			FbxClassId classid = node->GetClassId();
			if (node->GetNodeAttribute())
			{
				FbxNodeAttribute::EType at = node->GetNodeAttribute()->GetAttributeType();

				if (at == FbxNodeAttribute::eMesh)
				{
					FbxMesh* mesh = node->GetMesh();
					int polies = mesh->GetPolygonCount();
					//mesh->GetPolygonGroup()
					int polyvertexcnt = mesh->GetPolygonVertexCount();
					int* vertices = mesh->GetPolygonVertices();

					// Vertex data
					int numcontrolpoints = mesh->GetControlPointsCount();
					FbxVector4* controlpoints = mesh->GetControlPoints();

					// Normals
					FbxArray<FbxVector4> normals;
					mesh->GetPolygonVertexNormals(normals);

					// Texture coordinates
					//get all UV set names
					FbxStringList lUVSetNameList;
					mesh->GetUVSetNames(lUVSetNameList);
					int numUVmaps = mesh->GetUVLayerCount();
					FbxStringListItem* uvmap = lUVSetNameList.GetItemAt(0);
					FbxArray<FbxVector2> uvcoords;
					mesh->GetPolygonVertexUVs(uvmap->mString, uvcoords);

					//					assert(numcontrolpoints == normals.Size() == uvcoords.Size());


					std::map<FbxUniqueMeshVertex, int32_t> unique_vertices;

					int nextmeshvertexnr = 0;
					int currentindex = 0;
					for (int32_t poly = 0; poly < polies; ++poly)
					{
						int32_t polygonsize = mesh->GetPolygonSize(poly);
						if (polygonsize == 4)
						{
							// 0 1 3 1 2 3
							int v0 = mesh->GetPolygonVertex(poly, 0);
							int v1 = mesh->GetPolygonVertex(poly, 1);
							int v2 = mesh->GetPolygonVertex(poly, 2);
							int v3 = mesh->GetPolygonVertex(poly, 3);

							data->index_array().add(v0);
							data->index_array().add(v1);
							data->index_array().add(v3);

							data->index_array().add(v1);
							data->index_array().add(v2);
							data->index_array().add(v3);
						}

						else if (polygonsize == 3)
						{
							// 0 1 2
							int vi0 = mesh->GetPolygonVertex(poly, 0);
							int vi1 = mesh->GetPolygonVertex(poly, 1);
							int vi2 = mesh->GetPolygonVertex(poly, 2);

							int index0 = currentindex++;
							int index1 = currentindex++;
							int index2 = currentindex++;

							FbxVector4& v0 = controlpoints[vi0];
							FbxVector4& v1 = controlpoints[vi1];
							FbxVector4& v2 = controlpoints[vi2];

							FbxVector4& n0 = normals[index0];
							FbxVector4& n1 = normals[index1];
							FbxVector4& n2 = normals[index2];

							FbxVector2& uv0 = uvcoords[index0];
							FbxVector2& uv1 = uvcoords[index1];
							FbxVector2& uv2 = uvcoords[index2];

							MeshVertex mv0 = create_mesh_vertex(v0, n0, uv0);
							MeshVertex mv1 = create_mesh_vertex(v1, n1, uv1);
							MeshVertex mv2 = create_mesh_vertex(v2, n2, uv2);

							data->vertex_array().add(mv0);
							data->vertex_array().add(mv1);
							data->vertex_array().add(mv2);

							data->index_array().add(index0);
							data->index_array().add(index1);
							data->index_array().add(index2);
						}
					}
					printf("");
				}
			}
		}
	}
	return data;
}
*/