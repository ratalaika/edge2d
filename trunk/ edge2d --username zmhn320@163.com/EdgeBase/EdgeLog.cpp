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
#include "EdgeLog.h"
#include "EdgeLogManager.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include <iostream>
#include <iomanip>

#ifdef _MSC_VER
#pragma warning( disable:4996 )
#endif

namespace	Edge
{
	Log::Log( const string &name, const string &file ) 
		: mName( name )
	{
		mFile.open( file.c_str() );
	}

	Log::~Log()
	{
		mFile.close();
	}

	void	Log::logMessage( ELogLevel level, const string &msg )
	{
		if( !LogManager::getSingletonPtr()->canLog( level ) )
		{
			return ;
		}

		/// append log level information
		string levelInfo;
		switch( level )
		{
		case LL_INFORMATION:
			levelInfo = "INFO    : ";
			break;
		case LL_WARNING:
			levelInfo = "WARNING : " ;
			break;
		case LL_ERROR:
			levelInfo = "ERROR   : " ;
			break;
		default:
			levelInfo = "UNKNOWN LOG LEVEL : " ;
		}

		LogManager::getSingletonPtr()->_routeMessage( mName, level, msg );

		struct tm *pTime;
		time_t ctTime; 
		time( &ctTime );
		pTime = localtime( &ctTime );
		mFile << std::setw(2) << std::setfill('0') << pTime->tm_hour
			<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
			<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec 
			<< ": " << levelInfo << msg << std::endl;

		mFile.flush();
	}
}
