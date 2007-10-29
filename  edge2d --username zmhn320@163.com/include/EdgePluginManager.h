/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
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
	 * plugin manager, it will manage all plugins. You should use this class to load /unload
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

		/**
		 * unload a plugin, and will delete the plugin object
		 *
		 */
		void unload( Plugin *plugin );

		/**
		 * unload all plugins.
		 *
		 */
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
