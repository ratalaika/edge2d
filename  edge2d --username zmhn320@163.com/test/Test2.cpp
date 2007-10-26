/**
 * Edge 2007.10.12 ×ÛºÏ²âÊÔ
 *
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

/// HGE particle system
#include "../plugins/hgeParticleSystem/hgeparticle.h"

#ifdef _DEBUG
#pragma comment( lib, "../lib/hgeParticleSystem_D.lib" )
#else
#pragma comment( lib, "../lib/hgeParticleSystem.lib" )
#endif

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
Font		*g_font;
Image		*g_target;
Image       *g_image;
Image		*g_bkImage;

Image		*g_parTex;
HGE::hgeParticleSystem *g_hgePar1;
HGE::hgeParticleSystem *g_hgePar2;

DWORD		g_FPS;
DWORD		g_lastTime;
float		g_dt;
DWORD		g_runTime;

static wstring TEST_INFO = L"	\
 * Kevin Lynx - Edge Engine Experiment 5\n \
 * 2007.10.8 - 10.13\n \
 * ÒýÇæ×ÛºÏ²âÊÔ\n \
 * HGEÁ£×ÓÏµÍ³¾²Ì¬¿â²å¼þ²âÊÔ\n  \
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
	engine->getArchiveManager()->createArchive( ".\\hgePar", "FileSystem" );

	/// create an image
	g_image = engine->getImageFactory()->createImage( "image", ".\\data", "tuziki.png" );
	g_bkImage = engine->getImageFactory()->createImage( "bkimg", ".\\data", "bg2.png" );

	g_bkImage->setRect( 0, 0, WINDOW_W, WINDOW_H );

	/// create a default font
	TTFontParam fontParam;
	g_font = engine->getFontFactory()->createFont( "", fontParam );

	/// create a target
	g_target = engine->getImageFactory()->createTarget( "", 256, 256 );  
	g_target->setColor( Color( 255, 255, 255, 255 ) );

	/// init timer
	g_lastTime = timeGetTime();
	g_runTime = 0;

	/// create HGE particle system
	g_parTex = engine->getImageFactory()->createImage( "partex", ".\\hgePar", "particles.png" );
	g_parTex->setRect( 0, 0, 32, 32 );
	g_parTex->setRenderMode( RM_ALPHAADD | RM_COLORMUL );
	g_hgePar1 = new HGE::hgeParticleSystem( "hgePar\\particle1.psi", g_parTex );
	g_hgePar2 = new HGE::hgeParticleSystem( "hgePar\\particle2.psi", g_parTex ); 

	g_hgePar1->FireAt( 200, 300 );
	g_hgePar2->FireAt( 400, 200 );

	return true;
}

void update()
{

	Window *window = EdgeEngine::getSingletonPtr()->getWindow();
	Graphics *graphics = EdgeEngine::getSingletonPtr()->getGraphics();

	while( !window->isClosed() )
	{
		window->handleMessage();

		/// update hge particle systems
		g_hgePar1->Update( g_dt );
		g_hgePar2->Update( g_dt );

		graphics->beginScene();
		graphics->setRenderTarget( g_target );
		graphics->clear( ARGB( 128, 255, 255, 255 ) );
		g_image->render( 10, 10 );
		graphics->renderRect( 1, 1, 255, 255, ARGB( 255, 255, 0, 0) );
		graphics->endScene();
//*/
		graphics->beginScene();
		graphics->restoreScreenTarget();
		graphics->clear( ARGB( 255, 0, 0, 0 ) );
		
		g_bkImage->render( 0, 0 );
		g_image->render( 100, 100, 0.2f );
		g_target->renderAt( 400, 100, 600, 80, 700, 400, 500, 300 );

		graphics->renderRect( 10, 10, WINDOW_W - 10, WINDOW_H - 10 );
	
		/// render hge particle systems
		g_hgePar1->Render();
		g_hgePar2->Render();

		g_font->render( 0, 0, Font::ETA_DEFAULT, Color(), L"FPS : %d", g_FPS );
		g_font->render( 0, 20, Font::ETA_DEFAULT, Color(), L"Time : %d", g_runTime / 1000 );
		g_font->render( 0, 40, Font::ETA_DEFAULT, Color( 255, 0, 255, 0 ), TEST_INFO.c_str() );
		
		graphics->endScene();

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
	delete g_hgePar1;
	delete g_hgePar2;

	EdgeEngine *engine = EdgeEngine::getSingletonPtr();

	/// destroy the image and the font, in fact, it will be done automatically
	engine->getFontFactory()->removeFont( "font" );

	delete engine;
}
