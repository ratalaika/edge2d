/**
 *
 *
 *
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
	 * Exception class
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
		 * getFullDescription
		 *
		 */
		virtual string	getFullDescription();

		/**
		 * getCode
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