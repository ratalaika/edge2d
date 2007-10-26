// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "irrKlang" library.
// For conditions of distribution and use, see copyright notice in irrKlang.h

#ifndef __I_AUDIO_STREAM_LOADER_H_INCLUDED__
#define __I_AUDIO_STREAM_LOADER_H_INCLUDED__

#include "IUnknown.h"
#include "IReadFile.h"

namespace irr
{
namespace audio
{
	class IAudioStream;

//!	Class which is able to create an audio file stream from a file.
class IAudioStreamLoader : public IUnknown
{
public:

	//! destructor
	virtual ~IAudioStreamLoader() {};

	//! Returns true if the file maybe is able to be loaded by this class.
	/** This decision should be based only on the file extension (e.g. ".wav"). The given
	filename string is guaranteed to be lower case. */
	virtual bool isALoadableFileExtension(const c8* fileName) = 0;

	//! Creates an audio file input stream from a file
	/** \return Pointer to the created audio stream. Returns 0 if loading failed.
	If you no longer need the stream, you should call IAudioFileStream::drop().
	See IUnknown::drop() for more information. */
	virtual IAudioStream* createAudioStream(irr::io::IReadFile* file) = 0;
};


} // end namespace audio
} // end namespace irr

#endif

