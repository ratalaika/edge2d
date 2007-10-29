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
#ifndef EDGE_ANIMSPRITE_FACTORY_H
#define EDGE_ANIMSPRITE_FACTORY_H

#include "EdgeSingleton.h"
#include <map>
#include <string>

using std::map;
using std::string;

namespace Edge
{
	class AnimSprite;

	/**
	 * AnimSpriteFactory, you can use this factory to create an anim-sprite for you.
	 *
	 */
	class AnimSpriteFactory : public Singleton<AnimSpriteFactory>
	{
	public:
		/**
		 * AnimSprite list
		 *
		 */
		typedef map<string, AnimSprite*> AnimSpriteList;

	public:
		/**
		 * Constructor
		 *
		 */
		AnimSpriteFactory();

		/**
		 * Destructor
		 *
		 */
		~AnimSpriteFactory();

		/**
		 * create an AnimSprite, it the sprite is already exist, return it.
		 *
		 */
		AnimSprite *createAnimSprite( const string &file );

		/**
		 * get an AnimSprite
		 *
		 */
		AnimSprite *getAnimSprite( const string &name );

		/**
		 * destroy an AnimSprite
		 *
		 */
		void destroyAnimSprite( const string &name );
		void destroyAnimSprite( AnimSprite *sprite );

		/**
		 * destroyAllSprite, called by destructor
		 *
		 */
		void destroyAllAnimSprites();

	private:
		void modifyName( string &name );
	private:
		/// anim sprite list
		AnimSpriteList mAnimSprites;
	};
}

#endif