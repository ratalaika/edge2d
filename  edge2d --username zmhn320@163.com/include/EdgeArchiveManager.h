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
#ifndef EDGE_ARCHIVEMANAGER_H
#define EDGE_ARCHIVEMANAGER_H

#include "EdgeCompile.h"
#include <string>
#include <map>
#include "EdgeDataStream.h"
#include "EdgeSingleton.h"
#include "EdgeArchive.h"

using std::map;
using std::string;

namespace Edge
{
	/**
	 * ArchiveManager manages load/unload Archives using ArchiveFactory, you can 
	 * regisgter your own ArchiveFactory and so you can create your own Archive.
	 * And this object will also store all archives it creates.
	 * 
	 * @remarks this is the only class you should pay attention to in IO module.
	 * Usually you can use an ArchiveManager to create an Archive, and later use
	 * the Archive to create a DataStream, a DataStream usually implements a file.
	 * @par you donot need to create an ArchiveManager yourself. And ArchiveManager
	 * is a basic module and it's created by EdgeEngine.Anytime you can access the manager
	 * by EdgeEngine::getSingleton().getArchiveManager, all ArchiveManager::getSingleton
	 */
	class EDGE_EXPORT ArchiveManager : public Singleton<ArchiveManager>
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ArchiveManager();

		/**
		 * Destructor
		 *
		 */
		~ArchiveManager();

		/**
		 * addArchiveFactory, you create an ArchiveFactory and register it in the manager
		 *
		 * @remarks When you create your own Archive plugin, you can register it using this
		 * function.
		 */
		void	addArchiveFactory( ArchiveFactory *archiveFactory );

		/**
		 * removeArchiveFactory
		 *
		 * this function just remove the archive factory from the list, and will not 
		 * delete the archive factory.
		 */
		void	removeArchiveFactory( ArchiveFactory *archiveFactory );

		/**
		 * createArchive, it will create an archive using the exist ArchiveFactory
		 * so , you can NOT delete an archive yourself, instead you must call 
		 * destroyArchive to delete an archive.
		 * Also when the ArchiveManager is deleted, it will automatically delete
		 * all the archives it stores.
		 *
		 * @remarks If the archive has already been created, this function will return the precreated archive.
		 * You can specify some other data to create the archive by pInData.
		 */
		Archive *createArchive( const string &name, const string &type, void *pInData = 0 );
	
		/**
		 * destroyArchive
		 *
		 */
		void	destroyArchive( Archive *archive );
		void	destroyArchive( const string &name );

		/**
		 * destroyAllArchives, it will called by the destructor
		 *
		 */
		void	destroyAllArchives();

		/**
		 * getArchive
		 *
		 * @warning if the archive is not exist, this function will throw an exception.
		 */
		Archive *getArchive( const string &name );

	private:

		/**
		 * selfGetArchive, this class will use it to check whether the archive is already exist.
		 *
		 */
		Archive *_selfGetArchive( const string &name );

		/**
		 * modify the name to the correct format
		 *
		 */
		void _modifyName( string &name );

	private:
		typedef map<string, ArchiveFactory*> ArchiveFactoryMap;
		typedef map<string, Archive*> ArchiveMap;

	private:
		/// archive factory list
		ArchiveFactoryMap mArchiveFactories;
		/// created archive list
		ArchiveMap mArchives;
	};
}

#endif
