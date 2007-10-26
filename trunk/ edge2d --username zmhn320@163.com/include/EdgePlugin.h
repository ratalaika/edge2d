/**
 *
 *
 *
 */
#ifndef EDGE_PLUGIN_H
#define EDGE_PLUGIN_H

#include "EdgeCompile.h"
#include <string>

#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using std::string;

/**
* declare some dynamic library loading functions
*
*/
#if EDGE_PLATFORM == EDGE_PLATFORM_WIN32

#define PLUGIN_HANDLE HINSTANCE
#define PLUGIN_LOAD( a ) LoadLibrary( a )
#define PLUGIN_GETFUNC( a, b ) GetProcAddress( a, b )
#define PLUGIN_UNLOAD( a ) FreeLibrary( a )

#endif

namespace Edge
{

	/**
	 * plugin function , a plugin must define a function like this.
	 *
	 * when a plugin is loaded, this function will be called automatically.
	 */
	typedef bool (*PLUGIN_INIT_FUNC) ();
	
	/**
	 * plugin function , a plugin must define a function like this.
	 *
	 * This function will be called automatically before the plugin is unloaded
	 */
	typedef void (*PLUGIN_RELEASE_FUNC) ();

	/**
	 * a plugin can be registered into Edge engine.
	 *
	 */
	class EDGE_EXPORT Plugin
	{
	public:
		/**
		 * Constructor, specify the plugin's file name, i.e : D3D8Graphics.dll
		 *
		 */
		Plugin( const string &file );

		/**
		 * Destructor
		 *
		 */
		~Plugin();

		/**
		 * load the plugin, it will call the 'plugin_initiate' function in the plugin file.
		 *
		 */
		bool	load();

		/**
		 * unload the plugin, you should not call this function directly, instead call 
		 * PluginManager::unload. 
		 *
		 * it will call the 'plugin_release' function automatically
		 */
		void	unload();

		/**
		 * get a function in the plugin.
		 *
		 */
		void	*getFuncAdd( const string &funName );

		/**
		 * get the plugin 's name, usually the file name
		 *
		 */
		const string &getName() { return mName; }

	private:
		/// plugin handle
		PLUGIN_HANDLE mHandle;
		/// plugin name
		string mName;
	};

}

#endif