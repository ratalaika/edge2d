/**
 *
 *
 */
#ifndef EDGE_D3D8IMAGE_H
#define EDGE_D3D8IMAGE_H

#include "EdgeImage.h"
#include "EdgeD3D8Graphics.h"

namespace Edge
{
   /**
	* D3D8Image
	*
	*
	*/
	class D3D8Image : public Image
	{
	public:
		/**
		 * Constructor
		 *
		 */
		D3D8Image( const string &name, ImageFactory *parent, Graphics *graphisc, EImageType type = IT_IMAGE );

		/**
		 * Destructor
		 *
		 */
		~D3D8Image();

		/**
		 * initiate
		 *
		 */
		bool	create( int width, int height );
		bool	create( ImageDataPtr imageData );

		/**
		 * release
		 *
		 */
		void	release();


		void	setRenderMode( int mode ) ;

		Image *clone( const string &name, float x, float y, float w, float h );

		/**
		 *
		 */
		void	setColor( const Color &color, int index = -1 );

		/**
		 * render
		 *
		 */
		void	render( float x, float y );
		
		/**
		 *
		 */
		void	render( float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f ) ;

		/**
		 *
		 */
		void	renderStretch( float x1, float y1, float x2, float y2 );
		
		/**
		 *
		 */
		void	renderAt( float x0, float y0, float x1, float y1, 
							      float x2, float y2, float x3, float y3 );

		/**
		 *
		 */
		void	setRect( float x, float y, float w, float h );

		/**
		 *
		 */
		void	setHotSpot( float x, float y );

		/**
		 *
		 */
		void	setFlip( bool bX, bool bY );

		/**
		 *
		 *
		 */
		void	*lock( int left = 0, int top = 0, int width = 0, int height = 0, bool bReadOnly = true );

		/**
		 *
		 *
		 */
		Color   getColor( int index = 0 ) { return mQuad.v[index].color ; }
		/**
		 *
		 */
		void	unlock();

		/**
 		 *
		 */
		IDirect3DSurface8 *&getSurface()
		{
			return m_pSurface;
		}

		/**
		 * getQuad
		 *
		 */
		const Quad &getQuad() const
		{
			return mQuad;
		}

		/**
		 * OnLostDevice
		 *
		 */
		void	OnLostDevice();

		/**
		 * OnResetDevice
		 *
		 */
		void	OnResetDevice();

		/**
		 * code trunk to create IDirect3DTexture8 object 
		 *
		 */
		static void createD3DTexture( IDirect3DDevice8 *pDevice, int width, int height, 
									  EImageType type, IDirect3DTexture8 **pOut );

		/**
		 * get IDirect3DTexture8 size
		 *
		 */
		static void _getTextureSize( IDirect3DTexture8 *pTexture, int *w, int *h );

		/**
		 *
		 *
		 */
		IDirect3DTexture8 *&getD3DTexture()
		{
			return m_pTexture;
		}

	private:
		/**
		 * setQuad
		 *
		 */
		void setQuadDefault();

	private:
		/// device
		IDirect3DDevice8 *m_pDevice;
		/// texture
		IDirect3DTexture8 *m_pTexture;
		/// surface
		IDirect3DSurface8 *m_pSurface;
		/// quad, used to render
		Quad mQuad;
	};

}

#endif