/**
 *
 *
 *
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
	 * manitain some data that can be read
	 *
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
		 *
		 */
		virtual size_t read( void *buf, size_t count ) = 0;

		/**
		 * read a line
		 *
		 * count here identifies the buf size
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
		 */
		size_t getSize() { return mSize; }

		/**
		 * getName
		 */
		string &getName() { return mName; }
	protected:
		size_t mSize;
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
		 *
		 */
		size_t read( void *buf, size_t count );
		
		/**
		 *
		 */
		void seek( size_t pos );

		/**
		 *
		 */
		bool eof();

		/**
		 *
		 */
		size_t getPos();

		/**
		 * open a file
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
		 *
		 */
		size_t read( void *buf, size_t count );

		/**
		 *
		 */
		void seek( size_t pos );

		/**
		 *
		 */
		bool eof();

		/**
		 *
		 */
		size_t getPos();

		/**
		 * getPointer, get the memory address, so you can copy data to it.
		 *
		 */
		unsigned char *getPointer() { return mData; }

	protected:
		/// memory
		unsigned char *mData;
		/// read position
		size_t mPos;
	};
}

#endif