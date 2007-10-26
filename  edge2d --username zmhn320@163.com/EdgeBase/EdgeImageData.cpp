/**
 *
 *
 */
#include "EdgeImageData.h"
#include <memory.h>

namespace Edge
{
	ImageData::ImageData()
	{
		mWidth = mHeight = 0;
		mData = 0;
	}

	ImageData::ImageData( const ImageData &id )
	{
		create( id.mWidth, id.mHeight );
		memcpy( mData, id.mData, sizeof( unsigned char ) * mWidth * mHeight );
	}

	ImageData &ImageData::operator = ( const ImageData &id )
	{
		if( this == &id )
		{
			return *this;
		}

		release();

		create( id.mWidth, id.mHeight );
		memcpy( mData, id.mData, sizeof( unsigned char ) * mWidth * mHeight );

		return *this;
	}

	ImageData::~ImageData()
	{
		release();
	}

	bool	ImageData::create( unsigned width, unsigned height )
	{
		mWidth = width;
		mHeight = height;

		/// 32-bits , 4 bytes
		mData = new unsigned char [ width * height * 4 ];

		return true;
	}

	void	ImageData::release()
	{
		delete [] mData;
		mData = 0;
	}

	unsigned char *ImageData::getData()
	{
		return mData;
	}
}