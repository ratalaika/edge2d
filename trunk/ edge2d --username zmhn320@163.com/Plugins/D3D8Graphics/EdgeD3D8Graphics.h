/**
 *
 *
 */
#ifndef EDGE_D3D8GRAPHICS_H
#define EDGE_D3D8GRAPHICS_H

#include <d3d8.h>
#include <d3dx8.h>
#include "EdgeGraphics.h"
#include "EdgeWindow.h"
#include "EdgeSingleton.h"
#include <vector>

using std::vector ;

#define VERTEX_BUFFER_SIZE 4000
#define VERTEX_FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1

namespace Edge
{

	/**
	 * Primitive type
	 *
	 */
	enum EPrimiType
	{
		PT_LINE,
		PT_QUAD
	};

	/**
	 * Vertex
	 *
	 */
	struct Vertex
	{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tx, ty;
	};

	/**
	 * Line
	 *
	 */
	struct Line
	{
		Vertex v[2];
	};

	/**
	 * Quad 
	 *
	 */
	struct Quad
	{
		Vertex v[4];
		IDirect3DTexture8  *tex;
		int mode;

		Quad()
		{
			tex = 0;
			mode = RM_DEFAULT;
		}
	};

	/**
	 * translate Color to D3D color
	 *
	 */
	inline D3DCOLOR GET_D3DCOLOR( const Color &color )
	{
		return D3DCOLOR_ARGB( color.GETA(), color.GETR(), color.GETG(), color.GETB() );
	}


	/**
	 * Graphics
	 *
	 * The manager class which will contact with the low level graphics api
	 */
	class D3D8Graphics : public Singleton<D3D8Graphics>, public Graphics
	{
		friend class D3D8Image;
	
		/**
		 * when the d3d device is lost, d3d8 core will notify all the register callback function
		 *
		 */
		typedef void (*OnLostFunc)();
		typedef void (*OnResetFunc)();
		typedef vector<OnLostFunc> OnLostFuncList;
		typedef vector<OnResetFunc> OnResetFuncList;

	public:
		/**
		 * Constructor
		 *
		 */
		D3D8Graphics();

		/**
		 * Destructor
		 *
		 */
		~D3D8Graphics();

		/**
		 * init function
		 *
	 	 * create the direct3d device and some basic objects
		 */
		bool	initiate( Window *window, int width, int height, bool bWindowed = true, int bits = 32 );

		/**
		 * release
		 *
		 */
		void	release();

		/**
		 *
		 *
		 */
		void	reset( int width, int height, bool bWindowed = true, int bits = 32 );

		/**
		 * beginScene
		 *
		 * begin to render
		 */
		bool	beginScene();

		/**
		 * endScene
		 *
		 * it will end the rendering and present the whole scene
		 */
		void	endScene();

		/**
		 * clear
		 *
		 * it will clear the screen
		 */
		void	clear( const Color &color );

		/**
		 * setRenderTarget
		 *
		 * when calling this function, everything will be rendered on the target
		 * until you call restoreScreenTarget
		 */
		void	setRenderTarget( Image *target );

		/**
		 * restoreScreenTarget
		 *
	 	 * when calling this function, everything will be rendered on the screen
	  	 */
	 	void	restoreScreenTarget();

		/**
		 * renderLine
		 *
		 */
		void	renderLine( float x1, float y1, float x2, float y2, const Color &color = Color() );

		/**
		 * renderRectangle
		 *
		 */
		void	renderRect( float left, float top, float right, float bottom, const Color &color = Color() );

	    /**
		 * renderQuad
		 *
		 */
		void	renderQuad( const Quad &quad );

		/**
		 * renderImage
		 *
		 */
		void	renderImage( const Image *image );

		/**
		 * D3D8Font plugin will use this function, it will render the primitives in the queue.	
		 *
		 */
		void	renderFont();

		/**
		 *
		 *
		 */
		string	&getType();
		
		/**
		 *
		 */
		void	getCustomData( const string &type, void *pOutData );

		/**
		 *
		 */
		void	addCustomData( const string &type, void *pInData );

		/**
		 *
		 */
		void	removeCustomData( const string &type, void *pInData );

		/**
		 *
		 */
		void	OnGetFocus();

		/**
		 *
		 */
		void	OnLostFocus();

	private:
		/**
		 * notify the lost device information to the out world
		 *
		 */
		void	_notifyLost();

		/**
		 * notify the reset device information to the out world
		 *
		 */
		void	_notifyReset();

		/**
		 * set PRESENT_PARAMETERS, the result is in m_curPP
		 *
		 */
		void	_setPP( int width, int height, bool bWindowed, int bits );

		/**
		 * when the device is lost, call this function
		 *
		 */
		void	_reset( bool bRestMode = false );

		/**
		 *
		 *
		 */
		void	_setRenderMode( int mode );

		/**
		* setRenderState
		*
		*/
		void	_setRenderState();

		/**
		* createVertexData
		*
		*/
		bool	_createVertexData();

		/**
		 * _recreateScreenTarget,
		 *
		 */
		void	_recreateScreenTarget();

		/**
		 *
		 *
		 */
		void	_render_batch( bool bEnd = true );

		/**
		 *  find appropriate full screen video mode
		 *
		 */
		D3DFORMAT _findFSMode( int width, int height, int bits );

		/**
		 * 
		 *
		 */
		int		_getDepth( D3DFORMAT format );
		int		_checkFormat( D3DFORMAT format );
		bool	_is32Bits( D3DFORMAT format );
		bool	_is16Bits( D3DFORMAT format );
		string  _getFormatDesc( D3DFORMAT format );

	private:
		/// direct3d object
		IDirect3D8	*m_pD3D;
		/// device
		IDirect3DDevice8 *m_pD3DDevice;
		/// desktop mode
		D3DDISPLAYMODE m_desktopMode;
		/// device window handle
		HWND m_hWnd;
		/// render window
		Window *m_pWindow;
		/// the screen target
		IDirect3DSurface8 *m_pScreenTarget;
		/// present parameters
		D3DPRESENT_PARAMETERS m_PPFS;
		D3DPRESENT_PARAMETERS m_PPW;
		D3DPRESENT_PARAMETERS *m_curPP;
		/// the screen format
		int m_screenFormat;
		/// the screen bit-depth
		int m_screenDepth;
		/// the current blend mode
		int m_curRenderMode;
		/// vertex buffer
		IDirect3DVertexBuffer8 *m_pVB;
		IDirect3DIndexBuffer8 *m_pIB;
		int m_PrimiCount;
		BYTE *m_pVertexData;
		/// a flag identiy whether is in the rendering
		bool m_bRendering;
		/// the current primitive type
		EPrimiType m_curPrimi;
		/// the current texture
		IDirect3DTexture8 *m_curTexture;

		/// callback functions, restore all the d3d8 objects even
		/// these objects D3D8Graphics doessenot know
		OnLostFuncList mOnLostFuncs;
		OnResetFuncList mOnResetFuncs;
	
	};

}

#endif