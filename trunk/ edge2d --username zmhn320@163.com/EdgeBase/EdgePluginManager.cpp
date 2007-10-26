/**
 *
 *
 */
#include "EdgePlugin.h"
#include "EdgePluginManager.h"
#include "EdgeLogManager.h"
#include "EdgeException.h"

namespace Edge
{
	PluginManager::PluginManager()
	{
	}

	PluginManager::~PluginManager()
	{
		release();
	}

	bool PluginManager::initiate()
	{
		return true;
	}

	void PluginManager::release()
	{
		unloadAll();
	}
	
	Plugin *PluginManager::load(const string &name )
	{
		Plugin *plugin = new Plugin( name );
		plugin->load();

		mPlugins[ name ] = plugin;

		return plugin;
	}

	void PluginManager::unload( const string &name )
	{
		PluginList::iterator it = mPlugins.find( name );

		if( it == mPlugins.end() )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "Cannot find plugin to unload : " + name );
		}
		it->second->unload();
		delete it->second ;
	}

	void PluginManager::unload( Plugin *plugin )
	{
		unload( plugin->getName() );
	}

	void PluginManager::unloadAll()
	{
		for( PluginList::iterator it = mPlugins.begin(); it != mPlugins.end(); ++ it )
		{
			it->second->unload();
			delete it->second;
		}

		mPlugins.clear();
	}

	Plugin *PluginManager::getPlugin( const string &name )
	{
		PluginList::iterator it = mPlugins.find( name );

		if( it == mPlugins.end() )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot find the plugin : " + name,
				"PluginManager::getPlugin" );
		}
		
		return it->second ;
	}

}
