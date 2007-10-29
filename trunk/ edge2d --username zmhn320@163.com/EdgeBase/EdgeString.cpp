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
#include "EdgeString.h"
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#include <windows.h>
#endif

namespace Edge
{
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32 // win32 implemention

	wstring toWideString( const char* pStr , int len )
	{
		// figure out how many wide characters we are going to get 
		int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
		if ( len == -1 )
			-- nChars ; 
		if ( nChars == 0 )
			return L"" ;

		// convert the narrow string to a wide string 
		// nb: slightly naughty to write directly into the string like this
		wstring buf ;
		buf.resize( nChars ) ; 
		MultiByteToWideChar( CP_ACP , 0 , pStr , len , 
			const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

		return buf ;
	}

	wstring toWideString( const string &str )
	{
		return toWideString( str.c_str(), static_cast<int>( str.size() ) );
	}

	string toNarrowString( const wchar_t* pStr , int len )
	{
		// figure out how many narrow characters we are going to get 
		int nChars = WideCharToMultiByte( CP_ACP , 0 , 
			pStr , len , NULL , 0 , NULL , NULL ) ; 
		if ( len == -1 )
			-- nChars ; 
		if ( nChars == 0 )
			return "" ;

		// convert the wide string to a narrow string
		// nb: slightly naughty to write directly into the string like this
		string buf ;
		buf.resize( nChars ) ;
		WideCharToMultiByte( CP_ACP , 0 , pStr , len , 
			const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ; 

		return buf ; 
	}

	string toNarrowString( const wstring &str )
	{
		return toNarrowString( str.c_str(), static_cast<int>( str.size() ) );
	}

#endif

}
