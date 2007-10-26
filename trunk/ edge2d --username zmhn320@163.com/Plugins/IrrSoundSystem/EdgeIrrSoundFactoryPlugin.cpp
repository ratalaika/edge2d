/**
 *
 *
 */
#include "EdgeIrrSoundFactory.h"
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
	
		/// sound factory
		SoundFactory *sf = new IrrSoundFactory();
		/// register the sound factory
		engine->registerSoundFactory( sf );

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		delete IrrSoundFactory::getSingletonPtr(); 
	}
	
}