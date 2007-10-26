/**
*
*
*
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
		 * addListener
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
		void	destroyLog( Log *log );
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

