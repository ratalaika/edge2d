/**
 *
 *
 *
 */
#include "EdgeGUIButton.h"
#include "EdgeImage.h"
#include "EdgeFont.h"
#include <algorithm>

namespace Edge
{
	ButtonWidget::ButtonWidget( GUIWidget *parent, int id ) :
		GUIWidget( parent, id )
	{
		mState = BS_NORMAL;
		mbDown = false;
	}

	ButtonWidget::~ButtonWidget()
	{
	}

	void ButtonWidget::addListener( ButtonListener *listener )
	{
		mListeners.push_back( listener );
	}

	void ButtonWidget::removeListener( ButtonListener *listener )
	{
		ListenerList::iterator it = std::find( mListeners.begin(), mListeners.end(), listener );
		if( it != mListeners.end() )
		{
			mListeners.erase( it );
		}
	}

	void ButtonWidget::setImage( ButtonWidget::EBState state, Image *image, const fRect &rect )
	{
		mRes[state].state = state;
		mRes[state].image = image;

		if( equals( rect.mLeft, 0 ) && equals( rect.mRight, 0 ) && 
			equals( rect.mTop, 0 ) && equals( rect.mBottom, 0 ) )
		{
			mRes[state].rect.set( 0, 0, 1.0f * image->getWidth(), 1.0f * image->getHeight() );
		}
		else
		{
			mRes[state].rect = rect;
		}
	}

	void ButtonWidget::update( float dt )
	{
		
	}

	void ButtonWidget::render()
	{
		if( !mbVisible )
		{
			return ;
		}

		if( mRes[mState].image != 0 )
		{
			fRect &rect = mRes[mState].rect;
			Image *image = mRes[mState].image ;
			image->setRect( rect.mLeft, rect.mTop, rect.mRight - rect.mLeft, rect.mBottom - rect.mTop );
			image->render( mAbsDimension.mLeft, mAbsDimension.mTop );
		}
		else if( mRes[BS_NORMAL].image != 0 )
		{
			fRect &rect = mRes[BS_NORMAL].rect;
			Image *image = mRes[BS_NORMAL].image ;
			image->setRect( rect.mLeft, rect.mTop, rect.mRight - rect.mLeft, rect.mBottom - rect.mTop );
			image->render( mAbsDimension.mLeft, mAbsDimension.mTop );
		}

		/// render text in the center of the button
		if( mText.size() != 0 && mFont != 0 )
		{
			float cx = mAbsDimension.mLeft + ( mAbsDimension.mRight - mAbsDimension.mLeft ) / 2;
			float cy = mAbsDimension.mTop + ( mAbsDimension.mBottom - mAbsDimension.mTop ) / 2;
			
			mFont->render( cx, cy, mText, mTextColor, Font::ETA_CENTER | Font::ETA_MIDDLE );
		}
	}

	void ButtonWidget::mouseMove( const fPoint &pos )
	{
		if( !mAbsDimension.collide( pos.mX, pos.mY ) )
		{
			mState = BS_NORMAL;
		}
		else if( mState != BS_PUSH )
		{
			mState = BS_MOUSE_ON;
		}
	}

	void ButtonWidget::mouseLButton( const fPoint &pos, bool bdown )
	{
		if( !mAbsDimension.collide( pos.mX, pos.mY ) )
		{
			/// the mouse point is not in the button area
			return ;
		}

		if( bdown )
		{
			mState = BS_PUSH;
			mbDown = true;
		}
		else
		{
			mState = BS_MOUSE_ON;

			if( mbDown )
			{
				for( ListenerList::iterator it = mListeners.begin(); it != mListeners.end(); ++ it )
				{
					(*it)->ButtonClick( mID );
				}

				mbDown = false;
			}
		}
	}
}

