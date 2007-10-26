/**
 * Test Edge Simple GUI 
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
#include "edgegui/EdgeGUIManager.h"
#include "edgegui/EdgeGuiButton.h"

using namespace Edge;

#ifdef _DEBUG
#pragma comment( lib, "../lib/EdgeBase_D.lib" )
#pragma comment( lib, "../lib/EdgeHelper_D.lib" )
#pragma comment( lib, "../lib/EdgeGUI_D.lib" )
#else
#pragma comment( lib, "../lib/EdgeBase.lib" )
#pragma comment( lib, "../lib/EdgeHelper.lib" )
#pragma comment( lib, "../lib/EdgeGUI.lib" )
#endif

const wstring g_info = L" \
*-- Edge ¼òÒ×GUI²âÊÔ\n \
";

class MyApp : public Application, public ButtonListener
{
	enum BTN_ID
	{
		BTN_1,
		BTN_2,
		BTN_EXIT,
		BTN_FUN
	};

public:
	MyApp() 
	{
#ifdef EDGE_DEBUG
		mConfigFile = "plugin_d.config";
#else
		mConfigFile = "plugin.config";
#endif
		mCaption = "Edge GUI Test - Kevin Lynx - 2007.10.24";
	}

	~MyApp() { release(); }

	bool init();
	void release();
	bool update();
	void render();

	/// button event
	void ButtonClick( int id )
	{
		if( id == BTN_EXIT )
		{
			EdgeEngine::getSingleton().shutdown();
		}
		if( id == BTN_FUN )
		{
			mPanel->show( !mPanel->isVisible() );

			if( mPanel->isVisible() )
			{
				mBtnFun->setText( L"Òþ²Ø°´Å¥" );
			}
			else
			{
				mBtnFun->setText( L"ÏÔÊ¾°´Å¥" );
			}
		}
	}

private:
	Font *mFont;
	Image *mImgBk;
	Image *mImgLogo;

	ButtonWidget *mButton;
	ButtonWidget *mButton2;
	GUIWidget *mPanel;

	ButtonWidget *mBtnExit;
	ButtonWidget *mBtnFun;
};


bool MyApp::init()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	ImageFactory *ifac = engine->getImageFactory();
	ArchiveManager *am = engine->getArchiveManager();
	SoundFactory *sfac = engine->getSoundFactory();

	/// create fonts
	TTFontParam font_param;
	font_param.setHeight( -12 );
	font_param.setFaceName( "ËÎÌå" );
	mFont = engine->getFontFactory()->createFont( "font", font_param );

	/// create image
	am->createArchive( "./data", "FileSystem" );
	mImgBk = ifac->createImage( "./data:bk2.png" );
	mImgBk->setRect( 0, 0, mWidth * 1.0f, mHeight * 1.0f );
	mImgLogo = ifac->createImage( "./data:edge_logo.jpg" );
	mImgLogo->setHotSpot( mImgLogo->getWidth() / 2, mImgLogo->getHeight() / 2 );

	/// init gui
	GUIManager *guimgr = new GUIManager();

	mPanel = new GUIWidget();
	mPanel->setDimension( fRect( 0, 0, mWidth, mHeight ) );

	Image *img_btn = ifac->createImage( "./data:btn-send.png" );
	mButton = new ButtonWidget( mPanel, BTN_1 );
	mButton->setImage( ButtonWidget::BS_NORMAL, img_btn, fRect( 0, 0, 75, 27 ) );
	mButton->setImage( ButtonWidget::BS_MOUSE_ON, img_btn, fRect( 76, 0, 76 + 76, 27 ) );
	mButton->setImage( ButtonWidget::BS_PUSH, img_btn, fRect( 152, 0, 152 + 76, 27 ) );
	mButton->setDimension( fRect( 300, 200, 375, 227 ) );	

	mButton2 = new ButtonWidget( mPanel, BTN_2 );
	mButton2->setImage( ButtonWidget::BS_NORMAL, img_btn, fRect( 0, 0, 75, 27 ) );
	mButton2->setImage( ButtonWidget::BS_MOUSE_ON, img_btn, fRect( 76, 0, 76 + 76, 27 ) );
	mButton2->setImage( ButtonWidget::BS_PUSH, img_btn, fRect( 152, 0, 152 + 76, 27 ) );
	mButton2->setDimension( fRect( 400, 200, 475, 227 ) );	


	Image *img_btn_n = ifac->createImage( "./data:stonebutton_n.png" );
	Image *img_btn_o = ifac->createImage( "./data:stonebutton_d.png" );
	Image *img_btn_p = ifac->createImage( "./data:stonebutton_i.png" );

	mBtnExit = new ButtonWidget( 0, BTN_EXIT );
	mBtnExit->setImage( ButtonWidget::BS_NORMAL, img_btn_n );
	mBtnExit->setImage( ButtonWidget::BS_MOUSE_ON, img_btn_o );
	mBtnExit->setImage( ButtonWidget::BS_PUSH, img_btn_p );
	mBtnExit->setDimension( fRect( 500, 500, 667, 552 ) );
	mBtnExit->setFont( mFont );
	mBtnExit->setTextColor( Color( 255, 255, 0, 0 ) );
	mBtnExit->setText( L"ÍË³ö" );
	mBtnExit->addListener( this );

	mBtnFun = new ButtonWidget( 0, BTN_FUN );
	mBtnFun->setImage( ButtonWidget::BS_NORMAL, img_btn_n );
	mBtnFun->setImage( ButtonWidget::BS_MOUSE_ON, img_btn_o );
	mBtnFun->setImage( ButtonWidget::BS_PUSH, img_btn_p );
	mBtnFun->setDimension( fRect( 300, 500, 467, 552 ) );
	mBtnFun->setFont( mFont );
	mBtnFun->setTextColor( Color( 255, 255, 0, 0 ) );
	mBtnFun->setText( L"Òþ²Ø°´Å¥" );
	mBtnFun->addListener( this );

	setConstFPS( 100 );

	return true;
}

void MyApp::release()
{
	delete mBtnFun;
	delete mButton2;
	delete mPanel;
	delete mBtnExit;
	delete mButton;
	delete GUIManager::getSingletonPtr();
}

bool MyApp::update()
{
	
	InputManager *im = EdgeEngine::getSingleton().getInputManager();
	if( im->isKeyDown( Edge::KC_ESCAPE ) )
	{
		return false;
	}
	
	GUIManager::getSingleton().update( getDeltaTime() );

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

	GUIManager::getSingleton().render();

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

