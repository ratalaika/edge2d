/* Experiment for EDGE Graphics Module 
*
* Kevin Lynx
* 10.6.2007
*/
#ifndef EDGE_IMAGE_H
#define EDGE_IMAGE_H

#include "EdgeCompile.h"
#include "EdgeSharedPtr.h"
#include "EdgeImageData.h"
#include "EdgeColor.h"
#include <string>

using std::string;

namespace Edge
{
	class Graphics;
	class ImageFactory;

	/**
	* Image
	*
	* an image implents a bitmap, a picture,a render target
	* You can render an image on to screen, other image.
	* An image is always 32-bits.
	*
	* An Image's name is its full path like : .\\data\\image.jpg, the name is automatically
	* made, and  you can get an image by the full path name later. 
	*/
	class EDGE_EXPORT Image
	{
	public:
		/**
		 * the Image's type
		 *
		 */
		enum EImageType
		{
			IT_TARGET, // as a render target
			IT_IMAGE   // as a normal image
		};

	public:
		/**
		* Constructor
		*
		*/
		Image( const string &name, ImageFactory *parent, Graphics *graphics, EImageType type = IT_IMAGE ) ;

		/**
		* Destructor
		*
		*/
		virtual ~Image() {}

		/**
		* initiate
		*
		*/
		virtual bool    create( ImageDataPtr imageData ) = 0;
		virtual bool	create( int width, int height ) = 0;

		/**
		 * release
		 *
		 */
		virtual void	release() = 0;

		/**
		 * clone an image
		 *
		 */
		virtual Image *clone( const string &name, float x, float y, float w, float h ) = 0;

		/**
		 * lock the image, so you can access data directly.
		 *
		 */
		virtual void	*lock( int left = 0, int top = 0, int width = 0, int height = 0, 
					  		   bool bReadOnly = true ) { return 0 ;}

		/**
		 * unlock the image, you must call unlock if you called lock
		 *
		 */
		virtual void	unlock() {};

		virtual void	setRenderMode( int mode ) {}

		/**
		 * setColor
		 * Sets color for the specified vertex or the entire image.
		 * @param color
		 * @param index Optional index of the vertex to be changed. Vertices are indexed clockwise
		 * starting from top-left one in the range 0-3. If =-1 or omitted, all four vertices will be changed. 
		 * If the implemented Image use software SDK, index maybe igoned.
		 *
		 */
		virtual void	setColor( const Color &color, int index = -1 ) { }

		/**
		 * render the image to the screen.
		 * @param x Screen x-coordinate to render the image at.
		 * @param y Screen y-coordinate to render the image at
		 */
		virtual void	render( float x, float y ) = 0;

		/**
		 * render the image to the screen with scaling and rotation. 
		 * @param x Screen x-coordinate to render the image at.
		 * @param y Screen y-coordinate to render the image at
		 * @rot image rotation in radians.
		 * @hscale Horizontal scaling factor. 1.0 means default size. If omitted, no scaling will 
		 *         be applied. 
		 * @vscale Vertical scaling factor. 1.0 means default size. If = 0.0 or omitted, hscale is
		 *         used for both horizontal and vertical scaling. 
		 */
		virtual void	render( float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f ) = 0;

		/**
		 * Renders the image to the screen with stretching. 
		 * @param x1 The image's top-left on screen x-coordinate
		 * @param y1 The image's top-left corner on-screen y-coordinate. 
		 * @param x2 The image's bottom-right corner on-screen x-coordinate.
		 * @param y2 The image's bottom-right corner on-screen y-coordinate. 
		 */
		virtual void	renderStretch( float x1, float y1, float x2, float y2 ) = 0;

		/**
		 * Renders the image to arbitrary quad on the screen. 
		 *
		 * Some software Graphics plugin can igon to implement this function.
		 */
		virtual void	renderAt( float x0, float y0, float x1, float y1, 
							      float x2, float y2, float x3, float y3 ) {}

		/**
		 * Set the region to use for the image. The region usually means the rect on the image file.
		 *
		 */
		virtual void	setRect( float x, float y, float w, float h ) 
		{
			mRegionX = x; mRegionY = y; mRegionW = w; mRegionH = h;
		}

		/**
		 * Sets the image anchor point. the anchor point will affect scale, rot etc.
		 * @param x Anchor point x-coordinate.
		 * @param y Anchor point y-coordinate.
		 */
		virtual void	setHotSpot( float x, float y ) { mHotSpotX = x; mHotSpotY = y; }

		/**
		 * Flips the image horizontally and/or vertically. 
		 * @param bX If true, the image will be flipped horizontally. 
		 * @param bY If true, the image will be flipped vertically. 
		 */
		virtual void	setFlip( bool bX, bool bY ) { mXFlip = bX; mYFlip = bY; };

		void getRect( float *x, float *y, float *w, float *h )
		{
			*x = mRegionX; *y = mRegionY; *w = mRegionW; *h = mRegionH;
		}

		/** 
		 * getWidth, the width and the size here is the whole image size, but not the using region
		 *
		 */
		virtual int getWidth( bool bOriginal = false )  const { return bOriginal ? mOriWidth : mWidth;   }
		virtual int getHeight( bool bOriginal = false ) const { return bOriginal ? mOriHeight : mHeight; }

		int getType()   const { return mType; }
		string &getName()     { return mName;  }

		virtual Color getColor( int index = 0 ) = 0;
	
	protected:
		Image( const Image &image );
		const Image &operator = ( const Image & );

	protected:
		/// name
		string mName;
		/// parent
		ImageFactory *mParent;
		/// graphics
		Graphics *mGraphics;
		/// image size
		int mWidth, mHeight;
		/// original size, maybe it's bigger than image size
		int mOriWidth, mOriHeight;
		/// format
		int mFormat;
		/// type
		EImageType mType;
		/// hotspot
		float mHotSpotX, mHotSpotY;
		/// using region
		float mRegionX, mRegionY, mRegionW, mRegionH;
		/// flip flags
		bool mXFlip, mYFlip;
		
	};

	typedef shared_ptr<Image> ImagePtr;
}

#endif
