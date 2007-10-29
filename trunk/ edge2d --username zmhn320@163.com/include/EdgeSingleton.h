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
#ifndef _EDGE_SINGLETON_H
#define _EDGE_SINGLETON_H

#ifdef _MSC_VER
#pragma warning( disable:4311 )
#pragma warning( disable:4312 )
#endif

namespace Edge 
{
	/** Template class for creating single-instance global classes.
	*/
	template <typename T> 
	class Singleton
	{
	public:
		Singleton()
		{
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
#else
			ms_Singleton = static_cast< T* >( this );
#endif
		}

		~Singleton()
		{  
			ms_Singleton = 0;  
		}

		static T& getSingleton()
		{	
			return ( *ms_Singleton ); 
		}

		static T* getSingletonPtr()
		{ 
			return ms_Singleton; 
		}

	protected:
		static T* ms_Singleton;
	};

	template <typename T> T* Singleton<T>::ms_Singleton = 0;
} // namespace Edge

#endif 