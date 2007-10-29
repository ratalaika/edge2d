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
	 * a log will wirte messages to a file
	 * @remarks Usually you should use LogManager::logMessage to write messages to a log file.
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
		 * logMessage, write message to the log file.
		 *
		 */
		void	logMessage( ELogLevel level, const string &msg );

	private:
		string mName;
		ofstream mFile;
	};
}


#endif
