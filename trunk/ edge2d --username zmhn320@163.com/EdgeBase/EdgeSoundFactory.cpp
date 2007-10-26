/**
 *
 *
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
		modifyName( cname );

		SoundMap::iterator it = mSounds.find( cname );
		if( it == mSounds.end() )
		{
			return NULL;
		}

		return it->second;
	}

	void SoundFactory::destroySound( const string &name )
	{
		string cname( name );
		modifyName( cname );

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

	void SoundFactory::saveSound( Sound *sound )
	{
		mSounds[ sound->getName() ] = sound;
	}

	void SoundFactory::modifyName( string &name )
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