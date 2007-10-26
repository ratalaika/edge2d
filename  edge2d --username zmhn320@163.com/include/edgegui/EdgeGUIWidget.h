/**
 *
 *
 *
 */
#ifndef EDGE_GUIWIDGET_H
#define EDGE_GUIWIDGET_H

#include "EdgeRect.h"
#include "EdgePoint.h"
#include "EdgeColor.h"
#include "EdgeInputManager.h"
#include <string>
#include <list>

using std::wstring;
using std::list;

namespace Edge
{
	class Font;

	/**
	 * the base class to implement a gui widget
	 *
	 *
	 */
	class GUIWidget
	{
	public:
		/**
		 * children list
		 *
		 */
		typedef list<GUIWidget*> ChildList;

	public:
		/**
		 * Constructor
		 *
		 */
		GUIWidget( GUIWidget *parent = 0, int id = 0 );

		/**
		 * Destructor
		 *
		 */
		virtual ~GUIWidget();

		/**
		 * update the widget, it will be called by the GUIManager
		 *
		 */
		virtual void update( float dt ) {}

		/**
		 * render the widget, it will be called by the GUIManager
		 *
		 */
		virtual void render() {}

		/**
		 * get the widget's parent, it maybe null.
		 *
		 */
		GUIWidget *getParent();

		/**
		 * enable/disable the widget
		 *
		 */
		void enable( bool benable );

		/**
		 * check whether the widget is enable
		 *
		 */
		bool isEnable();

		/**
		 * show/hide the widget
		 *
		 */
		void show( bool bshow );

		/**
		 * check whether the widget is visible
		 *
		 */
		bool isVisible();

		/**
		 * set this widget to get focus.
		 *
		 */
		void setFocus( bool bfocus );

		/**
		 * check whether the widget is focused
		 *
		 */
		bool isFocused();

		/**
		 * set dimension, it's relative the widget's parent.
		 *
		 */
		void setDimension( const fRect &rect );

		/**
		 * get dimension
		 *
		 */
		fRect getDimension();

		/**
		 * get absolute dimension
		 *
		 */
		fRect getAbsDimension();

		/**
		 * set a font, the widget will use this font to render text, it will not copy the font.
		 *
		 */
		void setFont( Font *font );

		/** 
		 * set the text color
		 *
		 */
		void setTextColor( const Color &color );

		/**
		 * set the text for this widget.
		 *
		 */
		void setText( const wstring &text );

		/**
		 * get the widget's text
		 *
		 */
		const wstring &getText();

		/**
		 * check whether a point is in the widget.
		 *
		 */
		bool isPointIn( float x, float y );

	public:
		/**
		 * mouse move event
		 *
		 */
		virtual void mouseMove( const fPoint &pos )  {}

		/**
		 * mouse button event
		 *
		 */
		virtual void mouseLButton( const fPoint &pos, bool bdown ) {}
		virtual void mouseRButton( const fPoint &pos,bool bdown ) {}
		virtual void mouseWheel( int offset ) {}

		/**
		 * key event
		 *
		 */
		virtual void keyClick( EKeyCode key ) {}


	protected:
		/**
		 * add a widget to this widget as a child. It only add the widget to the list
		 *
		 */
		void _addChild( GUIWidget *child );

		/**
		 * remove a child
		 *
		 */
		void _removeChild( GUIWidget *child );

		/**
		 * set a parent to this widget. it willnot add itself to the parent's list.
		 *
		 */
		void _setParent( GUIWidget *parent );

	protected:
		/// parent
		GUIWidget *mParent;
		/// children
		ChildList mChildren;
		/// font
		Font *mFont;
		/// text
		wstring mText;
		/// text color
		Color mTextColor;
		/// enable
		bool mbEnable;
		/// visible
		bool mbVisible;
		/// focused
		bool mbFocused;
		/// absolute dimension		
		fRect mAbsDimension;
		/// id
		int mID;
	};
}

#endif