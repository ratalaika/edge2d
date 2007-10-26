/**
 *
 *
 *
 */
#include "EdgeLzoArchive.h"
#include "EdgeArchiveManager.h"
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
	
		/// lzo archive factory
		ArchiveFactory *af = new LzoArchiveFactory();
		/// add this archive factory to the archive manager
		engine->getArchiveManager()->addArchiveFactory( af );

		return true;
	}

	/**
	 * plugin_release, when this plugin is unloaded, this function will be called automatically
	 *
	 */
	extern "C" EXPORT void plugin_release()
	{
		/// remove the zip archive factory
		ArchiveManager *am = ArchiveManager::getSingletonPtr();
		if( am != NULL )
		{
			am->removeArchiveFactory( LzoArchiveFactory::getSingletonPtr() );
		}
		/// delete it
		delete LzoArchiveFactory::getSingletonPtr();
	}
}
