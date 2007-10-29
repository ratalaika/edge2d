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