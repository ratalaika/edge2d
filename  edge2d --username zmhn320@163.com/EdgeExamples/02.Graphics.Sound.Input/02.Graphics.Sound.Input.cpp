/**
 * This tutorial is a part of Edge2d.
 * Edge2d is a very object-oriented and plugin-based 2d game engine.
 *
 * Author : Kevin Lynx
 * Date   : 10.29.2007
 *
 * This tutorial will show you how to use Graphics/Input/Sound in Edge
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
		mPluginFile = "config/02.plugin_d.config";
#else
		mPluginFile = "config/02.plugin.config";
#endif
		mCaption = "Edge Example 02 Graphics. Sound. Input";
	}

	~MyApp() { release(); }

	bool init() ;
	void release() ;
	bool update();
	void render();

private:
	Font *mFont;
	Image *mImgLogo;
	Sound *mSndBack;
	Sound *mSndClick;
};

bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	SoundFactory *sfac = engine->getSoundFactory();
	ArchiveManager *am = engine->getArchiveManager();

	/// create an archive which contains data file, before you can load 
	/// any resources , you must create an archive which contain the resources.
	am->createArchive( "./media", "FileSystem" );

	/// create images, note the param, it contains the archive name and the file name.
	mImgLogo = ifac->createImage( "./media:edge_logo.jpg" );

	/// create sound
	mSndBack = sfac->createSound( "./media:getout.ogg" );
	mSndClick = sfac->createSound( "./media:logo.wav" );

	/// play the sounds
	mSndBack->play( true );
	mSndClick->play();

	/// create fonts
	TTFontParam param;
	mFont = EdgeEngine::getSingleton().getFontFactory()->createFont( "font", param );
	
	setConstFPS( 100 );
	return true;
}

void MyApp::release()
{
	/// because Edge will manage the Image, Sound, Font 
	/// resources . So you donot need to delete these resources yourself.
	/// Edge will do that for you! :)
}

bool MyApp::update()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	InputManager *im = engine->getSingletonPtr()->getInputManager();

	if( im->isKeyDown( KC_ESCAPE ) )
	{
		/// when you use Application class, you can return false to 
		/// shutdown the application.
		return false; 
	}
	
	if( im->isKeyDown( KC_RETURN ) &&
		im->isModifierKeyDown( MKC_ALT ) )
	{
		mbWindowed = !mbWindowed;
		engine->resetWindow( mWidth, mHeight, mbWindowed );
		/// and play a sound
		if( mSndClick->isFinished() )
		{
			mSndClick->stop();
			mSndClick->play();
		}
	}

	return true;
}

void MyApp::render()
{
	Graphics *graphics = EdgeEngine::getSingleton().getGraphics();
	graphics->beginScene();
	graphics->clear( Color( 255, 0, 0, 0 ) );

	mImgLogo->setFlip( false, false );
	mImgLogo->setColor( Color( 255, 255, 255, 255 ) );
	mImgLogo->render( 10, 40 );
	mImgLogo->render( 280, 40, 0.5f, 1.5f );
	mImgLogo->renderStretch( 700, 40, 750, 90 );
	mImgLogo->renderAt( 10, 200, 200, 250, 210, 430, 90, 400 );
	mImgLogo->setColor( Color( 128, 255, 255, 255 ) );
	mImgLogo->render( 200, 400 );
	mImgLogo->setFlip( true, false );
	mImgLogo->render( 450, 400 );

	mFont->render( 0, 20, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), L"Press ALT+ENTER to switch screen mode" );
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
