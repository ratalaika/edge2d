/**
 * This tutorial is a part of Edge2d.
 * Edge2d is a very object-oriented and plugin-based 2d game engine.
 *
 * Author : Kevin Lynx
 * Date   : 10.29.2007
 *
 * This tutorial will show you how to build a simple Edge application.
 *
 */
#include "Edge.h"
#include "helper/EdgeHelper.h"

using namespace Edge;

#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#ifdef EDGE_DEBUG
#pragma comment( lib, "EdgeBase_D.lib" )
#pragma comment( lib, "EdgeHelper_D.lib" )
#else
#pragma comment( lib, "EdgeBase.lib" )
#pragma comment( lib, "EdgeHelper.lib" )
#endif
#endif

/**
 * the applicaton class, here i will use the Edge Helper library.
 *
 */
class MyApp : public Application
{
public:
	MyApp() 
	{
#ifdef EDGE_DEBUG
		mPluginFile = "config/01.plugin_d.config";
#else
		mPluginFile = "config/01.plugin.config";
#endif
		mCaption = "Edge Example 01 HelloWorld";
	}

	~MyApp() { release(); }

	bool init() ;
	void release() {}
	bool update() { return true; }
	void render();

private:
	Font *mFont;
};

bool MyApp::init()
{
	TTFontParam param;
	mFont = EdgeEngine::getSingleton().getFontFactory()->createFont( "font", param );
	
	setConstFPS( 100 );
	return true;
}

void MyApp::render()
{
	Graphics *graphics = EdgeEngine::getSingleton().getGraphics();
	graphics->beginScene();
	graphics->clear( Color( 255, 0, 0, 0 ) );
	
	mFont->render( mWidth / 2.0f, mHeight / 2.0f, L"Hello world!", Color( 255, 255, 255, 255 ), Font::ETA_CENTER );
	mFont->render( 0, 0, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), L"Const FPS : %d", getFPS() );
	graphics->endScene();
}

/**
 * Edge application entry function
 *
 */
int main( int argc, char **argv )
{
	MyApp app;

	/**
	 * I recommend you put all your edge code in a try...catch block, 
	 * so that you can catch edge exceptions.
	 */
	try
	{
		if( !app._init() )
		{
			return 0;
		}
	
		app.run();
	}
	catch( Exception &e )
	{
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
		MessageBoxA( NULL, e.getFullDescription().c_str(), "ERROR", MB_OK );
#else
		cout << e.getFullDescription() << endl;
#endif
	}

	return 0;
}
