/**
 *
 *
 *
 */
#ifndef EDGE_IMAGEDATA_H
#define EDGE_IMAGEDATA_H

#include "EdgeCompile.h"
#include "EdgeSharedPtr.h"

namespace Edge
{
	/**
	 * maintain the data for an image, the data includes the pixel information, 
	 * and image information( width, height ). The pixel data is 32-bit, and it's 
	 * ARGB format. 
	 *
	 * BYET *data = imageData->getData();
	 * so : data[0] = b, data[1] = g, data[2] = r, data[3] = a
	 * Also you can do this :
	 * DWORD *data = (DWORD*)imageData->getData();
	 * so : data[pixel] = 0xaarrggbb.
	 */
	class EDGE_EXPORT ImageData
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ImageData();

		/**
		 * Copy constructor, copy the data but not the pointer
		 *
		 */
		ImageData( const ImageData &id );

		/**
		 * operator = 
		 *
		 */
		ImageData &operator = ( const ImageData &id );

		/**
		 * Destructor
		 *
		 */
		~ImageData();

		/**
		 * create, it will allocate memory to store the data.
		 *
		 */
		bool create( unsigned width, unsigned height ) ;

		/**
		 * release, frees the memory, automatically called by destructor
		 *
		 */
		void release();

		/**
		 * getData, returns the pointer points to the memory
		 *
		 */
		unsigned char *getData();

		/**
		 * 
		 *
		 */
		unsigned getWidth()  { return mWidth; }
		unsigned getHeight() { return mHeight; }

	protected:
		/// pixel data
		unsigned char *mData;
		/// width
		unsigned mWidth;
		/// height
		unsigned mHeight;
	};

	typedef shared_ptr<ImageData> ImageDataPtr;
}

#endif
