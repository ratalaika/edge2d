/**
 *
 *
 *
 */
#ifndef EDGE_FIIMAGECODEC_H
#define EDGE_FIIMAGECODEC_H

#include "EdgeImageCodec.h"
#include "EdgeSingleton.h"

namespace Edge
{
	/**
	 * implement an image codec, using FreeImage library. You can create your 
	 * own image codec.
	 *
	 */
	class FIImageCodec : public Singleton<FIImageCodec>, public ImageCodec
	{
	public:
		/**
		 * Constructor
		 *
		 */
		FIImageCodec();

		/**
		 * Destructor
		 *
		 */
		~FIImageCodec();

		/**
		 *
		 */
		bool	initiate();

		/**
		 *
		 */
		void	release();

		/**
		 *
		 */
		ImageDataPtr decode( DataStreamPtr stream );
	};
}

#endif