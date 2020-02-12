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
#include <fbxsdk.h>

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

	EState 	GetState() override;
	bool 	Open(void *pStreamData) override;
	bool 	Close() override;
	bool 	Flush() override;
	int32_t Write(const void *pData, int32_t pSize) override;
	int32_t Read(void *pData, int32_t pSize) const override;
	//char* 	ReadString(char *pBuffer, int32_t pMaxSize, bool pStopAtFirstWhiteSpace = false) override;
	int32_t	GetReaderID() const override;
	int32_t GetWriterID() const override;
	void 	Seek(const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos) override;
	long 	GetPosition() const override;
	void 	SetPosition(long pPosition) override;
	int32_t GetError() const override;
	void 	ClearError() override;

private:
	mutable long m_stream_position = 0;
	uint8_t* m_data = nullptr;
	size_t m_data_size = 0;
	int32_t m_reader_ID = -1;
	int32_t m_writer_ID = -1;

	QVector<uint8_t> m_write_data;
	EMemoryStreamType m_stream_type = READ;
};

/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

//
//
// The example illustrates how to:
//        1) create a file custom stream class
//        2) write a text file using the stream
//        3) read the generated file via the stream
//
/*
#include <fbxsdk.h>

// Custom stream 
class CustomStreamClass : public FbxStream
{
public:
        CustomStreamClass( FbxManager* pSdkManager, const char* mode )
        {
                mFile = NULL;

                // expect the mode to contain something
                if (mode == NULL) return;


                FBXSDK_strcpy(mFileName, 30, "CustomStreamClass_temp.txt");
                FBXSDK_strcpy(mMode, 3, (mode) ? mode : "r");

                if ( mode[0] == 'r' )
                {
                        const char* format = "FBX (*.fbx)";
                        mReaderID = pSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( format );
                        mWriterID = -1;
                }
                else
                {
                        const char* format = "FBX ascii (*.fbx)";
                        mWriterID = pSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription( format );
                        mReaderID = -1;
                }
        }

        ~CustomStreamClass()
        {
                Close();
        }

        virtual EState GetState()
        {
                return mFile ? FbxStream::eOpen : eClosed;
        }

        virtual bool Open( void* /*pStreamData
/*        {
                // This method can be called several times during the
                // Initialize phase so it is important that it can handle 
                // multiple opens
                if (mFile == NULL)
                        FBXSDK_fopen(mFile, mFileName, mMode);

                if (mFile != NULL)
            fseek( mFile, 0L, SEEK_SET );

                return ( mFile != NULL );
        }

        virtual bool Close()
        {
                // This method can be called several times during the
                // Initialize phase so it is important that it can handle multiple closes
                if ( mFile )
                        fclose( mFile );
                mFile = NULL;
                return true;
        }

        virtual bool Flush()
        {
                return true;
        }

        virtual int Write(const void* pData, int pSize)
        {
                if ( mFile == NULL )
                        return 0;
                return (int)fwrite( pData, 1, pSize, mFile );
        }

        virtual int Read (void* pData, int pSize) const
        {
                if ( mFile == NULL )
                        return 0;
                return (int)fread( pData, 1, pSize, mFile );
        }

        virtual int GetReaderID() const
        {
                return mReaderID;
        }

        virtual int GetWriterID() const
        {
                return mWriterID;
        }

    void Seek( const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos )
    {
        switch ( pSeekPos )
        {
            case FbxFile::eBegin:
                fseek( mFile, (long)pOffset, SEEK_SET );
                break;
            case FbxFile::eCurrent:
                fseek( mFile, (long)pOffset, SEEK_CUR );
                break;
            case FbxFile::eEnd:
                fseek( mFile, (long)pOffset, SEEK_END );
                break;
        }
    }

        virtual long GetPosition() const
        {
                if ( mFile == NULL )
                        return 0;
                return ftell( mFile );
        }
        virtual void SetPosition( long pPosition )
        {
                if ( mFile )
                        fseek( mFile, pPosition, SEEK_SET );
        }

        virtual int GetError() const
        {
                if ( mFile == NULL )
                        return 0;
                return ferror( mFile );
        }
        virtual void ClearError()
        {
                if ( mFile != NULL )
                        clearerr( mFile );
        }

private:
        FILE*   mFile;
        int             mReaderID;
        int             mWriterID;
        char    mFileName[30];
        char    mMode[3];
};



int testExport()
{
        // create a SdkManager
        FbxManager* lSdkManager = FbxManager::Create();
        // create an IOSettings object
        FbxIOSettings* ios = FbxIOSettings::Create( lSdkManager, IOSROOT );
        // create an empty scene.
        FbxScene* lScene = FbxScene::Create( lSdkManager, "" );
        // create an exporter.
        FbxExporter* lExporter = FbxExporter::Create( lSdkManager, "" );
        // create a stream.
        CustomStreamClass stream( lSdkManager, "wb" );
        // can pass in a void* data pointer to be passed to the stream on FileCreate
        void* streamData = NULL;
        // initialize the exporter.
        if ( !lExporter->Initialize( &stream, streamData, -1, ios ) )
                return -1;
        // export the scene.
        if ( !lExporter->Export( lScene ) )
                return -1;
        // destroy the exporter.
        lExporter->Destroy();

        return 0;
}

int testImport()
{
        // create a SdkManager
        FbxManager* lSdkManager = FbxManager::Create();
        // create an IOSettings object
        FbxIOSettings* ios = FbxIOSettings::Create( lSdkManager, IOSROOT );
        // create an empty scene
        FbxScene* lScene = FbxScene::Create( lSdkManager, "" );
        // create an importer.
        FbxImporter* lImporter = FbxImporter::Create( lSdkManager, "" );
        // create a stream. (CustomStreamClass – stream initialization in constructor)
        CustomStreamClass stream( lSdkManager, "rb" );
        // can pass in a void* data pointer to be passed to the stream on FileOpen
        void* streamData = NULL;
        // initialize the importer with a stream
        if ( !lImporter->Initialize( &stream, streamData, -1, ios ) )
                return -1;
        // import the scene.
        if ( !lImporter->Import( lScene ) )
                return -1;
        // destroy the importer.
        lImporter->Destroy();

        return 0;
}

int main(int /*argc, char** /*argv
{
        int ret;

        ret = testExport();
        if ( ret ) return ret;
        
        ret = testImport();
        if ( ret ) return ret;

      */