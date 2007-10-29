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
#ifndef EDGE_SOUND_H
#define EDGE_SOUND_H

#include "EdgeCompile.h"
#include "EdgeDataStream.h"
#include <string>

using std::string;

namespace Edge
{
	/**
	 * A sound object represents a sound or a music. It's created by the SoundFactory.
	 *
	 */
	class EDGE_EXPORT Sound
	{
	public:
		/**
		 * 
		 *
		 */
		Sound( const string &name ) : mName( name ) {}

		/**
		 *
		 *
		 */
		virtual ~Sound() {}
	
		/**
		 * create a sound
		 *
		 */
		virtual bool create() = 0;

		/**
		 * release the sound, it will be called automatically by the destructor
		 *
		 */
		virtual void release() = 0;

		/**
		 * play the sound
		 *
		 * @param bLoop if true, the sound is looped until call stop
		 * @param volume sound volume, 1.0f means maximum volume, 0.0f means minmum volume
		 * @param pan -1.0f to 1.01f, 0.0f means center
		 * @param pitch playback speed, default is 1.0f
		 */
		virtual void play( bool bLoop = false, float volume = 1.0f, float pan = 0.0f, float pitch = 1.0f ) = 0;

		/**
		 * pause or unpause the sound
		 * 
		 */
		virtual void pause( bool bPaused = true ) = 0;

		/**
		 * stop the sound, it will free some resources
		 *
		 */
		virtual void stop() = 0;

		/**
		 * set pan , when the sound is playing, it will work. Otherwise, this function will do nothing
		 *
		 */
		virtual void setPan( float pan ) = 0;

		/**
		 * set pitch
		 *
		 */
		virtual void setPitch( float pitch ) = 0;

		/**
		 * set volume
		 *
		 */
		virtual void setVolume( float volume ) = 0;

		/**	
		 * get pan
		 *
		 */
		virtual float getPan() = 0;

		/**
		 * get pitch
		 *
		 */
		virtual float getPitch() = 0;

		/**
		 * get volume
		 *
		 */
		virtual float getVolume() = 0;

		/**
		 * isFinished, when the sound is not looped, the sound will finish at last.
		 *
		 */
		virtual bool isFinished() = 0;

		/**
		 * isLooped, return true if the sound is looped
		 *
		 */
		virtual bool isLooped() = 0;

		/**
		 * return true if the sound is paused
		 *
		 */
		virtual bool isPaused() = 0;

		/**
		 * getName
		 *
		 */
		string &getName() { return mName; }

	protected:
		/// the sound name
		string mName;
	};
}

#endif