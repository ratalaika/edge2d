/**
 *
 *
 *
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

	Image		*ImageFactory::getImage( const string &name )
	{
		string file( name );
		modifyName( file );

		ImageMap::iterator it = mImages.find( file );
		if( it == mImages.end() )
		{
			return NULL;
		}

		return it->second ;
	}

	void		ImageFactory::removeImage( const string &name )
	{
		string file( name );
		modifyName( file );

		ImageMap::iterator it = mImages.find( file );
		if( it != mImages.end() )
		{
			delete it->second ;
			mImages.erase( it );
		}
	}

	void		ImageFactory::removeImage( Image *image )
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

	void		ImageFactory::removeAllImages()
	{
		for( ImageMap::iterator it = mImages.begin(); 
			 it != mImages.end(); ++ it )
		{
			delete it->second;
		}

		mImages.clear();
	}

	void	ImageFactory::saveImage( const string &name, Image *image )
	{
		mImages[ name ] = image;
	}

	void	ImageFactory::modifyName( string &name )
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
