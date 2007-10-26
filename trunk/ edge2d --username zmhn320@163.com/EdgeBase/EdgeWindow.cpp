/**
 *
 *
 *
 */
#include "EdgeWindow.h"
#include "EdgeException.h"
#include <algorithm>

namespace Edge
{
	Window::Window() : mCaption( "EDGE Window" )
	{
		mWidth = mHeight = 0;
		mWindowed = true;
		mClosed = false;
	}

	Window::~Window()
	{
	}

	void	Window::addListener( WindowListener *listener )
	{
		mListeners.push_back( listener ) ;
	}

	void	Window::removeListener( WindowListener *listener )
	{
		mListeners.erase( std::find( mListeners.begin(), mListeners.end(), listener ) );
	}
}