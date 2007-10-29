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
#ifndef EDGE_RECT_H
#define EDGE_RECT_H

#include "EdgeMath.h"

namespace Edge
{
	/**
	 * implement a simple rect class
	 *
	 *
	 */
	template <typename T>
	class rect
	{
	public:
		/**
		 * Constructors
		 *
		 */
		rect() : mLeft( 0 ), mTop( 0 ), mRight( 0 ), mBottom( 0 ) {}
		rect( T left, T top, T right, T bottom ) : mLeft( left ), mTop( top ), mRight( right ), mBottom( bottom ) {}
		rect( const rect<T> &other ) 
		{
			mLeft = other.mLeft; mTop = other.mTop; mRight = other.mRight; mBottom = other.mBottom ;
		}

		/**
		 * check whether the two rects is the same
		 *
		 */
		bool operator == ( const rect<T> &other )
		{
			return mLeft == other.mLeft &&
				   mTop == other.mTop &&
				   mRight == other.mRight &&
				   mBottom == other.mBottom;
		}

		/**
		 * is not the same
		 *
		 */
		bool operator != ( const rect<T> &other )
		{
			return mLeft != other.mLeft ||
				   mTop != other.mTop ||
				   mRight != other.mRight ||
				   mBottom != other.mBottom;
		}

		/**
		 * operator =
		 *
		 */
		const rect<T> &operator = ( const rect<T> &other )
		{
			mLeft = other.mLeft; mTop = other.mTop; mRight = other.mRight; mBottom = other.mBottom ;

			return *this;
		}

		/** 
		 * collide with a point,if the point is in the rect, retrun true
		 *
		 */
		bool collide( const T &x, const T &y )
		{
			return x >= mLeft && x <= mRight && y >= mTop && y <= mBottom;
		}

		/**
		 * collide with a rect, if these two rects have intersects, return true
		 *
		 */
		bool collide( const rect<T> &other )
		{
			return mTop > other.mTop &&
				   mBottom < other.mBottom &&
				   mLeft > other.mLeft &&
				   mRight < other.mRight ;
		}

		/**
		 * get width
		 *
		 */
		T getWidth() 
		{
			return mRight - mLeft;
		}

		/**
		 * get height
		 *
		 */
		T getHeight()
		{
			return mBottom - mTop;
		}

		/**
		 *
		 */
		void set( T left, T top, T right, T bottom )
		{
			mLeft = left; mTop = top; mRight = right; mBottom = bottom;
		}

	public:
		T mLeft, mTop, mRight, mBottom;
	};

	typedef rect<int> iRect;
	typedef rect<float> fRect;
}

#endif