/* irrKlang.h -- interface of the 'irrKlang' library

  Copyright (C) 2002-2007 Nikolaus Gebhardt

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
*/

#ifndef __IRR_KLANG_H_INCLUDED__
#define __IRR_KLANG_H_INCLUDED__

#include "irrTypes.h"
#include "IUnknown.h"
#include "IVirtualUnknown.h"

#include "ESoundOutputDrivers.h"
#include "ESoundEngineOptions.h"
#include "EStreamModes.h"
#include "SAudioStreamFormat.h"
#include "ISoundEngine.h"
#include "ISoundSource.h"
#include "ISound.h"
#include "IAudioStream.h"
#include "IAudioStreamLoader.h"
#include "ISoundEffectControl.h"
#include "ISoundStopEventReceiver.h"
#include "IFileFactory.h"
#include "IReadFile.h"

//! irrKlang Version
#define IRR_KLANG_VERSION "1.0-RC1"

/*! \mainpage irrKlang 1.0-RC1 API documentation
 *
 * <div align="center"><img src="logobig.png" ></div>
 *
 * \section intro Introduction
 *
 * Welcome to the irrKlang API documentation. irrKlang is a high level 2D and 3D 
 * cross platform sound engine and audio library which plays several file formats like
 * <ul>
 *               <li>RIFF WAVE (*.wav)</li>
 *               <li>Ogg Vorbis (*.ogg)</li>
 *               <li>MPEG-1 Audio Layer 3 (*.mp3)</li>
 *               <li>Amiga Modules (*.mod)</li>
 *               <li>Impulse Tracker (*.it)</li>
 *               <li>Scream Tracker 3 (*.s3d)</li>
 *               <li>Fast Tracker 2 (*.xm)</li>        
 * </ul>
 * It is also able to run on different operating systems and use several output drivers:
 *<ul>
 *       <li>Windows 98, ME, NT 4, 2000, XP, Vista</li>
 *	<ul>
 *       <li>DirectSound</li>
 *       <li>WinMM</li>
 *	</ul>
 *       <li>Linux / *nix</li>	
 *	<ul>
 *       <li>ALSA</li>
 *	</ul>
 *  <li>MacOSX (x86)</li>
 *	<ul>
 *       <li>CoreAudio</li>
 *	</ul>
 * </ul>
 * On these pages you'll find any information you'll need to develop applications with
 * irrKlang using C++. If you are looking for a tutorial on how to start, you'll
 * find some on the homepage of irrKlang at
 * <A HREF="http://www.ambiera.com/irrklang" >http://www.ambiera.com/irrklang</A>
 * or inside the SDK in the directory \examples.
 *
 * The irrKlang library is intended to be an easy-to-use 3d and 2d sound engine, so
 * this documentation is an important part of it. If you have any questions or
 * suggestions, just send a email to the author of the engine, Nikolaus Gebhardt
 * (niko (at) irrlicht3d.org).
 *
 *
 * \section links Links
 *
 * <A HREF="namespaces.html">Namespaces</A>: A very good place to start reading
 * the documentation.<BR>
 * <A HREF="annotated.html">Class list</A>: List of all classes with descriptions.<BR>
 * <A HREF="functions.html">Class members</A>: Good place to find forgotten features.<BR>
 *
 *
 * <br/>
 * \section irrexample Quick Start Example
 *
 * To simply start the engine and play a mp3 file, use code like this:
 * \code
 * #include <iostream>
 * #include <irrKlang.h>
 * #pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
 *
 * int main(int argc, const char** argv)
 * {
 *	irr::audio::ISoundEngine* engine = irr::audio::createIrrKlangDevice();
 *	if (!engine) return 1; // could not start engine
 *
 *	engine->play2D("someMusic.mp3", true); // play some mp3 file, looped
 * 
 *	std::cin.get(); // wait until user presses a key
 * 
 *	engine->drop(); // delete engine
 *	return 0;
 * } 
 * \endcode
 *
 * A mp3 file is being played until the user presses enter in this example. 
 * As you can see, irrKlang uses <A HREF="namespaces.html">namespaces</A>, most of
 * the useful classes are located in the namespace irr::audio. If you don't want to write 
 * this in front of every class and function you are using, simply write 
 * \code
 * using namespace irr;
 * using namespace audio;
 * \endcode
 * in front of your code, as also shown in the next example.
 * <br/>
 * <br/>
 *
 *
 * \section irrexample2 A More Interactive Example
 *
 * The following is a simple interactive application, starting up the sound engine and 
 * playing some streaming .ogg music file and a .wav sound effect every time the user
 * presses a key.
 *
 * \code
 * #include <iostream>
 * #include <irrKlang.h>
 * using namespace irr;
 * using namespace audio;
 *
 * #pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
 *
 *
 * int main(int argc, const char** argv)
 * {
 * 	// start the sound engine with default parameters
 * 	ISoundEngine* engine = createIrrKlangDevice();
 *
 * 	if (!engine)
 * 		return 0; // error starting up the engine
 *
 * 	// play some sound stream, looped
 * 	engine->play2D("../../media/helltroopers.ogg", true);
 *
 * 	std::cout << "\nHello World!\n";
 *
 * 	char i = 0;
 *
 * 	while(i != 'q')
 * 	{
 * 		std::cout << "Press any key to play some sound, press 'q' to quit.\n";
 *
 * 		// play a single sound
 * 		engine->play2D("../../media/bell.wav");
 *
 * 		std::cin >> i; // wait for user to press some key
 * 	}
 *
 * 	engine->drop(); // delete engine
 * 	return 0;
 * }
 *
 * \endcode
 */

