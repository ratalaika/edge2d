/**
 * Test TTFont 
 *
 * Kevin Lynx
 * 2007.10.21
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
*-- ����TTFont���\n \
*-- ���û�����ƣ�ʹ��GDI����ö�true type font�Ŀ���\n \
*-- �����ֺŲ�������\n \
*-- ���������������ķ�ʽ���������壬Ҳ������\n \
*-- Ҫ�Ա���ԭʼ��GDI��ʽ���޸�plugin.config�ļ�������D3D8Font.dll�������\n \
*-- Kevin Lynx\n \
*-- 2007.10.22\n \
*-- patch version features :\n \
*-- ���������������ϼ�࣬�Ӷ�������֮ǰ�����ֺ���ʾ�������BUG\n \
*-- ����֧�������ֿ�������ֺ�\n \
*-- ��demoʹ�õ�������18������\n \
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
		mCaption = "Edge TTFont plugin Test - Kevin Lynx - 2007.10.21";
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
	font_param.setFaceName( "����" );
	mFont = engine->getFontFactory()->createFont( "font", font_param );

	/// create image
	am->createArchive( "./data", "FileSystem" );
	mImgBk = ifac->createImage( "./data:bk2.png" );
	mImgBk->setRect( 0, 0, mWidth * 1.0f, mHeight * 1.0f );
	mImgLogo = ifac->createImage( "./data:edge_logo.jpg" );
	mImgLogo->setHotSpot( mImgLogo->getWidth() / 2, mImgLogo->getHeight() / 2 );

	iPoint p;

	//setConstFPS( 100 );

	return true;
}

void MyApp::release()
{
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

