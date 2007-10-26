/**
 *
 *
 *
 */
#include "EdgeD3D8Font.h"
#include "EdgeEngine.h"
#include "EdgeGraphics.h"
#include "EdgeException.H"
#include "EdgeLogManager.h"
#include "EdgeArchiveManager.h"
#include <algorithm>

#pragma comment( lib, "d3dx8.lib" )

#pragma warning( disable:4996 )

namespace Edge
{
	////////////////////////////////////////////////////////////////////////////////
	//
	// D3D8FontParam
	//
	////////////////////////////////////////////////////////////////////////////////
	D3D8FontParam::D3D8FontParam()
	{
		ZeroMemory( &mLogFont, sizeof( mLogFont ) );

		strcpy( mLogFont.lfFaceName, "System" );
		mLogFont.lfHeight = -12;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	// D3D8Font
	//
	////////////////////////////////////////////////////////////////////////////////
	D3D8Font::D3D8Font( FontFactory *parent, const string &name ) :
		Font( parent, name )
	{

	}
		
	D3D8Font::~D3D8Font()
	{
		release();
	}

	bool D3D8Font::create( const FontParam &param )
	{
		/// check whether the Graphics( Renderer ) Module is D3D8
		Graphics *graphics = EdgeEngine::getSingletonPtr()->getGraphics();
		if( graphics->getType() != "D3D8" )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "D3D8Font needs D3D8 Render plugin!",
				"D3D8Font::create" );
		}

		/// get the creation param
		//const D3D8FontParam &d3dfont_param = static_cast< const D3D8FontParam&>( param );
		const TTFontParam &ttfont_param = static_cast<const TTFontParam&>( param );
		D3D8FontParam d3dfont_param;
		d3dfont_param.setFaceName( ttfont_param.mFaceName.c_str() );
		d3dfont_param.setWidth( ttfont_param.mWidth );
		d3dfont_param.setHeight( ttfont_param.mHeight );
		d3dfont_param.setWeight( ttfont_param.mWeight );
		d3dfont_param.setItalic( ttfont_param.mItalic );
		d3dfont_param.setUnderline( ttfont_param.mUnderLine );

		/// get the D3D8 device
		IDirect3DDevice8 *device ;
		graphics->getCustomData( "DEVICE", &device );

