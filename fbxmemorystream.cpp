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

#include "fbxmemorystream.h"

FbxMemoryStream::FbxMemoryStream(fbxsdk::FbxManager* pSdkManager)
: m_stream_type(WRITE)
{
	m_writer_ID = pSdkManager->GetIOPluginRegistry()->FindWriterIDByExtension("FBX");
}

FbxMemoryStream::FbxMemoryStream(fbxsdk::FbxManager* pSdkManager, uint8_t* data, size_t data_size)
: m_data(data)
, m_data_size(data_size)
, m_stream_type(READ)
{
	m_reader_ID = pSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX (*.fbx)");
}

FbxStream::EState FbxMemoryStream::GetState()
{
	if (m_stream_type == READ)
	{
		if (m_data == nullptr)
			return FbxStream::eClosed;

		if (m_data_size == 0)
			return FbxStream::eEmpty;
	}
	return eOpen;
}

bool FbxMemoryStream::Open(void *pStreamData)
{
	m_stream_position = 0;
	return true;
}

bool FbxMemoryStream::Close()
{
	m_stream_position = 0;
	if (m_stream_type == WRITE)
	{
		m_data = new uint8_t[m_write_data.size()];
		m_data_size = m_write_data.size();
		memcpy(m_data, &m_write_data[0], m_data_size);
		m_write_data.clear();
	}

	return true;
}

bool FbxMemoryStream::Flush()
{
	return true;
}

int32_t FbxMemoryStream::Write(const void *pData, int32_t pSize)
{
	if (m_stream_type == WRITE)
	{
		const uint8_t* writebuffer = static_cast<const uint8_t*>(pData);

		if (m_stream_position == m_write_data.size())
		{
			for (int32_t i = 0; i < pSize; ++i)
			{
				m_write_data.push_back(writebuffer[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < pSize; ++i)
			{
				m_write_data[m_stream_position + i] = writebuffer[i];
			}
		}
		m_stream_position += pSize;
		return pSize;
	}

	return 0;
}

int32_t FbxMemoryStream::Read(void *pData, int32_t pSize) const
{
	if (m_stream_position + pSize > (m_data_size - 1))
	{
		int32_t readsize = m_data_size - m_stream_position - 1;
		if (readsize > 0)
		{
			memcpy(pData, m_data + m_stream_position, readsize);
			m_stream_position += readsize;
		}
		return readsize;
	}

	memcpy(pData, m_data + m_stream_position, pSize);
	m_stream_position += pSize;
	return pSize;
}

int32_t	FbxMemoryStream::GetReaderID() const
{
	return m_reader_ID;
}

int32_t FbxMemoryStream::GetWriterID() const
{
	return m_writer_ID;
}

void FbxMemoryStream::Seek(const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos)
{
	switch (pSeekPos)
	{
	case FbxFile::eBegin:
		m_stream_position = pOffset;
		break;

	case FbxFile::eCurrent:
		m_stream_position += pOffset;
		break;

	case FbxFile::eEnd:
		break;
	}

	if (m_stream_position > m_data_size - 1)
		m_stream_position = m_data_size - 1;
}

long FbxMemoryStream::GetPosition() const
{
	return m_stream_position;
}

void FbxMemoryStream::SetPosition(long pPosition)
{
	m_stream_position = pPosition;
}

int32_t FbxMemoryStream::GetError() const
{
	return 0;
}

void FbxMemoryStream::ClearError()
{

}

uint8_t* FbxMemoryStream::data()
{
	return m_data;
}

size_t FbxMemoryStream::data_size() const
{
	return m_data_size;
}
