/**
 *
 *
 */
#ifndef EDGE_D3D8FONT_H
#define EDGE_D3D8FONT_H

#include "EdgeFont.h"
#include "EdgeSingleton.h"
#include <vector>
#include <string>
#include <d3dx8core.h>

using std::string;
using std::vector;

#ifdef _MSC_VER
#pragma warning( disable:4996 )
#endif

namespace Edge
{
	/**
	 * D3D8FontParam
	 *
	 * Before you create a D3D8Font, you can specify some properties for mLogFont,
	 * if you donot, D3D8FontParam will make it default.
	 */
	struct D3D8FontParam : public FontParam
	{
		LOGFONT mLogFont;
	
		D3D8FontParam();
		~D3D8FontParam() {}

		void setHeight( LONG height ) { mLogFont.lfHeight = height; }
		void setWidth( LONG width )   { mLogFont.lfWidth = width;   }
		void setWeight( LONG weight ) { mLogFont.lfWeight = weight; }
		void setItalic( bool italic ) { mLogFont.lfItalic = italic; }
		void setUnderline( bool ul )  { mLogFont.lfUnderline = ul;  }
		void setFaceName( const TCHAR *name){ strcpy( mLogFont.lfFaceName, name ) ; }
	};

	/**
	 * D3D8Font, if you want to use this plugin, make sure the render system is D3D8
	 *
	 */
	class D3D8Font : public Font
	{
	public:
		/**
		 * constructor
		 *
		 */
		D3D8Font( FontFactory *parent, const string &name );
		
		/**
		 * Destructor
		 *
		 */
		~D3D8Font();

		/**
		 * 
		 */
		bool create( const FontParam &param );

		bool create( const DataStreamPtr &stream );

		/**
		 *
		 */
		void release();

		/**
		 *
		 */
		void render( float x, float y, const wstring &text, const Color &color = Color(), int align = 0 ) ;

		/**
		 * It only can render 512 characters
		 * 
		 */
		void render( float x, float y, int align, const Color &color, const wchar_t *format, ... ) ;

		/**
		 * OnLostDevice
		 *
		 */
		void OnLostDevice();

		/**
		 * OnResetDevice
		 *
		 */
		void OnResetDevice();

		/**
		 * get the ID3DXFont object
		 *
		 */
		ID3DXFont *getDXFont()  { return mDXFont; }

	private:
		/// D3D8 stuff
		ID3DXFont *mDXFont;
	};

	/**
	 *
	 *
	 */
	class D3D8FontFactory : public Singleton<D3D8FontFactory>, public FontFactory
	{
	public:
		/**
		 *
		 */
		D3D8FontFactory();

		/**
		 *
		 */
		~D3D8FontFactory();

		/**
		 *
		 */
		bool initiate();

		/**
		 *
		 */
		void release();

		/**
		 *
		 */
		//FontPtr createFont( const FontParam &param );

		/**
		 *
		 */
		Font *createFont( const string &name, const FontParam &param );
		Font *createFont( const string &name, const string &file );

		/**
		 * 
		 *
		 */
		void notifyDeath( Font *font );

		/**
		 * OnLostDevice
		 *
		 */
		static void OnLostDevice();

		/**
		 * 
		 *
		 */
		static void OnResetDevice();

	public:
		/// a flag specified whether the D3D8FontFactory is alive
		static int mAliveCount;

	private:
		/// used to restore all the ID3DXFont object when the device is lost
		typedef vector<D3D8Font*> D3D8FontList;

		D3D8FontList mD3D8Fonts;
	public:
		/**
		 * get the d3d font list
		 *
		 */
		D3D8FontList &getD3D8FontList()
		{
			return mD3D8Fonts;
		}

	};

}

#endif