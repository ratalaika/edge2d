/**
 *
 *
 */
#ifndef EDGE_OIS_INPUTMANAGER_H
#define EDGE_OIS_INPUTMANAGER_H

#include "EdgeSingleton.h"
#include "EdgeInputManager.h"
#include "OIS/include/OIS.h"

namespace OIS
{
	/** 
	 * an extend class to OIS::Mouse
	 *
	 */
	class MouseEx : public OIS::Mouse
	{
	public:
		void setMousePos( int x, int y )
		{
			mState.X.abs = x;
			mState.Y.abs = y;
		}

		void setRegion( int width, int height )
		{
			mState.width = width;
			mState.height = height;
		}
	};


}

namespace Edge
{
	/**
	 *
	 *
	 */
	class OISInputManager : public Singleton<OISInputManager>, public InputManager
	{
	public:
		/**
		 *
		 *
		 */
		OISInputManager();

		/**
		 *
		 */
		~OISInputManager();

		/**
		 *
		 */
		bool initiate( Window *window );

		/**
		 *
		 */
		void release();
		
		/**
		 *
		 */
		void reset( int width, int height );

		/**
		 * update
		 *
		 */
		void update();

		/**
		 *
		 *
		 */
		bool isKeyDown( EKeyCode key );
		
		/**
		 *
		 */
		bool isModifierKeyDown( EMoKeyCode key );

		/**
		 *
		 */
		void getMousePos( int *x, int *y, int *z );
		void setMousePos( int x, int y );

		int getMousePosX();
		int getMousePosY();
		int getMousePosZ();

	private:
		/// ois input manager
		OIS::InputManager *mOISIM;
		/// ois keyboard
		OIS::Keyboard *mKeyboard;
		/// ois mosue
		OIS::MouseEx *mMouse;
	};
}

#endif