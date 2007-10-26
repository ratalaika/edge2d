/**
 *
 *
 */
#include "EdgeWin32Window.h"
#include "EdgeException.h"

namespace Edge
{
#define WINDOW_CLASS_NAME "EDGE_WINDOW_CLASS" 

	//// message callback
	LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		LONG_PTR lptr = ::GetWindowLongPtr( hWnd, GWLP_USERDATA );

		if( lptr != NULL )
		{
			Win32Window *window = reinterpret_cast<Win32Window*>( lptr );
			return window->handleMessage( message, wParam, lParam );
		}

		return ::DefWindowProc( hWnd, message, wParam, lParam );
	}

	Win32Window::Win32Window() : Window()//,
		//mIconRes( "" )
	{
		mIconRes = 0;
		memset( &mWndClass, 0, sizeof( mWndClass ) );
		mInstance = 0;
		mWnd = 0;
		mbCustomWndClass = false;
	}

	Win32Window::~Win32Window()
	{
		destroy();
	}

	bool Win32Window::create( const string &caption, int width, int height, bool bWindowed )
	{
		mCaption = caption;
		mWidth = width;
		mHeight = height;
		mWindowed = bWindowed;
		mInstance = GetModuleHandle( 0 );

		for( size_t i = 0; i < mListeners.size(); ++ i )
		{
			mListeners.at( i )->OnInit();
		}
		
		/// ready to create
		if( !mbCustomWndClass )
		{
			mWndClass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
			mWndClass.lpfnWndProc	= WndProc;
			mWndClass.cbClsExtra	= 0;
			mWndClass.cbWndExtra	= 0;	
			mWndClass.hInstance		= mInstance;
			mWndClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
			mWndClass.hbrBackground	= (HBRUSH)GetStockObject( BLACK_BRUSH );
			mWndClass.lpszMenuName	= NULL;		
			mWndClass.lpszClassName	= WINDOW_CLASS_NAME;
			mWndClass.hIcon			= LoadIcon( mInstance,( mIconRes == 0 ? IDI_APPLICATION : mIconRes ) );
		}
		else
		{
			/// the mWndClass is set by you
		}

		if( !RegisterClass( &mWndClass ) )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "RegisterClass Failed",
				"Win32Window::create" );
		}

	
		width = mWidth + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2;
		height = mHeight + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

		m_RectW.left  = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
		m_RectW.top   = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
		m_RectW.right =  m_RectW.left + width;
		m_RectW.bottom=  m_RectW.top + height;
		m_StyleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; 

		m_RectF.left  = 0;
		m_RectF.top   = 0;
		m_RectF.right = mWidth;
		m_RectF.bottom= mHeight;
		m_StyleF = WS_POPUP | WS_VISIBLE; 

		if( mWindowed )
		{
			mWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME, mCaption.c_str(), m_StyleW,
					m_RectW.left, m_RectW.top, m_RectW.right - m_RectW.left, m_RectW.bottom - m_RectW.top,
					NULL, NULL, mInstance, NULL );
		}
		else
		{
			mWnd = CreateWindowEx( WS_EX_TOPMOST, WINDOW_CLASS_NAME, mCaption.c_str(), m_StyleF,
				0, 0, 0, 0,
				NULL, NULL, mInstance, NULL);
		}

		/// important, hack the message 
		::SetWindowLongPtr( mWnd, GWLP_USERDATA, static_cast<LONG>( reinterpret_cast<LONG_PTR>( this ) ) );

		return true;
	}

	void Win32Window::destroy()
	{
		UnregisterClass( WINDOW_CLASS_NAME, mInstance );
	}

	void Win32Window::close()
	{
		::SendMessage( mWnd, WM_CLOSE, 0, 0 );
	}

	void Win32Window::reset( int width, int height, bool bWindowed )
	{
		/// reset the window's size
		mWidth = width;
		mHeight = height;
		mWindowed = bWindowed;

		width = mWidth + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2;
		height = mHeight + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

		m_RectW.left  = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
		m_RectW.top   = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
		m_RectW.right =  m_RectW.left + width;
		m_RectW.bottom=  m_RectW.top + height;

		m_RectF.left  = 0;
		m_RectF.top   = 0;
		m_RectF.right = mWidth;
		m_RectF.bottom= mHeight;

		RECT *rc;
		LONG style;

		if( bWindowed ) 
		{
			rc = &m_RectW; 
			style = m_StyleW; 
		}
		else  
		{
			rc = &m_RectF; 
			style = m_StyleF; 
		}
		
		SetWindowLong( mWnd, GWL_STYLE, style );
		style = GetWindowLong( mWnd, GWL_EXSTYLE );
		
		if( bWindowed )
		{
			SetWindowLong( mWnd, GWL_EXSTYLE, style & (~WS_EX_TOPMOST) );
			SetWindowPos( mWnd, HWND_NOTOPMOST, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_FRAMECHANGED );
		}
		else
		{
			SetWindowLong( mWnd, GWL_EXSTYLE, style | WS_EX_TOPMOST );
			SetWindowPos( mWnd, HWND_TOPMOST, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_FRAMECHANGED);
		}
	}

	void Win32Window::show()
	{
		ShowWindow( mWnd, SW_SHOW );
	}

	void Win32Window::showMouse( bool bShow )
	{
		ShowCursor( bShow );
	}

	void Win32Window::handleMessage()
	{
		MSG  msg;
		memset( &msg, 0, sizeof( MSG ) );
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	
		mClosed = msg.message == WM_QUIT;
	}

	void Win32Window::addListener( WindowListener *listener )
	{
		mListeners.push_back( listener );
		listener->OnAdd( this );
	}
	
	void Win32Window::getCustomData( const string &type, void *pOutData )
	{
		if( type == "HANDLE" )
		{
			HWND *pWnd = (HWND*) pOutData;
			*pWnd = mWnd;
		}
		if( type == "HINSTANCE" )
		{
			HINSTANCE *pInstance = (HINSTANCE*) pOutData;
			*pInstance = mInstance;
		}
	}

	void Win32Window::setCustomData( const string &type, void *pInData )
	{
		if( type == "WNDCLASS" )
		{
			/// WNDCLASS wndClass; setCustomData( "WNDCLASS", &wndClass );
			mWndClass = *(WNDCLASS*)pInData;
			mbCustomWndClass = true;
		}
		if( type == "ICON" )
		{
			/// you can set this data anytime
			/// setCustomData( "ICON", MAKEINTRESOURCE( IDI_ICON );
			const char *ch = (const char*) pInData;
			mIconRes = ch;
			if( mWnd != 0 )
			{
				SetClassLong( mWnd, GCL_HICON, (LONG)LoadIcon( mInstance, ch ));
			}
		}
	}

	void Win32Window::setCaption( const string &caption )
	{
		mCaption = caption;

		/// set the window's caption
		SetWindowText( mWnd, mCaption.c_str() );
	}

	LRESULT Win32Window::handleMessage( UINT message, WPARAM wParam, LPARAM lParam )
	{
		if( message == WM_ACTIVATEAPP )
		{
			if( wParam == TRUE )
			{
				/// active, get focus
				for( size_t i = 0; i < mListeners.size(); ++ i )
				{
					mListeners.at( i )->OnGetFocus();
				}
			}
			else
			{
				/// inactive, lost focus
				for( size_t i = 0; i < mListeners.size(); ++ i )
				{
					mListeners.at( i )->OnLostFocus();
				}
			}

			return 0;
		}
		else if( message == WM_DESTROY ) 
		{
			for( size_t i = 0; i < mListeners.size(); ++ i )
			{
				mListeners.at( i )->OnDestroy();
			}

			::PostQuitMessage( 0 );

			return 0;
		}

		return ::DefWindowProc( mWnd, message, wParam, lParam );
	}
}