/**
 *
 *
 */
#include "EdgeD3D8Graphics.h"
#include "EdgeD3D8ImageFactory.h"
#include "EdgeEngine.h"

#define EXPORT __declspec(dllexport)

namespace Edge
{
	/**
	 * plugin_initiate, when this plugin is loaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT bool plugin_initiate()
	{
		EdgeEngine *engine = EdgeEngine::getSingletonPtr();

		/// graphics
		Graphics *graphics = new D3D8Graphics();
		/// reginster the graphics 
		engine->registerGraphics( graphics );
		
		/// image factory
		ImageFactory *imageFac = new D3D8ImageFactory();
		/// register the image factory
		engine->registerImageFactory( imageFac );

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		delete D3D8ImageFactory::getSingletonPtr(); 
		delete D3D8Graphics::getSingletonPtr();

		/// keep it's correct
		EdgeEngine::getSingleton().registerGraphics( 0 );
		EdgeEngine::getSingleton().registerImageFactory( 0 );
	}
	
}