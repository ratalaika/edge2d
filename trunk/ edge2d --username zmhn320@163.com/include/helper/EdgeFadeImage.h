/**
*
*
*
*/
#ifndef EDGE_FADE_IMAGE_H_
#define EDGE_FADE_IMAGE_H_

namespace Edge
{
	class Image;

	/**
	* FadeImage
	*
	* implements an effect that will fade out an image and later fade it out
	*/
	class FadeImage 
	{
	public:
		enum
		{
			FADE_OUT, 
			FADE_IN,
			STAY,
			FINISHED
		};
	public:
		FadeImage( Edge::Image *texture, float x, float y, float speed,
			float delayTime );
		virtual ~FadeImage();

		virtual bool	update( float dt );
		virtual void	render();
		virtual void	reset();
		int 	getState() { return mState; }
	protected:
		Edge::Image *mTexture;
		float mX, mY;
		float mSpeed;
		float mAlpha;
		int mState;
		float mStayTime;
		float mDelayTime;
	};
}

#endif //end _FADE_IMAGE_H_
