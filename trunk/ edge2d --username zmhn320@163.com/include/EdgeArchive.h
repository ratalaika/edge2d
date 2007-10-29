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
	 * Archive-handling class.
	 * @remarks Archive, an archive means a directory on file system, or a package like
	 * zip package lzo package etc.
	 * The archive's name is very necessary. To a FileSystemArchive, it's the directory 
	 * path, to a ZipArchive, it's the zip file's file name(including the path ).
	 *
	 * @par Every archive's name is unique in the global range.
	 */
	class EDGE_EXPORT Archive
	{
	public:
		/**
		 * Constructor, you donot need to create an archive yourself, you can get an 
		 * archive by ArchiveManager::createArchive
		 *
		 */
		Archive( const string &name, const string &type );

		/**
		 * Destructor
		 *
		 */
		virtual ~Archive() {}

		/** 
		 * load the archive.
		 *
		 * @ramarks some archive needs to be initiated.
		 * @warning this function is only called when you call ArchiveManager::createArchive.
		 * so you should not call this function.
		 */
		virtual bool	load() = 0;

		/** 
		 * unload the archive.
		 *
		 * @ramarks called when the archive is deleted.
		 */
		virtual void	unload() = 0;

		/**
		 * open a file and it will not manage the object.
		 *
		 * @remarks this is the only function you will pay attention to. It will open a file in 
		 * the archive.
		 */
		virtual DataStreamPtr open( const string &file ) = 0;

		/**
		 * get the archive name of this archive.
		 *
		 */
		string &getName() { return mName; }

		/**
		 * get the type name of this archive.
		 *
		 */
		string &getType() { return mType; }

	protected:
		/// archive name, should be a file name for a compressed achive like : ./data.zip
		string mName;
		/// archive type, can be "FileSystem", "Zip", "Lzo" etc...
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
		 * get the archive factory type.
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
		 * @copydoc Archive::load
		 *
		 */
		bool	load();

		/**
		 * @copydoc Archive::unload
		 *
		 */
		void	unload();

		/**
		 * @copydoc Archive::open
		 *
		 */
		DataStreamPtr	open( const string &file );
		
	private:
		/// path
		string mPath;
	};

	/**
	 * FileSystemArchiveFactory
	 *
	 * @remarks used to create a FileSystemArchive. 
	 */
	class EDGE_EXPORT FileSystemArchiveFactory : public Singleton<FileSystemArchiveFactory>,
		public ArchiveFactory
	{
	public:
		/**
		 * create a FileSystem archive. 
		 *
		 */
		Archive	*createArchive( const string &name, void *pInData = 0 );

		/**
		 * destroyo a FileSystem archive.
		 *
		 */
		void destroyArchive( Archive *archive );

		/**
		 * get this archive type.
		 *
		 */
		string getType() { return "FileSystem"; }
	};
}

#endif