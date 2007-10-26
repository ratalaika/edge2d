/**
 *
 *
 *
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