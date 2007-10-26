/**
 *
 *
 */
#include "EdgeZipArchive.h"
#include "EdgeException.h"

namespace Edge
{
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZipArchive
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	ZipArchive::ZipArchive( const string &name, const string &type, const string &psd ) :
		Archive( name, type ), mPassword( psd )
	{
		mZipHandle = 0;
	}

	ZipArchive::~ZipArchive()
	{
	}

	bool ZipArchive::load()
	{
		/// open the zip file
		mZipHandle = OpenZip( mName.c_str(), mPassword.c_str() );
		if( mZipHandle == 0 )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_OPEN_FILE, "Load zip archive failed",
				"ZipArchive::load" );
		}

		return true;
	}

	void ZipArchive::unload()
	{
		CloseZip( mZipHandle );
	}

	DataStreamPtr ZipArchive::open( const string &file )
	{
		string cfile( file );
		/// deal with the file name
		for( size_t i = 0; i < cfile.size(); ++ i )
		{
			if( cfile.at( i ) == '\\' )
			{
				cfile.at( i ) = '/';
			}
		}
		
		/// get a file in the zip archive
		ZIPENTRY ze;
		int index;
		FindZipItem( mZipHandle, cfile.c_str(), true, &index, &ze );
		if( index == -1 )
		{
			EDGE_EXCEPT( Exception::EC_FILE_NOT_FOUND, "Cannot find the file : " + file + 
				" in ZipArchive : " + mName, "ZipArchive::open" );
		}
		
		/// allocate memory to get the file
		MemoryDataStream *stream = new MemoryDataStream( cfile, ze.unc_size );
		unsigned char *pBuf = stream->getPointer();
		UnzipItem( mZipHandle, index, pBuf, ze.unc_size );
		pBuf[ze.unc_size] = 0;

		return DataStreamPtr( stream );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZipArchiveFactory
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	Archive *ZipArchiveFactory::createArchive( const string &name, void *pInData )
	{
		const char *psd = (const char*) pInData;
		string password( psd == 0 ? "":psd );
		return new ZipArchive( name, getType(), password );
	}
	
	void ZipArchiveFactory::destroyArchive( Archive *archive )
	{
		delete archive;
	}

}