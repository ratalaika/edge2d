/**
 *
 *
 *
 */
#include "EdgeException.h"
#include "EdgeLogManager.h"

#ifdef _MSC_VER
#pragma warning( disable:4996 )
#endif
namespace Edge
{
	Exception::Exception( EExceptionCodes code, const string &desc, const string &source,
		const char *file, long line ) :
	mDesc( desc ), mSource( source ), mFile( file )
	{
		mCode = code;
		mLine = line;

		LogManager::getSingletonPtr()->logMessage( LL_ERROR, getFullDescription() ); 
	}

	Exception::~Exception()
	{

	}

	string	Exception::getFullDescription()
	{
		char line[30];
		sprintf( line, "%d", mLine );

		string str( "An exception has been thrown.\n\n" );
		str += "--------------------DETAILS--------------------------\n" ;
		str += " Code   : " + codeToStr( mCode ) ;
		str += "\n Desc   : " + mDesc;
		str += "\n Source : " + mSource;
		str += "\n File   : " + mFile;
		str += "\n Line   : " + string( line );
		str += "\n----------------------------------------------------";
		return str;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////
	string	Exception::codeToStr( EExceptionCodes code )
	{
		switch( code )
		{
		case EC_INVALIDPARAMS:
			return "EC_INVALIDPARAMS";
		case EC_FILE_NOT_FOUND:
			return "EC_FILE_NOT_FOUND";
		case EC_CANNOT_OPEN_FILE:
			return "EC_CANNOT_OPEN_FILE";
		case EC_CANNOT_LOAD_TEXTURE:
			return "EC_CANNOT_LOAD_TEXTURE";
		case EC_FAILED_LOCKTEXTURE:
			return "EC_FAILED_LOCKTEXTURE";
		case EC_OTHER_ERROR:
			return "EC_OTHER_ERROR";
		case EC_UNSUPPORT_FUNCTION:
			return "EC_UNSUPPORT_FUNCTION";
		case EC_CANNOT_CREATE_FONT:
			return "EC_CANNOT_CREATE_FONT";
		case EC_LOAD_PLUGIN_FAILED:
			return "EC_LOAD_PLUGIN_FAILED";
		case EC_UNLOAD_PLUGIN_FAILED:
			return "EC_UNLOAD_PLUGIN_FAILED";
		case EC_GENERAL:
			return "EC_GENERAL";
		}

		return "";
	}
}