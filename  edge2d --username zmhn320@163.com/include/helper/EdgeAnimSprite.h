/**
 *
 *
 *
 */
#ifndef EDGE_ANIMSPRITE_H
#define EDGE_ANIMSPRITE_H

#include "EdgeDataStream.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

namespace Edge
{
	class Image;

	struct AnimInfo
	{
		char name[64];
		float speed;
		int play_count;
		int frame_count;
		int start_f, end_f;
	};

	struct FrameInfo
	{
		char file[256];
		float x, y, w, h;
	};

	/**
	 * AnimState
	 *
	 */
	class AnimState
	{
	public:
		/**
		 * animation resources
		 *
		 */
		struct AnimRes
		{
			AnimRes( Image *image = 0, float x = 0, float y = 0, float w = 0, float h = 0 ) :
				mImage( image ), mX( x ), mY( y ), mW( w ), mH( h )
			{
			}

			Image *mImage;
			float mX, mY, mW, mH;
		};
		
		/// resources container
		typedef vector<AnimRes*> AnimResList;		

	public:
		/**
		 * Construct a AnimState, reads data from buffer.
		 *
		 */
		AnimState( const AnimInfo *animInfo, const FrameInfo *frameInfo );
		
		/**
		 * Destructor
		 *
		 */
		~AnimState();

		/**
		 * update the animation, if finished return true
		 *
		 */
		bool update( float dt );

		/**
		 * normal render
		 *
		 */
		void render( float x, float y );

		/**
		 * reset the state
		 *
		 */
		void reset();

	public:
		/// anim name
		string mName;
		/// speed
		float mSpeed;
		/// play count
		int mPlayCount;
		/// frame count
		int mFrameCount;
		/// animation resources
		AnimResList mAnimRes;	
		/// current frame
		int mCurFrame;
		/// current play count
		int mCurCount;
		/// current time
		float mCurTime;
		/// whether the animation is finished
		bool mbFinished;
	};

	/**
	 * AnimSprite will manage a 2d frame animation.
	 *
	 *
	 */
	class AnimSprite
	{
	public:
		/**
		 * animation name list
		 *
		 */
		typedef vector<string> AnimNameList;
		
		/**
		 * animation state list
		 *
		 */
		typedef map<string, AnimState*> AnimStateList;

	public:
		/**
		 *
		 *
		 */
		AnimSprite( const string &name );

		/**
		 *
		 *
		 */
		~AnimSprite();

		/**
		 * create an animation sprite
		 *
		 */
		bool create( DataStreamPtr &stream );

		/**
		 * release, called by the destructor
		 *
		 */
		void release();

		/**
		 * get animation state name list
		 *
		 */
		AnimNameList &getAnimNameList();

		/**
		 * set current animation
		 *
		 */
		void setCurAnim( const string &name );

		/**
		 * set the current animation state's speed
		 *
		 * @param speed frame change time
		 */
		void setCurAnimSpeed( float speed );

		/**
		 * update the animation, it the animation is finished, it returns true
		 *
		 */
		bool update( float dt );

		/**
		 * render the animation, here i only provide a default rendering, if you want to
		 * do more complex rendering, you can access the current animation state's image.
		 *
		 */
		void render( float x, float y );

		/**
		 * get the current animation state's image, so you can render the animtion more complexly.
		 *
		 */
		Image *getCurFrameImage();

		/**
		 * get this animation sprite name
		 *
		 */
		string &getName() { return mName; }

	protected:
		/// animation states
		AnimStateList mAnimStates;
		/// animation name list
		AnimNameList mAnimNames;
		/// current animation
		AnimState *mCurAnimState;
		/// name
		string mName;
	};
}

#endif