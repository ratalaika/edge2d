/**
*
*
*
*/
#include "EdgeFadeImage.h"
#include "EdgeImage.h"
#include "EdgeGraphics.h"

namespace Edge
{
	FadeImage::FadeImage( Edge::Image *texture, 
		float x, float y, float speed, float delayTime ) 
	{
		mTexture = texture;
		mX = x; mY = y;
		mSpeed = speed;
		mAlpha = 1.0f;
		mState = FADE_OUT;
		mDelayTime = delayTime;
	}

	FadeImage::~FadeImage()
	{
	}

	bool	FadeImage::update( float dt )
	{
		if( mState == FINISHED )
		{
			return true;
		}

		if( mState == FADE_OUT )
		{
			mAlpha += mSpeed * dt;
			if( mAlpha > 250.0f )
			{
				mAlpha = 250.0f;
				mState = STAY;
				mStayTime = mDelayTime;
			}
		}
		else if( mState == FADE_IN )
		{
			mAlpha -= mSpeed * 2 * dt;
			if( mAlpha < 5.0f )
			{
				mState = FINISHED;
				return true;
			}			
		}
		else if( mState == STAY )
		{
			mStayTime -= dt;
			if( mStayTime < 0.0f )
			{
				mState = FADE_IN;
			}
		}

		int curAlpha = static_cast<int>( mAlpha );

		mTexture->setColor( ARGB( curAlpha, 255, 255, 255 ) );

		return false;
	}

	void	FadeImage::render()
	{
		mTexture->render( mX, mY );
	}

	void	FadeImage::reset()
	{
		mAlpha = 1.0f;
		mState = FADE_OUT;
	}

}