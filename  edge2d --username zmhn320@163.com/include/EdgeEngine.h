/**
 *
 *
 *
 */
#ifndef EDGE_ENGINE_H
#define EDGE_ENGINE_H

#include "EdgeCompile.h"
#include "EdgeSingleton.h"
#include <string>

using std::string;

namespace Edge
{
	class Graphics;
	class ImageFactory;
	class LogManager;
	class ArchiveManager;
	class ImageCodec;
	class ArchiveFactory;
	class Window;
	class FontFactory;
	class PluginManager;
	class InputManager;
	class SoundFactory;
	class Timer;

	/**
	 * The top most class, that will manage all the classes
	 *
	 */
	class EDGE_EXPORT EdgeEngine : public Singleton<EdgeEngine>
	{
	public:
		/**
		 * Constructor
		 *
		 */
		EdgeEngine();

		/**
		 * Destructor
		 *
		 */
		~EdgeEngine();

		/**
		 * get edge engins's version description, if you want to get the number, just use EDGE_VERSION
		 *
		 */
		static const string getVersionDesc();

		/**
		 * initBase, initiate all the basic objects that will be used by engine
		 *
		 */
		bool	initBase( const string &logFile = "Edge.log" );

		/**
		 * initPlugin
		 * Before you initiate, you must register all kinds of necessary plugins.
		 * You can use register* functions to registers plugins.
		 */
		bool	initPlugin( const string &windowCaption, int width, int height,
			                bool bWindowed = true, int bits = 32 );

		/**
		 * reset the render window, including the Window and the Graphics.
		 *
		 */
		void	resetWindow( int width, int height, bool bWindowed, int bits = 32 );

		/**
		 * release
		 *
		 */
		void	release();

		/**
		 * shutdown, you can call this function anytime to close window.
		 * In your game loop : you'll always judge whehter window->isClosed() to 
		 * determine the game is exit. When you call EdgeEngine::shutdown, 
		 * window->isClosed() will return true.
		 *
		 */
		void	shutdown();

		/**
		 * sleep
		 *
		 */
		void	(*sleep)( unsigned long millseconds );

		/**
		 * register the graphics object
		 *
		 */
		void	registerGraphics( Graphics *graphics );

		/**
		 * register the image factory
		 *
		 */
		void	registerImageFactory( ImageFactory *fac );

		/**
		 * register the image codec
		 *
		 */
		void	registerImageCodec( ImageCodec *imageCodec );

		/**
		 * register the window
		 *
		 */
		void	registerWindow( Window *window );

		/**
		 * register the timer
		 *
		 */
		void	registerTimer( Timer *timer );

		/**
		 * register the FontFactory
		 *
		 */
		void	registerFontFactory( FontFactory *fac );

		/**
		 * register the InputManager
		 *
		 */
		void	registerInputManager( InputManager *im );

		/**
		 * register the sound factory
		 *
		 */
		void	registerSoundFactory( SoundFactory *sf );

		/**
		 * get
		 *
		 */
		Graphics		*getGraphics() ;
		ImageFactory	*getImageFactory();
		ImageCodec		*getImageCodec();
		ArchiveManager	*getArchiveManager();
		LogManager		*getLogManager();
		Window			*getWindow();
		Timer			*getTimer();
		FontFactory		*getFontFactory();
		PluginManager   *getPluginManager();
		InputManager    *getInputManager();
		SoundFactory    *getSoundFactory();

	private:
		/// archive manager
		ArchiveManager *mArchiveManager;
		/// log manager
		LogManager *mLogManager;
		/// plugin manager
		PluginManager *mPluginManager;
		/// Graphics
		Graphics *mGraphics;
		/// image factory
		ImageFactory *mImageFactory;
		/// image codec
		ImageCodec *mImageCodec;
		/// window
		Window *mWindow;
		/// timer
		Timer *mTimer;
		/// font factory
		FontFactory *mFontFactory;
		/// input manager
		InputManager *mInputManager;
		/// sound factory
		SoundFactory *mSoundFactory;

		/// the basic ArchiveFactory
		ArchiveFactory *mBasicArchiveFactory;
	};
}

#endif