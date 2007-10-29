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
#ifndef EDGE_IMAGEFACTORY_H
#define EDGE_IMAGEFACTORY_H

#include "EdgeCompile.h"
#include <map>
#include <string>
#include "EdgeImage.h"

using std::map;
using std::string;

namespace Edge
{
	/**
	 * ImageFactory will create Images for you, and it will
	 * also maintain the Image's pointer, and later you should delete the image
	 * use destroy function.
	 *
	 */
	class EDGE_EXPORT ImageFactory 
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ImageFactory();

		/**
		 * Destructor
		 *
		 */
		virtual ~ImageFactory();

		/**
		 * create the factory
		 *
		 */
		virtual bool initiate() { return true; }

		/**
		 * release the factory, it will be called automatically by the constructor
		 *
		 */
		virtual void release() {}

		/**
		 * createImage
		 * this function will save the pointer, and you can get the created image by its full path name.
		 * If there is an image which has the same archive name and file name in the image list, this 
		 * function will not create the image, and only return the created image in the list.
		 *
		 * Note : file parameter contains the archive name and the file name, they are separated by ':'
		 * i.e : ./data:image.png 
		 */
		virtual Image* createImage( const string &file ) = 0;

		/**
		 * create an image without data.Maybe you want to write data onto the image through lock/unlock
		 * function.
		 *
		 */
		virtual Image* createImage( const string &name, int width, int height ) = 0;

		/**
		 * copy an image from an exist image
		 *
		 * @remarks if you want to create an image which is a part of another image, you should call
		 * this function.
		 */
		virtual Image* copyImage( const string &name, const string &origin, float x, float y, float w, float h ) = 0;

		/**
		 * create a render target. 
		 * @remarks a render target can be render anything onto it.
		 *
		 */
		virtual Image* createTarget( const string &name, int width, int height ) = 0;
		
		/**
		 * get an image/target you have created before.
		 *
		 */
		Image* getImage( const string &name );

		/**
		 * removeImage
		 * this function will remove the Image in the list and also will destroy the 
		 * Image object from memory
		 * 
		 */
		void removeImage( const string &name );

		/**
		 * remove an image directly.
		 *
		 */
		void removeImage( Image *image );

		/**
		 * removeAllImages, called by the destructor.
		 *
		 */
		void removeAllImages();

		
		/**
		 * notify a child's death, when some children of this factory are destroyed, this function
		 * will be called
		 *
		 */
		virtual void notifyDeath( const Image *image ) {}

	protected:
		
		/**
		 * selfGetImage, this class will use it to check whether the image is already exist.
		 *
		 */
		Image *_selfGetImage( const string &name );

		/**
		 * saveImage, save the image object in the list
		 *
		 */
		void _saveImage( const string &name, Image *image );

		/**
		 * modifyName, modify the image's name so that it's the correct format. 
		 * the correct format is like : ./data/image.jpg
		 *
		 */
		void _modifyName( string &name );

	protected:
		/// 
		typedef map<string, Image*>	ImageMap;

	protected:
		ImageMap	mImages;
	};
}


#endif
