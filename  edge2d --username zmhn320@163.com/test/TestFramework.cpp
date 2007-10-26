/**
 * Edge øÚº‹≤‚ ‘
 *
 * Kevin Lynx
 * 2007.10.17
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
#include <sstream>

using namespace Edge;

#ifdef _DEBUG
#pragma comment( lib, "../lib/EdgeBase_D.lib" )
#pragma comment( lib, "../lib/EdgeHelper_D.lib" )
#else
#pragma comment( lib, "../lib/EdgeBase.lib" )
#pragma comment( lib, "../lib/EdgeHelper.lib" )
#endif

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
		mCaption = "Edge Framework Test - Kevin Lynx - 2007.10.17";
	}

	~MyApp() { release(); }

	bool init();
	void release();
	bool update();
	void render();

private:
	FadeImage *mFadeLogo;
	Image *mImgLogo;
	Image *mImgBack;
	Font *mFont;
};


bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	ArchiveManager *am = engine->getArchiveManager();

	am->createArchive( "./data", "FileSystem" );

	/// create images
	mImgLogo = ifac->createImage( "./data/edge_logo.jpg" );
	mImgLogo->setHotSpot( mImgLogo->getWidth() / 2.0f, mImgLogo->getHeight() / 2.0f );
	mImgBack = ifac->createImage( "./data/bk2.png" );
	mImgBack->setRect( 0, 0, mWidth, mHeight );
	mImgBack->setRenderMode( RM_ALPHABLEND | RM_COLORMUL );
	mImgBack->setColor( 0xff008800, 0 );
	mImgBack->setColor( 0xffff0000, 1 );
	mImgBack->setColor( 0xffffff00, 2 );
	mImgBack->setColor( 0xffffffff, 3 );
	
	/// create logo fade in/out object
	mFadeLogo = new FadeImage( mImgLogo, mWidth / 2.0f, mHeight / 2.0f, 80, 2.0f );
	
	/// create fonts
	Edge::TTFontParam fntParam;
	fntParam.setFaceName( "ÀŒÃÂ" );
	mFont = engine->getFontFactory()->createFont( "defaultFont", fntParam );

	setConstFPS( 100 );

	return true;
}

void MyApp::release()
{
	delete mFadeLogo;
}

bool MyApp::update()
{
	if( mFadeLogo->getState() != FadeImage::FINISHED )
	{
		mFadeLogo->update( getDeltaTime() );
	}
	
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
	graphics->clear( ARGB( 255, 0, 0, 0 ) );

	mImgBack->render( 0, 0 );
	
	if( mFadeLogo->getState() != FadeImage::FINISHED )
	{
		mFadeLogo->render();
	}	

	mFont->render( 0, 0, Font::ETA_DEFAULT, Color( 255, 0, 255, 255 ), L"Const FPS : %d", getFPS() );
	mFont->render( 0, 20, L"EdgeøÚº‹≤‚ ‘" );

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