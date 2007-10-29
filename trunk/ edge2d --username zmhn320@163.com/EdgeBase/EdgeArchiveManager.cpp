/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
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
		_modifyName( cname );

		Archive *archive = _selfGetArchive( cname );
		if( archive != 0 )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The Archive " + name + " is already created!" );
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
		_modifyName( cname );
		
		Archive *archive = _selfGetArchive( cname );
		if( archive == 0 )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the archive : " + name,
				"ArchiveManager::destroyArchive" );
		}
		
		destroyArchive( archive );
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
		_modifyName( cname );
	
		Archive *archive = _selfGetArchive( cname );
		if( archive == 0 )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the archive : " + name,
				"ArchiveManager::getArchive" );
		}

		return archive;
	}

	////////////////////////////////////////////////////////////////////////////////////
	Archive *ArchiveManager::_selfGetArchive( const string &name )
	{
		ArchiveMap::iterator it = mArchives.find( name );
		if( it == mArchives.end() )
		{
			return 0;
		}
		
		return it->second ;
	}

	void ArchiveManager::_modifyName( string &name )
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
