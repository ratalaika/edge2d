/**
 *
 *
 *
 */
#include "EdgeLzoArchive.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"

namespace Edge
{
	/**
	 * callback functions to get messages from lzo
	 *
	 */
	void lzo_callback( const char *msg )
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, msg );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	LzoArchive
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	LzoArchive::LzoArchive( const string &name, const string &type ) :
		Archive( name, type )
	{
		mLzoArchive = 0;
	}

	LzoArchive::~LzoArchive()
	{
	}

	bool LzoArchive::load()
	{
		mLzoArchive = lzo_load_archive( mName.c_str() );
		if( mLzoArchive == 0 )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_OPEN_FILE, "lzo_load_archive failed!",
				"LzoArchive::load" );
		}
		return true;
	}

	void LzoArchive::unload()
	{
		lzo_free_archive( mLzoArchive );
	}

	DataStreamPtr LzoArchive::open( const string &file )
	{
		lzo_file_info *info = lzo_get_file_info( mLzoArchive, file.c_str() );
		if( info == 0 )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_OPEN_FILE, "Cannot open file " + file + " in the archive " + 
				mName, "LzoArchive::open" );
		}

		/// read data 
		MemoryDataStream *stream = new MemoryDataStream( info->name, info->decom_size );
		unsigned char *pBuf = stream->getPointer();
		lzo_read_file( mLzoArchive, info, pBuf );
		pBuf[info->decom_size] = 0;

		return DataStreamPtr( stream );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	LzoArchiveFactory
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	LzoArchiveFactory::LzoArchiveFactory() :
		ArchiveFactory()
	{
		lzoex_init( 0 );
	}

	Archive *LzoArchiveFactory::createArchive( const string &name, void *pInData )
	{
		return new LzoArchive( name, getType() );
	}
	
	void LzoArchiveFactory::destroyArchive( Archive *archive )
	{
		delete archive;
	}

}