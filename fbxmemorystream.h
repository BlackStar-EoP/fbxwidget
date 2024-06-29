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

#include <QVector>

class FbxMemoryStream : public FbxStream
{
public:
	enum EMemoryStreamType
	{
		READ,
		WRITE
	};

public:
	FbxMemoryStream(fbxsdk::FbxManager* pSdkManager);
	FbxMemoryStream(fbxsdk::FbxManager* pSdkManager, uint8_t* data, size_t data_size);

	EState 	 GetState() override;
	bool 	 Open(void *pStreamData) override;
	bool 	 Close() override;
	bool 	 Flush() override;
	size_t   Write(const void* pData, FbxUInt64 pSize) override;
	size_t   Read(void* pData, FbxUInt64 pSize) const override;
	int32_t	 GetReaderID() const override;
	int32_t  GetWriterID() const override;
	void 	 Seek(const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos) override;
	FbxInt64 GetPosition() const override;
	void     SetPosition(FbxInt64 pPosition) override;
	int32_t  GetError() const override;
	void 	 ClearError() override;

	uint8_t* data();
	size_t data_size() const;
private:
	mutable FbxInt64 m_stream_position = 0;
	uint8_t* m_data = nullptr;
	size_t m_data_size = 0;
	int32_t m_reader_ID = -1;
	int32_t m_writer_ID = -1;

	QVector<uint8_t> m_write_data;
	EMemoryStreamType m_stream_type = READ;
};