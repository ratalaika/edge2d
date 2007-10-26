/**
*
*
*
*/
#include "EdgeArchive.h"

namespace Edge
{
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Archive
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	Archive::Archive( const string&name, const string&type ) :
		mName( name ), mType( type )
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FileSystemArchive
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	FileSystemArchive::FileSystemArchive( const string &name, const string &type ) :
		Archive( name, type )
	{
	}
	
	FileSystemArchive::~FileSystemArchive()
	{

	}

	bool	FileSystemArchive::load()
	{
		/// check whether the last character is '\' or '/'
		/// if there isnot, i will append this character
		char last = mName.at( mName.size() - 1 );
		if( last != '/' && last != '\\' )
		{
			mPath = mName + '/';
		}
		else
		{
			mPath = mName;
		}

		return true;
	}

	void	FileSystemArchive::unload()
	{
	}

	DataStreamPtr	FileSystemArchive::open( const string &file )
	{
		DataStream *ds = new FileSystemDataStream( mPath + file );

		return DataStreamPtr( ds );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	FileSystemArchiveFactory
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	Archive *FileSystemArchiveFactory::createArchive( const string &name, void *pInData )
	{
		return new FileSystemArchive( name, getType() );
	}
	
	void FileSystemArchiveFactory::destroyArchive( Archive *archive )
	{
		delete archive;
	}
}