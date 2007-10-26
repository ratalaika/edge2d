/**
*
*
*/
#include "EdgeString.h"
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#include <windows.h>
#endif

namespace Edge
{
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32 // win32 implemention

	wstring toWideString( const char* pStr , int len )
	{
		// figure out how many wide characters we are going to get 
		int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
		if ( len == -1 )
			-- nChars ; 
		if ( nChars == 0 )
			return L"" ;

		// convert the narrow string to a wide string 
		// nb: slightly naughty to write directly into the string like this
		wstring buf ;
		buf.resize( nChars ) ; 
		MultiByteToWideChar( CP_ACP , 0 , pStr , len , 
			const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

		return buf ;
	}

	wstring toWideString( const string &str )
	{
		return toWideString( str.c_str(), static_cast<int>( str.size() ) );
	}

	string toNarrowString( const wchar_t* pStr , int len )
	{
		// figure out how many narrow characters we are going to get 
		int nChars = WideCharToMultiByte( CP_ACP , 0 , 
			pStr , len , NULL , 0 , NULL , NULL ) ; 
		if ( len == -1 )
			-- nChars ; 
		if ( nChars == 0 )
			return "" ;

		// convert the wide string to a narrow string
		// nb: slightly naughty to write directly into the string like this
		string buf ;
		buf.resize( nChars ) ;
		WideCharToMultiByte( CP_ACP , 0 , pStr , len , 
			const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ; 

		return buf ; 
	}

	string toNarrowString( const wstring &str )
	{
		return toNarrowString( str.c_str(), static_cast<int>( str.size() ) );
	}

#endif

}
