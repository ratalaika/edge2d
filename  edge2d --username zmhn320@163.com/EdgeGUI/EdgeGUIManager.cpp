/**
 *
 *
 *
 */
#include "EdgeGUIManager.h"
#include "EdgeGUIWidget.h"
#include "EdgeEngine.h"
#include "EdgeInputManager.h"
#include <algorithm>

namespace Edge
{
	GUIManager::GUIManager()
	{
		mFocusWidget = 0;
	}

	GUIManager::~GUIManager()
	{
		release();
	}

	bool GUIManager::initiate()
	{
		return true;
	}
	
	void GUIManager::release()
	{
	}

	void GUIManager::addWidget( GUIWidget *widget )
	{
		mWidgets.push_back( widget );
	}

	void GUIManager::removeWidget( GUIWidget *widget )
	{
		WidgetList::iterator it = std::find( mWidgets.begin(), mWidgets.end(), widget );
		if( it != mWidgets.end() )
		{
			mWidgets.erase( it );
		}
	}

	void GUIManager::setFocus( GUIWidget *widget )
	{
		mFocusWidget = widget;
	}

	void GUIManager::update( float dt )
	{
		GUIWidget *widget;
		float mx, my, mz;
		InputManager *im = EdgeEngine::getSingleton().getInputManager();

		for( WidgetList::iterator it = mWidgets.begin(); it != mWidgets.end(); ++ it )
		{
			/// update the widget
			(*it)->update( dt );

			/// mouse events
			widget = *it;
			mx = static_cast<float>( im->getMousePosX() );
			my = static_cast<float>( im->getMousePosY() );
			mz = static_cast<float>( im->getMousePosZ() );

			if( widget != 0 && widget->isEnable() && widget->isVisible() )
			{
				_processMouse( widget, mx, my, mz );
			}
		}

	}

	void GUIManager::render()
	{
		for( WidgetList::iterator it = mWidgets.begin(); it != mWidgets.end(); ++ it )
		{
			(*it)->render();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	void GUIManager::_processMouse( GUIWidget *widget, float mx, float my, float mz )
	{
		static float s_last_mx = mx;
		static float s_last_my = my;
		static float s_last_mz = mz;

		if( s_last_mx != mx || s_last_my != my )
		{
			widget->mouseMove( fPoint( mx, my ) );
		}
		if( s_last_mz != mz )
		{
			widget->mouseWheel( static_cast<int>( mz - s_last_mz ) );
		}
		
		InputManager *im = EdgeEngine::getSingleton().getInputManager();
		if( im->isKeyDown( Edge::KC_MOUSELEFT ) )
		{
			widget->mouseLButton( fPoint( mx, my ), true );
		}
		else
		{
			widget->mouseLButton( fPoint( mx, my ), false );
		}
		if( im->isKeyDown( Edge::KC_MOUSERIGHT ) )
		{
			widget->mouseRButton( fPoint( mx, my ), true );
		}
		else
		{
			widget->mouseRButton( fPoint( mx, my ), false );
		}
	}

}