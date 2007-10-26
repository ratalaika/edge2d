/**
 * Edge≤‚ ‘ZipArchive
 *
 * Kevin Lynx
 * 2007.10.18
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
		mCaption = "Edge ZipArchive plugin Test - Kevin Lynx - 2007.10.18";
	}

	~MyApp() { release(); }

	bool init();
	void release();
	bool update();
	void render();

private:
	Font *mFont;
	wstring mFileInfo;
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

	/// create a file system archive
	Archive *fileArchive = am->createArchive( "./sound", "FileSystem" );
	/// create a zip archive
	Archive *zipArchive = am->createArchive( "./data.zip", "Zip", "kevin" );
	/// and open a file in the zip archive
	DataStreamPtr stream = zipArchive->open( "root.txt" );

	/// read data from the stream (MemoryDataStream)
	size_t size = stream->getSize();
	char *str = new char [size+1];
	stream->read( str, size );
	str[size] = 0;
	mFileInfo = L"data.zip / root.txt : \n" + toWideString( str, strlen( str ) + 1 );
	delete [] str;

	/// create images from the zip archive
	mImgBk = ifac->createImage( "data.zip:img/bk2.png" );
	mImgLogo = ifac->createImage( "data.zip:img/edge_logo.jpg" );
	
	mImgBk->setRect( 0, 0, float(mWidth), float(mHeight) );
	mImgLogo->setHotSpot( mImgLogo->getWidth() / 2.0f, mImgLogo->getHeight() / 2.0f );

	/// create sounds from the zip archive
	mSndBk = sfac->createSound( "data.zip:snd/getout.ogg" );
	mSndBk->play( true );

	/// create fonts
	Edge::TTFontParam fntParam;
	fntParam.setFaceName( "ÀŒÃÂ" );
	mFont = engine->getFontFactory()->createFont( "defaultFont", fntParam );

	setConstFPS( 100 );

	///////////////////////////////////////////////////////////////////////////////
	LogManager::getSingleton().logMessage( LL_INFORMATION, "Test MemoryDataStream..." );
	DataStreamPtr s1( new FileSystemDataStream( "root.txt" ) );
	size = s1->getSize();
	typedef shared_ptr<MemoryDataStream> MemoryDataStreamPtr;
	MemoryDataStreamPtr s2( new MemoryDataStream( "", size ) );

	s1->read( s2->getPointer(), size );

	s1->seek( 5 );
	s2->seek( 5 );
	if( s1->getPos() == s2->getPos() )
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, "seek / getPos correct" );
	}

	char cstr[5] = {0};
	s1->read( cstr, 4 );
	LogManager::getSingleton().logMessage( LL_INFORMATION, "s1 read 4 : " + string( cstr ) );
	s2->read( cstr, 4 );
	LogManager::getSingleton().logMessage( LL_INFORMATION, "s2 read 4 : " + string( cstr ) );
	

	///////////////////////////////////////////////////////////////////////////////

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
	graphics->clear( ARGB( 255, 0, 0, 0 ) );

	mImgBk->render( 0, 0 );
	mImgLogo->render( mWidth / 2.0f, mHeight / 2.0f );

	mFont->render( 0, 0, Font::ETA_DEFAULT, Color( 255, 0, 255, 255 ), L"Const FPS : %d", getFPS() );
	mFont->render( 0, 20, L"Edge ZipArchive ≤Âº˛≤‚ ‘" );
	mFont->render( 100, 50, mFileInfo );

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
