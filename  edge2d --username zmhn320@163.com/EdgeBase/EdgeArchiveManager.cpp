/**
 *
 *
 */
#include "EdgeArchiveManager.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"

namespace Edge
{
	ArchiveManager::ArchiveManager()
	{
	}

	ArchiveManager::~ArchiveManager()
	{
		destroyAllArchives();
	}

	void	ArchiveManager::addArchiveFactory( ArchiveFactory *archiveFactory )
	{
		mArchiveFactories[ archiveFactory->getType() ] = archiveFactory;	

		LogManager::getSingletonPtr()->logMessage( LL_INFORMATION,
			"Added ArchiveFactory : " + archiveFactory->getType() );
	}

	void	ArchiveManager::removeArchiveFactory( ArchiveFactory *archiveFactory )
	{
		ArchiveFactoryMap::iterator it = mArchiveFactories.find(
			archiveFactory->getType() );
		
		if( it != mArchiveFactories.end() )
		{
			mArchiveFactories.erase( it );
		}

		LogManager::getSingletonPtr()->logMessage( LL_INFORMATION,
			"Removed ArchiveFactory : " + archiveFactory->getType() );
	}

	Archive *ArchiveManager::createArchive( const string &name, const string &type, void *pInData )
	{
		string cname( name );
		modifyName( cname );

		Archive *archive = getArchive( cname );
		if( archive != 0 )
		{
			return archive;
		}

		ArchiveFactoryMap::iterator it = mArchiveFactories.find( type );
		
		if( it == mArchiveFactories.end() )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the ArchiveFactory, Type : " + type,
				"FileMananger::createArchive" );
		}

		archive = it->second->createArchive( cname, pInData );

		archive->load();

		/// add the archive into the list
		mArchives[ cname ] = archive;
	
		LogManager::getSingletonPtr()->logMessage( LL_INFORMATION,
			"Add Archive ... Name : " + cname + " Type : " + type );

		return archive;
	}

	void	ArchiveManager::destroyArchive( Archive *archive )
	{
		ArchiveFactoryMap::iterator it = mArchiveFactories.find( archive->getType() );
		
		if( it == mArchiveFactories.end() )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the ArchiveFactory, Type : " + 
				archive->getType(), "FileMananger::destroyArchive" );
		}

		/// remove the archive from the archive list
		ArchiveMap::iterator ait = mArchives.find( archive->getName() );
		if( ait == mArchives.end() )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "The archive isnot stored , Name: " + 
				archive->getName(), "FileMananger::destroyArchive" );
		}
		mArchives.erase( ait );

		archive->unload();

		LogManager::getSingletonPtr()->logMessage( LL_INFORMATION,
			"Destroy Archive ... Name : " + archive->getName() + " Type : " + archive->getType() );

		it->second->destroyArchive( archive );

	}

	void	ArchiveManager::destroyArchive( const string &name )
	{
		string cname( name );
		modifyName( cname );
		
		destroyArchive( getArchive( cname ) );

	}

	void	ArchiveManager::destroyAllArchives()
	{
		for( ArchiveMap::iterator it = mArchives.begin(); it != mArchives.end(); ++ it )
		{
			Archive *archive = it->second ;
			ArchiveFactoryMap::iterator itm = mArchiveFactories.find( archive->getType() );

			if( itm == mArchiveFactories.end() )
			{
				EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the ArchiveFactory, Type : " + 
					archive->getType(), "FileMananger::destroyAllArchives" );
			}

			archive->unload();

			LogManager::getSingletonPtr()->logMessage( LL_INFORMATION,
				"Destroy Archive ... Name : " + archive->getName() + " Type : " + archive->getType() );

			itm->second->destroyArchive( archive );
		}

		mArchives.clear();
	}

	Archive	*ArchiveManager::getArchive( const string &name )
	{
		string cname( name );
		modifyName( cname );

		ArchiveMap::iterator it = mArchives.find( cname );
		if( it == mArchives.end() )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "Cannot find the archive , Name: " + name );
			return 0;
		}
		
		return it->second ;
	}

	void ArchiveManager::modifyName( string &name )
	{
		/// make '\\' to '/'
		for( size_t i = 0; i < name.size(); ++ i )
		{
			if( name.at( i ) == '\\' )
			{
				name.at( i ) = '/';
			}
		}

		/// append './' if there isnot
		if( name.size() < 2 || ( !( name.at( 0 ) == '.' && name.at( 1 ) == '/' ) ) )
		{
			name.insert( 0, "./" );
		}

	}

}
