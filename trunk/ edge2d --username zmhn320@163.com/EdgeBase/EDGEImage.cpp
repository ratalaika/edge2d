/* Experiment for EDGE Graphics Module 
*
* Kevin Lynx
* 10.6.2007
*/
#include "EDGEImage.h"
#include "EDGEGraphics.h"

namespace Edge
{
	Image::Image( const string &name, ImageFactory *parent, Graphics *graphics, EImageType type ) :
		mName( name )
	{
		mParent = parent;
		mGraphics = graphics;
		mHeight = mWidth = 0;
		mOriHeight = mOriWidth = 0;
		mFormat = 0;
		mType = type;

		mHotSpotX = mHotSpotY = 0.0f;
		mRegionX = mRegionY = mRegionW = mRegionH = 0;
		mXFlip = mYFlip = false;
	}
}
