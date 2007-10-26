/**
 *
 *
 *
 */
#ifndef EDGE_ARCHIVE_H
#define EDGE_ARCHIVE_H

#include "EdgeCompile.h"
#include <string>
#include "EdgeDataStream.h"
#include "EdgeSharedPtr.h"
#include "EdgeSingleton.h"

using std::string;

namespace Edge
{
	/**
	 * Archive, an archive means a directory on file system, or a package like
	 * zip package etc.
	 * The archive's name is very necessary. To a FileSystemArchive, it's the directory 
	 * path, to a ZipArchive, it's the zip file's file name(including the path ).
	 *
	 * Every archive's name is unique in the global range.
	 */
	class EDGE_EXPORT Archive
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Archive( const string &name, const string &type );

		/**
		 * Destructor
		 *
		 */
		virtual ~Archive() {}

		/** 
		 * load the archive
		 *
		 *
		 */
		virtual bool	load() = 0;

		/** 
		 * unload the archive
		 *
		 */
		virtual void	unload() = 0;

		/**
		 * open a file and it will not manage the object.
		 *
		 */
		virtual DataStreamPtr open( const string &file ) = 0;

		string &getName() { return mName; }
		string &getType() { return mType; }

	protected:
		/// name
		string mName;
		/// type
		string mType;
	};

	typedef shared_ptr<Archive> ArchivePtr;

	/**
	 * ArchiveFactory, it's used to create an Archive.
	 *
	 */
	class EDGE_EXPORT ArchiveFactory
	{
	public:
		/**
		 * createArchive, you can specify a password to pInData if you want to 
		 * create a Zip archive.
		 *
		 */
		virtual Archive	*createArchive( const string &name, void *pInData = 0 ) = 0;

		/**
		 * destroyArchive
		 *
		 */
		virtual void destroyArchive( Archive *archive ) = 0;

		/**
		 * getType
		 *
		 */
		virtual string getType() = 0;
	};

	/**
	 * FileSystemArchive, it's the basic archive, other archive can be Zip, Pkg, etc
	 * The name of a FileSystemArchive must be it's directory path. 
	 *
	 */
	class EDGE_EXPORT FileSystemArchive : public Archive
	{
	public:
		/**
		 * Constructor
		 *
		 * @param name the path of this file system archive
		 */
		FileSystemArchive( const string &name, const string &type );

		/**
		 * Destructor
		 *
		 */
		~FileSystemArchive();

		/**
		 *
		 */
		bool	load();

		/**
		 *
		 */
		void	unload();

		/**
		 *
		 */
		DataStreamPtr	open( const string &file );
		
	private:
		/// path
		string mPath;
	};

	/**
	 * 
	 *
	 */
	class EDGE_EXPORT FileSystemArchiveFactory : public Singleton<FileSystemArchiveFactory>,
		public ArchiveFactory
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
		string getType() { return "FileSystem"; }
	};
}

#endif