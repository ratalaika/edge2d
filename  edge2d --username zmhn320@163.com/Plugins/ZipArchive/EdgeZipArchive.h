/**
 *
 *
 *
 */
#ifndef EDGE_ZIPARCHIVE_H
#define EDGE_ZIPARCHIVE_H

#if defined (__WIN32__) || defined(_WIN32)
#include <windows.h>
#else
#define ZIP_STD
#endif

#include "EdgeArchive.h"
#include "unzip/unzip.h"

namespace Edge
{
	/**
	 * ZipArchive, it will read data from a zip file.
	 * The name of a ZipArchive is the file name of this zip.
	 * This ZipArchive can only load a zip file on the file system.
	 *
	 */
	class ZipArchive : public Archive
	{
	public:
		/**
		 *
		 *
		 */
		ZipArchive( const string &name, const string &type, const string &psd );

		/**
		 *
		 *
		 */
		~ZipArchive();

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
		 * open a file in the zip archive
		 *
		 */
		DataStreamPtr open( const string &file );

	private:
		/// the archive's password.
		string mPassword;
		/// zip file handle
		HZIP mZipHandle;
	};

	/**
	 * ZipArchiveFactory
	 *
	 *
	 */
	class ZipArchiveFactory : public Singleton<ZipArchiveFactory>, public ArchiveFactory
	{
	public:
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
		string getType() { return "Zip"; }

	};
}

#endif
