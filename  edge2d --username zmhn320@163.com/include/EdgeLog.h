/**
 *
 *
 *
 */
#ifndef EDGE_LOG_H
#define EDGE_LOG_H

#include "EdgeCompile.h"
#include <fstream>
#include <string>

using std::ofstream ;
using std::string;

namespace Edge
{
	/**
	 * log level
	 * it will help you write some useful information
	 *
	 */
	enum ELogLevel
	{
		LL_ERROR,
		LL_WARNING,
		LL_INFORMATION,
	};

	/**
	 * a log wirte will wirte messages to a file
	*	
	*/
	class EDGE_EXPORT Log
	{
	public:
		/**
		 * Constructor will open a file for output
		 *
		 * 
		 */
		Log( const string &name, const string &file );

		/**
		 * Destructor will close the file
		 *
		 */
		~Log();

		/**
		 * get the log name
		 *
		 */
		string	&getName()	{	return mName; }

		/**
		 * logMessage
		 *
		 */
		void	logMessage( ELogLevel level, const string &msg );

	private:
		string mName;
		ofstream mFile;
	};
}


#endif