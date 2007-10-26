/**
 *
 *
 *
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

