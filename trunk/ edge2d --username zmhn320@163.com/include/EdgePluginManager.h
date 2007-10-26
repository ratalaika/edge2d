/**
 *
 *
 *
 */
#ifndef EDGE_PLUGINMANAGER_H
#define EDGE_PLUGINMANAGER_H

#include "EdgeCompile.h"
#include "EdgeSingleton.h"
#include <map>
#include <string>

using std::map;
using std::string;

namespace Edge
{
	class Plugin;

	/**
	 * plugin manager, it will manage all plugin. You should use this class to load /unload
	 * a plugin.Donot create a plugin yourself.
	 *
	 */
	class EDGE_EXPORT PluginManager : public Singleton<PluginManager>
	{
	public:
		/**
		 * Constructor
		 *
		 */
		PluginManager();

		/**
		 * Destructor
		 *
		 */
		~PluginManager();

		/**
		 * initiate, initiate the plugin manager
		 *
		 */
		bool initiate();

		/**
		 * release the plugin manager, it will  be called automatically by the destructor, and 
		 * it will unload all the loaded plugin .That means, if you load a plugin, you donot need
		 * to unload the plugin yourself, the manager here will help you.
		 */
		void release();

		/**
		 * load a plugin, it will store the plugin.
		 *
		 */
		Plugin *load( const string &name );

		/**
		 * unload a plugin, and will delete the plugin object
		 *
		 */
		void unload( const string &name );
		void unload( Plugin *plugin );
		void unloadAll();

		/**
		 * get a loaded plugin
		 *
		 */
		Plugin *getPlugin( const string &name );

	private:
		/// plugin list
		typedef map<string, Plugin*> PluginList;

	private:
		PluginList mPlugins;
	};
}

#endif