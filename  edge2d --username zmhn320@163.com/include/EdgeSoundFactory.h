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
#ifndef EDGE_SOUNDFACTORY_H
#define EDGE_SOUNDFACTORY_H

#include "EdgeCompile.h"
#include <string>
#include <map>

using std::string;
using std::map;

namespace Edge
{
	class Sound;

	/**
	 * a sound factory will manages sounds. You should create/destroy a sound by the Soundfactory
	 *
	 */
	class EDGE_EXPORT SoundFactory
	{
	public:
		/**
		 * sound list
		 *
		 */
		typedef map<string, Sound*> SoundMap;

	public:
		/**
		 * Constructor
		 *
		 */
		SoundFactory();

		/**
		 * Destructor
		 *
		 */
		virtual ~SoundFactory();

		/**
		 * initiate the sound factory
		 *
		 */
		virtual bool initiate() = 0;

		/**
		 * release the sound factory, it will be called automatically by the destructor
		 *
		 */
		virtual void release() = 0;

		/**
		 * set all the sounds volume, This value is multiplied with all sounds played.
		 *
		 */
		virtual void setVolume( float volume ) = 0;

		/**
		 * get all the sound volume
		 *
		 */
		virtual float getVolume() = 0;

		/**
		 * stop all the sounds
		 *
		 */
		virtual void stop() = 0;

		/** 
		 * pause or unpause all the sounds
		 *
		 */
		virtual void pause( bool bPause = true ) = 0;

		/**
		 * create a sound, and later you should call destroy to destroy the sound
		 *
		 * @param file contains archive name and the sound file name, separated by ':" i.e : ./sound:a.wav
		 */
		virtual Sound *createSound( const string &file ) = 0;

		/**
		 * get a sound in the sound list
		 *
		 */
		virtual Sound *getSound( const string &name );

		/**
		 * destroy a sound
		 *
		 */
		virtual void destroySound( Sound *sound ) ;

		/**
		 * destroy a sound
		 *
		 */
		virtual void destroySound( const string &name ) ;

		/**
		 * destroy all sounds
		 *
		 */
		virtual void destroyAllSounds() ;

	protected:

		/**
		 * save sound in the sound list
		 *
		 */
		void _saveSound( Sound *sound ) ;

		/**
		 * selfGetSound, this class will use it to check whether the sound is already exist.
		 *
		 */
		Sound *_selfGetSound( const string &name );

		/**
		 * modify the name to the correct format
		 *
		 */
		void _modifyName( string &name );

	protected:
		/// sound list
		SoundMap mSounds;

	};
}

#endif