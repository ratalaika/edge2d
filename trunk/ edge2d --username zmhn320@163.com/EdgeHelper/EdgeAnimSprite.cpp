/**
 *
 *
 *
 */
#include "EdgeException.h"
#include "EdgeAnimSprite.h"
#include "EdgeImage.h"
#include "EdgeImageFactory.h"
#include "EdgeEngine.h"
#include "EdgeLogManager.h"
#include <stdio.h>
#include <algorithm>
#include <sstream>

namespace Edge
{
	////////////////////////////////////////////////////////////////////////////////
	//
	//	AnimState
	//
	////////////////////////////////////////////////////////////////////////////////
	AnimState::AnimState( const AnimInfo *animInfo, const FrameInfo *frameInfo ) :
		mName( animInfo->name )
	{
		mSpeed = animInfo->speed;
		mPlayCount = animInfo->play_count;
		mFrameCount = animInfo->frame_count ;
		mCurFrame = 0;
		mCurCount = 0;
		mCurTime = 0.0f;
		mbFinished = false;

		int start_f = animInfo->start_f ;
		int end_f = animInfo->end_f ;
		AnimRes *animRes;
		ImageFactory *fac = EdgeEngine::getSingleton().getImageFactory();

		for( int f = start_f; f <= end_f; ++ f )
		{
			animRes = new AnimRes( fac->createImage( frameInfo[f].file ), 
				frameInfo[f].x, frameInfo[f].y, frameInfo[f].w, frameInfo[f].h );
			mAnimRes.push_back( animRes );
		}
	}

	AnimState::~AnimState()
	{
		for( AnimResList::iterator it = mAnimRes.begin(); it != mAnimRes.end(); ++ it )
		{
			delete (*it);
		}
	}

	bool AnimState::update( float dt )
	{
		if( mbFinished )
		{
			return true;
		}

		mCurTime += dt;
		if( mCurTime >= mSpeed )
		{
			mCurTime = 0.0f;
			mCurFrame ++;
			if( mCurFrame >= mFrameCount )
			{
				if( mPlayCount != -1 )
				{
					mCurCount ++;
					if( mCurCount >= mPlayCount )
					{
						mbFinished = true;
					}
				}
				mCurFrame = 0;
			}
		}

		return mbFinished;
	}

	void AnimState::render( float x, float y )
	{
		AnimRes *res = mAnimRes.at( mCurFrame );

		res->mImage->setRect( res->mX, res->mY, res->mW, res->mH );
		res->mImage->render( x, y );
	}

	void AnimState::reset()
	{
		mCurTime = 0.0f;
		mbFinished = false;
		mCurFrame = 0;
		mCurCount = 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	//	AnimSprite
	//
	////////////////////////////////////////////////////////////////////////////////
	

	AnimSprite::AnimSprite( const string &name ) : mName( name )
	{
		mCurAnimState = 0;
	}

	AnimSprite::~AnimSprite()
	{
		release();
	}

	bool AnimSprite::create( DataStreamPtr &stream )
	{
		size_t size = stream->getSize();
		char *buf = new char [size];
		stream->read( buf, size );

		int animCount, frameCount;
		AnimInfo *anims;
		FrameInfo *frames;

		/// read animation info and frame info
		sscanf( buf, "%d%d", &animCount, &frameCount );
	
		anims = new AnimInfo [animCount];
		frames = new FrameInfo [frameCount];

		int offset = sizeof( int ) * 2 + 2;
		memcpy( anims, buf + offset, sizeof( AnimInfo ) * animCount );
		offset += sizeof( AnimInfo ) * animCount;

		memcpy( frames, buf + offset, sizeof( FrameInfo ) * frameCount );

		delete [] buf;

		/// ready to create AnimState
		AnimState *state;
		for( int i = 0; i < animCount; ++ i )
		{
			state = new AnimState( &anims[i], frames );
			mAnimStates.insert( std::make_pair( state->mName, state ) );
			mAnimNames.push_back( state->mName );
		}

		if( mAnimNames.size() != 0 && mCurAnimState == NULL )
		{
			setCurAnim( mAnimNames.at( 0 ) );
		}

		delete [] anims;
		delete [] frames;

		return true;
	}

	void AnimSprite::release()
	{
		for( AnimStateList::iterator it = mAnimStates.begin(); it != mAnimStates.end(); ++ it )
		{
			delete it->second;
		}

		mAnimStates.clear();
	}

	AnimSprite::AnimNameList &AnimSprite::getAnimNameList()
	{
		return mAnimNames;
	}

	void AnimSprite::setCurAnim( const string &name )
	{
		AnimStateList::iterator it = mAnimStates.find( name );
		if( it == mAnimStates.end() )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot find the animation state : " + name,
				"AnimSprite::setCurAnim" );
		}

		mCurAnimState = it->second ;
		/// reset it
		mCurAnimState->reset();
	}

	void AnimSprite::setCurAnimSpeed( float speed )
	{
		mCurAnimState->mSpeed = speed;
	}

	bool AnimSprite::update( float dt )
	{
#ifdef EDGE_DEBUG
		if( mCurAnimState == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "The current animation state is NULL",
				"AnimSprite::update" );
		}
#endif
		
		return mCurAnimState->update( dt );
	}

	void AnimSprite::render( float x, float y )
	{
#ifdef EDGE_DEBUG
		if( mCurAnimState == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "The current animation state is NULL",
				"AnimSprite::render" );
		}
#endif
		mCurAnimState->render( x, y );
	}

	Image *AnimSprite::getCurFrameImage()
	{
		return mCurAnimState->mAnimRes.at( mCurAnimState->mCurFrame )->mImage;
	}
}
