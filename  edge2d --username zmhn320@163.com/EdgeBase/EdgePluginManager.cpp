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
