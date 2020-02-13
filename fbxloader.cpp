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
	m_fbx_manager = FbxManager::Create();
	m_fbx_io_settings = FbxIOSettings::Create(m_fbx_manager, IOSROOT);
	m_scene = FbxScene::Create(m_fbx_manager, "");
	FbxImporter* lImporter = FbxImporter::Create(m_fbx_manager, "");

	FbxMemoryStream memory_stream(m_fbx_manager, data, data_size);

	void* streamData = NULL;
	// initialize the importer with a stream
	if (!lImporter->Initialize(&memory_stream, NULL, -1, m_fbx_io_settings))
		printf("error1");
	//return -1;
// import the scene.
	if (!lImporter->Import(m_scene))
		printf("error1");
	//return -1;
// destroy the importer.
	lImporter->Destroy();

	//return 0;
}

FbxLoader::~FbxLoader()
{
	if (m_scene != nullptr)
		m_scene->Destroy();

	m_fbx_manager->Destroy();
	m_fbx_io_settings->Destroy();
}

FbxScene* FbxLoader::scene() const
{
	return m_scene;
}

uint8_t* FbxLoader::triangulate(size_t& data_size)
{
	fbxsdk::FbxGeometryConverter converter(m_fbx_manager);
	converter.Triangulate(m_scene, true);

	/// export test
	//// create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(m_fbx_manager, "MemoryExporter");

	//// initialize the exporter by providing a filename and the IOSettings to use
	FbxMemoryStream outputStream(m_fbx_manager);
	bool succes = lExporter->Initialize(&outputStream, nullptr, -1, m_fbx_io_settings);

	// export the scene.
	succes = lExporter->Export(m_scene);
	// destroy the exporter
	lExporter->Destroy();
	data_size = outputStream.data_size();
	return outputStream.data();
}