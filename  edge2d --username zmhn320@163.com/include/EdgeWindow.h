/**
 *
 *
 *
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
		 * addListener
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
		 */
		void setWidth( int width )   { mWidth = width; }
		
		/**
		 * setHeight
		 * 
		 */
		void setHeight( int height ) { mHeight = height; }

		/**
		 * setCaption
		 *
		 */
		virtual void setCaption( const string &caption ) = 0 ;
	
		/**
		 * getCustomData
		 * 
		 */
		virtual void getCustomData( const string &type, void *pOutData ) {}

		/**
		 * setCustormData, before created the window, you can use this function to modify 
		 * some window's style.
		 *
		 */
		virtual void setCustomData( const string &type, void *pInData ) {}

		/// get
		int getWidth()             { return mWidth;    }
		int getHeight()            { return mHeight;   }
		const string &getCaption() { return mCaption;  }
		bool isWindowed()          { return mWindowed; }
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