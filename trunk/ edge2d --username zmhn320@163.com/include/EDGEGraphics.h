/**
* Experiment for EDGE Graphics Module 
*
* Kevin Lynx
* 10.6.2007
* History : 
* 10.8.2007 version 2
*/
#ifndef EDGE_GRAPHICS_H
#define EDGE_GRAPHICS_H

#include "EdgeCompile.h"
#include "EdgeColor.h"
#include "EdgeWindow.h"
#include <string>

using std::string;

namespace Edge
{

	class Window;
	class Image;

	/**
	 * RenderMode
	 *
	 */
	enum ERenderMode
	{
		RM_COLORADD = 1,
		RM_COLORMUL = 0,
		RM_ALPHABLEND = 2,
		RM_ALPHAADD = 0,
		RM_DEFAULT = RM_COLORMUL | RM_ALPHABLEND
	};

	/**
	* Graphics
	*
	* The manager class which will contact with the low level graphics api
	*/
	class EDGE_EXPORT Graphics : public WindowListener
	{
	public:
		/**
		* Constructor
		*
		*/
		Graphics() {}

		/**
		* Destructor
		*
		*/
		virtual ~Graphics() {}

		/**
		* init function
		*
		* create the direct3d device and some basic objects
		*/
		virtual bool	initiate( Window *window, int width, int height, 
								  bool bWindowed = true, int bits = 32) = 0;

		/**
		* release
		*
		*/
		virtual void	release() = 0;

		/**
		 * reset the screen display mode.Donot call this function directly, instead you 
		 * should call EdgeEngine::resetWindow function. 
		 *
		 */
		virtual void	reset( int width, int height, bool bWindowed = true, int bits = 32 ) {}

		/**
		* beginScene
		*
		* begin to render
		*/
		virtual bool	beginScene() = 0;

		/**
		* endScene
		*
		* it will end the rendering and present the whole scene
		*/
		virtual void	endScene() = 0;

		/**
		* clear
		*
		* it will clear the screen
		*/
		virtual void	clear( const Color &color ) = 0;

		/**
		* setRenderTarget
		*
		* when calling this function, everything will be rendered on the target
		* until you call restoreScreenTarget
		*/
		virtual void	setRenderTarget( Image *target ) = 0;

		/**
		* restoreScreenTarget
		*
		* when calling this function, everything will be rendered on the screen
		*/
		virtual void	restoreScreenTarget() = 0;

		/**
		* renderLine
		*
		*/
		virtual void	renderLine( float x1, float y1, float x2, float y2, const Color &color = Color()) = 0;

		/**
		* renderRectangle
		*
		*/
		virtual void	renderRect( float left, float top, float right, float bottom, const Color &color = Color() ) = 0;

		/**
		 * renderImage, you donot need to call this function to render an image, 
		 * just call Image::render to render itself.
		 *
		 * this function is always called by an Image object. 
		 */
		virtual void	renderImage( const Image *image ) = 0;

		/**
		 * renderText, you donot need to call this function to render texts, just call Font::render
		 *
		 *
		 */
		virtual void	renderFont() {}

		/**
		 * getType, identify this Grpahics plugin 's type, i.e : D3D8
		 *
		 */
		virtual string &getType() = 0;

		/**
		 * getCustomData, custom data means, i.e, in D3D8Graphics, you can use this function
		 * to get the IDirect3DDevice object.
		 *
		 */
		virtual void getCustomData( const string &type, void *pOutData ) {}
		
		/**
		 * add some special data 
		 *
		 */
		virtual void addCustomData( const string &type, void *pInData ) {}

		/**
		 * remove some special data
		 *
		 */
		virtual void removeCustomData( const string &type, void *pInData ) {}
	};

}

#endif 
