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
		Font *font = _selfGetFont( name );
		if( font == 0 )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the font : " + name,
				"FontFactory::getFont" );
		}
		return font;
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

	///////////////////////////////////////////////////////////////////////////////////////////
	Font *FontFactory::_selfGetFont( const string &name )
	{
		FontMap::iterator it = mFonts.find( name ) ;

		if( it == mFonts.end() )
		{
			return 0;
		}

		return it->second;
	}

	void FontFactory::_modifyName( string &name )
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