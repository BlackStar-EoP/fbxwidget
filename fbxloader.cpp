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

FbxLoader::FbxLoader()
{
	// create a SdkManager
	m_fbx_manager = FbxManager::Create();
	m_fbx_io_settings = FbxIOSettings::Create(m_fbx_manager, IOSROOT);
	m_importer = FbxImporter::Create(m_fbx_manager, "MemoryImporter");
	m_exporter = FbxExporter::Create(m_fbx_manager, "MemoryExporter");
}

FbxLoader::~FbxLoader()
{
	m_fbx_manager->Destroy();
}

FbxScene* FbxLoader::import_from_memory(uint8_t* data, size_t data_size)
{
	FbxScene* scene = FbxScene::Create(m_fbx_manager, "MemoryImporter");
	

	FbxMemoryStream memory_stream(m_fbx_manager, data, data_size);

	if (!m_importer->Initialize(&memory_stream, nullptr, -1, m_fbx_io_settings))
	{
		scene->Destroy();
		return nullptr;
	}

	if (!m_importer->Import(scene))
	{
		scene->Destroy();
		return nullptr;
	}

	return scene;
}

bool FbxLoader::triangulate(FbxScene* scene)
{
	fbxsdk::FbxGeometryConverter converter(m_fbx_manager);
	return converter.Triangulate(scene, true);
}

uint8_t* FbxLoader::export_to_memory(FbxScene* scene, size_t& data_size)
{
	FbxMemoryStream outputStream(m_fbx_manager);
	bool succes = m_exporter->Initialize(&outputStream, nullptr, -1, m_fbx_io_settings);

	succes = m_exporter->Export(scene);
	
	data_size = outputStream.data_size();
	return outputStream.data();
}

bool FbxLoader::export_to_file(FbxScene* scene, const std::string& filename)
{
	FbxExporter* lExporter = FbxExporter::Create(m_fbx_manager, "FileExporter");
	lExporter->Initialize(filename.c_str(), -1, m_fbx_io_settings);
	bool succes = lExporter->Export(scene);
	lExporter->Destroy();
	return succes;
}