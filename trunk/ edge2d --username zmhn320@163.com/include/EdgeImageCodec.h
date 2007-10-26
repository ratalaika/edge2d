/**
 *
 *
 *
 */
#ifndef EDGE_IMAGECODEC_H
#define EDGE_IMAGECODEC_H

#include "EdgeCompile.h"
#include <string>
#include "EdgeImageData.h"
#include "EdgeDataStream.h"

using std::string;

namespace Edge
{
	/**
	 * ImageCodec will use a image codec library and Edge's IO module to load an image, 
	 * and create an ImageData object to create an Image
	 */
	class EDGE_EXPORT ImageCodec
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ImageCodec() {}

		/**
		 * destructor
		 *
		 */
		virtual ~ImageCodec() {}

		/**
		 * initiate
		 *
		 */
		virtual bool	initiate() = 0;

		/**
		 * release
		 *
		 */
		virtual void	release() = 0;

		/**
		 * decode, it will decode an Image
		 *
		 */
		virtual ImageDataPtr	decode( DataStreamPtr stream ) = 0;
	};
}

#endif