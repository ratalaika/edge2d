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