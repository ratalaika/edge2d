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
#ifndef EDGE_POINT_H
#define EDGE_POINT_H

namespace Edge
{
	/**
	 * point class
	 *
	 */
	template <typename T>
	class point
	{
	public:
		/** 
		 * Constructors
		 *
		 */
		point() : mX( 0 ), mY( 0 ) {}
		point( T x, T y ) : mX( x ), mY( y ) {}
		point( const point<T> &other ) 
		{
			mX = other.mX; mY = other.mY;
		}

		/**
		 * operator = 
		 *
		 */
		const point<T> &operator = ( const point<T> &other )
		{
			mX = other.mX; mY = other.mY;
			return *this;
		}

		/**
		 * operator == 
		 *
		 */
		bool operator == ( const point<T> &other )
		{
			return mX == other.mX && mY == other.mY ;
		}

		/**
		 * operator !=
		 *
		 */
		bool operator != ( const point<T> &other )
		{
			return mX != other.mX || mY != other.mY;
		}

	public:
		T mX, mY;
	};

	typedef point<int> iPoint;
	typedef point<float> fPoint;
}

#endif