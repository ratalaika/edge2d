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
#ifndef EDGE_DATASTREAM_H
#define EDGE_DATASTREAM_H

#include "EdgeCompile.h"
#include "EdgeSharedPtr.h"
#include <string>
#include <fstream>

using std::string;

namespace Edge
{
	/**
	 * General purpose class used for encapsulating the reading of data. 
	 * @remarks this class usually implements a file or a trunk of memory.
	 * you can use this class to read data from a file or a truck of memory.
	 * @warning you cannot create a DataStream yourself, you must get a DataStream
	 * from an Archive.
	 *
	 */
	class EDGE_EXPORT DataStream
	{
	public:
		/**
		 * Constructor
		 *
		 */
		DataStream( const string &name );

		/**
		 * Destructor
		 *
		 */
		virtual ~DataStream();

		/**
		 * read data
		 * @remarks you must allocate enough memory to get the data. 
		 * @param buf the output buffer
		 * @param count how many bytes you want to read
		 * @return the size this function has read in bytes.
		 *
		 */
		virtual size_t read( void *buf, size_t count ) = 0;

		/**
		 * read a line
		 * @remarks you must allocate enough memory to get the data.
		 * and it will read count - 1 bytes in max. and will put count - 1
		 * to 0.
		 * @param buf the output buffer
		 * @param count count here identifies the buf size
		 */
		virtual size_t readLine( void *buf, size_t count ) ;

		/**
		 * repositions the read point to a specified byte, releative to the begining.
		 *
		 */
		virtual void seek( size_t pos ) = 0;

		/**
		 * check whether the stream has reached the end
		 *
		 */
		virtual bool eof() = 0;

		/**
		 * get the current position in the stream
		 *
		 */
		virtual size_t getPos() = 0;

		/**
		 * getSize
		 *
		 * @return the size of this stream.
		 */
		size_t getSize() { return mSize; }

		/**
		 * getName
		 */
		string &getName() { return mName; }
	protected:
		/// stream size
		size_t mSize;
		/// stream name
		string mName;
	};

	typedef shared_ptr<DataStream> DataStreamPtr;

	/**
	 * FileStreamDataStream, implements a file on the filesystem
	 * The name of FileStreamDataStream is the file name.
	 */
	class EDGE_EXPORT FileSystemDataStream : public DataStream
	{
	public:
		/**
		 * Constructor
		 *
		 */
		FileSystemDataStream( const string &file );

		/**
		 * Destructor
		 *
		 */
		~FileSystemDataStream();

		/**
		 * @copydoc DataStream::read
		 */
		size_t read( void *buf, size_t count );
		
		/**
		 * @copydoc DataStream::seek
		 */
		void seek( size_t pos );

		/**
		 * @copydoc DataStream::eof
		 */
		bool eof();

		/**
		 * @copydoc DataStream::getPos
		 */
		size_t getPos();

	private:
		/**
		 * open a file.
		 *
		 */
		virtual bool open( const string &file );

		/**
		 * close a file
		 *
		 */
		virtual void close();

	protected:
		/// stream
		std::ifstream mStream;
	};

	/**
	 * MemoryDataStream, implements a stream in memory.
	 * 
	 *
	 */
	class EDGE_EXPORT MemoryDataStream : public DataStream
	{
	public:
		/**
		 * Constructor, it will allocate memroy and later you can copy data to it.
		 *
		 */
		MemoryDataStream( const string &name, size_t size );

		/**
		 * Destructor, it will delete the memory.
		 *
		 */
		~MemoryDataStream();

		/**
		 * @copydoc DataStream::read
		 */
		size_t read( void *buf, size_t count );

		/**
		 * @copydoc DataStream::seek
		 */
		void seek( size_t pos );

		/**
		 * @copydoc DataStream::eof
		 */
		bool eof();

		/**
		 * @copydoc DataStream::getPos
		 */
		size_t getPos();

		/**
		 * getPointer, get the memory address, so you can copy data to it.
		 *
		 * @warning make sure the size of the data you want to write is smaller than getSize.
		 */
		unsigned char *getPointer() { return mData; }

	protected:
		/// memory address pointer
		unsigned char *mData;
		/// read position
		size_t mPos;
	};
}

#endif
