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

#include "fbxtreewidget.h"
#include "fbxloader.h"

void FBXTreeWidget::parse_nodes(QTreeWidgetItem* root_widget_item, fbxsdk::FbxNode* root_node)
{
	int32_t childcount = root_node->GetChildCount();
	for (int32_t i = 0; i < childcount; ++i)
	{
		fbxsdk::FbxNode* child = root_node->GetChild(i);
		QTreeWidgetItem* child_widget_item = new QTreeWidgetItem();
		child_widget_item->setText(0, child->GetName());
		root_widget_item->addChild(child_widget_item);
		if (child->GetChildCount() > 0)
			parse_nodes(child_widget_item, child);
	}

}

void FBXTreeWidget::give_fbx_data(const QString& filename, uint8_t* data, size_t size)
{
	clear();

	delete[] m_fbx_data;
	m_fbx_data = data;
	delete m_fbx_loader;
	m_fbx_loader = new FbxLoader(data, size);

	FbxScene* scene = m_fbx_loader->scene();
	fbxsdk::FbxNode* root_node = scene->GetRootNode();
	QTreeWidgetItem* root_widget_item = new QTreeWidgetItem(this);
	root_widget_item->setText(0, root_node->GetName());

	parse_nodes(root_widget_item, root_node);
}

