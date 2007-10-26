/**
 *
 *
 *
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
