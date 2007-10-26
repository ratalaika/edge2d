// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "irrKlang" library.
// For conditions of distribution and use, see copyright notice in irrKlang.h

#ifndef __I_AUDIO_STREAM_H_INCLUDED__
#define __I_AUDIO_STREAM_H_INCLUDED__

#include "IUnknown.h"
#include "SAudioStreamFormat.h"

namespace irr
{
namespace audio
{


//!	Reads and decodes audio data into an usable audio stream for the ISoundEngine
class IAudioStream : public IUnknown
{
public:

	//! destructor
	virtual ~IAudioStream() {};

	//! returns format of the audio stream
	virtual SAudioStreamFormat getFormat() = 0;

	//! sets the position of the audio stream.
	/** For example to let the stream be read from the beginning of the file again, 
	setPosition(0) would be called. This is usually done be the sound engine to
	loop a stream after if has reached the end. Return true if sucessful and 0 if not. */
	virtual bool setPosition(s32 pos) = 0;

    //! tells the audio stream to read frameCountToRead audio frames into the specified buffer
	/** \param target: Target data buffer to the method will write the read frames into. The
	specified buffer will be at least getFormat().getFrameSize()*frameCountToRead bytes big.
	\param frameCountToRead: amount of frames to be read.
	\returns Returns amount of frames really read. Should be frameCountToRead in most cases. */
	virtual s32 readFrames(void* target, s32 frameCountToRead) = 0;
};


} // end namespace audio
} // end namespace irr

#endif

