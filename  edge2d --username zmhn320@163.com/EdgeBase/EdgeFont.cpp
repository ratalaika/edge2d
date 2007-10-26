/**
 *
 *
 *
 */
#include "EdgeFont.h"
#include "EdgeException.h"

namespace Edge
{
	FontFactory::FontFactory()
	{
	}

	FontFactory::~FontFactory()
	{
		removeAllFonts();
	}

	Font *FontFactory::getFont( const string &name )
	{
		FontMap::iterator it = mFonts.find( name ) ;

		if( it == mFonts.end() )
		{
			return 0;
		}

		return it->second;
	}

	void FontFactory::removeFont( const string &name )
	{
		FontMap::iterator it = mFonts.find( name );
		
		if( it != mFonts.end() )
		{
			delete it->second ;
			mFonts.erase( it );
		}
	}

	void FontFactory::removeFont( Font *font )
	{
		removeFont( font->getName() );
	}

	void FontFactory::removeAllFonts()
	{
		for( FontMap::iterator it = mFonts.begin(); it != mFonts.end(); ++ it)
		{
			delete it->second ;
		}

		mFonts.clear();
	}

	void FontFactory::modifyName( string &name )
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