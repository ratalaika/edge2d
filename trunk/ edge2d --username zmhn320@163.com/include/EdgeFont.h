/**
 *
 *
 *
 */
#ifndef EDGE_FONT_H
#define EDGE_FONT_H

#include "EdgeCompile.h"
#include "EdgeSharedPtr.h"
#include "EdgeColor.h"
#include "EdgeDataStream.h"
#include <string>
#include <map>

using std::wstring;
using std::map;
using std::string;

namespace Edge
{

	class FontFactory;

	/**
	 * FontParam, contains the information to create a font
	 *
	 */
	struct FontParam
	{
		virtual ~FontParam() {}
	};

	/**
	 * TTFontParam, describes a true type font/ system font information
	 *
	 */
	struct TTFontParam : public FontParam
	{
		string mFaceName;
		long mHeight;
		long mWidth;
		long mWeight;
		bool mItalic;
		bool mUnderLine;

		TTFontParam() : mFaceName( "System" )
		{
			mHeight = -12;
			mWidth = 0;
			mWeight = 0;
			mItalic = false;
			mUnderLine = false;
		}

		void setHeight( long height ) { mHeight = height; }
		void setWidth( long width )   { mWidth = width;   }
		void setWeight( long weight ) { mWeight = weight; }
		void setItalic( bool italic ) { mItalic = italic; }
		void setUnderline( bool ul )  { mUnderLine = ul;  }
		void setFaceName( const string &name){ mFaceName = name; }

	};

	/**
	 * Font, use font to render text
	 *
	 */
	class EDGE_EXPORT Font
	{
	public:
		/**
		 * text align
		 *
		 */
		enum ETextAlign
		{
			ETA_LEFT = 0x00,
			ETA_RIGHT = 0x01,
			ETA_TOP = 0x00,
			ETA_BOTTOM = 0x02,
			ETA_CENTER = 0x04,     /// center in horizontal
			ETA_MIDDLE = 0x08,     /// center in veri
			ETA_DEFAULT = ETA_LEFT | ETA_TOP
		};
	public:
		/**
		 * Constructor
		 *
		 */
		Font( FontFactory *parent, const string &name ) : mParent( parent ), mName( name ) 
		{
			mScale = 1.0f;
			mSpacing = 0.0f;
			mLineSpacing = 0.0f;
		}

		/**
		 * Destructor
		 *
		 */
		virtual ~Font() {}

		/**
		 * create
		 *
		 */
		virtual bool create( const FontParam &param ) = 0;

		virtual bool create( const DataStreamPtr &stream ) = 0;

		/**
		 * release, automatically called by destructor
		 *
		 */
		virtual void release() = 0;

		/**
		 * scale the font
		 *
		 */
		virtual void setScale( float scale ) { mScale = scale; }

		/**
		 * set the spacing between characters
		 *
		 */
		virtual void setSpacing( float spacing ) { mSpacing = spacing; }

		/**
		 * set the spacing between lines
		 *
		 */
		virtual void setLineSpacing( float linespace ) { mLineSpacing = linespace; }

		/** 
		 * get a string's width
		 *
		 */
		virtual float getStringWidth( const wstring &str ) { return 0.0f; }

		/**
		 * get the font's height
		 *
		 */
		virtual float getHeight() { return 0.0f ;}

		/**
		 * get the current font scaling
		 *
		 */
		virtual float getScale() { return mScale; }

		/**
		 * get the current spacing between characters
		 *
		 */
		virtual float getSpacing() { return mSpacing; }

		/**
		 * get the current spacing between lines
		 *
		 */
		virtual float getLineSpacing() { return mLineSpacing; }

		/**
		 * render, render some texts
		 *
		 */
		virtual void render( float x, float y, const wstring &text, const Color &color = Color(), int align = 0 ) = 0;

		/**
		 * render format strings
		 *
		 */
		virtual void render( float x, float y, int align, const Color &color, const wchar_t *format, ... ) = 0;

		string &getName() { return mName; }

	protected:
		/// name 
		string mName;
		/// scale
		float mScale;
		/// spacing between characters
		float mSpacing;
		/// spacing between lines
		float mLineSpacing;
		/// parent, when the object is destroyed, it will notify its parent
		FontFactory *mParent;
	};

	/// font ptr
	typedef shared_ptr<Font> FontPtr;

	/**
	 * FontFactory, it can create a font, and store a font
	 *
	 */
	class EDGE_EXPORT FontFactory
	{
	public:
		/**
		 * Constructor
		 *
		 */
		FontFactory();

		/**
		 * Destructor
		 *
		 */
		virtual ~FontFactory() ;

		/**
		 * initiate, initiate the factory
		 *
		 */
		virtual bool initiate() { return true; }

		/**
		 * release, it will be called automatically by constructor
		 *
		 */
		virtual void release() {}

		/**
		 * createFont
		 * this function will not save the pointer, and when the returned value
		 * is destroyed, the Font object also will be destroyed. 
		 */
		//virtual FontPtr	createFont( const FontParam &param ) = 0;

		/**
		 * createFont
		 * this function will save the pointer, and you should specify an unique
		 * name for the Font.And later you can get the Font by its name.
		 */
		virtual Font *createFont( const string &name, const FontParam &param ) = 0;

		virtual Font *createFont( const string &name, const string &file ) = 0;

		/**
		 * getFont
		 *
		 */
		Font*	getFont( const string &name );

		/**
		 * removeFont
		 * this function will remove the Font in the list and also will destroy the 
		 * Font object from memory
		 * 
		 */
		void	removeFont( const string &name );
		void	removeFont( Font *font );

		/**
		 * removeAllFonts
		 *
		 */
		void	removeAllFonts();

		/**
		 * notify a child's death, when some children of this factory are destroyed, this function
		 * will be called
		 *
		 */
		virtual void notifyDeath( const Font *font ) {}

	protected:
		/**
		 * saveFont, save the font object in the list
		 *
		 */
		void saveFont( const string &name, Font *font )
		{
			mFonts[ name ] = font;
		}

		/**
		 *
		 *
		 */
		void modifyName( string &name );

	protected:
		/// 
		typedef map<string, Font*>	FontMap;

	protected:
		FontMap	mFonts;

	};
}

#endif

