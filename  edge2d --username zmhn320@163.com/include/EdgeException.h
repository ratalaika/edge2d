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
#ifndef EDGE_EXCEPTION_H
#define EDGE_EXCEPTION_H

#include "EdgeCompile.h"
#include <string>

using std::string;

#define EDGE_EXCEPT( code, desc, src ) throw( Edge::Exception( code, desc, src, __FILE__, __LINE__ ) )

namespace Edge
{
	/**
	 * Exception class. 
	 * @remarks In edge, when an error occured, the engine will throw an exception instead the
	 * return value of a function. 1
	 *
	 */
	class EDGE_EXPORT Exception
	{
	public:
		/**
		 * Exception codes
		 *
		 */
		enum EExceptionCodes
		{
			EC_INVALIDPARAMS,
			EC_FILE_NOT_FOUND,
			EC_CANNOT_OPEN_FILE,
			EC_CANNOT_LOAD_TEXTURE,
			EC_FAILED_LOCKTEXTURE,
			EC_UNSUPPORT_FUNCTION,
			EC_CANNOT_CREATE_FONT,
			EC_LOAD_PLUGIN_FAILED,
			EC_UNLOAD_PLUGIN_FAILED,
			/// TODO : add more codes
			EC_OTHER_ERROR,
			EC_GENERAL
		};
	public:
		/**
		 * Constructor 
		 *
		 * @param codes : EExceptionCodes
		 * @param desc  : the description for this exception
		 * @param source: where the exception happend, maybe a function name
		 * @param file  : the source file where the exception happened
		 * @param line  : the source code line where the exception throwed
		 */
		Exception( EExceptionCodes code, const string &desc, const string &source,
			       const char *file, long line );

		/**
		 * Destructor
		 *
		 */
		virtual ~Exception();

		/**
		 * Returns a string with the full description of this error.
		 * @remarks The description contains the error number, the description
         * supplied by the thrower, what routine threw the exception,
         * and will also supply extra platform-specific information
         * where applicable. 
		 *
		 */
		virtual string	getFullDescription();

		/**
		 * getCode, get the error code.
		 *
		 */
		EExceptionCodes	getCode() { return mCode; }

		/**
		 * getDescription
		 *
		 */
		string	getDescription() { return mDesc; }

		/**
		 * getSource
		 *
		 */
		string	getSource() { return mSource; }

		/**
		 * getFile
		 *
		 */
		string	getFile() { return mFile; }

		/**
		 * getLine
		 *
		 */
		long	getLine() { return mLine; }

	protected:
		/** 
		 * codeToStr
		 *
		 */
		string codeToStr( EExceptionCodes code );

	protected:
		EExceptionCodes	mCode;
		string mDesc;
		string mSource;
		string mFile;
		long  mLine;
	};
}

#endif