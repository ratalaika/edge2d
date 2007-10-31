/**
 * This tutorial is a part of Edge2d.
 * Edge2d is a very object-oriented and plugin-based 2d game engine.
 *
 * Author : Kevin Lynx
 * Date   : 10.31.2007
 *
 * This tutorial will show you how to use ResourceManager in Helper library.
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
		mPluginFile = "config/06.plugin_d.config";
#else
		mPluginFile = "config/06.plugin.config";
#endif
		mCaption = "Edge Example 06 Helper Resource Manager";
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
	/// animation
	AnimSprite *mAnimDone;
	AnimSprite::AnimNameList *mAnimNameList;
	int mCurAnim;
};

bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	SoundFactory *sfac = engine->getSoundFactory();
	ArchiveManager *am = engine->getArchiveManager();

	/// create the anim sprite manager, because helper library is seprated from the base library
	/// edge engine will not manage AnimSpriteManager, so you should create it yourself.
	/// Even if you use ResourceManager to create an animation, you must create AnimSpriteFactory
	/// first, if you donot create it, resource manager willnot create animation .
	AnimSpriteFactory *asf = new AnimSpriteFactory();

	/// create the resource manager
	ResourceManager *resMgr = new ResourceManager();

	/// load the resource config file
	resMgr->initiate( "./media/resources.xml" );	

	/// load all groups resources
	resMgr->loadAllGroups();

	/// get resources from the resource manager
	mFont = resMgr->getFont( "normal_font" );
	mImgLogo = resMgr->getImage( "logo" );
	mSndBack = resMgr->getSound( "back" );
	mSndClick = resMgr->getSound( "click" );
	mAnimDone = resMgr->getAnim( "done" );

	/// get the animation name list, you can switch to an animation by its name
	mAnimNameList = &mAnimDone->getAnimNameList();
	mCurAnim = 0;

	/// play sounds
	mSndBack->play( true );
	mSndClick->play();

	setConstFPS( 100 );
	return true;
}

void MyApp::release()
{
	/// because Edge will manage the Image, Sound, Font 
	/// resources . So you donot need to delete these resources yourself.
	/// Edge will do that for you! :)

	/// Edge Base willnot manage these factories in the Helper library, so it's up to you.
	delete AnimSpriteFactory::getSingletonPtr();
	delete ResourceManager::getSingletonPtr();
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
		/// play the sound
		if( mSndClick->isFinished() )
		{
			mSndClick->stop();
			mSndClick->play();
		}
	}

	/// update the animation
	mAnimDone->update( getDeltaTime() );

	/// switch the animation
	if( im->isKeyDown( KC_SPACE ) )
	{
		mAnimDone->setCurAnim( mAnimNameList->at( mCurAnim ) );
		mCurAnim = ( mCurAnim + 1 ) % mAnimNameList->size() ;
	}

	return true;
}

void MyApp::render()
{
	Graphics *graphics = EdgeEngine::getSingleton().getGraphics();

	graphics->beginScene();
	graphics->clear( Color( 255, 0, 0, 0 ) );

	mImgLogo->render( 220, 180 );
	/// render the animation
	mAnimDone->render( 300, 300 );

	mFont->render( 0, 20, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), L"Press ALT+ENTER to switch screen mode" );
	mFont->render( 0, 40, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), L"Press SPACE to switch animation state" );
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
