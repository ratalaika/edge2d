/**
*
*
*
*/
#include <sstream>
#include "EdgeTTFont.h"
#include "EdgeException.h"
#include "EdgeImageFactory.h"
#include "EdgeImage.h"
#include "EdgeEngine.h"
#include "EdgeLogManager.h"
#include "EdgeArchiveManager.h"

namespace Edge
{

	TTFont::TTFont( FontFactory *parent, const string &name ) :
	Font( parent, name )
	{
		mUsedCount = 0;
		memset( &mNextRect, 0, sizeof( RECT ) );
	}

	TTFont::~TTFont()
	{
		release();
	}

	bool TTFont::create( const FontParam &param )
	{
		const TTFontParam &ttparam = static_cast<const TTFontParam&> ( param );
		mSize = -ttparam.mHeight ;
		
		if( mSize > TEXTURE_WIDTH )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Font size cannot be greater than 256",
				"TTFont::create" );
		}

		mDC = ::CreateCompatibleDC( NULL );
		
		LOGFONT LogFont;
		ZeroMemory( &LogFont, sizeof(LogFont) );
		LogFont.lfHeight            = -mSize;
		LogFont.lfWidth             = 0;
		LogFont.lfEscapement        = 0;
		LogFont.lfOrientation       = 0;
		LogFont.lfWeight            = ttparam.mWeight;
		LogFont.lfItalic            = ttparam.mItalic;
		LogFont.lfUnderline         = ttparam.mUnderLine;
		LogFont.lfStrikeOut         = FALSE;
		LogFont.lfCharSet           = DEFAULT_CHARSET;
		LogFont.lfOutPrecision      = OUT_DEFAULT_PRECIS; 
		LogFont.lfClipPrecision     = CLIP_DEFAULT_PRECIS; 
		LogFont.lfQuality           = DEFAULT_QUALITY;
		LogFont.lfPitchAndFamily    = DEFAULT_PITCH;
		lstrcpy( LogFont.lfFaceName, ttparam.mFaceName.c_str() );

		mGDIFont = CreateFontIndirect( &LogFont );

		if( mGDIFont == 0 )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_CREATE_FONT, "Cannot create a GDI font",
				"TTFont::create" );
		}

		BITMAPINFO bmi;
		ZeroMemory( &bmi.bmiHeader, sizeof( BITMAPINFOHEADER ) );
		bmi.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
		bmi.bmiHeader.biWidth       = mSize;
		bmi.bmiHeader.biHeight      = -mSize;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biBitCount    = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		mBitmap = CreateDIBSection( mDC, &bmi, DIB_RGB_COLORS, (void**)&mBits, NULL, 0 );
		if( mBitmap == 0 || mBits == 0 )
		{
			EDGE_EXCEPT( Exception::EC_GENERAL, "Create DIB Section for a DC failed",
				"TTFont::create" );
		}

		::SelectObject( mDC, mBitmap );
		::SelectObject( mDC, mGDIFont );
		
		::SetTextColor( mDC, RGB( 255, 255, 255 ) );
		::SetBkColor( mDC, RGB( 0, 0, 0 ) );
		::SetTextAlign( mDC, TA_TOP );

		/// create an image 
		ImageFactory *ifac = EdgeEngine::getSingleton().getImageFactory();
		mCurImage = ifac->createImage( _getNewImageName(), TEXTURE_WIDTH, TEXTURE_HEIGHT );
		mNextRect.left = 1; /// doesnot begin at 0 to aviod some odd things.
		mNextRect.top = 1;
		mNextRect.right = mNextRect.left + mSize;
		mNextRect.bottom = mNextRect.top + mSize;

		return true;
	}

	bool TTFont::create( const DataStreamPtr &stream )
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
	void TTFont::release()
	{
		DeleteObject( mGDIFont );
		DeleteObject( mBitmap );
		DeleteDC( mDC );

		/// release all the images
		ImageFactory *ifac = EdgeEngine::getSingleton().getImageFactory();
		if( ifac != 0 )
		{
			size_t size = mImgNameList.size();
			for( size_t i = 0; i < size; ++ i )
			{
				ifac->removeImage( mImgNameList.at( i ) );
			}
		}
		mImgNameList.clear();
	}

	void TTFont::render( float x, float y, const wstring &text, const Color &color, int align )
	{
		float w = getStringWidth( text );
		float h = getHeight();
		
		if( align & ETA_CENTER )
		{
			x -= w / 2;
		}
		else if( align & ETA_RIGHT )
		{
			x -= w;
		}
		if( align & ETA_BOTTOM )
		{
			y -= h;
		}
		else if( align & ETA_MIDDLE )
		{
			y -= h / 2;
		}

		float rx = x;
		float ry = y;

		size_t size = text.size();
		for( size_t i = 0; i < size; ++ i )
		{
			if( text.at( i ) == '\n' )
			{
				ry += ( h + mLineSpacing );
				rx = x;
				continue;
			}

			_renderChar( rx, ry, color, text.at( i ) );
			rx += ( mSpacing + _getCharWidth( text.at( i ) ) );
		}
	
	}

	void TTFont::render( float x, float y, int align, const Color &color, const wchar_t *format, ...)
	{
		wchar_t text[512];
		va_list argList;
		va_start( argList, format );

		vswprintf( text, format, argList );

		render( x, y, text, color, align );
	}

	float TTFont::getStringWidth( const wstring &str )
	{
		float width = 0.0f;
		size_t size = str.size();

		for( size_t i = 0; i < size; ++ i )
		{
			width += _getCharWidth( str.at( i ) );
		}

		return width;
	}

	float TTFont::getHeight()
	{
		return mScale * mSize;
	}

	///////////////////////////////////////////////////////////////////////////////////
	void TTFont::_renderChar( float x, float y, const Color &color, const wchar_t ch )
	{
		/// whether this character is created yet
		if( mCharBuf[ch].mbCreated )
		{
			/// render directly
			Image *image = mCharBuf[ch].mImage;
			RECT &rect = mCharBuf[ch].mRect ;
			float rx = static_cast<float>( rect.left );
			float ry = static_cast<float>( rect.top );
			float rw = static_cast<float>( mSize );
			float rh = static_cast<float>( mSize );
			image->setColor( color );
			image->setRect( rx, ry, rw, rh + 1 );/// rh + 1 to avoid odd things.
			image->setHotSpot( 0, 0 );
			image->render( x, y, 0.0f, mScale, mScale );
		}
		else
		{
			/// then create it
			RECT rect_char = { 0, 0, mSize, mSize };
			::FillRect( mDC, &rect_char, (HBRUSH)GetStockObject( BLACK_BRUSH ) );
			wchar_t buf[2];
			buf[0] = ch;
			buf[1] = '\0';
			::TextOutW( mDC, 0, 0, buf, 1 ); /// must text out 1 character
			//::ExtTextOutW( mDC, 0, 0, ETO_OPAQUE, 0, buf, 1, 0 ); // this is ok
			
			/// write data onto the image
			DWORD *img_bits = (DWORD*)mCurImage->lock( mNextRect.left, mNextRect.top, mSize, mSize, false );
			
			for( int y = 0; y < mSize; ++ y )
			{
				for( int x = 0; x < mSize; ++ x )
				{
					DWORD color = mBits[ y * mSize + x ];			
					img_bits[ y * TEXTURE_WIDTH + x ] = ARGB( color & 0xff, 255, 255, 255 );///maybe not correct
				}
			}
			
			/// ok
			mCurImage->unlock();

			/// save the data
			mCharBuf[ ch ].mImage = mCurImage;
			mCharBuf[ ch ].mbCreated = true;
			mCharBuf[ ch ].mRect = mNextRect;

			/// update rect
			mNextRect.left += ( mSize + 1 );  /// + 1 to aviod some odd things.
			if( mNextRect.left + mSize > TEXTURE_WIDTH )
			{
				mNextRect.top += ( mSize + 1 ); /// + 1 to aviod some odd things.
				if( mNextRect.top + mSize > TEXTURE_HEIGHT )
				{
					/// create new image.
					mUsedCount ++;
					mCurImage = _getNewImage();
					mNextRect.left = 0;
					mNextRect.top = 0;
					mNextRect.right = mSize;
					mNextRect.bottom = mSize;
				}
				else
				{
					mNextRect.left = 0;
					mNextRect.right = mSize;
					mNextRect.bottom = mNextRect.top + mSize;
				}
			}
			else
			{
				mNextRect.right = mNextRect.left + mSize;
			}

		}
	}

	string TTFont::_getNewImageName()
	{
		std::ostringstream stream;
		stream << "TTFontImage:" << mName << mUsedCount ;
		/// save the image name
		mImgNameList.push_back( stream.str() );

		return stream.str();
	}

	Image *TTFont::_getNewImage()
	{
		ImageFactory *ifac = EdgeEngine::getSingleton().getImageFactory();
		return ifac->createImage( _getNewImageName(), TEXTURE_WIDTH, TEXTURE_HEIGHT );
	}

	void TTFont::_calNextRect()
	{

	}

	float TTFont::_getCharWidth( const wchar_t chr )
	{
		static float s_size_2 = mSize / 2.0f;

		return ( chr & 0xff00 ) >= 128 ? mScale * mSize : mScale * s_size_2; 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TTFontFactory
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	TTFontFactory::TTFontFactory()
	{
	}

	TTFontFactory::~TTFontFactory()
	{
		release();
	}

	bool TTFontFactory::initiate()
	{
		return true;
	}

	void TTFontFactory::release()
	{
	}

	Font *TTFontFactory::createFont( const string &name, const FontParam &param )
	{
		/// check whether the font is already created
		Font *font = _selfGetFont( name );
		if( font != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The font : " + name + " is already created" );
			return font;
		}

		font = new TTFont( this, name );
		font->create( param );

		_saveFont( name, font );

		return font;
	}

	Font *TTFontFactory::createFont( const string &name, const string &file )
	{
		string cfile( file );
		_modifyName( cfile );

		/// check whether the font is already created
		Font *font = _selfGetFont( name );
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

		font = new TTFont( this, name );
		font->create( stream );

		_saveFont( name, font );
		return font;

	}
}