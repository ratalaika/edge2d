/**
 *
 *
 *
 */
#include "EdgeAnimSprite.h"
#include "EdgeAnimSpriteFactory.h"
#include "EdgeArchiveManager.h"
#include "EdgeArchive.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"

namespace Edge
{
	AnimSpriteFactory::AnimSpriteFactory()
	{
	}

	AnimSpriteFactory::~AnimSpriteFactory()
	{
		destroyAllAnimSprites();
	}

	AnimSprite *AnimSpriteFactory::createAnimSprite( const string &file )
	{
		string cfile( file );
		modifyName( cfile );

		/// check whether the AnimSprite is already created
		AnimSprite *sprite = getAnimSprite( cfile ) ;
		if( sprite != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The AnimSprite : " + cfile + " is already created!" );
			return sprite;
		}

		string archiveName, fileName;

		size_t i = cfile.rfind( ':' );
		size_t size = cfile.size();
		archiveName.assign( cfile, 0, i );
		fileName.assign( cfile, i + 1, size - i );

		/// create a data stream
		Archive *archive = Edge::ArchiveManager::getSingletonPtr()->getArchive( archiveName );
		DataStreamPtr stream = archive->open( fileName );

		/// create an AnimSprite
		sprite = new AnimSprite( cfile );
		sprite->create( stream );

		mAnimSprites.insert( AnimSpriteList::value_type( cfile, sprite ) );

		return sprite;
	}

	AnimSprite *AnimSpriteFactory::getAnimSprite( const string &name )
	{
		string cname( name );
		modifyName( cname );

		AnimSpriteList::iterator it = mAnimSprites.find( cname );
		if( it == mAnimSprites.end() )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "Cannot find the AnimSprite : " + cname );
			return NULL;
		}

		return it->second;
	}

	void AnimSpriteFactory::destroyAnimSprite( const string &name )
	{
		string cname( name );
		modifyName( cname );

		AnimSpriteList::iterator it = mAnimSprites.find( cname );
		if( it != mAnimSprites.end() )
		{
			delete it->second ;
		}
	}

	void AnimSpriteFactory::destroyAnimSprite( AnimSprite *sprite )
	{
		destroyAnimSprite( sprite->getName() );
	}

	void AnimSpriteFactory::destroyAllAnimSprites()
	{
		for( AnimSpriteList::iterator it = mAnimSprites.begin(); it != mAnimSprites.end(); ++ it )
		{
			delete it->second;
		}
		mAnimSprites.clear();
	}

	void AnimSpriteFactory::modifyName( string &name )
	{
		/// make '\\' to '/'
		for( size_t i = 0; i < name.size(); ++ i )
		{
			if( name.at( i ) == '\\' )
			{
				name.at( i ) = '/';
			}
		}

		/// append './' if there isnot
		if( name.size() < 2 || ( !( name.at( 0 ) == '.' && name.at( 1 ) == '/' ) ) )
		{
			name.insert( 0, "./" );
		}
	}
}