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
#include "EdgeLogManager.h"
#include <algorithm>
#include <stdarg.h>

#ifdef _MSC_VER
#pragma warning( disable:4996 )
#endif

namespace Edge
{
	LogManager::LogManager()
	{
		mCurrentLog = NULL;
		setLogLevel( LL_INFORMATION );
	}

	LogManager::~LogManager()
	{
		destroyAllLogs();
	}

	void	LogManager::addListener( LogListener *listener )
	{
		mLogListeners.push_back( listener );
	}

	void	LogManager::removeListener( LogListener *listener )
	{
		mLogListeners.erase( std::find( mLogListeners.begin(), 
			mLogListeners.end(), listener ) );
	}

	Log		*LogManager::createLog( const string &name, const string &file )
	{
		/// maybe here is a bug. If mLogs already has a log named name
		/// the insert will failed, and LogManager doesnot maintain the log
		Log *log = new Log( name, file );
		mLogs[ name ] = log;
		
		if( mCurrentLog == NULL )
		{
			mCurrentLog = log;
		}
		return log;
	}

	Log		*LogManager::getLog( const string &name )
	{
		LogMap::iterator it = mLogs.find( name );
		if( it != mLogs.end() )
		{
			return it->second;
		}
		return NULL;
	}

	Log		*LogManager::getCurrentLog()
	{
		return mCurrentLog;
	}

	void	LogManager::destroyLog( const string &name )
	{
		LogMap::iterator it = mLogs.find( name );
		if( it != mLogs.end() )
		{
			delete it->second ;
			mLogs.erase( it );
		}
	}

	void	LogManager::destroyLog( Log *log )
	{
		destroyLog( log->getName() );
	}
	
	void	LogManager::destroyAllLogs()
	{
		for( LogMap::iterator it = mLogs.begin(); it != mLogs.end(); ++ it )
		{
			delete it->second;
		}
		mLogs.clear();
	}

	Log		*LogManager::setCurrentLog( Log *log )
	{
		Log *prevLog = mCurrentLog;
		mCurrentLog = log;
		return prevLog;
	}

	void	LogManager::logMessage( ELogLevel level, const string &msg )
	{
		if( mCurrentLog != NULL )
		{
			mCurrentLog->logMessage( level, msg );
		}
	}
	
	void	LogManager::logMessage( const string &logname, ELogLevel level, const string &msg )
	{
		Log *log = getLog( logname );
		if( log != NULL )
		{
			log->logMessage( level, msg );
		}
	}

	void	LogManager::logMessageF( ELogLevel level, const char *format, ... )
	{
		char msg[512]; 
		va_list list;
		va_start( list, format );
		vsprintf( msg, format, list );
		va_end( list );

		logMessage( level, msg );
	}

	void	LogManager::logMessageF( const string &name, ELogLevel level, const char *format, ... )
	{
		char msg[512];
		va_list list;
		va_start( list, format );
		vsprintf( msg, format, list );
		va_end( list );

		logMessage( name, level, msg );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	bool	LogManager::canLog( ELogLevel level )
	{
		return level <= mLogLevel;
	}

	void	LogManager::_routeMessage( const string &logName, ELogLevel level, const string &msg )
	{
		for( size_t i = 0; i < mLogListeners.size(); ++ i )
		{
			mLogListeners.at( i )->log( logName, level, msg );
		}
	}
}

