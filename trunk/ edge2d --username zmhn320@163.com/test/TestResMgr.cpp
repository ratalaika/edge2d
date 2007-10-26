/**
 * Test Edge Resource Manager
 *
 * Kevin Lynx
 * 2007.10.24
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
*-- Edge 综合测试\n \
*-- 资源管理器测试\n \
*-- Kevin Lynx\n \
*-- 2007.10.24\n \
";

class MyApp : public Application
{
public:
	MyApp() 
	{
#ifdef EDGE_DEBUG
		mConfigFile = "plugin_d.config";
#else
		mConfigFile = "plugin.config";
#endif
		mCaption = "Edge Resource Manager Test - Kevin Lynx - 2007.10.24";
	}

	~MyApp() { release(); }

	bool init();
	void release();
	bool update();
	void render();

private:
	Font *mFont;
	Image *mImgBk;
	Image *mImgLogo;
	Sound *mSndBk;
};


bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	ArchiveManager *am = engine->getArchiveManager();
	SoundFactory *sfac = engine->getSoundFactory();

	/// load resources
	ResourceManager *resMgr = new ResourceManager();
	resMgr->initiate( "./resources.xml" );

	resMgr->loadAllGroups();

	mFont = resMgr->getFont( "font1" );
	mImgBk = resMgr->getImage( "back" );
	mImgLogo = resMgr->getImage( "logo" );
	mSndBk = resMgr->getSound( "back" );
	
	mImgBk->setRect( 0, 0, mWidth, mHeight );
	mImgLogo->setHotSpot( mImgLogo->getWidth() / 2.0f, mImgLogo->getHeight() / 2.0f );
	mSndBk->play( true );

	setConstFPS( 100 );

	return true;
}

void MyApp::release()
{
	delete ResourceManager::getSingletonPtr();
}

bool MyApp::update()
{
	
	InputManager *im = EdgeEngine::getSingleton().getInputManager();
	if( im->isKeyDown( Edge::KC_ESCAPE ) )
	{
		return false;
	}
	
	return true;
}

void MyApp::render()
{
	Graphics *graphics = EdgeEngine::getSingleton().getGraphics();
	graphics->beginScene();
	graphics->clear( ARGB( 255, 128, 128, 128 ) );

	mImgBk->render( 0, 0 );
	mImgLogo->render( mWidth / 2.0f, mHeight / 2.0f );

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

