/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef EDGE_APPLICATION_H
#define EDGE_APPLICATION_H

#include "EdgeWindow.h"
#include "EdgeSingleton.h"

namespace Edge
{
	/**
	 * This is a framework class to create a basic Edge Application. If you want to 
	 * use this framework, just inherit from this class.
	 *
	 */
	class Application : public Singleton<Application>, WindowListener
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Application();

		/**
		 * Destructor
		 *
		 */
		virtual ~Application();

		/**
		 * 
		 *
		 */
		bool _init();

		/**
		 *
		 *
		 */
		void _release();

		/**
		 * setConstFPS
		 *
		 */
		void setConstFPS( int fps );

		/**
		 *
		 *
		 */
		float getDeltaTime() ;

		/**
		 * get the real fps
		 *
		 */
		int getFPS();

		/**
		 *
		 *
		 */
		virtual bool run();

		/**
		 * preinit, you can use this function to modify some properties like window's style
		 *
		 */
		virtual void preinit() {  }

		/**
		 * custom init, put your initial code here
		 *
		 */
		virtual bool init()    { return true; }

		/**
		 * custorm release, put your relase code here, this function willnot called automatically,
		 * so call it yourself.
		 *
		 */
		virtual void release() {}

		/**
		 * update the logic, put your logic code truck here. If you want to exit, just return false.
		 *
		 */
		virtual bool update()  { return true; }

		/**
		 * render the scene,put your render code here
		 *
		 */
		virtual void render() ;

		/**
		 * before window is created, this function will be called
		 *
		 */
		void OnInit();

		/**
		 * when the window get focus, this function will be called.
		 *
		 */
		void OnGetFocus() ;

		/**
		 * when the window lost focus, this function will be called.
		 *
		 */
		void OnLostFocus() ;

	protected:
		/**
		 * load plugins
		 *
		 */
		void loadPlugins();

		/** 
		 * calcualte the fps
		 *
		 */
		void calFPS( unsigned long curTime, unsigned long dt );

	protected:
		/// default log file name
		string mLogFile;
		/// default plugin config file name, configs the plugins
		string mPluginFile;
		/// window's caption
		string mCaption;
		/// window's width
		int mWidth;
		/// windows' height
		int mHeight;
		/// whether windowed
		bool mbWindowed;
		/// depth
		int mDepth;
		/// whether can be suspend
		bool mbSuspend;
		/// whether the window is active
		bool mbActive;

		///	fps stuff
		int mConstFPS;
		int mRealFPS;
		/// last frame time
		unsigned long mLastTime;
		/// delta time
		unsigned long mDeltaTime;
		/// delay time when in the const fps mode
		unsigned long mDelayTime;

	};
}

#endif