#if defined(IRRKLANG_STATIC)
    #define IRRKLANG_API
#else
    #if (defined(WIN32) || defined(WIN64) || defined(_MSC_VER))
        #ifdef IRRKLANG_EXPORTS
        #define IRRKLANG_API __declspec(dllexport)
        #else
        #define IRRKLANG_API __declspec(dllimport)
        #endif // IRRKLANG_EXPORT
    #else
        #define IRRKLANG_API __attribute__((visibility("default")))
    #endif // defined(WIN32) || defined(WIN64)
#endif // IRRKLANG_STATIC

#if defined(_STDCALL_SUPPORTED)
#define IRRKLANGCALLCONV __stdcall  // Declare the calling convention.
#else
#define IRRKLANGCALLCONV
#endif // STDCALL_SUPPORTED

//! Everything in the irrKlang Sound Engine can be found in this namespace.
namespace irr
{
namespace audio
{
	//! Creates an irrKlang device. The irrKlang device is the root object for using the sound engine.
	/** \param driver The sound output driver to be used for sound output. Use audio::ESOD_AUTO_DETECT
	to let irrKlang decide which driver will be best.
	\param options A combination of audio::E_SOUND_ENGINE_OPTIONS literals. Default value is ESEO_DEFAULT_OPTIONS.
	\param sdk_version_do_not_use Don't use or change this parameter. Always set it to
	IRRKLANG_SDK_VERSION, which is done by default. This is needed for sdk version checks.
	\return Returns pointer to the created irrKlang device or null if the
	device could not be created. If you don't need the device, use ISoundEngine::drop() to
	delete it. See IUnknown::drop() for details.
	*/
	IRRKLANG_API ISoundEngine* IRRKLANGCALLCONV createIrrKlangDevice(
		E_SOUND_OUTPUT_DRIVER driver = ESOD_AUTO_DETECT,
		int options = audio::ESEO_DEFAULT_OPTIONS,
		const char* sdk_version_do_not_use = IRR_KLANG_VERSION);
}

	// THE FOLLOWING IS AN EMPTY LIST OF ALL SUB NAMESPACES
	// EXISTING ONLY FOR THE DOCUMENTATION SOFTWARE DOXYGEN.

	//! This namespace contains basic classes like vectors and helper functions and constants
	namespace core
	{
	}

	//! This namespace provides interfaces for input/output: Reading and writing files.
	namespace io
	{
	}

	//! Most of the irrKlang classes for sound output can be found here.
	namespace audio
	{
	}
}


/*! \file irrKlang.h
    \brief Main header file of the irrKlang sound library, the only file needed to include.
*/

#endif

