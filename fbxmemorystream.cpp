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

FbxMemoryStream::FbxMemoryStream(FbxManager* pSdkManager, uint8_t* data, size_t data_size)
: m_data(data)
, m_data_size(data_size)
{
	const char* format = "FBX (*.fbx)";
	m_reader_ID = pSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription(format);
}

FbxStream::EState FbxMemoryStream::GetState()
{
	if (m_data == nullptr)
		return FbxStream::eClosed;

	if (m_data_size == 0)
		return FbxStream::eEmpty;

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
	return true;
}

bool FbxMemoryStream::Flush()
{
	return true;
}

int32_t FbxMemoryStream::Write(const void *pData, int32_t pSize)
{
	return 0;
}

int32_t FbxMemoryStream::Read(void *pData, int32_t pSize) const
{
	if (m_stream_position + pSize > (m_data_size - 1))
	{
		int32_t readsize = m_data_size - m_stream_position - 1;
		if (readsize > 0)
			memcpy(pData, m_data + m_stream_position, readsize);
		return readsize;
	}

	memcpy(pData, m_data + m_stream_position, pSize);
	return pSize;
}

//char* FbxMemoryStream::ReadString(char *pBuffer, int32_t pMaxSize, bool pStopAtFirstWhiteSpace = false)
//{
//
//}

int32_t	FbxMemoryStream::GetReaderID() const
{
	return m_reader_ID;
}

int32_t FbxMemoryStream::GetWriterID() const
{
	return -1;
}

void FbxMemoryStream::Seek(const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos)
{
	switch (pSeekPos)
	{
	case FbxFile::eBegin:
		m_stream_position = 0;
		//fseek(mFile, (long)pOffset, SEEK_SET);
		break;
	case FbxFile::eCurrent:
		//fseek(mFile, (long)pOffset, SEEK_CUR);
		m_stream_position += pOffset;
		break;
	case FbxFile::eEnd:
		m_stream_position = m_data_size - 1;
		//fseek(mFile, (long)pOffset, SEEK_END);
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