		/// ready to create the font
		if( FAILED( D3DXCreateFontIndirect( device, &d3dfont_param.mLogFont, &mDXFont ) ) )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_CREATE_FONT, "Create ID3DXFont FAILED",
				"D3D8Font::create" );
		}

		return true;
	}

	bool D3D8Font::create( const DataStreamPtr &stream )
	{
		char type[64];
		char faceName[64];
		int height, width, weight, italic, underline;

		char *buf = new char [stream->getSize()];
		stream->read( buf, stream->getSize() );
		sscanf( buf, "%s%s%d%d%d%d%d", type, faceName, &height, &width, &weight, &italic, &underline );

		delete [] buf;

		if( strcmp( type, "[TTFont]" ) != 0 )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_CREATE_FONT, "Invalid font config file!",
				"TTFont::create" );
		}
		TTFontParam font_param;
		font_param.setFaceName( faceName );
		font_param.setHeight( height );
		font_param.setWidth( width );
		font_param.setWeight( weight );
		font_param.setItalic( italic != 0 );
		font_param.setUnderline( underline != 0 );

		return create( font_param );

	}

	void D3D8Font::release()
	{
		/// be sure its parent is still alive
		if( D3D8FontFactory::mAliveCount > 0 && mParent != 0 )
		{
			mParent->notifyDeath( this );
		}

		mDXFont->Release();
	}

	void D3D8Font::render( float x, float y, const wstring &text, const Color &color, int align ) 
	{
		Graphics *graphics = EdgeEngine::getSingletonPtr()->getGraphics();
		graphics->renderFont();

		if( FAILED( mDXFont->Begin() ) )
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Begin to render font failed" );
		}
		
		LONG rx = static_cast<LONG>( x );
		LONG ry = static_cast<LONG>( y );
		RECT rect = { rx, ry, rx + 1000, ry + 1000 };
		D3DCOLOR d3dcolor = D3DCOLOR_ARGB( color.GETA(), color.GETR(), color.GETG(), color.GETB() );
		mDXFont->DrawTextW( text.c_str(), -1, &rect, DT_CALCRECT, d3dcolor );	

		LONG w = rect.right - rect.left ;
		LONG h = rect.bottom - rect.top;

		UINT format = DT_LEFT | DT_TOP | DT_NOCLIP;

		if( align & ETA_CENTER )
		{
			rect.left -= w / 2;
		}
		else if( align & ETA_RIGHT )
		{
			rect.left -= w;
		}
		if( align & ETA_BOTTOM )
		{
			rect.top -= h;
		}
		else if( align & ETA_MIDDLE )
		{
			rect.top -= h / 2;
		}

		if( mDXFont->DrawTextW( text.c_str(), -1, &rect, format, d3dcolor) == 0 )
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Render Text FAILED" );
		}

		mDXFont->End();

	}

	void D3D8Font::render( float x, float y, int align, const Color &color, const wchar_t *format, ... )
	{
		wchar_t text[512];
		va_list argList;
		va_start( argList, format );

		vswprintf( text, format, argList );

		render( x, y, text, color, align );
	}

	void D3D8Font::OnLostDevice()
	{
		mDXFont->OnLostDevice();
	}

	void D3D8Font::OnResetDevice()
	{
		mDXFont->OnResetDevice();
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	// D3D8FontFactory
	//
	////////////////////////////////////////////////////////////////////////////////
	int D3D8FontFactory::mAliveCount = 0;

	D3D8FontFactory::D3D8FontFactory()
	{
		mAliveCount ++;
	}

	D3D8FontFactory::~D3D8FontFactory()
	{
		mAliveCount --;
		release();
	}

	bool D3D8FontFactory::initiate()
	{
		Graphics *graphics = EdgeEngine::getSingleton().getGraphics();

		/// add callback functions to deal with the device lost stuff
		graphics->addCustomData( "ON_LOST_FUNC", OnLostDevice );
		graphics->addCustomData( "ON_RESET_FUNC", OnResetDevice );

		return true;
	}

	void D3D8FontFactory::release()
	{
		Graphics *graphics = EdgeEngine::getSingleton().getGraphics();

		/// remove the callback functions
		graphics->removeCustomData( "ON_LOST_FUNC", OnLostDevice );
		graphics->removeCustomData( "ON_RESET_FUNC", OnResetDevice );
	}
/*
	FontPtr D3D8FontFactory::createFont( const FontParam &param )
	{
		D3D8Font *font = new D3D8Font( this, "NoName" );
		font->create( param );

		/// save its DX object, later it can help it with restoring the DX object
		mD3D8Fonts.push_back( font );
		
		return FontPtr( font );
	}
*/
	Font *D3D8FontFactory::createFont( const string &name, const FontParam &param )
	{
		/// check whether the font is already created
		Font *font = getFont( name );
		if( font != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The font : " + name + " is already created" );
			return font;
		}

		D3D8Font *d3d8font = new D3D8Font( this, name );
		d3d8font->create( param );

		saveFont( name, d3d8font );

		/// save its DX object, later it can help it with restoring the DX object
		mD3D8Fonts.push_back( d3d8font );

		return d3d8font;
	}

	Font *D3D8FontFactory::createFont( const string &name, const string &file )
	{
		string cfile( file );
		modifyName( cfile );

		/// check whether the font is already created
		Font *font = getFont( name );
		if( font != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The font : " + name + " is already created" );
			return font;
		}

		string archiveName, fileName;

		size_t i = cfile.rfind( ':' );
		size_t size = cfile.size();
		archiveName.assign( cfile, 0, i );
		fileName.assign( cfile, i + 1, size - i );

		Archive *archive = ArchiveManager::getSingletonPtr()->getArchive( archiveName );
		DataStreamPtr stream = archive->open( fileName );

		D3D8Font *d3d8font = new D3D8Font( this, name );
		d3d8font->create( stream );

		/// save its DX object, later it can help it with restoring the DX object
		mD3D8Fonts.push_back( d3d8font );

		saveFont( name, d3d8font );
		return d3d8font;

	}

	void D3D8FontFactory::notifyDeath( Font *font )
	{
		D3D8Font *dxfont = static_cast<D3D8Font*>( font );
		mD3D8Fonts.erase( std::find( mD3D8Fonts.begin(), mD3D8Fonts.end(), dxfont ) ); 
	}

	void D3D8FontFactory::OnLostDevice()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, "D3D8FontFactory::OnLostDevice" );

		D3D8FontList &fonts = D3D8FontFactory::getSingleton().getD3D8FontList();
		for( size_t i = 0; i < fonts.size(); ++ i )
		{
			fonts.at( i )->OnLostDevice();
		}
	}

	void D3D8FontFactory::OnResetDevice()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, "D3D8FontFactory::OnResetDevice" );

		D3D8FontList &fonts = D3D8FontFactory::getSingleton().getD3D8FontList();
		for( size_t i = 0; i < fonts.size(); ++ i )
		{
			fonts.at( i )->OnResetDevice();
		}
	}
}