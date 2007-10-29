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
#ifndef EDGE_IMAGECODEC_H
#define EDGE_IMAGECODEC_H

#include "EdgeCompile.h"
#include <string>
#include "EdgeImageData.h"
#include "EdgeDataStream.h"

using std::string;

namespace Edge
{
	/**
	 * ImageCodec will use an image codec library and Edge's IO module to load an image, 
	 * and create an ImageData object to create an Image
	 */
	class EDGE_EXPORT ImageCodec
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ImageCodec() {}

		/**
		 * destructor
		 *
		 */
		virtual ~ImageCodec() {}

		/**
		 * initiate
		 *
		 */
		virtual bool	initiate() = 0;

		/**
		 * release
		 *
		 */
		virtual void	release() = 0;

		/**
		 * decode, it will decode an Image
		 *
		 * @remarks this is the only function you should pay attention to. It will decode
		 * the image file and return an ImageData object.
		 */
		virtual ImageDataPtr	decode( DataStreamPtr stream ) = 0;
	};
}

#endif