/**
 *
 *
 *
 */
#include "EdgeGUIWidget.h"
#include "EdgeGUIManager.h"
#include <algorithm>

namespace Edge
{
	GUIWidget::GUIWidget( GUIWidget *parent, int id )
	{
		_setParent( parent );
		mID = id;
		mParent = 0;
		mFont = 0;
		mbEnable = true;
		mbVisible = true;
		mbFocused = false;

		/// automatically add itself to gui manager
		GUIManager::getSingleton().addWidget( this );
	}

	GUIWidget::~GUIWidget()
	{
		if( mParent )
		{
			mParent->_removeChild( this );
		}
		GUIManager::getSingleton().removeWidget( this );
	}

	GUIWidget *GUIWidget::getParent()
	{
		return mParent;
	}

	void GUIWidget::enable( bool benable )
	{
		if( mbEnable == benable )
		{
			return ;
		}

		mbEnable = benable;
		for( ChildList::iterator it = mChildren.begin(); it != mChildren.end(); ++ it )
		{
			(*it)->enable( mbEnable );
		}
	}

	bool GUIWidget::isEnable()
	{
		return mbEnable;
	}

	void GUIWidget::show( bool bshow )
	{
		if( mbVisible == bshow )
		{
			return;
		}

		mbVisible = bshow;
		for( ChildList::iterator it = mChildren.begin(); it != mChildren.end(); ++ it )
		{
			(*it)->show( mbVisible );
		}
	}

	bool GUIWidget::isVisible()
	{
		return mbVisible;
	}

	void GUIWidget::setFocus( bool bfocus )
	{
		if( mbFocused == bfocus )
		{
			return ;
		}
		mbFocused = bfocus;
		GUIManager::getSingleton().setFocus( this );
	}

	bool GUIWidget::isFocused()
	{
		return mbFocused;
	}

	void GUIWidget::setDimension( const fRect &rect )
	{
		if( mParent == 0 )
		{
			mAbsDimension = rect;
		}
		else
		{
			fRect &parent_rect = mParent->getAbsDimension();
			mAbsDimension.mLeft = parent_rect.mLeft + rect.mLeft;
			mAbsDimension.mTop = parent_rect.mTop + rect.mTop;
			mAbsDimension.mRight = parent_rect.mLeft + rect.mRight;
			mAbsDimension.mBottom = parent_rect.mTop + rect.mBottom;
		}
	}

	fRect GUIWidget::getDimension()
	{
		if( mParent == 0 )
		{
			return mAbsDimension;
		}
		else
		{
			fRect &parent_rect = mParent->getAbsDimension();
			fRect rect( mAbsDimension.mLeft - parent_rect.mLeft,
				        mAbsDimension.mTop - parent_rect.mTop,
						mAbsDimension.mRight - parent_rect.mLeft,
						mAbsDimension.mBottom - parent_rect.mTop );
			return rect;
		}
	}

	fRect GUIWidget::getAbsDimension()
	{
		return mAbsDimension;
	}

	void GUIWidget::setFont( Font *font )
	{
		mFont = font;
	}

	void GUIWidget::setTextColor( const Color &color )
	{
		mTextColor = color;
	}

	void GUIWidget::setText( const wstring &text )
	{
		mText = text;
	}

	const wstring &GUIWidget::getText()
	{
		return mText;
	}

	bool GUIWidget::isPointIn( float x,float y )
	{
		return mAbsDimension.collide( x, y );
	}

	///////////////////////////////////////////////////////////////////////////////////////
	void GUIWidget::_addChild( GUIWidget *child )
	{
		ChildList::iterator it = std::find( mChildren.begin(), mChildren.end(), child );
		if( it != mChildren.end() )
		{
			return ;
		}
		{		
			mChildren.push_back( child );
		}
	}

	void GUIWidget::_removeChild( GUIWidget *child )
	{
		ChildList::iterator it = std::find( mChildren.begin(), mChildren.end(), child );
		if( it != mChildren.end() )
		{
			if( (*it)->isFocused() )
			{
				GUIManager::getSingleton().setFocus( 0 );
			}

			mChildren.erase( it );
		}
	}

	void GUIWidget::_setParent( GUIWidget *parent )
	{
		if( parent != 0 )
		{
			mParent = parent;
			mParent->_addChild( this );
		}
	}

}