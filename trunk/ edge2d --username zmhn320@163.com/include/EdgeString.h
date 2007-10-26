/**
 *
 *
 */
#ifndef EDGE_STRING_H
#define EDGE_STRING_H

#include "EdgeCompile.h"
#include <string>

using std::wstring;
using std::string;

namespace Edge
{
	wstring EDGE_EXPORT toWideString( const char* pStr , int len );
	wstring EDGE_EXPORT toWideString( const string &str );
	string  EDGE_EXPORT toNarrowString( const wchar_t* pStr , int len );
	string  EDGE_EXPORT toNarrowString( const wstring &str );

}

#endif