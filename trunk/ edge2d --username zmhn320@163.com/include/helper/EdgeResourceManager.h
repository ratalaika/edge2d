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
#ifndef EDGE_RESOURCEMANAGER_H
#define EDGE_RESOURCEMANAGER_H

#include "EdgeSingleton.h"

#include <string>
#include <map>

using std::string;
using std::map;

class TiXmlElement;
class TiXmlDocument;

namespace Edge
{
	class Image;
	class Sound;
	class AnimSprite;
	class Font;

	/**
	 * ResourceManager will manage resources for you. Currently supported resources include :
	 * images, sounds, animations
	 *
	 */
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		/**
		 * ResourceGroup
		 *
		 *
		 */
		typedef map<string, TiXmlElement*> ResGroup;
		
		/**
		 * resource name list
		 *
		 */
		typedef map<string, string> NameList;

	public:
		/**
		 * Constructor
		 *
		 */
		ResourceManager();

		/**
		 * Destructor
		 *
		 */
		~ResourceManager();

		/**
		 * initiate , it will load a xml resource config file.
		 *
		 */
		bool initiate( const string &file );

		/**
		 * release, called by destructor
		 *
		 */
		void release();

		/**
		 * load a group of resource
		 *
		 */
		bool loadGroup( const string &id );

		/**
		 * load all groups 
		 *
		 */
		bool loadAllGroups();

		/**
		 * get an image
		 *
		 */
		Image *getImage( const string &id );

		/**
		 * get a sound
		 *
		 */
		Sound *getSound( const string &id );

		/**
		 * get an animation
		 *
		 */
		AnimSprite *getAnim( const string &id );

		/**
		 * get a font
		 *
		 */
		Font *getFont( const string &id );

	protected:
		/// resource group
		ResGroup mResGroup;
		/// image name list
		NameList mImgNameList;
		/// sound name list
		NameList mSndNameList;
		/// animation name list
		NameList mAnimNameList;
		/// Font name list
		NameList mFontNameList;
		/// TinyXml document
		TiXmlDocument *mXmlDoc;
	};
}

#endif