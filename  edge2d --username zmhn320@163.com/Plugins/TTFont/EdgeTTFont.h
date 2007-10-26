/**
*
*
*
*/
#ifndef EDGE_TTFONT_H
#define EDGE_TTFONT_H

#include "EdgeSingleton.h"
#include "EdgeFont.h"
#include <windows.h>
#include <vector>

using std::vector;

namespace Edge
{
	class Image;

	/// max characters
#define MAX_CHAR 65536
	/// a single texture to store characters
#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256
	/**
	* character info
	*
	*/
	struct CharInfo
	{
		Image *mImage;    /// the image this character uses
		RECT mRect;       /// the rect on the image. 
		bool mbCreated;   /// whether this character is created

		CharInfo()
		{
			mbCreated = false;
		}
	};

	/** 
	* True type font,it's fast than gdi., you must delete this object before you 
	* delete ImageFactory .
	*
	*/
	class TTFont : public Font
	{
	public:
		/**
		* used image name
		*
		*/
		typedef vector<string> ImageNameList;

	public:
		/**
		* Constructor
		*
		*/
		TTFont( FontFactory *parent, const string &name ) ;

		/**
		* Destructor
		*
		*/
		~TTFont() ;

		/**
		* create
		*
		*/
		bool create( const FontParam &param ) ;
		bool create( const DataStreamPtr &stream );

		/**
		* release, automatically called by destructor
		*
		*/
		void release() ;

		/** 
		*
		*
		*/
		void setScale( float scale ) { mScale = scale; }

		/** 
		*
		*
		*/
		void setSpacing( float spacing ) { mSpacing = spacing ; }

		/**
		*
		*
		*/
		float getStringWidth( const wstring &str );

		/**
		*
		*/
		float getHeight();

		/**
		* render, render some texts
		*
		*/
		void render( float x, float y, const wstring &text, const Color &color = Color(), int align = 0 );

		/**
		* render format strings
		*
		*/
		void render( float x, float y, int align, const Color &color, const wchar_t *format, ... ) ;

	protected:

		/**
		* render a single character
		*
		*/
		void _renderChar( float x, float y, const Color &color, const wchar_t ch );

		/**
		* put a text on the texture
		*
		*/
		void _writeText( const wchar_t chr );

		/**
		* get a new image name, it will save the image name in the list
		*
		*/
		string _getNewImageName();

		/**
		* get a character's width
		*
		*/
		float _getCharWidth( const wchar_t chr );


		/**
		* calcuate the next rect
		*
		*/
		void _calNextRect();

		/**
		* get a new image to store characters
		*
		*/
		Image *_getNewImage();

	private:
		/// a character's size ( usually width == height )
		int mSize;
		/// dc
		HDC mDC;
		/// gdi font
		HFONT mGDIFont;
		/// bitmap
		HBITMAP mBitmap;
		/// pixel data 
		DWORD *mBits;
		/// current image that will be put text data
		Image *mCurImage;
		/// used image count
		int mUsedCount;
		/// the next rect which will be put text on
		RECT mNextRect;
		/// Image name list, used to delete images when TTFont object is deleted
		ImageNameList mImgNameList;
		/// characters buffer
		CharInfo mCharBuf[MAX_CHAR];
	};

	/**
	*
	*
	*/
	class TTFontFactory : public Singleton<TTFontFactory>, public FontFactory
	{
	public:
		/**
		*
		*
		*/
		TTFontFactory();

		/**
		*
		*
		*/
		~TTFontFactory();

		/**
		*
		*
		*/
		bool initiate();

		/**
		*
		*
		*/
		void release();
		/**
		*
		*/
		Font *createFont( const string &name, const FontParam &param );
	
		Font *createFont( const string &name, const string &file );
	
	};
}

#endif
