/**
 *
 *
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
		 * createArchive, it will new a archive using the exist ArchiveFactory
		 * so , you can NOT delete an archive yourself, instead you must call 
		 * destroyArchive to delete an archive.
		 * Also when the ArchiveManager is deleted, it will automatically delete
		 * all the archives it stores.
		 *
		 * If the archive has already been created, this function will return the precreated archive.
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
		 */
		Archive *getArchive( const string &name );

	private:
		void modifyName( string &name );

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
