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
#include "EdgeSound.h"
#include "EdgeSoundFactory.h"
#include "EdgeException.h"

namespace Edge
{
	SoundFactory::SoundFactory()
	{
	}

	SoundFactory::~SoundFactory()
	{
	}

	Sound *SoundFactory::getSound( const string &name )
	{
		string cname( name );
		_modifyName( cname );

		Sound *sound = _selfGetSound( cname );
		if( sound == NULL )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Cannot find the sound : " + name,
				"SoundFactory::getSound" );
		}
		return sound;
	}

	void SoundFactory::destroySound( const string &name )
	{
		string cname( name );
		_modifyName( cname );

		SoundMap::iterator it = mSounds.find( cname );
		if( it != mSounds.end() )
		{
			delete it->second;
			mSounds.erase( it );
		}
	}

	void SoundFactory::destroySound( Sound *sound )
	{
		destroySound( sound->getName() );
	}

	void SoundFactory::destroyAllSounds()
	{
		for( SoundMap::iterator it = mSounds.begin(); it != mSounds.end(); ++ it )
		{
			delete it->second;
		}

		mSounds.clear();
	}

	/////////////////////////////////////////////////////////////////////////
	Sound *SoundFactory::_selfGetSound( const string &name )
	{
		SoundMap::iterator it = mSounds.find( name );
		if( it == mSounds.end() )
		{
			return NULL;
		}

		return it->second;
	}

	void SoundFactory::_modifyName( string &name )
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
	void SoundFactory::_saveSound( Sound *sound )
	{
		mSounds[ sound->getName() ] = sound;
	}

}