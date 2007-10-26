/**
 *
 *
 */
#include "EdgeD3D8Image.h"
#include "EdgeException.h"
#include "EdgeD3D8ImageFactory.h"
#include "EdgeLogManager.h"

namespace Edge
{
	D3D8Image::D3D8Image( const string &name, ImageFactory *parent, Graphics *graphics, EImageType type ) :
		Image( name, parent, graphics,type ), m_pTexture( 0 ), m_pSurface( 0 )
	{
		D3D8Graphics *d3d8Graphics = static_cast<D3D8Graphics*>( graphics ); 
		mFormat = d3d8Graphics->m_screenFormat ;		
		m_pDevice = d3d8Graphics->m_pD3DDevice ;

		setQuadDefault();
	}

	D3D8Image::~D3D8Image()
	{
		release();
	}

	bool	D3D8Image::create( int width, int height )
	{
		mWidth = width; 
		mHeight = height;

		createD3DTexture( m_pDevice, width, height, mType, &m_pTexture );

		m_pTexture->GetSurfaceLevel( 0, &m_pSurface );

		mQuad.tex = m_pTexture;

		_getTextureSize( m_pTexture, &mOriWidth, &mOriHeight );

		setRect( 0, 0, static_cast<float>( mOriWidth ), static_cast<float>( mOriHeight ) );

		LogManager::getSingleton().logMessageF( LL_INFORMATION, "Image Size : %d x %d, D3D8 Texture Size : %d x %d", 
			width, height, mOriWidth, mOriHeight );

		return true;
	}

