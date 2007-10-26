/**
 *
 *
 *
 */
#ifndef EDGE_ANIMSPRITE_FACTORY_H
#define EDGE_ANIMSPRITE_FACTORY_H

#include "EdgeSingleton.h"
#include <map>
#include <string>

using std::map;
using std::string;

namespace Edge
{
	class AnimSprite;

	/**
	 * AnimSpriteFactory, you can use this factory to create an anim-sprite for you.
	 *
	 */
	class AnimSpriteFactory : public Singleton<AnimSpriteFactory>
	{
	public:
		/**
		 * AnimSprite list
		 *
		 */
		typedef map<string, AnimSprite*> AnimSpriteList;

	public:
		/**
		 * Constructor
		 *
		 */
		AnimSpriteFactory();

		/**
		 * Destructor
		 *
		 */
		~AnimSpriteFactory();

		/**
		 * create an AnimSprite, it the sprite is already exist, return it.
		 *
		 */
		AnimSprite *createAnimSprite( const string &file );

		/**
		 * get an AnimSprite
		 *
		 */
		AnimSprite *getAnimSprite( const string &name );

		/**
		 * destroy an AnimSprite
		 *
		 */
		void destroyAnimSprite( const string &name );
		void destroyAnimSprite( AnimSprite *sprite );

		/**
		 * destroyAllSprite, called by destructor
		 *
		 */
		void destroyAllAnimSprites();

	private:
		void modifyName( string &name );
	private:
		/// anim sprite list
		AnimSpriteList mAnimSprites;
	};
}

#endif