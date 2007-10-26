/**
 * 测试Edge引擎：渲染模块/IO模块/窗口模块/字体模块
 *
 *
 * 本次测试内容：
 * - 窗口/全屏切换，处理设备丢失，主动切换显示模式
 * - GDI字体
 * - Render Target的测试
 * - 图片的渲染
 * - 图片的载入解码，含IO测试
 * - 使用Edge.dll
 * - 测试插件
 *
 * Kevin Lynx
 * 2007.10.12
 *
 */
#ifdef _DEBUG
#include "vld.h"
#endif

#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#include <windows.h>
#endif

#include "EdgeEngine.h"
#include "EdgeGraphics.h"
#include "EdgeException.h"
#include "EdgeArchive.h"
#include "EdgeArchiveManager.h"
#include "EdgeImage.h"
#include "EdgeImageFactory.h"
#include "EdgePluginManager.h"
#include "EdgeFont.h"
#include "EdgeWindow.h"

using namespace Edge;

#pragma comment( lib, "winmm.lib" ) // for timeGetTime
#ifdef _DEBUG
#pragma comment( lib, "../lib/EdgeBase_D.lib" )
#else
#pragma comment( lib, "../lib/EdgeBase.lib" )
#endif
//// const variables
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const bool WINDOWED = true;

//// functions
bool init();
void destroy();
void update();
bool calFPS();
void loadPlugn();

//// variables
//ImagePtr	g_bkImage;
Image		*g_bkImage;
Image		*g_sprite;
DWORD		g_FPS;
DWORD		g_lastTime;
float		g_dt;
DWORD		g_runTime;
//FontPtr		g_font;
Font		*g_font;
Font		*g_font2;

//ImagePtr	g_target;
Image		*g_target;

static wstring TEST_INFO = L"	\
 * Kevin Lynx - Edge Engine Experiment 5\n \
 * 2007.10.8 - 10.13\n \
 * 本次测试内容：\n	\
 * - 窗口/全屏切换，处理设备丢失，主动切换显示模式\n	\
 * - GDI字体\n		\
 * - Render Target的测试\n	\
 * - 图片的渲染\n	\
 * - 图片的载入解码，含IO测试\n	\
 * - Edge.dll 测试\n \
 * - 插件测试\n  \
";


//// program entry
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrev,          
				    LPSTR szCmdLine, int nCmdShow )
{
	try
	{
		init();
		update();
		destroy();
	}
	catch( Exception &e )
	{
		MessageBox( NULL, e.getFullDescription().c_str(), "ERROR", MB_OK );
	}

	return 0;
}

void loadPlugin()
{
	char file[64];
	FILE *fp = fopen( "plugin.config", "r" );

	EdgeEngine *engine = EdgeEngine::getSingletonPtr();

	while( !feof( fp ) )
	{
		fscanf( fp, "%s", file );
		engine->getPluginManager()->load( file );
	}

	fclose( fp );
}

bool init()
{
	new EdgeEngine();

	/// create the engine core
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();
	
	/// init the basic component
	engine->initBase();

	loadPlugin();

	/// startup
	engine->initPlugin( "", WINDOW_W, WINDOW_H, true, 32 );

	/// load an image
	Archive *archive = engine->getArchiveManager()->createArchive( ".\\data", "FileSystem" );
	g_bkImage = engine->getImageFactory()->createImage( "bg2.png", ".\\data", "bg2.png" );
	g_sprite = engine->getImageFactory()->createImage( "sprite",  ".\\data", "zazaka.png" ); 

	g_sprite->setHotSpot( 32, 32 );
	g_bkImage->setRect( 0, 0, 1.0f * WINDOW_W, 1.0f * WINDOW_H );

	g_bkImage->setRenderMode( RM_COLORADD | RM_ALPHABLEND );
	g_bkImage->setColor(0xFF000000,0);
	g_bkImage->setColor(0xFF000000,1);
	g_bkImage->setColor(0xFF000040,2);
	g_bkImage->setColor(0xFF000040,3);

	/// create a default font
	TTFontParam fontParam;
	g_font = engine->getFontFactory()->createFont( "", fontParam );

	fontParam.setFaceName( "楷体" );
	g_font2 = engine->getFontFactory()->createFont( "font", fontParam );

	/// create a target
	g_target = engine->getImageFactory()->createTarget( "", 256, 256 );  
	g_target->setColor( Color( 255, 255, 255, 255 ) );

	/// init timer
	g_lastTime = timeGetTime();
	g_runTime = 0;

	return true;
}

void update()
{

	Window *window = EdgeEngine::getSingletonPtr()->getWindow();
	Graphics *graphics = EdgeEngine::getSingletonPtr()->getGraphics();

	while( !window->isClosed() )
	{
		window->handleMessage();

		graphics->beginScene();
		graphics->setRenderTarget( g_target );
		graphics->clear( ARGB( 128, 255, 255, 255 ) );
		g_sprite->render( 100, 100 );
		graphics->renderRect( 1, 1, 255, 255, ARGB( 255, 255, 0, 0) );
		graphics->endScene();
//*/
		graphics->beginScene();
		graphics->restoreScreenTarget();
		graphics->clear( ARGB( 255, 0, 0, 0 ) );
		
		g_bkImage->render( 0, 0 );

		g_sprite->render( 200, 100 );
		g_target->renderAt( 400, 100, 600, 80, 700, 400, 500, 300 );

		graphics->renderRect( 10, 10, WINDOW_W - 10, WINDOW_H - 10 );
	
		g_font->render( 0, 0, Font::ETA_DEFAULT, Color(), L"FPS : %d", g_FPS );
		g_font->render( 0, 20, Font::ETA_DEFAULT, Color(), L"Time : %d", g_runTime / 1000 );
		g_font2->render( 0, 40, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), TEST_INFO.c_str() );

		graphics->endScene();

		/// as an example here, i will reset the display mode after 3 seconds
		static int s_state = 0;
		if( g_runTime / 1000 <= 10 && g_runTime / 1000 >= 5 && s_state == 0 )
		{
			g_bkImage->setRect( 0, 0, 1024, 768 );
			EdgeEngine::getSingleton().resetWindow( 1024, 768, false, 16 );
			s_state = 1;
		}
		if( g_runTime / 1000 > 10 && s_state == 1 )
		{
			g_bkImage->setRect( 0, 0, 640, 480 );
			EdgeEngine::getSingleton().resetWindow( 640, 480, true );
			s_state = 2;
		}

		/// show the fps on the title bar, update the information every second
		if( calFPS() )
		{
			char info[256];
			sprintf_s( info, "EDGE Expreiment : Window/Fullscreen switch - 2007.10.11 - FPS : %d", g_FPS );
			window->setCaption( info );

		}
	}

	window->destroy();
}

bool calFPS()
{
	static DWORD s_pastTime;
	static DWORD s_frame = 0;

	s_frame ++;

	DWORD curTime = timeGetTime();
	DWORD dt = ( curTime - g_lastTime );
	
	g_runTime += dt;
	
	g_dt = dt / 1000.0f;
	if( g_dt > 0.2f ) g_dt = 0.2f;
	g_lastTime = curTime;
	s_pastTime += dt;
	if( s_pastTime >= 1000 )
	{
		g_FPS = s_frame;
		s_frame = 0; 
		s_pastTime = 0;

		return true;
	}
	
	return false;
}

void destroy()
{
	EdgeEngine *engine = EdgeEngine::getSingletonPtr();

	/// destroy the image and the font, in fact, it will be done automatically
	engine->getFontFactory()->removeFont( "font" );

	delete engine;
}
