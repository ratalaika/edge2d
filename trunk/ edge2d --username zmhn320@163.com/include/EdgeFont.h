/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
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
	 * @remarks it's an abstract structure, when you want to create a font, you usually
	 * need to create a FontParam structure to specify some properties. But you need to 
	 * create a TTFontParam instead FontParam.
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
		/// specifies the typeface name of the font. 
		string mFaceName;
		/// i.e -12, the logic height of the font
		long mHeight;
		/// usually canbe 0.
		long mWidth;
		long mWeight;
		/// whether italic
		bool mItalic;
		/// whether underline.
		bool mUnderLine;

		TTFontParam() : mFaceName( "System" )
		{
			mHeight = -18;
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
	 * Font, use font to render text.
	 * @remarks this class is an abstract class.And you donot need to create a font yourslef.
	 * You can call FontFactory::createFont to create a font for you.
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
	 * FontFactory, it can create a font, and store a font. 
	 * @remarks usually you want to create a font to render some texts, you need to use this 
	 * class to create a font for you.
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
		 * this function will save the pointer, and you should specify an unique
		 * name for the Font.And later you can get the Font by its name.
		 */
		virtual Font *createFont( const string &name, const FontParam &param ) = 0;

		/**
		 * You can write a font config file to specify a FontParam, so you can create
		 * a font from a font config file. 
		 *
		 *
		 */
		virtual Font *createFont( const string &name, const string &file ) = 0;

		/**
		 * getFont
		 *
		 * get a font you have created before.
		 * @warning if the font is not exist, this function will throw an exception.
		 */
		Font*	getFont( const string &name );

		/**
		 * removeFont
		 * this function will remove the Font in the list and also will destroy the 
		 * Font object from memory
		 * 
		 */
		void	removeFont( const string &name );

		/**
		 * 
		 *
		 */
		void	removeFont( Font *font );

		/**
		 * removeAllFonts, called by the destructor. 
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
		 *
		 * selfGetFont, this class will use it to check whether the font is already exist.
		 */
		Font *_selfGetFont( const string &name );

		/**
		 * saveFont, save the font object in the list
		 *
		 */
		void _saveFont( const string &name, Font *font )
		{
			mFonts[ name ] = font;
		}

		/**
		 *
		 *
		 */
		void _modifyName( string &name );

	protected:
		/// 
		typedef map<string, Font*>	FontMap;

	protected:
		FontMap	mFonts;

	};
}

#endif

