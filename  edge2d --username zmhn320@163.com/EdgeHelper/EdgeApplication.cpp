/**
 *
 *
 */
#include "EdgeApplication.h"
#include "EdgeEngine.h"
#include "EdgeException.h"
#include "EdgeGraphics.h"
#include "EdgeTimer.h"
#include "EdgeWindow.h"
#include "EdgePluginManager.h"
#include "EdgeInputManager.h"

namespace Edge
{
	Application::Application() :
		mLogFile( "Edge.log" ), mPluginFile( "plugin.ini" ), mCaption( "Edge Window" )
	{
		mWidth = 800;
		mHeight = 600;
		mbWindowed = true;
		mDepth = 32;
		mbSuspend = true;
		mbActive = true;

		mConstFPS = 0;
		mRealFPS = 0;
		mLastTime = 0;
		mDelayTime = 0;
		mDeltaTime = 0;
	}

	Application::~Application()
	{
		_release();
	}

	bool Application::_init()
	{
		EdgeEngine *engine = new EdgeEngine();
		engine->initBase( mLogFile );

		loadPlugins();
		/// set this as a window's event listener
		engine->getWindow()->addListener( this );
		/// init all the plugins
		engine->initPlugin( mCaption, mWidth, mHeight, mbWindowed, mDepth );
		/// custom init,like load resources
		return init();
	}

	void Application::_release()
	{
		EdgeEngine *engine = EdgeEngine::getSingletonPtr();
		engine->getWindow()->destroy();
		delete engine;
	}

	void Application::render()
	{
	}

	bool Application::run()
	{
		EdgeEngine *engine = EdgeEngine::getSingletonPtr();
		Timer *timer = engine->getTimer();
		Window *window = engine->getWindow();
		InputManager *im = engine->getInputManager();

		unsigned long curTime;
		long tmp_dt;
		unsigned long dt;
		mLastTime = timer->getCurTime();

		while( !window->isClosed() )
		{
			window->handleMessage();

			/// suspend
			if( mbSuspend && !mbActive )
			{
				engine->sleep( 1000 );
				continue;
			}

			im->update();

			curTime = timer->getCurTime();
			tmp_dt = curTime - mLastTime;
			dt = tmp_dt < 0 ? 0 : (unsigned long)tmp_dt;

			if( dt >= mDelayTime )
			{
				if( !update() )
				{
					return false;
				}

				render();
				
				calFPS( curTime, dt );
			}
			else
			{
				engine->sleep( mDelayTime - dt );
			}
		}

		return false;
	}

	void Application::setConstFPS( int fps )
	{
		if( fps <= 0 )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Invalid FPS value",
				"Application::setConstFPS" );
		}

		mConstFPS = fps;
		mDelayTime = 1000 / mConstFPS;
	}

	float Application::getDeltaTime()
	{
		return mDeltaTime / 1000.0f;
	}

	int Application::getFPS()
	{
		return mRealFPS;
	}

	void Application::OnInit()
	{
		preinit();
	}

	void Application::OnGetFocus()
	{ 
		mbActive = true; 
		mLastTime = EdgeEngine::getSingleton().getTimer()->getCurTime();
	}

	void Application::OnLostFocus()
	{
		mbActive = false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	void Application::loadPlugins()
	{
		char file[64];
		FILE *fp = fopen( mPluginFile.c_str(), "r" );

		EdgeEngine *engine = EdgeEngine::getSingletonPtr();

		while( !feof( fp ) )
		{
			fscanf( fp, "%s", file );
			engine->getPluginManager()->load( file );
		}

		fclose( fp );
	}

	void Application::calFPS( unsigned long curTime, unsigned long dt )
	{
		static unsigned long s_pastTime;
		static int s_newfps;

		s_pastTime += dt;
		if( s_pastTime >= 1000 )
		{
			mRealFPS = s_newfps;
			s_pastTime = 0;
			s_newfps = 0;
		}
		else
		{
			s_newfps ++;
		}

		mLastTime = curTime;
		mDeltaTime = dt;
	}
}