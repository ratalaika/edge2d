/**
 *
 *
 */
#ifndef EDGE_WIN32WINDOW_H
#define EDGE_WIN32WINDOW_H

#include "EdgeWindow.h"
#include <windows.h>

namespace Edge
{
	/**
	 * implement a Win32 Window
	 *
	 */
	class Win32Window : public Window
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Win32Window();

		/**
		 * Destructor
		 *
		 */
		~Win32Window();

		/**
		 *
		 */
		bool create( const string &caption, int width, int height, bool bWindowed );

		/**
		 *
		 */
		void destroy();

		/**
		 *
		 */
		void reset( int width, int height, bool bWindowed );

		/**
		 *
		 */
		void show();

		/**
		 *
		 *
		 */
		void close();

		/**
		 *
		 */
		void showMouse( bool bShow );

		/**
		 * 
		 */
		void handleMessage();

		/**
		 *
		 */
		void setCaption( const string &caption );

		/**
		 *
		 *
		 */
		void addListener( WindowListener *listener );
		
		/**
		 *
		 */
		void getCustomData( const string &type, void *pOutData );

		/**
		 * if you want to modify the window's style, then call this function like :
		 * setCustomData( "WNDCLASS", &wndClass );
		 * in your WindowListener::OnInit, or before Window::create
		 * If you call this function, Window will use your own WNDCLASS to create the window,
		 * otherwise, Window will create the window default.
		 *
		 */
		void setCustomData( const string &type, void *pInData );

	public:
		/**
		 * handleMessage
		 *
		 */
		LRESULT handleMessage( UINT msg, WPARAM wparam, LPARAM lparam );

	private:
		/// access the data when you need to modify style of the window
		WNDCLASS mWndClass;
		/// instance 
		HINSTANCE mInstance;
		/// HWND
		HWND mWnd;
		/// Windowed style and rect
		LONG m_StyleW;
		RECT m_RectW;
		/// fullscreen style and rect
		LONG m_StyleF;
		RECT m_RectF;
		/// whether the mWndClass is set by the custom user
		bool mbCustomWndClass;
		/// icon resource name
		const char* mIconRes;
	};
}

#endif