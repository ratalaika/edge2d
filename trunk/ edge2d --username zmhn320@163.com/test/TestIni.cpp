/**
 * Test Edge Ini Parser
 *
 * Kevin Lynx
 * 2007.10.29
 *
 */
#ifdef _DEBUG
#include "vld.h"
#endif

#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#include <windows.h>
#endif

#include "Edge.h"
#include "helper/EdgeHelper.h"

using namespace Edge;

#ifdef _DEBUG
#pragma comment( lib, "../lib/EdgeBase_D.lib" )
#pragma comment( lib, "../lib/EdgeHelper_D.lib" )
#else
#pragma comment( lib, "../lib/EdgeBase.lib" )
#pragma comment( lib, "../lib/EdgeHelper.lib" )
#endif

const wstring g_info = L" \
*-- Edge ×ÛºÏ²âÊÔ\n \
*-- INI¶ÁÈ¡²âÊÔ\n \
*-- Kevin Lynx\n \
*-- 2007.10.29\n \
";

class MyApp : public Application
{
public:
	MyApp() 
	{
#ifdef EDGE_DEBUG
		mPluginFile = "plugin_d.config";
#else
		mPluginFile = "plugin.config";
#endif
		mIniParser.createDirectly( "./config.ini" );
		mCaption = mIniParser.readString( "Game", "window caption", "Edge Window" );
		mWidth = mIniParser.readInt( "Game", "window width", 640 );
		mHeight = mIniParser.readInt( "Game", "window height", 480 );
		mbWindowed = mIniParser.readBool( "Game", "windowed", true );
	}

	~MyApp() { release(); }

	bool init();
	void release();
	bool update();
	void render();

private:
	Font *mFont;
	IniParser mIniParser;
};


bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	ArchiveManager *am = engine->getArchiveManager();
	SoundFactory *sfac = engine->getSoundFactory();

	/// create a font
	TTFontParam param;
	param.setFaceName( "ËÎÌå" );
	mFont = engine->getFontFactory()->createFont( "font", param );

	setConstFPS( 100 );

	return true;
}

void MyApp::release()
{
	mIniParser.save( "config.ini" );
}

bool MyApp::update()
{
	
	InputManager *im = EdgeEngine::getSingleton().getInputManager();
	if( im->isKeyDown( Edge::KC_ESCAPE ) )
	{
		return false;
	}
	if( im->isKeyDown( Edge::KC_RETURN ) &&
		im->isModifierKeyDown( Edge::MKC_ALT ) )
	{
		mbWindowed = !mbWindowed;
		EdgeEngine::getSingleton().resetWindow( mWidth, mHeight, mbWindowed );
		/// save to ini
		mIniParser.writeBool( "Game", "windowed", mbWindowed );
	}

	if( im->isKeyDown( Edge::KC_F2 ) )
	{
		EdgeEngine::getSingleton().resetWindow( 640, 480, true );
	}

	if( im->isKeyDown( Edge::KC_F3 ) )
	{
		EdgeEngine::getSingleton().resetWindow( 800, 600, true );
	}

	if( im->isKeyDown( Edge::KC_F6 ) )
	{
		mbWindowed = !mbWindowed;
		EdgeEngine *engine = EdgeEngine::getSingletonPtr();
		engine->getGraphics()->reset( mWidth, mHeight, mbWindowed );
		engine->getWindow()->reset( mWidth, mHeight, mbWindowed );
		engine->getInputManager()->reset( mWidth, mHeight );
	}

	return true;
}

void MyApp::render()
{
	Graphics *graphics = EdgeEngine::getSingleton().getGraphics();
	graphics->beginScene();
	graphics->clear( ARGB( 255, 0, 0, 0 ) );

	mFont->render( 0, 20, g_info );
	mFont->render( 0, 0, Font::ETA_DEFAULT, ARGB( 255, 0, 255, 0 ), L"FPS:%d", getFPS() );
	
	graphics->endScene();
}

/////////////////////////////////////////////////////////////////////////////////////
///
/// program entry
///
/////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
	MyApp app;

	try
	{
		if( !app._init() )
		{
			return 0;
		}
		/// log the arguments
		for( int i = 0; i < argc; ++ i)
		{
			LogManager::getSingleton().logMessage( LL_INFORMATION, argv[i] );
		}

		app.run();
	}
	catch( Exception &e )
	{
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
		MessageBoxA( NULL, e.getFullDescription().c_str(), "ERROR", MB_OK );
#endif
	}
	return 0;
}

