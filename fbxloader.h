#pragma once

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

#include <stdint.h>

#pragma warning (push, 0)
#include <fbxsdk.h>
#pragma pop

#include <string>

class FbxMemoryStream;

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	FbxScene* import_from_memory(uint8_t* data, size_t data_size);

	bool triangulate(FbxScene* scene);
	uint8_t* export_to_memory(FbxScene* scene, size_t& data_size);
	bool export_to_file(FbxScene* scene, const std::string& filename);

private:
	FbxManager* m_fbx_manager = nullptr;
	FbxIOSettings* m_fbx_io_settings = nullptr;
	FbxImporter* m_importer = nullptr;
	FbxExporter* m_exporter = nullptr;
};