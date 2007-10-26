/**
 *
 *
 *
 */
#ifndef EDGE_GUIMANAGER_H
#define EDGE_GUIMANAGER_H

#include "EdgeSingleton.h"
#include <list>

using std::list;

namespace Edge
{
	class GUIWidget;

	/**
	 * GUI Manager will manage all the gui events and all the gui controls.
	 *
	 *
	 */
	class GUIManager : public Singleton<GUIManager>
	{
	public:
		/**
		 * gui widgets list
		 *
		 */
		typedef list<GUIWidget*> WidgetList;

	public:
		/**
		 * Constructor
		 *
		 */
		GUIManager();

		/**
		 * Destructor
		 *
		 */
		~GUIManager();

		/**
		 * initiate the gui 
		 *
		 */
		bool initiate();

		/**
		 * release
		 *
		 */
		void release();

		/**
		 * add a gui widget, you create the widget yourself ,so you should delete it yourself.
		 * add/remove widget only deal with the gui widget list.
		 *
		 */
		void addWidget( GUIWidget *widget );

		/**
		 * remove a gui widget from the list
		 *
		 */
		void removeWidget( GUIWidget *widget );

		/**
		 * set a gui widget to get the focus, you donot need to call this function. Just call
		 * GUIWidget::getFocus() function instead
		 *
		 */
		void setFocus( GUIWidget *widget );

		/**
		 * update gui system, this function will deal with gui events.And send the events to these
		 * gui widget who will deal with it.
		 */
		void update( float dt );

		/**
		 * render all the gui widgets
		 *
		 */
		void render();

	protected:
		/**
		 * mouse event
		 *
		 */
		void _processMouse( GUIWidget *widget, float mx, float my, float mz );

	protected:
		/// gui widgets list
		WidgetList mWidgets;
		/// the focus widget
		GUIWidget *mFocusWidget;
		
	};
}

#endif
