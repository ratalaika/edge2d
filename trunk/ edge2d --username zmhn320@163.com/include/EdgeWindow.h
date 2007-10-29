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
#ifndef EDGE_WINDOW_H
#define EDGE_WINDOW_H

#include "EdgeCompile.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Edge
{
	class Window;

	/**
	 * WindowListener, you can inherient from this class and add to the Window, 
	 * And later if there are some window messages like OnGetFocus, you will get it.
	 *
	 */
	class EDGE_EXPORT WindowListener
	{
	public:
		/**
		 * Constructor
		 *
		 */
		WindowListener() : mWindow( 0 ) { }

		/**
		 * Destructor
		 *
		 */
		virtual ~WindowListener() {}
		
		/**
		 * called before the Window is created
		 * 
		 */
		virtual void	OnInit() {}

		/** 
		 * called before the Window is destroyed
		 *
		 */
		virtual void	OnDestroy() {}

		/** 
		 * called when this listener is added into the Window
		 *
		 */
		virtual void	OnAdd( Window *window ) { mWindow = window; }

		/**
		 * called when the window lost focus
		 *
		 */
		virtual void	OnLostFocus() {}

		/**
		 * called when the window got focus
		 *
		 */
		virtual void	OnGetFocus() {}

		/// TODO : other messages

	protected:
		/// Window
		Window *mWindow;
	};

	/** 
	 * Graphics module will render everything on the Window
	 *
	 *
	 */
	class EDGE_EXPORT Window
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Window();

		/**
		 * Destructor
		 *
		 */
		virtual ~Window();

		/**
		 * create, it will call all the listeners' OnInit first.
		 *
		 */
		virtual bool create( const string &caption, int width, int height, bool bWindowed ) = 0;

		/**
		 * destroy the window, called by destructor
		 *
		 */
		virtual void destroy() = 0;

		/**
		 * reset, reset the window's size and it's style. Donot call this function, the function
		 * is called by EdgeEngine.
		 *
		 */
		virtual void reset( int width, int height, bool bWindowed ) {}

		/**
		 * show, show the window
		 *
		 */
		virtual void show() = 0;

		/**
		 * close the window
		 *
		 */
		virtual void close() {}

		/**
		 * show mouse cursor
		 *
		 */
		virtual void showMouse( bool bShow ) {}

		/**
		 * handleMessage
		 *
		 */
		virtual void handleMessage() = 0;

		/**
		 * addListener,add a window listener so that you can be notified by the window's messages.
		 *
		 */
		virtual void addListener( WindowListener *listener );

		/**
		 * removeListener, it only remove the listener from the listener list, so it's your job
		 * to delete it. You new it and you delete it.
		 *
		 */
		virtual void removeListener( WindowListener *listener );

		/**
		 * setWidth
		 *  
		 * @remarks set the window's width, you must call this function before create,otherwise it willnot
		 * make any effect.
		 */
		void setWidth( int width )   { mWidth = width; }
		
		/**
		 * setHeight
		 *  
		 * @remarks set the window's height, you must call this function before create,otherwise it willnot
		 * make any effect.
		 */
		void setHeight( int height ) { mHeight = height; }

		/**
		 * setCaption
		 *
		 * @remarks set the windows's caption ( title ) , you can call this function anytime you want.
		 */
		virtual void setCaption( const string &caption ) = 0 ;
	
		/**
		 * getCustomData
		 * 
		 * @remarks you can get some special data by this function like window's handle in Win32Window.
		 */
		virtual void getCustomData( const string &type, void *pOutData ) {}

		/**
		 * setCustormData, before created the window, you can use this function to modify 
		 * some window's style.
		 *
		 */
		virtual void setCustomData( const string &type, void *pInData ) {}

		/** 
		 * get the window's width.
		 *
		 */
		int getWidth()             { return mWidth;    }

		/** 
		 * get the window's height.
		 *
		 */
		int getHeight()            { return mHeight;   }

		/**
		 * get the window's caption.
		 *
		 */
		const string &getCaption() { return mCaption;  }

		/**
		 * check whether the application is in windowed mode.
		 *
		 */
		bool isWindowed()          { return mWindowed; }

		/**
		 * check whether the window is closed. 
		 * @remarks when the window is closed, that means the application is end.
		 * so this flag can be the flag which can identify whether the game is end.
		 *
		 */
		bool isClosed()			   { return mClosed;   }

	protected:
		/// listener container
		typedef vector<WindowListener*> WindowListenerList;

	protected:
		/// listener list
		WindowListenerList mListeners;
		/// window 's properties
		int mWidth;
		int mHeight;
		string mCaption;
		bool mWindowed;
		/// whether the window is closed
		bool mClosed;
	};
}

#endif