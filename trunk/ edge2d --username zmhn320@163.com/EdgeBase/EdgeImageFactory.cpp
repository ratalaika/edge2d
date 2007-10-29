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
#include "EdgeImageFactory.h"
#include "EdgeGraphics.h"
#include "EdgeException.h"

namespace Edge
{
	ImageFactory::ImageFactory()
	{
	}

	ImageFactory::~ImageFactory()
	{
		removeAllImages();
	}

	Image *ImageFactory::getImage( const string &name )
	{
		string cname( name );
		_modifyName( cname );

		Image *image = _selfGetImage( cname );
		if( image == NULL )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the image : " + name,
				"ImageFactory::getImage" );
		}
		return image;
	}

	void ImageFactory::removeImage( const string &name )
	{
		string file( name );
		_modifyName( file );

		ImageMap::iterator it = mImages.find( file );
		if( it != mImages.end() )
		{
			delete it->second ;
			mImages.erase( it );
		}
	}

	void ImageFactory::removeImage( Image *image )
	{
		for( ImageMap::iterator it = mImages.begin(); 
			 it != mImages.end(); )
		{
			if( it->second == image )
			{
				delete image;
				mImages.erase( it );
				return;
			}
			else
			{
				++ it;
			}
		}
	}

	void ImageFactory::removeAllImages()
	{
		for( ImageMap::iterator it = mImages.begin(); 
			 it != mImages.end(); ++ it )
		{
			delete it->second;
		}

		mImages.clear();
	}

	//////////////////////////////////////////////////////////////////////////////
	Image *ImageFactory::_selfGetImage( const string &name )
	{
		ImageMap::iterator it = mImages.find( name );
		if( it == mImages.end() )
		{
			return NULL;
		}

		return it->second ;
	}

	void ImageFactory::_saveImage( const string &name, Image *image )
	{
		mImages[ name ] = image;
	}

	void ImageFactory::_modifyName( string &name )
	{
		/// make '\\' to '/'
		for( size_t i = 0; i < name.size(); ++ i )
		{
			if( name.at( i ) == '\\' )
			{
				name.at( i ) = '/';
			}
		}

		/// append './' if there isnot
		if( name.size() < 2 || ( !( name.at( 0 ) == '.' && name.at( 1 ) == '/' ) ) )
		{
			name.insert( 0, "./" );
		}
	}
}
