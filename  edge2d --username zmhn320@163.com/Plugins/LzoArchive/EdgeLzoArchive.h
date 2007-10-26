/**
 *
 *
 *
 */
#ifndef EDGE_LZOARCHIVE_H
#define EDGE_LZOARCHIVE_H

#include "EdgeArchive.h"
#include "lzoarchive.h"

namespace Edge
{
	/**
	 * LzoArchive, it will read data from a lzo archive file(created by Kevin Lynx).
	 * The name of a LzoArchive is the file name of this lzo.
	 * This LzoArchive can only load a lzo archive file on the file system.
	 *
	 */
	class LzoArchive : public Archive
	{
	public:
		/**
		 *
		 */
		LzoArchive( const string &name, const string &type );

		/**
		 *
		 *
		 */
		~LzoArchive();

		/**
		 *
		 *
		 */
		bool load();

		/**
		 *
		 */
		void unload();

		/**
		 * open a file in the lzo archive
		 *
		 */
		DataStreamPtr open( const string &file );

	private:
		lzo_archive *mLzoArchive;
	};

	/**
	 * LzoArchiveFactory
	 *
	 *
	 */
	class LzoArchiveFactory : public Singleton<LzoArchiveFactory>, public ArchiveFactory
	{
	public:
		/**
		 * constructor, it will init lzo library.
		 *
		 */
		LzoArchiveFactory();

		/**
		 *
		 */
		Archive	*createArchive( const string &name, void *pInData = 0 );

		/**
		 *
		 */
		void destroyArchive( Archive *archive );

		/**
		 *
		 */
		string getType() { return "Lzo"; }

	};
}

#endif
