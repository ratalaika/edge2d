/**
 *
 *
 *
 */
#include "EdgeWin32Window.h"
#include "EdgeWin32Timer.h"
#include "EdgeEngine.h"

#define EXPORT __declspec(dllexport)

#pragma comment( lib, "winmm.lib" ) // for timeGetTime

namespace Edge
{
	/**
	 * some platform depentdent functions
	 *
	 */
	extern "C" EXPORT void sleep( unsigned long millseconds )
	{
		Sleep( millseconds );
	}

	/**
	 * plugin_initiate, when this plugin is loaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT bool plugin_initiate()
	{
		EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	
		/// window
		Window *window = new Win32Window();
		/// register the window
		engine->registerWindow( window ); 
		/// timer
		Timer *timer = new Win32Timer();
		/// register the timer
		engine->registerTimer( timer );
		/// register the sleep function
		engine->sleep = sleep;

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		delete EdgeEngine::getSingleton().getTimer();
		delete EdgeEngine::getSingleton().getWindow();
	}
	
}