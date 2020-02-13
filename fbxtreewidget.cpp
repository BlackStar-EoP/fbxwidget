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

FBXTreeWidget::FBXTreeWidget(QWidget* parent)
: QTreeWidget(parent)
{
	setColumnCount(2);

	QTreeWidgetItem *HeaderItem = headerItem();

	HeaderItem->setText(0, "Node name");
	HeaderItem->setText(1, "Node type");
}


FBXTreeWidget::~FBXTreeWidget()
{
}

void FBXTreeWidget::parse_nodes(QTreeWidgetItem* root_widget_item, fbxsdk::FbxNode* root_node)
{
	int32_t childcount = root_node->GetChildCount();
	for (int32_t i = 0; i < childcount; ++i)
	{
		fbxsdk::FbxNode* child = root_node->GetChild(i);

		int32_t node_attribute_count = child->GetNodeAttributeCount();

		QTreeWidgetItem* child_widget_item = new QTreeWidgetItem();
		child_widget_item->setText(0, child->GetName());

		if (node_attribute_count == 0)
			printf("");

		int32_t material_count = child->GetMaterialCount();
		for (int32_t mat = 0; mat < material_count; ++mat)
		{
			fbxsdk::FbxSurfaceMaterial* material = child->GetMaterial(mat);
			QString material_name = material->GetName();
			QTreeWidgetItem* material_widget_item = new QTreeWidgetItem();
			material_widget_item->setText(0, material_name);
			child_widget_item->addChild(material_widget_item);
			
		}

		QString attribute_type_name = "";
		for (int32_t attr = 0; attr < node_attribute_count; ++attr)
		{
			const fbxsdk::FbxNodeAttribute* node_attribute = child->GetNodeAttributeByIndex(attr);
			fbxsdk::FbxNodeAttribute::EType attr_type = node_attribute->GetAttributeType();
			switch (attr_type)
			{
			case fbxsdk::FbxNodeAttribute::eUnknown: attribute_type_name = "eUnknown"; break;
			case fbxsdk::FbxNodeAttribute::eNull: attribute_type_name = "eNull"; break;
			case fbxsdk::FbxNodeAttribute::eMarker: attribute_type_name = "eMarker"; break;
			case fbxsdk::FbxNodeAttribute::eSkeleton: attribute_type_name = "eSkeleton"; break;
			case fbxsdk::FbxNodeAttribute::eMesh: 
			{
				m_num_meshes++;
				const fbxsdk::FbxMesh* mesh = static_cast<const fbxsdk::FbxMesh*>(node_attribute);
				attribute_type_name = "eMesh ";
				int32_t polygoncount = mesh->GetPolygonCount();
				attribute_type_name += "Polygons: " + QString::number(polygoncount);
				for (int32_t p = 0; p < polygoncount; ++p)
				{
					int32_t polygonsize = mesh->GetPolygonSize(p);
					if (polygonsize < 3)
						throw new std::runtime_error("Polygon size in mesh is less than 3!");
					else if (polygonsize == 3)
						m_num_triangles++;
					else if (polygonsize == 4)
						m_num_quads++;
					else
						m_num_ngons++;
				}
				attribute_type_name += " Tris: " + QString::number(m_num_triangles) + QString(" Quads: ") + QString::number(m_num_quads) +
					QString(" Ngons : ") + QString::number(m_num_ngons);
				break;
			}

			case fbxsdk::FbxNodeAttribute::eNurbs: attribute_type_name = "eNurbs"; break;
			case fbxsdk::FbxNodeAttribute::ePatch: attribute_type_name = "ePatch"; break;
			case fbxsdk::FbxNodeAttribute::eCamera: attribute_type_name = "eCamera"; break;
			case fbxsdk::FbxNodeAttribute::eCameraStereo: attribute_type_name = "eCameraStereo"; break;
			case fbxsdk::FbxNodeAttribute::eCameraSwitcher: attribute_type_name = "eCameraSwitcher"; break;
			case fbxsdk::FbxNodeAttribute::eLight: attribute_type_name = "eLight"; break;
			case fbxsdk::FbxNodeAttribute::eOpticalReference: attribute_type_name = "eOpticalReference"; break;
			case fbxsdk::FbxNodeAttribute::eOpticalMarker: attribute_type_name = "eOpticalMarker"; break;
			case fbxsdk::FbxNodeAttribute::eNurbsCurve: attribute_type_name = "eNurbsCurve"; break;
			case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface: attribute_type_name = "eTrimNurbsSurface"; break;
			case fbxsdk::FbxNodeAttribute::eBoundary: attribute_type_name = "eBoundary"; break;
			case fbxsdk::FbxNodeAttribute::eNurbsSurface: attribute_type_name = "eNurbsSurface"; break;
			case fbxsdk::FbxNodeAttribute::eShape: attribute_type_name = "eShape"; break;
			case fbxsdk::FbxNodeAttribute::eLODGroup: attribute_type_name = "eLODGroup"; break;
			case fbxsdk::FbxNodeAttribute::eSubDiv: attribute_type_name = "eSubDiv"; break;
			case fbxsdk::FbxNodeAttribute::eCachedEffect: attribute_type_name = "eCachedEffect"; break;
			case fbxsdk::FbxNodeAttribute::eLine: attribute_type_name = "eLine"; break;
			}
		}

		child_widget_item->setText(1, attribute_type_name);

		root_widget_item->addChild(child_widget_item);
		if (child->GetChildCount() > 0)
			parse_nodes(child_widget_item, child);
	}

}

void FBXTreeWidget::parse_fbx_scene(FbxScene* scene)
{
	m_num_meshes = 0;
	m_num_triangles = 0;
	m_num_quads = 0;
	m_num_ngons = 0;

	clear();

	fbxsdk::FbxNode* root_node = scene->GetRootNode();
	QTreeWidgetItem* root_widget_item = new QTreeWidgetItem(this);
	root_widget_item->setText(0, root_node->GetName());

	parse_nodes(root_widget_item, root_node);
}

uint32_t FBXTreeWidget::num_meshes() const
{
	return m_num_meshes;
}

uint32_t FBXTreeWidget::num_triangles() const
{
	return m_num_triangles;
}

uint32_t FBXTreeWidget::num_quads() const
{
	return m_num_quads;
}
uint32_t FBXTreeWidget::num_ngons() const
{
	return m_num_ngons;
}

bool FBXTreeWidget::needs_triangulation() const
{
	return m_num_quads > 0 ||
		   m_num_ngons > 0;
}

FbxLoader& FBXTreeWidget::fbx_loader()
{
	return m_fbx_loader;
}