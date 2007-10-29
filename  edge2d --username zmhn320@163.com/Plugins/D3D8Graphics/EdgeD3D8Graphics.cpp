/**
 *
 *
 */
#include "EdgeD3D8Graphics.h"
#include "EdgeException.h"
#include "EdgeD3D8Image.h"
#include "EdgeLogManager.h"
#include <algorithm>

#pragma comment( lib, "d3d8.lib" )
#pragma comment( lib, "d3dx8.lib" )

namespace Edge
{
	D3D8Graphics::D3D8Graphics()
	{
	}

	D3D8Graphics::~D3D8Graphics()
	{
		release();
	}

	void	D3D8Graphics::release()
	{
		m_pScreenTarget->Release();
		m_pIB->Release();
		m_pVB->Release();

		m_pD3DDevice->Release();
		m_pD3D->Release();

	}

	bool	D3D8Graphics::initiate( Window *window, int width, int height, bool bWindowed, int bits )
	{
		m_pWindow = window;

		window->getCustomData( "HANDLE", &m_hWnd );

		if( ( m_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) == NULL )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot create D3D8 object!",
				"D3D8Graphics::initiate" );
		}

		if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &m_desktopMode )))
		{	
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot determine the current display mode",
				"D3D8Graphics::initiate" );
		}

		_setPP( width, height, bWindowed, bits );

		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,              
			m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
			m_curPP, &m_pD3DDevice ) ) )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot create D3D8 device",
				"D3D8Graphics::initiate" );
		}
		
		/// log display information
		LogManager::getSingleton().logMessageF( LL_INFORMATION, 
			"Reset Display Mode : %d x %d x %s %s",  m_curPP->BackBufferWidth, m_curPP->BackBufferHeight, 
			_getFormatDesc( (D3DFORMAT)m_screenFormat ).c_str(),
			( bWindowed ? "Windowed":"Fullscreen" ) );

		_recreateScreenTarget();
		_setRenderState();

		if( !_createVertexData() )
		{
			return false;
		}

		return true;
	}

	void	D3D8Graphics::reset( int width, int height, bool bWindowed, int bits )
	{
		if( m_bRendering )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "You cannot reset display mode in beginScene/endScene",
				"D3D8Graphics::reset" );
		}

		_setPP( width, height, bWindowed, bits );
		_reset( true );
	
		/// log display information
		LogManager::getSingleton().logMessageF( LL_INFORMATION, 
			"Reset Display Mode : %d x %d x %s %s",  m_curPP->BackBufferWidth, m_curPP->BackBufferHeight, 
			_getFormatDesc( (D3DFORMAT)m_screenFormat ).c_str(), ( bWindowed ? "Windowed":"Fullscreen" ) );
	}

	bool	D3D8Graphics::beginScene()
	{
		m_bRendering = true;
		m_PrimiCount = 0;
		m_pD3DDevice->BeginScene();
		m_pVB->Lock( 0, 0, &m_pVertexData, 0 );

		return true;
	}

	void	D3D8Graphics::clear( const Color &color )
	{
		D3DCOLOR d3dcolor = GET_D3DCOLOR( color );

		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, d3dcolor, 1.0f, 0 );
	}

	void	D3D8Graphics::endScene()
	{
		_render_batch( true );

		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present( NULL, NULL, NULL, NULL );

		m_bRendering = false;
	}

	void	D3D8Graphics::setRenderTarget( Image *target )
	{
		D3D8Image *d3d8Target = static_cast<D3D8Image*>( target );

		if( d3d8Target == NULL )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Invalid render target", 
				"D3D8Graphics::setRenderTarget" );
		}
		
		if( d3d8Target->getType() != Image::IT_TARGET )
		{
			LogManager::getSingletonPtr()->logMessage( LL_WARNING,
				"Only IT_TARGET type image can be a render target." );
		}
		
		if( FAILED( m_pD3DDevice->SetRenderTarget( d3d8Target->getSurface(), NULL ) ) )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "SetRenderTarget Failed",
				"D3D8Graphics::setRenderTarget" );
		}
	}

	void	D3D8Graphics::restoreScreenTarget()
	{
		m_pD3DDevice->SetRenderTarget( m_pScreenTarget, NULL );
	}

	void	D3D8Graphics::renderLine( float x1, float y1, float x2, float y2, const Color &color )
	{
		if( m_curPrimi != PT_LINE || m_PrimiCount >= VERTEX_BUFFER_SIZE / 2 || 
			m_curTexture != 0 || m_curRenderMode != RM_DEFAULT )
		{
			_render_batch( false );
			m_curPrimi = PT_LINE;
			if( m_curRenderMode != RM_DEFAULT) 
			{
				_setRenderMode( RM_DEFAULT );
			}
			if( m_curTexture != NULL ) 
			{ 
				m_pD3DDevice->SetTexture( 0, 0 );
				m_curTexture = 0;
			}
		}

		D3DCOLOR col = GET_D3DCOLOR( color );
		Vertex data[] = {
			{ x1, y1, 1.0f, 1.0f, col, 0.0f, 0.0f },
			{ x2, y2, 1.0f, 1.0f, col, 0.0f, 0.0f }
		};

		memcpy( &m_pVertexData[ m_PrimiCount * 2 * sizeof( Vertex ) ], data, sizeof( Vertex ) * 2 );
		m_PrimiCount ++;
	}

	void	D3D8Graphics::renderRect( float left, float top, float right, float bottom, const Color &color )
	{
		renderLine( left, top, right, top, color );
		renderLine( right, top, right, bottom, color );
		renderLine( left, bottom, right, bottom, color );
		renderLine( left, top, left, bottom, color );
	}

	void	D3D8Graphics::renderQuad( const Quad &quad )
	{
		if( m_curPrimi != PT_QUAD || m_PrimiCount >= VERTEX_BUFFER_SIZE / 4 || 
			m_curTexture != quad.tex || m_curRenderMode != quad.mode )
		{
			_render_batch( false );

			m_curPrimi = PT_QUAD;
			if( m_curRenderMode != quad.mode ) 
			{
				_setRenderMode( quad.mode );
			}
			if( quad.tex != m_curTexture )
			{
				m_pD3DDevice->SetTexture( 0, quad.tex );
				m_curTexture = quad.tex;
			}
		}

		memcpy( &m_pVertexData[ m_PrimiCount * 4 * sizeof( Vertex ) ], quad.v, sizeof( Vertex ) * 4 );
		m_PrimiCount ++;
	}

	void	D3D8Graphics::renderImage( const Image *image )
	{
		const D3D8Image *d3d8image = static_cast<const D3D8Image*>( image );

		if( d3d8image == NULL )
		{
			EDGE_EXCEPT( Exception::EC_INVALIDPARAMS, "Invalid D3D8Image object",
				"D3D8Graphics::renderImage" );
		}

		renderQuad( d3d8image->getQuad() );
	}
    
	void	D3D8Graphics::renderFont()
	{
		/// to avoid some bugs here when using ID3DXFont font system.
		renderLine( -1, -1, -1, -1 );
	}

	string &D3D8Graphics::getType()
	{
		static string s_type = "D3D8";

		return s_type;
	}

	void	D3D8Graphics::getCustomData( const string &type, void *pOutData )
	{
		if( type == "DEVICE" )
		{
			IDirect3DDevice8 **device = (IDirect3DDevice8**) pOutData;
			*device = m_pD3DDevice;
		}
	}

	void	D3D8Graphics::addCustomData( const string &type, void *pInData )
	{
		if( type == "ON_LOST_FUNC" )
		{
			mOnLostFuncs.push_back( (OnLostFunc)pInData );
		}
		if( type == "ON_RESET_FUNC" )
		{
			mOnResetFuncs.push_back( (OnResetFunc)pInData );
		}
	}

	void	D3D8Graphics::removeCustomData( const string &type, void *pInData )
	{
		if( type == "ON_LOST_FUNC" )
		{
			mOnLostFuncs.erase( std::find( mOnLostFuncs.begin(), mOnLostFuncs.end(),
				(OnLostFunc)pInData ) );
		}
		if( type == "ON_RESET_FUNC" )
		{
			mOnResetFuncs.erase( std::find( mOnResetFuncs.begin(), mOnResetFuncs.end(),
				(OnResetFunc)pInData ) ) ;
		}
	}

	void	D3D8Graphics::OnGetFocus()
	{
		/// the device maybe lost, so rest the device and recreate some resources
		/// like render targets. etc
		LogManager::getSingleton().logMessage( LL_WARNING, "GotFocus...maybe reset the device" );
		_reset();		
	}

	void	D3D8Graphics::OnLostFocus()
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////
	void	D3D8Graphics::_setPP( int width, int height, bool bWindowed, int bits )
	{
		/// set Windowed PRESENT_PARAMETERS
		ZeroMemory( &m_PPW, sizeof( m_PPW ) );

		m_PPW.BackBufferFormat = m_desktopMode.Format ; 
		m_PPW.BackBufferCount = 1;  
		m_PPW.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_PPW.SwapEffect = D3DSWAPEFFECT_COPY; 
		m_PPW.BackBufferWidth = width;
		m_PPW.BackBufferHeight = height;
		m_PPW.hDeviceWindow = m_hWnd;
		m_PPW.Windowed = TRUE;

		/// set FullScreen PRESENT_PARAMTERS
		ZeroMemory( &m_PPFS, sizeof( m_PPFS ));

		D3DFORMAT fsformat = _findFSMode( width, height, bits );
		if( fsformat == D3DFMT_UNKNOWN )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot find appropriate full screen video mode",
				"D3D8Graphics::_setPP" );
		}

		m_PPFS.BackBufferWidth  = width;
		m_PPFS.BackBufferHeight = height;
		m_PPFS.BackBufferFormat = fsformat;
		m_PPFS.BackBufferCount  = 1;
		m_PPFS.MultiSampleType  = D3DMULTISAMPLE_NONE;
		m_PPFS.hDeviceWindow    = m_hWnd;
		m_PPFS.Windowed         = FALSE;
		m_PPFS.SwapEffect       = D3DSWAPEFFECT_COPY;//D3DSWAPEFFECT_FLIP;/// at present i donot know why, FLIP is not correct when render a render target
		m_PPFS.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_PPFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE ;

		if( bWindowed )
		{
			m_curPP = &m_PPW;
		}
		else
		{
			m_curPP = &m_PPFS;
		}
		
		m_screenFormat = m_curPP->BackBufferFormat;
	}
		
	void	D3D8Graphics::_reset( bool bResetMode )
	{
		if( !bResetMode && m_pD3DDevice->TestCooperativeLevel() != D3DERR_DEVICENOTRESET )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "No need to reset or Cannot be reset" );
			return;
		}
		/// notify the out world the lost message
		_notifyLost();
		
		/// release all the render targets
		m_pScreenTarget->Release();
		
		m_pD3DDevice->SetIndices( 0, 0 );
		m_pIB->Release();

		m_pD3DDevice->SetStreamSource( 0, NULL, sizeof( Vertex ) );
		m_pVB->Release();

		m_pD3DDevice->Reset( m_curPP );

		_recreateScreenTarget();
		_createVertexData();
		_setRenderState();

		/// notify the out world the reset message
		_notifyReset();
	}

	void	D3D8Graphics::_setRenderState()
	{
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_ALPHAREF,        0x01 );
		m_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

		/// to avoid some bugs on some vider card
		//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
		//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	}

	bool	D3D8Graphics::_createVertexData()
	{
		m_pD3DDevice->CreateVertexBuffer( VERTEX_BUFFER_SIZE * sizeof( Vertex ),
			D3DUSAGE_WRITEONLY,
			VERTEX_FVF,
			D3DPOOL_DEFAULT,
			&m_pVB );
		m_pD3DDevice->CreateIndexBuffer( VERTEX_BUFFER_SIZE * 6 / 4 * sizeof( WORD ),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&m_pIB );

		WORD *pIndices, n = 0;
		m_pIB->Lock( 0, 0, (BYTE**)&pIndices, 0 );
		for( int i = 0; i < VERTEX_BUFFER_SIZE / 4; ++ i )
		{
			*pIndices++ = n;
			*pIndices++ = n + 1;
			*pIndices++ = n + 2;
			*pIndices++ = n + 2;
			*pIndices++ = n + 3;
			*pIndices++ = n;
			n += 4;
		}
		m_pIB->Unlock();

		m_pD3DDevice->SetVertexShader( VERTEX_FVF );
		m_pD3DDevice->SetIndices( m_pIB, 0 );

		m_pD3DDevice->SetStreamSource( 0, m_pVB, sizeof( Vertex ) );

		m_curRenderMode = RM_DEFAULT;
		m_curPrimi = PT_LINE;
		m_curTexture = 0;
		m_pVertexData = 0;
		m_PrimiCount = 0;

		return true;
	}

	void	D3D8Graphics::_recreateScreenTarget()
	{
		/// save screen target
		m_pD3DDevice->GetRenderTarget( &m_pScreenTarget );
	}

	void	D3D8Graphics::_render_batch( bool bEnd )
	{
		if( m_PrimiCount == 0 )
		{
			return ; // no primitives to render
		}

		m_pVB->Unlock();

		if( m_curPrimi == PT_LINE )
		{
			m_pD3DDevice->DrawPrimitive( D3DPT_LINELIST, 0, m_PrimiCount );
		}
		if( m_curPrimi == PT_QUAD )
		{
			m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_PrimiCount << 2, 0, m_PrimiCount << 1 );
		}

		m_PrimiCount = 0;

		if( !bEnd )
		{
			/// for later use
			m_pVB->Lock( 0, 0, &m_pVertexData, 0 );
		}
	}

	void	D3D8Graphics::_setRenderMode( int mode )
	{
		if( ( mode & RM_ALPHABLEND ) != ( m_curRenderMode & RM_ALPHABLEND ) )
		{
			if( mode & RM_ALPHABLEND )
			{
				m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			}
			else
			{
				m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			}
		}

		if( ( mode & RM_COLORADD ) != ( m_curRenderMode & RM_COLORADD ) )
		{
			if( mode & RM_COLORADD )
			{
				m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_ADD );	
			}
			else
			{
				m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		
			}
		}

		m_curRenderMode = mode;
	}

	D3DFORMAT	D3D8Graphics::_findFSMode( int width, int height, int bits )
	{ 
		int value = 0;
		D3DDISPLAYMODE d3dMode;
		D3DFORMAT format = D3DFMT_UNKNOWN;

		unsigned modeCount = m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT );

		for( unsigned i = 0; i < modeCount; ++ i )
		{
			if( FAILED( m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, i, &d3dMode)))
				continue;
			if (d3dMode.Width != width || d3dMode.Height != height)
				continue;
			/*
			if( ( bits == 16 ) && ( !_is16Bits( d3dMode.Format ) ) )
				continue;
			if( ( bits == 32 ) && ( !_is32Bits( d3dMode.Format ) ) )
				continue;*/
			if( bits != _getDepth( d3dMode.Format ) )
				continue;
			int check_v = _checkFormat( d3dMode.Format );
			if( check_v > value )
			{
				format = d3dMode.Format ;
				value = check_v;
			}
		}

		return format;
	}

	int	D3D8Graphics::_checkFormat( D3DFORMAT format )
	{
		switch( format )
		{
		case D3DFMT_A1R5G5B5:
			return 3;
		case D3DFMT_X1R5G5B5:
			return 2;
		case D3DFMT_R5G6B5:
			return 1;
		case D3DFMT_X8R8G8B8:
			return 5;
		case D3DFMT_A8R8G8B8:
			return 6;
		case D3DFMT_R8G8B8:
			return 4;
			
		default:
			return 0;
		}
	}

	int D3D8Graphics::_getDepth( D3DFORMAT format )
	{
		switch( format )
		{
		case D3DFMT_A1R5G5B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_R5G6B5:
			return 16;
		case D3DFMT_X8R8G8B8:
		case D3DFMT_A8R8G8B8:
			return 32;
		case D3DFMT_R8G8B8:
			return 24;
			
		default:
			return 0;
		}
	}

	bool D3D8Graphics::_is32Bits( D3DFORMAT format )
	{
		return format == D3DFMT_X8R8G8B8 || 
			   format == D3DFMT_A8R8G8B8 ;
	}

	bool D3D8Graphics::_is16Bits( D3DFORMAT format )
	{
		return format == D3DFMT_A1R5G5B5 ||
			   format == D3DFMT_X1R5G5B5 ||
			   format == D3DFMT_R5G6B5;
	}

	string D3D8Graphics::_getFormatDesc( D3DFORMAT format )
	{
		switch( format )
		{
		case D3DFMT_A1R5G5B5:
			return "A1R5G5B5";
		case D3DFMT_X1R5G5B5:
			return "X1R5G5B5";
		case D3DFMT_R5G6B5:
			return "R5G6B5";
		case D3DFMT_X8R8G8B8:
			return "X8R8G8B8";
		case D3DFMT_A8R8G8B8:
			return "A8R8G8B8";
		case D3DFMT_R8G8B8:
			return "R8G8B8";
			
		default:
			return "UNKNOW_FORMAT";
		}
	}

	void	D3D8Graphics::_notifyLost()
	{
		/// notify other unknown DX objects
		for( size_t i = 0; i < mOnLostFuncs.size(); ++ i )
		{
			(*mOnLostFuncs.at( i ))();
		}
	}

	void	D3D8Graphics::_notifyReset()
	{
		/// notify other unknown DX objects
		for( size_t i = 0; i < mOnResetFuncs.size(); ++ i )
		{
			(*mOnResetFuncs.at( i ))();
		}
	}
}

