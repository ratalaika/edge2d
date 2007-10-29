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
#ifndef EDGE_COLOR_H
#define EDGE_COLOR_H

#include "EdgeCompile.h"

namespace Edge
{
	/// 32bits color, ARGB format
	typedef unsigned long COLOR32;

	/// create a 32bits color
	inline COLOR32 ARGB( COLOR32 a, COLOR32 r, COLOR32 g, COLOR32 b )
	{
		return ( (( a & 0xff ) << 24 ) | (( r & 0xff) << 16 ) | ( (g & 0xff ) << 8 ) | ( b & 0xff ) );
	}

	/// get the alpha componet 
	inline COLOR32 GETA( COLOR32 color )
	{
		return color >> 24;
	}


	/// get the red componet 
	inline COLOR32 GETR( COLOR32 color )
	{
		return ( color >> 16 ) & 0xff;
	}

	/// get the green componet 
	inline COLOR32 GETG( COLOR32 color )
	{
		return ( color >> 8 ) & 0xff;
	}

	/// get the blue componet 
	inline COLOR32 GETB( COLOR32 color )
	{
		return color & 0xff;
	}

	/// set the alpha componet 
	inline void SETA( COLOR32 &color, COLOR32 a )
	{
		color = ( color & 0x00ffffff ) | ( ( a & 0xff ) << 24 ) ;
	}

	/// set the red componet 
	inline void SETR( COLOR32 &color, COLOR32 r )
	{
		color = ( color & 0xff00ffff ) | (( r & 0xff ) << 16 );
	}

	/// set the green componet 
	inline void SETG( COLOR32 &color, COLOR32 g )
	{
		color = ( color & 0xffff00ff ) | (( g & 0xff ) << 8);
	}

	/// set the blue componet 
	inline void SETB( COLOR32 &color, COLOR32 b )
	{
		color = ( color & 0xffffff00 ) | (( b & 0xff ));
	}

	/**
	 * a tiny class to implement an ARGB color
	 * You'd better use this class to represent your color. Because COLOR32 is not safe.
	 * Why is COLOR32 not safe ? 
	 * - maybe on some machine COLOR32 is not 32-bit because unsigned long is not 32-bit
	 * - COLOR32 is ARGB format, but OpenGL or DirectX may not. So you should translate 
	 *   the color yourself.
	 *
	 */
	class Color
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Color()  { a = r = g = b = 255; } 

		Color( unsigned _a, unsigned _r, unsigned _g, unsigned _b ) :
			a( _a ), r( _r ), g( _g ), b( _b )
		{
		}

		Color( const Color &color ) :
			a( color.a ), r( color.r ), g( color.g ), b( color.b )
		{
		}

		Color( const COLOR32 &color )
		{
#ifdef EDGE_DEBUG
			/// make sure COLOR32 is a really 32-bit color component
			if( sizeof( COLOR32 ) != 4 )
			{
				a = r = g = b = 255;
			}
			else
			{
				a = static_cast<unsigned>( Edge::GETA( color ) );
				r = static_cast<unsigned>( Edge::GETR( color ) );
				g = static_cast<unsigned>( Edge::GETG( color ) );
				b = static_cast<unsigned>( Edge::GETB( color ) );
			}
#else
			a = static_cast<unsigned>( Edge::GETA( color ) );
			r = static_cast<unsigned>( Edge::GETR( color ) );
			g = static_cast<unsigned>( Edge::GETG( color ) );
			b = static_cast<unsigned>( Edge::GETB( color ) );
#endif
		}

		~Color() {}

		Color &operator = ( const Color &color )
		{
			a = color.a;
			r = color.r;
			g = color.g;
			b = color.b;

			return *this;
		}
			
		void ARGB( unsigned _a, unsigned _r, unsigned _g, unsigned _b )
		{
			a = _a; r = _r; g = _g; b = _b;
		}
		
		void SETA( unsigned _a )
		{
			a = _a;
		}

		void SETR( unsigned _r )
		{
			r = _r;
		}

		void SETG( unsigned _g )
		{
			g = _g;
		}

		void SETB( unsigned _b )
		{
			b = _b;
		}
	
		unsigned GETA() const /// when : const Color *p ; p->GETA(), the 'const' here is necessary for compile
		{
			return a;
		}

		unsigned GETR() const 
		{
			return r;
		}

		unsigned GETG() const 
		{
			return g;
		}

		unsigned GETB() const 
		{
			return b;
		}

	protected:
		unsigned a, r, g, b;
	};
}

#endif