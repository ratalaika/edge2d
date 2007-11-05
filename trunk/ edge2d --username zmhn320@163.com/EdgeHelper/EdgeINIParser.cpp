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
#include "EdgeINIParser.h"
#include "EdgeLogManager.h"
#include "EdgeArchive.h"
#include "EdgeDataStream.h"
#include "EdgeArchiveManager.h"
#include <fstream>
#include <sstream>

using std::vector;

namespace Edge
{
	IniParser::IniParser()
	{
	}

	IniParser::~IniParser()
	{
		release();
	}

	void IniParser::create( const string &file )
	{
		release();

		string cfile( file );
		_modifyName( cfile );

		string archiveName, fileName;

		size_t i = cfile.rfind( ':' );
		size_t size = cfile.size();
		archiveName.assign( cfile, 0, i );
		fileName.assign( cfile, i + 1, size - i );

		/// create a data stream
		Archive *archive = Edge::ArchiveManager::getSingletonPtr()->getArchive( archiveName );
		DataStreamPtr stream = archive->open( fileName );

		_parse( stream );		
	}

	void IniParser::createDirectly( const string &file )
	{
		release();

		_parse( DataStreamPtr( new FileSystemDataStream( file ) ) );
	}

	void IniParser::save( const string &file )
	{
		std::ofstream stream( file.c_str() );
		if( stream.fail() )
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Save ini file " + file + " failed" );
			return ;
		}

		for( IniSections::reverse_iterator it = mSections.rbegin(); it != mSections.rend(); ++ it )
		{
			stream << '[' << it->first << ']' << std::endl;
			for( IniSection::IniLines::reverse_iterator it2 = it->second.mLines.rbegin(); it2 != it->second.mLines.rend(); ++ it2 )
			{
				stream << it2->first << '=' << it2->second << std::endl;
			}
		}
	}

	void IniParser::release()
	{
		mSections.clear();
	}

	string IniParser::readString( const string &section, const string &key, const string &defaultStr )
	{
		IniSections::iterator it = mSections.find( section );
		if( it == mSections.end() )
		{
			return defaultStr;
		}
		IniSection::IniLines::iterator it2 = it->second.mLines.find( key );
		if( it2 == it->second.mLines.end() )
		{
			return defaultStr;
		}

		return it2->second ;
	}
	
	int IniParser::readInt( const string &section, const string &key, const int defaultInt )
	{
		std::ostringstream stream;
		stream << defaultInt;
		string str = readString( section, key, stream.str() );
		
		return atoi( str.c_str() );
	}

	float IniParser::readFloat( const string &section, const string &key, const float defaultFloat )
	{
		std::ostringstream stream;
		stream << defaultFloat ;
		string str = readString( section, key, stream.str() );

		return static_cast<float>( atof( str.c_str() ) ); 
	}

	bool IniParser::readBool( const string &section, const string &key, const bool defaultBool )
	{
		string str( defaultBool ? "true" : "false" );
		str = readString( section, key, str );

		return str == "1" || str == "true";
	}

	void IniParser::writeString( const string &section, const string &key, const string &value )
	{
		IniSection section_data;
		IniSections::iterator it = mSections.find( section );
		if( it != mSections.end() )
		{
			/// found the section
			IniSection::IniLines::iterator it2 = it->second.mLines.find( key );
			if( it2 != it->second.mLines.end() )
			{
				/// modify the key's value
				it2->second = value;
			}
			else
			{
				/// not found the key, so add it
				it->second.mLines[ key ] = value;
			}
		}
		else
		{
			/// not found the section, so add it
			section_data.mLines[ key ] = value;
			mSections[ section ] = section_data;
		}
	}

	void IniParser::writeInt( const string &section, const string &key, const int value )
	{
		std::ostringstream stream;
		stream << value;
		writeString( section, key, stream.str() );
	}

	void IniParser::writeFloat( const string &section, const string &key, const float value )
	{
		std::ostringstream stream;
		stream << value;
		writeString( section, key, stream.str() );
	}

	void IniParser::writeBool( const string &section, const string &key, const bool value )
	{
		writeString( section, key, value ? "true" : "false" );
	}

	///////////////////////////////////////////////////////////////////////////
	void IniParser::_parse( DataStreamPtr &stream )
	{
		/// parse the file
		IniSection section;
		string section_name = "";
		char buf[MAX_CHAR];
		string line;
		string key, value;
		string::size_type pos, pos2;

		while( !stream->eof() )
		{
			stream->readLine( buf, MAX_CHAR ); 	
			line = buf;

			pos = line.find_first_not_of( " \t" );

			/// igone blank line
			if( pos >= line.size() ) 
			{
				continue;
			}
			/// igone comment
			if( line.at( pos ) == ';' || line.at( pos ) == '#' )
			{
				continue;
			}

			/// find section ?
			if( line.at( pos ) == '[' )
			{
				/// save the previous section
				if( section_name != "" )
				{
					mSections[section_name] = section;
				}
				/// ready to read the next section
				section.mLines.clear();

				pos2 = line.find( ']', pos );
				if( pos2 >= line.size() )
				{
					LogManager::getSingleton().logMessage( LL_ERROR, "Parse Ini file Error : Invalid section" );
				}
				else
				{
					section_name = line.substr( pos + 1, pos2 - pos - 1 );
				}
			}
			/// find a new line contain a key-value
			else
			{
				/// find key
				pos2 = line.find_first_of( '=', pos );

				if( pos2 >= line.size() )
				{
					LogManager::getSingleton().logMessage( LL_ERROR, "Parse Ini file Error : Invalid Key-Value" );
				}
				else
				{
					string::size_type pos3 = line.find_last_not_of( " \t", pos2 - 1 );
					key = line.substr( pos, pos3 + 1 );

					/// find value
					pos3 = line.find_first_not_of( " \t", pos2 + 1 );
					pos2 = line.find_last_not_of( " \t" );
					value = line.substr( pos3, pos2 + 1 );

					/// save the key and the value
					section.mLines[ key ] = value;
				}
			}
		}// end 'while'

		/// save the last section
		mSections[section_name] = section;
	}

	void IniParser::_modifyName( string &name )
	{
		/// make '\\' to '/'
		for( size_t i = 0; i < name.size(); ++ i )
		{
			if( name.at( i ) == '\\' )
			{
				name.at( i ) = '/';
			}
		}

		/// append './' if there isnot
		if( name.size() < 2 || ( !( name.at( 0 ) == '.' && name.at( 1 ) == '/' ) ) )
		{
			name.insert( 0, "./" );
		}
	}
}
