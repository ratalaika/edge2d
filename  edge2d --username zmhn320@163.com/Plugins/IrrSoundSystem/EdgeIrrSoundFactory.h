/**
 *
 *
 */
#ifndef EDGE_IRRSOUNDFACTORY_H
#define EDGE_IRRSOUNDFACTORY_H

#include "EdgeSoundFactory.h"
#include "EdgeSingleton.h"

namespace irr
{
	namespace audio
	{
		class ISoundEngine;
	}
}

namespace Edge
{
	/**
	 *
	 *
	 */
	class IrrSoundFactory : public Singleton<IrrSoundFactory>, public SoundFactory
	{
	public:
		/**
		 *
		 *
		 */
		IrrSoundFactory();

		/**
		 *
		 *
		 */
		~IrrSoundFactory();

		/**
		 * 
		 *
		 */
		bool initiate();

		/**
		 *
		 *
		 */
		void release();

		/**
		 *
		 */
		void setVolume( float volume );

		/**
		 *
		 */
		float getVolume();

		/**
		 *
		 */
		void stop();

		/**
		 *
		 */
		void pause( bool bPause = true );

		/**
		 *
		 */
		Sound *createSound( const string &file );

	private:
		irr::audio::ISoundEngine *mSoundEngine;
	};
}

#endif