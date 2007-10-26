// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "irrKlang" library.
// For conditions of distribution and use, see copyright notice in irrKlang.h

#ifndef __I_SOUND_STOP_EVENT_RECEIVER_H_INCLUDED__
#define __I_SOUND_STOP_EVENT_RECEIVER_H_INCLUDED__

#include "IUnknown.h"
#include "SAudioStreamFormat.h"

namespace irr
{
namespace audio
{


//! Interface to be implemented by the user, which recieves sound stop events.
/** The interface has only one method to be implemented by the user: OnSoundStopped().
Implement this interface and set it via ISound::setSoundStopEventReceiver().
The sound stop event is guaranteed to be called when a sound or sound stream is finished,
either because the sound reached its playback end, its sound source was removed,
ISoundEngine::stopAllSounds() has been called or the whole engine was deleted. */
class ISoundStopEventReceiver
{
public:
    
	//! destructor
	virtual ~ISoundStopEventReceiver() {};

	//! Called when a sound has stopped playing. 
	/** This is the only method to be implemented by the user.
	The sound stop event is guaranteed to be called when a sound or sound stream is finished,
	either because the sound reached its playback end, its sound source was removed,
	ISoundEngine::stopAllSounds() has been called or the whole engine was deleted.
	Please note: Sound events will occur in a different thread when the engine runs in
	multi threaded mode (default). In single threaded mode, the event will happen while
	the user thread is calling ISoundEngine::update().
	\param sound: Sound which has been stopped. 
	\param userData: userData pointer set by the user when registering the interface
	via ISound::setSoundStopEventReceiver(). */
	virtual void OnSoundStopped(ISound* sound, void* userData) = 0;

};


} // end namespace audio
} // end namespace irr

#endif

