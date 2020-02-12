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

#include <QTreeWidget>

#include <fbxsdk.h>

#include <stdint.h>

class QByteArray;
class FbxLoader;

class FBXTreeWidget : public QTreeWidget
{
public:
	FBXTreeWidget(QWidget* parent);

	void parse_nodes(QTreeWidgetItem* root_widget_item, fbxsdk::FbxNode* root_node);
	void give_fbx_data(const QString& filename, uint8_t* data, size_t size);
	void parse_fbx_data(const QByteArray& data);

	uint32_t num_meshes() const;
	uint32_t num_triangles() const;
	uint32_t num_quads() const;
	uint32_t num_ngons() const;

	bool needs_triangulation() const;
	QByteArray triangulated_fbx();

private:
	uint8_t* m_fbx_data = nullptr;
	FbxLoader* m_fbx_loader = nullptr;

	uint32_t m_num_meshes = 0;
	uint32_t m_num_triangles = 0;
	uint32_t m_num_quads = 0;
	uint32_t m_num_ngons = 0;
};