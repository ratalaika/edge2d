/**
*
*
*
*/
#include "EdgeDataStream.h"
#include "EdgeException.h"

namespace Edge
{
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DataStream
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	DataStream::DataStream( const string &name ) : mName( name )
	{
		mSize = 0;
	}

	DataStream::~DataStream()
	{
	}

	size_t DataStream::readLine( void *buf, size_t count )
	{
		size_t i ;
		char *cbuf = (char*)buf;
		for( i = 0; i < count - 1; ++ i )
		{
			read( cbuf, 1 );
			if( cbuf[i] == '\n' || cbuf[i] == '\r' )
			{
				++ i;
				break;
			}
		}

		cbuf[i] = '\0';

		return i - 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FileSystemDataStream
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	FileSystemDataStream::FileSystemDataStream( const string &file ) :
	DataStream( file )
	{
		open( file );

		/// calculate the size
		mStream.seekg( 0, std::ios_base::end );
		mSize = mStream.tellg();
		mStream.seekg( 0, std::ios_base::beg );
	}

	FileSystemDataStream::~FileSystemDataStream()
	{
		close();
	}

	size_t FileSystemDataStream::read( void *buf, size_t count )
	{
		mStream.read( static_cast<char*>( buf ), (std::streamsize)count );
        return mStream.gcount();
	}

	void FileSystemDataStream::seek( size_t pos )
	{
		mStream.clear();
		mStream.seekg( (long)pos, std::ios_base::beg );
	}

	bool FileSystemDataStream::eof()
	{
		return mStream.eof();
	}

	size_t FileSystemDataStream::getPos()
	{
		return mStream.tellg(); 
	}

	bool FileSystemDataStream::open( const string &file )
	{
		mStream.open( file.c_str(), std::ios::in | std::ios::binary );
	
		if( mStream.fail() )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_OPEN_FILE, "File open error : " + file,
				"FileSystemDataStream::Open" );
		}

		return true;
	}

	void FileSystemDataStream::close()
	{
		mStream.close();
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MemoryDataStream
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	MemoryDataStream::MemoryDataStream( const string &name, size_t size ) :
		DataStream( name )
	{
		mPos = 0;
		mSize = size;
		mData = new unsigned char [mSize+1];
	}

	MemoryDataStream::~MemoryDataStream()
	{
		delete [] mData;
	}

	size_t MemoryDataStream::read( void *buf, size_t count )
	{
		size_t readCount = ( mSize - mPos < count ? mSize - mPos : count );
		memcpy( buf, mData + mPos, readCount );
		mPos += readCount;
		return readCount;
	}

	void MemoryDataStream::seek( size_t pos )
	{
		mPos = pos;
	}
	
	bool MemoryDataStream::eof()
	{
		return mPos >= mSize;
	}

	size_t MemoryDataStream::getPos()
	{
		return mPos;
	}
}

