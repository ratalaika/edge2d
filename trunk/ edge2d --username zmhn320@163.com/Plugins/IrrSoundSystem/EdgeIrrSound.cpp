/**
 *
 *
 */
#include "EdgeIrrSound.h"
#include "EdgeException.h"
#include "IrrKlang/include/irrKlang.h"

namespace Edge
{
	IrrSound::IrrSound( const string &name, irr::audio::ISoundEngine *sndEngine ) :
		Sound( name )
	{
		mIrrSndEngine = sndEngine;

		mIrrSource = 0;
		mIrrSound = 0;
		mbCreated = false;
		mbPlaying = false;
	}

	IrrSound::~IrrSound()
	{
		release();
	}

	bool IrrSound::create()
	{
		/// create a sound source
		mIrrSource = mIrrSndEngine->addSoundSource( mName.c_str() );

		if( mIrrSource == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Create IrrKlang Sound Source FAILED",
				"IrrSound::create" );
		}
		mbCreated = true;

		return true;
	}

	void IrrSound::release()
	{
		mIrrSndEngine->removeSoundSource( mIrrSource );

		if( mIrrSound != 0 )
		{
			mIrrSound->stop();
			mIrrSound->drop();
		}
	}
	
	void IrrSound::play( bool bLoop, float volume, float pan, float pitch )
	{
		if( !mbCreated )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "The Sound isnot created",
				"IrrSound::play" );
		}

		mIrrSound = mIrrSndEngine->play2D( mIrrSource, bLoop, true, true );
		mIrrSound->setIsPaused( false ); /// if no this code, it will not play the sound.
		mIrrSound->setVolume( volume );
		mIrrSound->setPan( pan );
		mIrrSound->setPlaybackSpeed( pitch );

		mbPlaying = true;
	}

	void IrrSound::pause( bool bPause )
	{
		if( mbPlaying )
		{
			mIrrSound->setIsPaused( bPause );
		}
	}

	void IrrSound::stop()
	{
		if( mbPlaying )
		{
			mIrrSound->stop();
			mIrrSound->drop();
			mbPlaying = false;
			mIrrSound = 0;
		}
	}

	void IrrSound::setPan( float pan )
	{
		if( mbPlaying )
		{
			mIrrSound->setPan( pan );
		}
	}

	void IrrSound::setPitch( float pitch )
	{
		if( mbPlaying )
		{
			mIrrSound->setPlaybackSpeed( pitch );
		}
	}

	void IrrSound::setVolume( float volume )
	{
		if( mbPlaying )
		{
			mIrrSound->setVolume( volume );
		}
	}
	
	float IrrSound::getPan()
	{
		if( mbPlaying )
		{
			return mIrrSound->getPan();
		}
		return 0.0f;
	}

	float IrrSound::getVolume()
	{
		if( mbPlaying )
		{
			return mIrrSound->getVolume();
		}

		return 0.0f;
	}
	
	float IrrSound::getPitch()
	{
		if( mbPlaying )
		{
			return mIrrSound->getPlaybackSpeed();
		}

		return 0.0f;
	}

	bool IrrSound::isFinished()
	{
		if( mbPlaying )
		{
			return mIrrSound->isFinished();
		}
		return true;
	}

	bool IrrSound::isLooped()
	{
		if( mbPlaying )
		{
			return mIrrSound->isLooped();
		}
		return false;
	}

	bool IrrSound::isPaused()
	{
		if( mbPlaying )
		{
			return mIrrSound->getIsPaused();
		}

		return true;
	}
	
}