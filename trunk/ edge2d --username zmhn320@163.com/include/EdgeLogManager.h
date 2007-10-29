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
#ifndef EDGE_LOGMANAGER_H
#define EDGE_LOGMANAGER_H

#include "EdgeCompile.h"
#include "EdgeSingleton.h"
#include "EdgeLog.h"
#include <map>
#include <vector>

namespace Edge
{
	/**
	 * The log listener is to help you get the log message so that you 
	 * can redirect the log message to anywhere you want
	 */
	class EDGE_EXPORT LogListener
	{
	public:
		/**
		 * called when there are some log messages
		 *
		 */
		virtual void log( const string &name, ELogLevel level, const string &message ) = 0;
	};

	/** 
	 * The log manager handles the creation and retrieval of logs for the
	 * application.
	 * @remarks it's the basic component for the engine, so it will be created when you call
	 * EdgeEngine::initBase.
	 */
	class EDGE_EXPORT LogManager : public Singleton<LogManager>
	{
	public:
		/**
		 * constructor
		 *
		 */
		LogManager();

		/**
		 * destructor
		 *
		 */
		~LogManager();

		/**
		 * addListener, you can add a log listener to redirect the log message.
		 *
		 */
		void	addListener( LogListener *listener );

		/**
		 * removeListener
		 *
		 */
		void	removeListener( LogListener *listener );

		/**
		 * createLog
		 * it will create a log for you, and you must delete the log
		 * by LogManager::destroyLog, donot delete a log manually.
         *
		 * The first created log will be the current log if the current 
		 * log is null.
		 */
		Log		*createLog( const string &name, const string &file );

		/**
		 * getLog
		 *
		 * get a log.
		 */
		Log		*getLog( const string &name );

		/**
		 * getCurrentLog
		 *
		 * messages always will be wrote in the current log
		 */
		Log		*getCurrentLog();

		/**
		 * destroyLog
		 *
		 */
		void	destroyLog( const string &name );

		/**
		 * destroyLog
		 *
		 */
		void	destroyLog( Log *log );

		/**
		 * destroyAllLogs, called by destructor.
		 *
		 */
		void	destroyAllLogs();

		/**
		 * setCurrentLog
		 * 
		 * set the log as the current log, and it will return the previous log
		 */
		Log		*setCurrentLog( Log *log );

		/**
		 * setLogLevel
		 * when you set a log level, some messages maybe willnot be wrote.
		 * The default log level is LL_INFORMATION, which means it will write all the 
		 * messages, if log level is LL_ERROR, then it will only write error 
		 * messages.If LL_WARNING, it will write error and warning informations
		 */
		void	setLogLevel( ELogLevel level ) { mLogLevel = level; }

		/**
		 * get current log level.
		 *
		 */
		ELogLevel getLogLevel() { return mLogLevel; }

		/**
		 * logMessage
		 * it will write message to the current log
		 */
		void	logMessage( ELogLevel level, const string &msg );

		/**
		 * it will write message to the log that specified
		 *
		 */
		void	logMessage( const string &logname, ELogLevel level, const string &msg );

		/**
		 * writes a formatted message to the log file
		 *
		 * it's a help function so maybe sometimes it cannot be used, and it only can 
		 * write 512 characters.
		 */
		void	logMessageF( ELogLevel level, const char *format, ... );

		/**
		 * writes a formatted message to the log file
		 *
		 * it's a help function so maybe sometimes it cannot be used, and it only can 
		 * write 512 characters.
		 */
		void	logMessageF( const string &logname, ELogLevel level, const char *format, ... );

	private:
		/** 
		 * canLog, checks whether the messages should be logged 
		 *
		 */
		bool	canLog( ELogLevel level );

		/**
		 * it will reroute the messages to all the LogListeners
		 *
		 */
		void	_routeMessage( const string &logName, ELogLevel level, const string &msg );
		friend class Log;

	private:
		typedef std::map<string, Log*>	LogMap;
		typedef std::vector<LogListener*> LogListenerList;

	private:
		LogMap	mLogs;
		LogListenerList mLogListeners;
		Log		*mCurrentLog;
		ELogLevel mLogLevel;
	};

}

#endif

