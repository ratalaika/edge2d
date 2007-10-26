/**
 *
 *
 */
#include "EdgeLogManager.h"
#include "EdgeFIImageCodec.h"
#include "FreeImage/FreeImage.h"

#ifdef _DEBUG
#pragma comment( lib, "../Plugins/FIImageCodec/FreeImage/FreeImaged.lib" )
#else
#pragma comment( lib, "../Plugins/FIImageCodec/FreeImage/FreeImage.lib" )
#endif

namespace Edge
{
	/// to catch the message sent by FreeImage library
	void FreeImageErrorHandler( FREE_IMAGE_FORMAT fif, const char *message )
	{
		LogManager::getSingleton().logMessage( LL_WARNING, "FreeImage Info : " + 
			string( message ) );
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FIImageCodec::FIImageCodec()
	{
	}

	FIImageCodec::~FIImageCodec()
	{
		release();
	}

	bool	FIImageCodec::initiate()
	{
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise( true );
#endif
		FreeImage_SetOutputMessage( FreeImageErrorHandler );

		return true;
	}

	void	FIImageCodec::release()
	{
#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif
	}

	ImageDataPtr FIImageCodec::decode( DataStreamPtr stream )
	{
		/// read all the data
		size_t size = stream->getSize();
		unsigned char *data = new unsigned char [size];
		stream->read( data, size );

		/// create a FreeImage stream
		FIMEMORY *mem = FreeImage_OpenMemory( data, size );
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileTypeFromMemory( mem, 0 );

		FIBITMAP *dib = FreeImage_LoadFromMemory( fifmt, mem, 0 );

		FreeImage_CloseMemory( mem );

		delete [] data;
		
		//// get the image's information
		unsigned width = FreeImage_GetWidth( dib );
		unsigned height = FreeImage_GetHeight( dib );
		unsigned bpp = FreeImage_GetBPP( dib );
		unsigned pitch = FreeImage_GetPitch( dib );

		/// log informations
		LogManager::getSingletonPtr()->logMessageF( LL_INFORMATION, 
			"Load Image : %s : %d * %d * %d", stream->getName().c_str(), width, height, bpp );
		
		/// and now, we have a FIBITMAP, we can access the pixel data in it
		/// if the image isnot 32-bits, convert it
		if( bpp != 32 )
		{
			dib = FreeImage_ConvertTo32Bits( dib );
			/// get the new Image's information
			pitch = FreeImage_GetPitch( dib );
		}

		/// create an ImageData object to store the image's data
		ImageData *imageData = new ImageData();
		imageData->create( width, height );

		/// copy image data
		const int BYTES_PER_PIXEL = 4;
		unsigned char *pSrcBits;
		unsigned char *pDestBits;
		unsigned bits = 0;
		unsigned dest = 0;

		pSrcBits = FreeImage_GetBits( dib );
		pDestBits = imageData->getData();	
		
		for( int y = height - 1; y >= 0; -- y )
		{
			memcpy( &pDestBits[ y * width * BYTES_PER_PIXEL ], &pSrcBits[ bits ], 
				width * BYTES_PER_PIXEL );

			bits += pitch;
		}

		/// ok, free something
		FreeImage_Unload( dib );

		return ImageDataPtr( imageData );
	}
}