	bool	D3D8Image::create( ImageDataPtr imageData )
	{
		if( mType != IT_IMAGE )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Only IT_IMAGE type image can be created from a file",
				"D3D8Image::create" );
		}

		unsigned width = imageData->getWidth();
		unsigned height = imageData->getHeight();

		create( width, height );

		/// get ready to copy pixel data
		const int BYTES_PER_PIXEL = 4;
		unsigned char *pSrcBits = imageData->getData();
		unsigned char *pDestBits;
		D3DLOCKED_RECT rect;
			
		if( FAILED( m_pTexture->LockRect( 0, &rect, NULL, 0 ) ) )
		{
			EDGE_EXCEPT( Exception::EC_FAILED_LOCKTEXTURE, "Lock texture failed",
				"D3D8Image::create" );
		}
		
		pDestBits = (unsigned char*) rect.pBits ;

		for( unsigned y = 0; y < height; ++ y )
		{
			memcpy( pDestBits, pSrcBits, width * BYTES_PER_PIXEL );

			pSrcBits += width * BYTES_PER_PIXEL;
			pDestBits += rect.Pitch ;
		}

		m_pTexture->UnlockRect( 0 );

		return true;
	}

	Image  *D3D8Image::clone( const string &name, float x, float y, float w, float h )
	{
		D3D8Image *img = new D3D8Image( name, mParent, mGraphics, mType );
		img->m_pTexture = m_pTexture; 
		img->m_pSurface = m_pSurface;
		img->mWidth = mWidth;
		img->mHeight = mHeight;
		img->mOriWidth = mOriWidth;
		img->mOriHeight = mOriHeight;
		img->setHotSpot( x, y );
		img->setFlip( false,false );
		img->setRect( x, y, w, h );
		img->mQuad.tex = m_pTexture;

		/// addref
		m_pTexture->AddRef();
		m_pSurface->AddRef();

		return img;
	}

	void	D3D8Image::createD3DTexture( IDirect3DDevice8 *pDevice, int width, int height, 
		EImageType type, IDirect3DTexture8 **pOut )
	{
		DWORD usage;
		D3DPOOL pool;

		if( type == IT_TARGET )
		{
			usage = D3DUSAGE_RENDERTARGET;
			pool = D3DPOOL_DEFAULT;
		}
		else if( type == IT_IMAGE )
		{
			usage = 0;
			pool = D3DPOOL_MANAGED;
		}
		else
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Unsupport Image type!",
				"D3D8Image::createD3DTexture" );
		}

		if( FAILED( D3DXCreateTexture( pDevice, width, height, 
			1, 
			usage,
			D3DFMT_A8R8G8B8,   /// always 32-bits
			pool,
			pOut ) ) )
		{
			EDGE_EXCEPT( Exception::EC_CANNOT_LOAD_TEXTURE, "Create Texture failed",
				"D3D8Image::createD3DTexture" );
		}

	}

	void	D3D8Image::release()
	{
		if( D3D8ImageFactory::mAliveCount > 0 && mParent != 0 )
		{
			mParent->notifyDeath( this );
		}

		m_pSurface->Release();
		m_pTexture->Release();
	}

	void	D3D8Image::setRenderMode( int mode )
	{
		mQuad.mode = mode;
	}

	void	D3D8Image::setColor( const Color &color, int index )
	{
		D3DCOLOR d3dcolor = GET_D3DCOLOR( color );

		if( index == -1 )
		{
			mQuad.v[0].color = mQuad.v[1].color = mQuad.v[2].color = mQuad.v[3].color = d3dcolor; 
		}
		else
		{
			mQuad.v[index].color = d3dcolor;
		}
	}
 
	void	D3D8Image::render( float x, float y )
	{
		float tempx1, tempy1, tempx2, tempy2;

		tempx1 = x - mHotSpotX;
		tempy1 = y - mHotSpotY;
		tempx2 = x + mRegionW - mHotSpotX;
		tempy2 = y + mRegionH - mHotSpotY;

		mQuad.v[0].x = tempx1; mQuad.v[0].y = tempy1;
		mQuad.v[1].x = tempx2; mQuad.v[1].y = tempy1;
		mQuad.v[2].x = tempx2; mQuad.v[2].y = tempy2;
		mQuad.v[3].x = tempx1; mQuad.v[3].y = tempy2;

		mGraphics->renderImage( this );
	}

	void	D3D8Image::render( float x, float y, float rot, float hscale, float vscale )
	{
		float tx1, ty1, tx2, ty2;
		float sint, cost;

		if( !( vscale > 0.0f ) && !( vscale < 0.0f )  ) 
		{
			vscale = hscale;
		}

		tx1 = -mHotSpotX * hscale;
		ty1 = -mHotSpotY * vscale;
		tx2 = ( mRegionW - mHotSpotX ) * hscale;
		ty2 = ( mRegionH - mHotSpotY ) * vscale;

		if( rot > 0.0f || rot < 0.0f )
		{
			cost = cosf( rot );
			sint = sinf( rot );

			mQuad.v[0].x  = tx1 * cost - ty1 * sint + x;
			mQuad.v[0].y  = tx1 * sint + ty1 * cost + y;	

			mQuad.v[1].x  = tx2 * cost - ty1 * sint + x;
			mQuad.v[1].y  = tx2 * sint + ty1 * cost + y;	

			mQuad.v[2].x  = tx2 * cost - ty2 * sint + x;
			mQuad.v[2].y  = tx2 * sint + ty2 * cost + y;	

			mQuad.v[3].x  = tx1 * cost - ty2 * sint + x;
			mQuad.v[3].y  = tx1 * sint + ty2 * cost + y;	
		}
		else
		{
			mQuad.v[0].x = tx1 + x; mQuad.v[0].y = ty1 + y;
			mQuad.v[1].x = tx2 + x; mQuad.v[1].y = ty1 + y;
			mQuad.v[2].x = tx2 + x; mQuad.v[2].y = ty2 + y;
			mQuad.v[3].x = tx1 + x; mQuad.v[3].y = ty2 + y;
		}

		mGraphics->renderImage( this );
	}

	void	D3D8Image::renderStretch( float x1, float y1, float x2, float y2 )
	{
		mQuad.v[0].x = x1; mQuad.v[0].y = y1;
		mQuad.v[1].x = x2; mQuad.v[1].y = y1;
		mQuad.v[2].x = x2; mQuad.v[2].y = y2;
		mQuad.v[3].x = x1; mQuad.v[3].y = y2;

		mGraphics->renderImage( this );
	}

	void	D3D8Image::renderAt( float x0, float y0, float x1, float y1, 
							    float x2, float y2, float x3, float y3 )
	{
		mQuad.v[0].x = x0; mQuad.v[0].y = y0;
		mQuad.v[1].x = x1; mQuad.v[1].y = y1;
		mQuad.v[2].x = x2; mQuad.v[2].y = y2;
		mQuad.v[3].x = x3; mQuad.v[3].y = y3;

		mGraphics->renderImage( this );

	}

	void	D3D8Image::setRect( float x, float y, float w, float h )
	{
		Image::setRect( x, y, w, h );

		float tx1, ty1, tx2, ty2;
		bool bX,bY;
		
		tx1 = mRegionX / mOriWidth; 
		ty1 = mRegionY / mOriHeight;
		tx2 = ( mRegionX + w ) / mOriWidth; 
		ty2 = ( mRegionY + h ) / mOriHeight;

		mQuad.v[0].tx = tx1; mQuad.v[0].ty = ty1; 
		mQuad.v[1].tx = tx2; mQuad.v[1].ty = ty1; 
		mQuad.v[2].tx = tx2; mQuad.v[2].ty = ty2; 
		mQuad.v[3].tx = tx1; mQuad.v[3].ty = ty2; 

		bX = mXFlip; bY = mYFlip;
		mXFlip = false; mYFlip = false;
	
		setFlip( bX, bY );
	}

	void	D3D8Image::setHotSpot( float x, float y )
	{
		mHotSpotX = x;
		mHotSpotY = y;
	}

	void	D3D8Image::setFlip( bool bX, bool bY )
	{
		float tx, ty;

		if( bX != mXFlip )
		{
			tx = mQuad.v[0].tx; mQuad.v[0].tx = mQuad.v[1].tx; mQuad.v[1].tx = tx;
			ty = mQuad.v[0].ty; mQuad.v[0].ty = mQuad.v[1].ty; mQuad.v[1].ty = ty;
			tx = mQuad.v[3].tx; mQuad.v[3].tx = mQuad.v[2].tx; mQuad.v[2].tx = tx;
			ty = mQuad.v[3].ty; mQuad.v[3].ty = mQuad.v[2].ty; mQuad.v[2].ty = ty;
			mXFlip = !mXFlip;
		}

		if( bY != mYFlip )
		{
			tx = mQuad.v[0].tx; mQuad.v[0].tx = mQuad.v[3].tx; mQuad.v[3].tx = tx;
			ty = mQuad.v[0].ty; mQuad.v[0].ty = mQuad.v[3].ty; mQuad.v[3].ty = ty;
			tx = mQuad.v[1].tx; mQuad.v[1].tx = mQuad.v[2].tx; mQuad.v[2].tx = tx;
			ty = mQuad.v[1].ty; mQuad.v[1].ty = mQuad.v[2].ty; mQuad.v[2].ty = ty;
			mYFlip = !mYFlip;
		}
	}

	void	*D3D8Image::lock( int left, int top, int width, int height, bool bReadOnly )
	{
		if( mType != IT_IMAGE )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Only IT_IMAGE Image can be locked",
				"D3D8Image::lock" );
		}

		RECT rect;
		D3DLOCKED_RECT locked_rect;
		DWORD flage = 0;

		if( width != 0 && height != 0 )
		{
			rect.left = left;
			rect.top = top;
			rect.right = left + width;
			rect.bottom = top + height;
		}
		else
		{
			memset( &rect, 0, sizeof( RECT ) );
		}

		if( bReadOnly )
		{
			flage = D3DLOCK_READONLY;
		}

		if( FAILED( m_pTexture->LockRect( 0, &locked_rect, &rect, flage ) ) )
		{
			EDGE_EXCEPT( Exception::EC_FAILED_LOCKTEXTURE, "Lock texture failed",
				"D3D8Image::lock" );
		}

		return locked_rect.pBits;
	}

	void	D3D8Image::unlock()
	{
		m_pTexture->UnlockRect( 0 );
	}

	void	D3D8Image::OnLostDevice()
	{
		m_pTexture->Release();
		m_pSurface->Release();
	}

	void	D3D8Image::OnResetDevice()
	{
		create( mWidth, mHeight );
	}

	///////////////////////////////////////////////////////////////////////////////////
	void	D3D8Image::setQuadDefault()
	{
		for( int j = 0; j < 4; ++ j )
		{
			mQuad.v[j].rhw = 1.0f;
			mQuad.v[j].z = 1.0f; 
		}
		mQuad.v[0].x = 0;
		mQuad.v[0].y = 0;
		mQuad.v[0].color = 0xffffffff;
		mQuad.v[0].tx = 0;
		mQuad.v[0].ty = 0;

		mQuad.v[1].x = 0;
		mQuad.v[1].y = 0;
		mQuad.v[1].color = 0xffffffff;
		mQuad.v[1].tx = 1;
		mQuad.v[1].ty = 0;

		mQuad.v[2].x = 0;
		mQuad.v[2].y = 0;
		mQuad.v[2].color = 0xffffffff;
		mQuad.v[2].tx = 1;
		mQuad.v[2].ty = 1;

		mQuad.v[3].x = 0;
		mQuad.v[3].y = 0;
		mQuad.v[3].color = 0xffffffff;
		mQuad.v[3].tx = 0;
		mQuad.v[3].ty = 1;

		mQuad.tex = m_pTexture;
		mQuad.mode = RM_DEFAULT;		
	}

	void	D3D8Image::_getTextureSize( IDirect3DTexture8 *pTexture, int *w, int *h )
	{
		D3DSURFACE_DESC TDesc;
		if( FAILED( pTexture->GetLevelDesc( 0, &TDesc ) ) )
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "_getTextureSize failed in D3D8Image" );
			return ;
		}

		*w = TDesc.Width ;
		*h = TDesc.Height ;
	}
}