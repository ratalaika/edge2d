/**
 *
 *
 */
#include "EdgeTTFont.h"
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
	
		/// font factory
		FontFactory *fontFactory = new TTFontFactory();
		/// register the font factory
		engine->registerFontFactory( fontFactory );

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		delete TTFontFactory::getSingletonPtr(); 
	}
	
}