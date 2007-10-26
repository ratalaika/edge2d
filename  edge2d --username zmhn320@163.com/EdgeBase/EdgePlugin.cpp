/**
 *
 *
 *
 */
#include "EdgePlugin.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"

namespace Edge
{
	Plugin::Plugin( const string &file ) : mName( file )
	{
		mHandle = 0;
	}

	Plugin::~Plugin()
	{
	}

	bool Plugin::load()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, 
			"Loading plugin : " + mName );

		mHandle = PLUGIN_LOAD( mName.c_str() );

		if( mHandle == 0 )
		{
			EDGE_EXCEPT( Exception::EC_LOAD_PLUGIN_FAILED, "Load plugin failed : " + mName,
				"Plugin::load" );
		}
		
		/// initiate the plugin
		PLUGIN_INIT_FUNC plugin_initiate = (PLUGIN_INIT_FUNC)getFuncAdd( "plugin_initiate" );
		plugin_initiate();

		return true;
	}

	void Plugin::unload()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, 
			"Unload plugin : " + mName );

		/// release the plugin
		PLUGIN_RELEASE_FUNC plugin_release = (PLUGIN_RELEASE_FUNC)getFuncAdd( "plugin_release" );
		plugin_release();

		if( !PLUGIN_UNLOAD( mHandle ) )
		{
			EDGE_EXCEPT( Exception::EC_UNLOAD_PLUGIN_FAILED, "Unload plugin failed : " + mName,
				"Plugin::unload" );
		}
	}

	void *Plugin::getFuncAdd( const string &funName )
	{
		void *p = (void*)PLUGIN_GETFUNC( mHandle, funName.c_str() );

		if( p == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot get the function in the plugin : " + funName,
				"Plugin::getFuncAdd" );
		}

		return p;
	}
}