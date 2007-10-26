/**
 *
 *
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
		virtual void destroySound( const string &name ) ;

		/**
		 * destroy all sounds
		 *
		 */
		virtual void destroyAllSounds() ;

		/**
		 * save sound in the sound list
		 *
		 */
		virtual void saveSound( Sound *sound ) ;

	protected:
		void modifyName( string &name );
	protected:
		/// sound list
		SoundMap mSounds;

	};
}

#endif