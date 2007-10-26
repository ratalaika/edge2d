/**
 * 实验帧动画控制系统
 * 
 * Kevin Lynx
 * 2007.10.16
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
#include "EdgeLogManager.h"
#include "EdgeInputManager.h"
#include "EdgeSoundFactory.h"
#include "EdgeSound.h"

#include "helper/EdgeAnimSprite.h"
#include "helper/EdgeAnimSpriteFactory.h"

using namespace Edge;

#pragma comment( lib, "winmm.lib" ) // for timeGetTime
#ifdef _DEBUG
#pragma comment( lib, "../lib/EdgeBase_D.lib" )
#pragma comment( lib, "../lib/EdgeHelper_D.lib" )
#else
#pragma comment( lib, "../lib/EdgeBase.lib" )
#pragma comment( lib, "../lib/EdgeHelper.lib" )
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
Font		*g_font;
Image		*g_back;
AnimSprite  *g_anim;
AnimSprite::AnimNameList *g_nameList;

DWORD		g_FPS;
DWORD		g_lastTime;
float		g_dt;
DWORD		g_runTime;

static wstring TEST_INFO = L"	\
 * Kevin Lynx - Edge Engine Experiment 6\n \
 * 2007.10.8 - 10.17\n \
 * 引擎综合测试\n \
 * 测试帧动画系统\n \
 * 左键改变动画状态\n \
 * ALT+ENTER切换屏幕\n \
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
#ifndef _DEBUG
#define PLUGIN_FILE "plugin.config"
#else
#define PLUGIN_FILE "plugin_d.config" 
#endif

	char file[64];
	FILE *fp = fopen( PLUGIN_FILE, "r" );

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

	/// create archives
	Archive *archive = engine->getArchiveManager()->createArchive( ".\\data", "FileSystem" );

	/// create an AnimSprite
	AnimSpriteFactory *animFac = new AnimSpriteFactory();
	g_anim = animFac->createAnimSprite( "data/done.anim" );
	g_nameList = &g_anim->getAnimNameList();

	/// create a default font
	TTFontParam fontParam;
	g_font = engine->getFontFactory()->createFont( "", fontParam );

	/// create images
	g_back = engine->getImageFactory()->createImage( "data/bk2.png" );
	g_back->setRect( 0, 0, 800, 600 );
	g_back->setRenderMode( RM_ALPHAADD | RM_COLORADD );
	g_back->setColor( ARGB( 255, 0, 0, 0 ), 0 );
	g_back->setColor( ARGB( 255, 100, 0, 0 ), 1 );
	g_back->setColor( ARGB( 255, 100, 100, 0 ), 2 );
	g_back->setColor( ARGB( 255, 255, 255, 255 ), 3 );

	/// init timer
	g_lastTime = timeGetTime();
	g_runTime = 0;
 
	EdgeEngine::getSingleton().getInputManager()->setMousePos( WINDOW_W / 2, WINDOW_H / 2 );

	EdgeEngine::getSingleton().getWindow()->showMouse( true );

	return true;
}

void update()
{

	Window *window = EdgeEngine::getSingletonPtr()->getWindow();
	Graphics *graphics = EdgeEngine::getSingletonPtr()->getGraphics();

	while( !window->isClosed() )
	{

		window->handleMessage();
		EdgeEngine::getSingleton().getInputManager()->update();
		InputManager *inputManager = EdgeEngine::getSingleton().getInputManager();
	
		g_anim->update( g_dt );

		/// switch animation state
		if( inputManager->isKeyDown( Edge::KC_MOUSELEFT ) )
		{
			static int s_curState = 0;
			g_anim->setCurAnim( g_nameList->at( s_curState ) );
			s_curState = ( s_curState + 1 ) % g_nameList->size() ;
		}

		/// switch display mode
		if( inputManager->isKeyDown( Edge::KC_F2 ) ||
			inputManager->isKeyDown( Edge::KC_LBRACKET ) ||
			( inputManager->isKeyDown( Edge::KC_RETURN ) && 
			inputManager->isModifierKeyDown( Edge::MKC_ALT ) ) )
		{
			if( window->isWindowed() )
			{
				EdgeEngine::getSingleton().resetWindow( 1024, 768, false, 16 );
				g_back->setRect( 0, 0, 1024, 768 );
			}
			else
			{
				EdgeEngine::getSingleton().resetWindow( 800, 600, true );
				g_back->setRect( 0, 0, 800, 600 );
			}
		}

		graphics->beginScene();
		graphics->clear( ARGB( 255, 0, 0, 0 ) );			

		g_back->render( 0, 0 );
		g_anim->render( 400, 300 );

		g_font->render( 0, 0, Font::ETA_DEFAULT, Color(), L"FPS : %d", g_FPS );
		g_font->render( 0, 20, Font::ETA_DEFAULT, Color(), L"Time : %d", g_runTime / 1000 );
		g_font->render( 0, 40, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), TEST_INFO.c_str() );
		
		graphics->endScene();

		/// show the fps on the title bar, update the information every second
		if( calFPS() )
		{
			char info[256];
			sprintf_s( info, "EDGE Expreiment : Animation Test 2007.10.17- FPS : %d", g_FPS );
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
	delete AnimSpriteFactory::getSingletonPtr();

	EdgeEngine *engine = EdgeEngine::getSingletonPtr();

	delete engine;
}
