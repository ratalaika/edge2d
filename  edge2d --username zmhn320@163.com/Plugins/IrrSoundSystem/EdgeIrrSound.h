/**
 *
 *
 */
#ifndef EDGE_IRRSOUND_H
#define EDGE_IRRSOUND_H

#include "EdgeSound.h"

namespace irr
{
	namespace audio
	{
		class ISoundSource;
		class ISound;
		class ISoundEngine;
	}
}

namespace Edge
{
	/**
	 *
	 *
	 */
	class IrrSound : public Sound
	{
	public:
		/**
		 *
		 *
		 */
		IrrSound( const string &name, irr::audio::ISoundEngine *sndEngine );

		/**
		 *
		 *
		 */
		~IrrSound();

		/**
		 *
		 */
		bool create();

		/**
		 *
		 */
		void release();

		/**
		 *
		 */
		void play( bool bLoop = false, float volume = 1.0f, float pan = 0.0f, float pitch = 1.0f );

		/**
		 * pause or unpause the sound
		 * 
		 */
		void pause( bool bPause = true ) ;

		/**
		 * stop the sound, it will free some resources
		 *
		 */
		void stop();

		/**
		 * set pan , when the sound is playing, it will work. Otherwise, this function will do nothing
		 *
		 */
		void setPan( float pan );

		/**
		 * set pitch
		 *
		 */
		void setPitch( float pitch );

		/**
		 * set volume
		 *
		 */
		void setVolume( float volume ) ;

		/**	
		 * get pan
		 *
		 */
		float getPan();

		/**
		 * get pitch
		 *
		 */
		float getPitch();

		/**
		 * get volume
		 *
		 */
		float getVolume();

		/**
		 * isFinished, when the sound is not looped, the sound will finish at last.
		 *
		 */
		bool isFinished();

		/**
		 * isLooped, return true if the sound is looped
		 *
		 */
		bool isLooped();

		/**
		 *
		 *
		 */
		bool isPaused();
		
	private:
		/// sound source
		irr::audio::ISoundSource *mIrrSource;
		/// sound playing instance
		irr::audio::ISound *mIrrSound;
		/// irrklang sound engine
		irr::audio::ISoundEngine *mIrrSndEngine;
		/// whether the sound is created
		bool mbCreated ;
		/// whether the sound is playing
		bool mbPlaying;

	};
}

#endif
