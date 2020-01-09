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

#include "mainwindow.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QFile>

#include "fbxtreewidget.h"

MainWindow::MainWindow()
{
	setAcceptDrops(true);

	m_fbx_widget = new FBXTreeWidget();
	setCentralWidget(m_fbx_widget);
	m_fbx_widget->show();
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls())
		e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->urls().size() != 1)
		return;

	QStringList accepted_types;
	accepted_types << "fbx";

	QString fname = e->mimeData()->urls()[0].toLocalFile();
	QFileInfo info(fname);
	if (info.exists())
	{
		if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive))
		{
			QFile f(fname);
			if (f.open(QIODevice::ReadOnly))
			{
				qint64 size = f.size();
				if (size > 0)
				{
					uint8_t* fbx_data = new uint8_t[size];
					memcpy(fbx_data, f.readAll().data(), size);
					f.close();
					m_fbx_widget->give_fbx_data(fname, fbx_data, size);
				}

			}
		}
	}
}