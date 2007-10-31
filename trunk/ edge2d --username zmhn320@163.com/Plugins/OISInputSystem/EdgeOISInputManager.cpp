/**
 *
 *
 */
#include "EdgeOISInputManager.h"
#include "EdgeWindow.h"
#include "EdgeLogManager.h"
#include <sstream>


namespace Edge
{
	OISInputManager::OISInputManager()
	{
	}

	OISInputManager::~OISInputManager()
	{
		release();
	}

	bool OISInputManager::initiate( Window *window )
	{
		size_t hWnd;
		window->getCustomData( "HANDLE", &hWnd );
		std::ostringstream windowHndStr;
		windowHndStr << hWnd ;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
		
		mOISIM = OIS::InputManager::createInputSystem( pl ); 

		mMouse = static_cast<OIS::MouseEx*>( mOISIM->createInputObject( OIS::OISMouse, false ) );
		mKeyboard = static_cast<OIS::Keyboard*>( mOISIM->createInputObject( OIS::OISKeyboard, false ) );

		mMouse->setRegion( window->getWidth(), window->getHeight() );

		return true;
	}

	void OISInputManager::reset( int width, int height )
	{
		mMouse->setRegion( width, height );
	}

	void OISInputManager::release()
	{
		mOISIM->destroyInputObject( mMouse );
		mOISIM->destroyInputObject( mKeyboard );
		OIS::InputManager::destroyInputSystem( mOISIM );	
	}

	void OISInputManager::update()
	{
		mMouse->capture();
		mKeyboard->capture();
	}

	bool OISInputManager::isKeyDown( EKeyCode key )
	{
		if( key > KC_MOUSEVALUE )
		{
			const OIS::MouseState &ms = mMouse->getMouseState();
			if( key == KC_MOUSELEFT )
				return ms.buttonDown( OIS::MB_Left ) ;
			if( key == KC_MOUSERIGHT )
				return ms.buttonDown( OIS::MB_Right ) ;
			if( key == KC_MOUSEMIDDLE )
				return ms.buttonDown( OIS::MB_Middle ) ;
		}
		else
		{
			return mKeyboard->isKeyDown( (OIS::KeyCode)key );
		}

		return false;
	}

	bool OISInputManager::isModifierKeyDown( EMoKeyCode key )
	{
		return mKeyboard->isModifierDown( (OIS::Keyboard::Modifier)key ); 
	}

	void OISInputManager::getMousePos( int *x, int *y, int *z )
	{
		const OIS::MouseState &ms = mMouse->getMouseState();

		*x = ms.X.abs ;
		*y = ms.Y.abs ;

		if( z != 0 )
		{
			*z = ms.Z.abs;
		}
	}

	void OISInputManager::setMousePos( int x, int y )
	{
		LogManager::getSingleton().logMessage( LL_WARNING, "In OISInputSystem, it does NOT support setMousePos" );

		mMouse->setMousePos( x, y );
	}

	int OISInputManager::getMousePosX()
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		return ms.X.abs ;
	}
	
	int OISInputManager::getMousePosY()
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		return ms.Y.abs ;
	}

	int OISInputManager::getMousePosZ()
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		return ms.Z.abs ;
	}

}
