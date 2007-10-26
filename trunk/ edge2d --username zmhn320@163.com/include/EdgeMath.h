/**
 *
 *
 *
 */
#ifndef EDGE_MATH_H
#define EDGE_MATH_H

namespace Edge
{
	#define ROUNDING_ERROR_32 0.000001f

	inline bool equals( const float a, const float b, const float tolerance = ROUNDING_ERROR_32)
	{
		return ( a + tolerance > b ) && ( a - tolerance < b );
	}


}

#endif