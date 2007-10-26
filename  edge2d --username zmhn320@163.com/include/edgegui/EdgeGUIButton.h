/**
 *
 *
 *
 */
#ifndef EDGE_GUIBUTTON_H
#define EDGE_GUIBUTTON_H

#include "EdgeGUIWidget.h"
#include "EdgeRect.h"
#include <vector>

using std::vector;

namespace Edge
{
	class Image;

	/**
	 * button event listener
	 *
	 */
	class ButtonListener
	{
	public:
		/// button click
		virtual void ButtonClick( int id ) {}
	};

	/**
	 * an image button
	 *
	 */
	class ButtonWidget : public GUIWidget
	{
	public:
		/**
		 * listener list
		 *
		 */
		typedef vector<ButtonListener*> ListenerList;

		/**
		 * button state
		 *
		 */
		enum EBState
		{
			BS_NORMAL,
			BS_MOUSE_ON,
			BS_PUSH,
			BS_DISABLED,
			BS_STATE_COUNT = 4
		};

		/**
		 * button resources
		 *
		 */
		struct ButtonRes
		{
			EBState state;
			Image *image;
			fRect rect;

			ButtonRes( EBState s = BS_NORMAL, Image *img = 0, const fRect &r = fRect( 0, 0, 0, 0 ) ) :
				state( s ), image( img ), rect( r )
			{
			}
		};

	public:
		/**
		 * Constructor
		 *
		 */
		ButtonWidget( GUIWidget *parent = 0, int id = 0 );

		/**
		 * Destructor
		 *
		 */
		virtual ~ButtonWidget();
		
		/**
		 * add a listener
		 *
		 */
		void addListener( ButtonListener *listener );

		/**
		 * remove a listener
		 *
		 */
		void removeListener( ButtonListener *listener );

		/**
		 * set the button images, it will not copy the image
		 *
		 */
		void setImage( EBState state, Image *image, const fRect &rect = fRect( 0, 0, 0, 0 ) );

		/**
		 *
		 *
		 */
		void update( float dt );

		/**
		 *
		 *
		 */
		void render();

		/**
		 * mouse move event
		 *
		 */
		virtual void mouseMove( const fPoint &pos );

		/**
		 * mouse button event
		 *
		 */
		virtual void mouseLButton( const fPoint &pos, bool bdown );

	protected:
		/// button resources
		ButtonRes mRes[BS_STATE_COUNT];
		/// listeners
		ListenerList mListeners;
		/// button state
		EBState mState;
		/// has been down
		bool mbDown;
	};
}

#endif