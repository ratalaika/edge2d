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
#ifndef EDGE_INIPARSER_H
#define EDGE_INIPARSER_H

#include "EdgeDataStream.h"
#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;

namespace Edge
{
#define MAX_CHAR 256
	/**
	 * Ini file section structure
	 *
	 */
	struct IniSection
	{
		/// represents one line of a ini file, one line contain a key and a value
		typedef map<string, string> IniLines;
		/// section data
		IniLines mLines;
	};

	/**
	 * An ini file parser.
	 *
	 * @remarks It can read/write an ini file.One IniParser object represents one Ini file. 
	 * so you can create as many ini parser as you want .In the other hand, you can use one
	 * ini parser to parse many ini files, everytime you want to parser other ini file, you 
	 * should call create function.
	 * Both the key and the value can contain space character like : window caption = Edge Window
	 * @warning this class only support 256 characters in one line.
	 */
	class IniParser
	{
	public:
		/// ini sections
		typedef map<string, IniSection> IniSections;

	public:
		/**
		 * Constructor
		 *
		 */
		IniParser();

		/**
		 * Destructor
		 *
		 */
		~IniParser();

		/**
		 * create, load an ini file and parse it.
		 *
		 * @remarks, like many functions in Edge, a file param here should be this format :
		 * "archiveName : fileName", so that it can use IO of Edge to load the file from
		 * anywhere, including some compressed archive. 
		 * @warning make sure the archive is exist.
		 */
		void create( const string &file );

		/**
		 * load an ini file from file system directly.
		 * @remarks this function is very different from create function, it willnot use IO of Edge
		 * to load the file, instead it loads the file directly from a folder. 
		 * So the file param is a normal path name like : ./data/config.ini
		 */
		void createDirectly( const string &file );

		/**
		 * save the ini file.
		 *
		 * @remarks file param is a normal file path name like : ./config/config.ini
		 * the order of the section and the key-value maybe not the same as the original ini file.
		 * @warning this can only save the ini file on the file system, it canot save it
		 * in a compressed archive.
		 */
		void save( const string &file );

		/**
		 * release, called by destructor
		 *
		 */
		void release();

		/**
		 * read a string in a section
		 *
		 * @remarks it support this format : value = a test string with space, and this function 
		 * will return 'a test string with space',the key can be this format too.
		 */
		string readString( const string &section, const string &key, const string &defaultStr = "" );

		/**
		 * read an int value in a section
		 *
		 */
		int readInt( const string &section, const string &key, const int defaultInt = 0 );

		/**
		 * read a float value in a section
		 *
		 */
		float readFloat( const string &section, const string &key, const float defaultFloat = 0.0f );

		/**
		 * read a boolean value in a section
		 *
		 */
		bool readBool( const string &section, const string &key, const bool defaultBool = false );

		/**
		 * write a string in a section
		 *
		 * @remarks if the key or the section is not exist, it will create them
		 */
		void writeString( const string &section, const string &key, const string &value );

		/**
		 * write a int value to a section
		 *
		 * @remarks if the key or the section is not exist, it will create them
		 */
		void writeInt( const string &section, const string &key, const int value );

		/**
		 * write a Float value to a section
		 *
		 * @remarks if the key or the section is not exist, it will create them
		 */
		void writeFloat( const string &section, const string &key, const float value );

		/**
		 * write a boolean value in a section
		 *
		 */
		void writeBool( const string &section, const string &key, const bool value );

		/**
		 * get the ini name
		 *
		 */
		const string &getName() { return mName; }

	private:
		/**
		 * parse a ini file
		 *
		 */
		void _parse( DataStreamPtr &stream );

		/**
		 * modify the file name to the correct format
		 *
		 */
		void _modifyName( string &name );

	private:
		/// ini sections
		IniSections mSections;
		/// ini file name
		string mName;
	};
}

#endif