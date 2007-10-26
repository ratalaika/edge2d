/**
 *
 *
 *
 */
#include "EdgeOISInputManager.h"
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
	
		/// input system
		InputManager *im = new OISInputManager();
		/// register 
		engine->registerInputManager( im );

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		delete OISInputManager::getSingletonPtr();
	}
	
}