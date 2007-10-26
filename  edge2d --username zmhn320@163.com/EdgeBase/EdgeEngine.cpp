/**
 *
 *
 *
 */
#include "EdgeEngine.h"
#include "EdgeLogManager.h"
#include "EdgeArchive.h"
#include "EdgeArchiveManager.h"
#include "EdgeImageCodec.h"
#include "EdgeGraphics.h"
#include "EdgeWindow.h"
#include "EdgeTimer.h"
#include "EdgeFont.h"
#include "EdgeImageFactory.h"
#include "EdgePluginManager.h"
#include "EdgeInputManager.h"
#include "EdgeSoundFactory.h"
#include "EdgeException.h"
#include <sstream>

namespace Edge
{
	EdgeEngine::EdgeEngine()
	{
		mGraphics = 0;
		mImageFactory = 0;
		mImageCodec = 0;
		mArchiveManager = 0;
		mLogManager = 0;
		mBasicArchiveFactory = 0;
		mPluginManager = 0;
		mFontFactory  = 0;
		mWindow = 0;
		mTimer = 0;
		mInputManager = 0;
		mSoundFactory = 0;
		sleep = 0;
	}

	EdgeEngine::~EdgeEngine()
	{
		release();
	}

	const string EdgeEngine::getVersionDesc()
	{
		std::ostringstream stream;
		stream << "Edge " << EDGE_VERSION_MAJOR << "." <<
			EDGE_VERSION_MINOR << "." << EDGE_VERSION_PATCH ;

		return stream.str();
	}

	bool	EdgeEngine::initBase( const string &logFile )
	{
		/// initiate some basic components.

		mLogManager = new LogManager();
		/// create a default logger
		mLogManager->createLog( "DefaultLog", logFile );
		mLogManager->logMessage( LL_INFORMATION, getVersionDesc() );
		mLogManager->logMessage( LL_INFORMATION, "Edge Engine Initiate" );

		mArchiveManager = new ArchiveManager();
		/// create the basic Archive Factory 
		mBasicArchiveFactory = new FileSystemArchiveFactory();
		mArchiveManager->addArchiveFactory( mBasicArchiveFactory );
		
		/// create the plugin manager
		mPluginManager = new PluginManager();
		mPluginManager->initiate();

		return true;
	}

	bool	EdgeEngine::initPlugin( const string &windowCaption, int width, int height, 
		                            bool bWindowed, int bits )
	{
		/// check whether all the necessary plugins is registered
		if( mWindow == 0 || mGraphics == 0 || mImageCodec == 0 ||
			mFontFactory == 0 || mImageFactory == 0 || mInputManager == 0 ||
			mTimer == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Not all the necessary plugins is registered!",
				"EdgeEngine::initPlugin" );
		}

		/// initiate all the plugins
				
		/// window plugin
		mWindow->create( windowCaption, width, height, bWindowed );
		/// timer
		mTimer->initiate();
		/// graphics plugin
		mWindow->addListener( mGraphics );
		mGraphics->initiate( mWindow, width, height, bWindowed, bits );
		/// image codec plugins
		mImageCodec->initiate();
		/// font factory
		mFontFactory->initiate();
		/// image factory plugins
		mImageFactory->initiate();
		/// input manager
		mInputManager->initiate( mWindow );
		/// sound factory
		if( mSoundFactory != 0 )
		{
			mSoundFactory->initiate();
		}

		return true;
	}
	
	void	EdgeEngine::resetWindow( int width, int height, bool bWindowed, int bits )
	{
		mWindow->reset( width, height, bWindowed );
		mGraphics->reset( width, height, bWindowed, bits );	
		mInputManager->reset( width, height );
	}

	void	EdgeEngine::release()
	{
		delete mArchiveManager;
		delete mBasicArchiveFactory;
		delete mPluginManager;

		mLogManager->logMessage( LL_INFORMATION, "Edge Engine Shutdown!" );	
		delete mLogManager;
	}
	
	void	EdgeEngine::shutdown()
	{
		mWindow->close();
	}

	void	EdgeEngine::registerGraphics( Graphics *graphics )
	{
		mGraphics = graphics;
	}

	void	EdgeEngine::registerImageFactory( ImageFactory *fac )
	{
		mImageFactory = fac;
	}

	void	EdgeEngine::registerImageCodec( ImageCodec *imageCodec )
	{
		mImageCodec = imageCodec;
	}

	void	EdgeEngine::registerWindow( Window *window )
	{
		mWindow = window;
	}
	
	void	EdgeEngine::registerTimer( Timer *timer )
	{
		mTimer = timer;
	}

	void	EdgeEngine::registerFontFactory( FontFactory *fac )
	{
		mFontFactory = fac;
	}

	void	EdgeEngine::registerInputManager( InputManager *im )
	{
		mInputManager = im;
	}

	void	EdgeEngine::registerSoundFactory( SoundFactory *sf )
	{
		mSoundFactory = sf;
	}

	Graphics *EdgeEngine::getGraphics()
	{
#ifdef EDGE_DEBUG
		if( mGraphics == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "Graphics plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mGraphics;
	}

	ImageFactory *EdgeEngine::getImageFactory()
	{
#ifdef EDGE_DEBUG
		if( mImageFactory == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "ImageFacotory plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mImageFactory;
	}

	ImageCodec *EdgeEngine::getImageCodec()
	{
#ifdef EDGE_DEBUG
		if( mImageCodec == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "ImageCodec plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mImageCodec;
	}

	ArchiveManager *EdgeEngine::getArchiveManager()
	{
		return mArchiveManager;
	}

	LogManager *EdgeEngine::getLogManager()
	{
		return mLogManager;
	}

	Window *EdgeEngine::getWindow()
	{
#ifdef EDGE_DEBUG
		if( mWindow == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "Window plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mWindow;
	}

	Timer *EdgeEngine::getTimer()
	{
#ifdef EDGE_DEBUG
		if( mTimer == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "Timer(system plugin) is not exist. Unload or Not Registered ?" );
		}
#endif
		return mTimer;
	}

	FontFactory *EdgeEngine::getFontFactory()
	{
#ifdef EDGE_DEBUG
		if( mFontFactory == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "FontFacotory plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mFontFactory;
	}

	PluginManager *EdgeEngine::getPluginManager()
	{
		return mPluginManager;
	}

	InputManager *EdgeEngine::getInputManager()
	{
#ifdef EDGE_DEBUG
		if( mInputManager == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "Input plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mInputManager;
	}

	SoundFactory *EdgeEngine::getSoundFactory()
	{
#ifdef EDGE_DEBUG
		if( mSoundFactory == 0 )
		{
			mLogManager->logMessage( LL_WARNING, "Sound plugin is not exist. Unload or Not Registered ?" );
		}
#endif
		return mSoundFactory;
	}
}