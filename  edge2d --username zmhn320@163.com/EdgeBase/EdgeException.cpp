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
#include "EdgeException.h"
#include "EdgeLogManager.h"

#ifdef _MSC_VER
#pragma warning( disable:4996 )
#endif
namespace Edge
{
	Exception::Exception( EExceptionCodes code, const string &desc, const string &source,
		const char *file, long line ) :
	mDesc( desc ), mSource( source ), mFile( file )
	{
		mCode = code;
		mLine = line;

		LogManager::getSingletonPtr()->logMessage( LL_ERROR, getFullDescription() ); 
	}

	Exception::~Exception()
	{

	}

	string	Exception::getFullDescription()
	{
		char line[30];
		sprintf( line, "%d", mLine );

		string str( "An exception has been thrown.\n\n" );
		str += "--------------------DETAILS--------------------------\n" ;
		str += " Code   : " + codeToStr( mCode ) ;
		str += "\n Desc   : " + mDesc;
		str += "\n Source : " + mSource;
		str += "\n File   : " + mFile;
		str += "\n Line   : " + string( line );
		str += "\n----------------------------------------------------";
		return str;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////
	string	Exception::codeToStr( EExceptionCodes code )
	{
		switch( code )
		{
		case EC_INVALIDPARAMS:
			return "EC_INVALIDPARAMS";
		case EC_FILE_NOT_FOUND:
			return "EC_FILE_NOT_FOUND";
		case EC_CANNOT_OPEN_FILE:
			return "EC_CANNOT_OPEN_FILE";
		case EC_CANNOT_LOAD_TEXTURE:
			return "EC_CANNOT_LOAD_TEXTURE";
		case EC_FAILED_LOCKTEXTURE:
			return "EC_FAILED_LOCKTEXTURE";
		case EC_OTHER_ERROR:
			return "EC_OTHER_ERROR";
		case EC_UNSUPPORT_FUNCTION:
			return "EC_UNSUPPORT_FUNCTION";
		case EC_CANNOT_CREATE_FONT:
			return "EC_CANNOT_CREATE_FONT";
		case EC_LOAD_PLUGIN_FAILED:
			return "EC_LOAD_PLUGIN_FAILED";
		case EC_UNLOAD_PLUGIN_FAILED:
			return "EC_UNLOAD_PLUGIN_FAILED";
		case EC_GENERAL:
			return "EC_GENERAL";
		}

		return "";
	}
}