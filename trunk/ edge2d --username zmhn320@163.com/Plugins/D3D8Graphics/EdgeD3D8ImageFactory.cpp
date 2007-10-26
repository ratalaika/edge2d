/**
 *
 *
 */
#include "EdgeD3D8ImageFactory.h"
#include "EdgeD3D8Image.h"
#include "EdgeD3D8Graphics.h"
#include "EdgeArchiveManager.h"
#include "EdgeEngine.h"
#include "EdgeImageCodec.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"
#include <algorithm>

namespace Edge
{
	
	int D3D8ImageFactory::mAliveCount = 0;

	D3D8ImageFactory::D3D8ImageFactory() :
		ImageFactory()
	{
		mAliveCount ++;
	}
	
	D3D8ImageFactory::~D3D8ImageFactory()
	{
		mAliveCount --;
		release();
	}

	bool	D3D8ImageFactory::initiate()
	{
		Graphics *graphics = EdgeEngine::getSingleton().getGraphics();

		/// add callback functions to deal with the device lost stuff
		graphics->addCustomData( "ON_LOST_FUNC", OnLostDevice );
		graphics->addCustomData( "ON_RESET_FUNC", OnResetDevice );

		return true;
	}

	void	D3D8ImageFactory::release()
	{
		Graphics *graphics = EdgeEngine::getSingleton().getGraphics();

		/// remove the callback functions
		graphics->removeCustomData( "ON_LOST_FUNC", OnLostDevice );
		graphics->removeCustomData( "ON_RESET_FUNC", OnResetDevice );
	}

	Image		*D3D8ImageFactory::createImage( const string &file )
	{
		string cfile( file );
		modifyName( cfile );

		/// check whether the image is already created
		Image *image = getImage( cfile ) ;
		if( image != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The image : " + cfile + " is already created!" );
			return image;
		}

		string archiveName, fileName;

		size_t i = cfile.rfind( ':' );
		size_t size = cfile.size();
		archiveName.assign( cfile, 0, i );
		fileName.assign( cfile, i + 1, size - i );

		Archive *archive = ArchiveManager::getSingletonPtr()->getArchive( archiveName );
		DataStreamPtr stream = archive->open( fileName );
		
		ImageCodec *imageCodec = EdgeEngine::getSingletonPtr()->getImageCodec();
		ImageDataPtr imageData = imageCodec->decode( stream );

		image = new D3D8Image( cfile, this, D3D8Graphics::getSingletonPtr() );
		image->create( imageData );

		saveImage( cfile, image );

		return image;
	}

	Image		*D3D8ImageFactory::createImage( const string &name, int width, int height )
	{
		string cname( name );
		modifyName( cname );

		Image *image = getImage( cname ) ;
		if( image != NULL )
		{
			return image;
		}

		D3D8Image *d3dimage = new D3D8Image( cname, this, D3D8Graphics::getSingletonPtr() );
		d3dimage->create( width, height );
		saveImage( cname, d3dimage );
		return d3dimage;
	}

	Image		*D3D8ImageFactory::copyImage( const string &name, const string &origin, float x, float y, float w, float h )
	{
		string cfile( origin );
		modifyName( cfile );
		
		/// get the original image
		Image *image = getImage( cfile ) ;
		if( image == NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "Want to copy an image, but the original image is not exist. Try to create it!");
			image = createImage( origin );
		}

		string cname( name );
		modifyName( cname );

		Image *new_img = image->clone( cname, x, y, w, h );

		saveImage( cname, new_img );

		return image;
	}

	Image		*D3D8ImageFactory::createTarget( const string &name, int width, int height )
	{
		D3D8Image *d3dimage = new D3D8Image( name, this, D3D8Graphics::getSingletonPtr(), Image::IT_TARGET );
		d3dimage->create( width, height );

		/// save the target information, so later its d3d resources can be restore
		mTargets.push_back( d3dimage );

		saveImage( name, d3dimage );
		return d3dimage;
	}

	void D3D8ImageFactory::notifyDeath( const Image *image )
	{
		if( image->getType() == Image::IT_TARGET )
		{
			mTargets.erase( std::find( mTargets.begin(), mTargets.end(), image ) );
		}
	}

	void D3D8ImageFactory::OnLostDevice()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, "D3D8ImageFactory::OnLostDevice" );

		TargetList &targets = D3D8ImageFactory::getSingleton().getTargetList();
		for( size_t i = 0; i < targets.size(); ++ i )
		{
			targets.at( i )->OnLostDevice();
		}
	}

	void D3D8ImageFactory::OnResetDevice()
	{
		LogManager::getSingleton().logMessage( LL_INFORMATION, "D3D8ImageFactory::OnResetDevice" );

		TargetList &targets = D3D8ImageFactory::getSingleton().getTargetList();
		for( size_t i = 0; i < targets.size(); ++ i )
		{
			targets.at( i )->OnResetDevice();
		}
	}
}
