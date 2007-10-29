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
		 * get the image's width 
		 *
		 */
		unsigned getWidth()  { return mWidth; }

		/**
		 * get the image's height.
		 *
		 */
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
