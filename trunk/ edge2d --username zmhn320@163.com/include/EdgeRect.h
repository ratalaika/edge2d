/**
 *
 *
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