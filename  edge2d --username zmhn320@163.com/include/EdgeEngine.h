/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
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
	 * The top most class, that will manage all the classes.
	 * 
	 * @remarks it's the most important class you should pay attention to.You can access
	 * most managers by it.
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
		 * You can use register* functions to register plugins.
		 * 
		 * @param windowCaption the render window's titile
		 * @param width the width of the window
		 * @param height the height of the window
		 * @param bWindowed whether the render mode is windowed
		 * @param bits the color depth 
		 */
		bool	initPlugin( const string &windowCaption, int width, int height,
			                bool bWindowed = true, int bits = 32 );

		/**
		 * reset the render window, including the Window and the Graphics.
		 *
		 * @remarks you can call this function to reset display mode anytime you want.
		 */
		void	resetWindow( int width, int height, bool bWindowed, int bits = 32 );

		/**
		 * release, called by destructor,so donot call this function yourself.
		 *
		 */
		void	release();

		/**
		 * shutdown the engine
		 * @remarks shutdown, you can call this function anytime to close window.
		 * In your game loop : you'll always judge whehter window->isClosed() to 
		 * determine the game is exit. When you call EdgeEngine::shutdown, 
		 * window->isClosed() will return true.
		 *
		 */
		void	shutdown();

		/**
		 * sleep
		 * sleep the main thread.
		 * @param millseconds how many millseconds you want to sleep.
		 *
		 */
		void	(*sleep)( unsigned long millseconds );

		/**
		 * register the graphics object. Called by Graphics plugin.
		 *
		 */
		void	registerGraphics( Graphics *graphics );

		/**
		 * register the image factory. Called by Graphics plugin.
		 *
		 */
		void	registerImageFactory( ImageFactory *fac );

		/**
		 * register the image codec. Called by ImageCodec plugin.
		 *
		 */
		void	registerImageCodec( ImageCodec *imageCodec );

		/**
		 * register the window. Called by System plugin.
		 *
		 */
		void	registerWindow( Window *window );

		/**
		 * register the timer. Called by System plugin.
		 *
		 */
		void	registerTimer( Timer *timer );

		/**
		 * register the FontFactory. Called by Font plugin.
		 *
		 */
		void	registerFontFactory( FontFactory *fac );

		/**
		 * register the InputManager. Called by Input plugin.
		 *
		 */
		void	registerInputManager( InputManager *im );

		/**
		 * register the sound factory. Called by Sound plugin.
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
