/**
 *
 *
 *
 */
#include "EdgeResourceManager.h"
#include "EdgeSoundFactory.h"
#include "EdgeImageFactory.h"
#include "EdgeFont.h"
#include "EdgeAnimSpriteFactory.h"
#include "EdgeEngine.h"
#include "EdgeArchiveManager.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"
#include "tinyxml.h"

namespace Edge
{
	ResourceManager::ResourceManager()
	{
		mXmlDoc = 0;
	}

	ResourceManager::~ResourceManager()
	{
		release();
	}

	bool ResourceManager::initiate( const string &file )
	{
		mXmlDoc = new TiXmlDocument();
		if( !mXmlDoc->LoadFile( file.c_str() ) )
		{
			EDGE_EXCEPT( Exception::EC_GENERAL, "Load resource config file failed : " + file,
				"ResourceManager::initiate" );
		}

		/// parse groups
		TiXmlElement *root = mXmlDoc->RootElement();  
		TiXmlElement *group = root->FirstChildElement() ;
		string id;

		while( group != 0 )
		{
			id = group->Attribute( "id" );
			if( id == "" )
			{
				EDGE_EXCEPT( Exception::EC_GENERAL, "Invalid Resource File, check ResourcesGroup element!",
					"ResourceManager::initiate" );
			}

			mResGroup[id] = group;

			group = group->NextSiblingElement();
		}

		return true;
	}

	void ResourceManager::release()
	{
		if( mXmlDoc != 0 )
		{
			mXmlDoc->Clear();
			delete mXmlDoc;
			mXmlDoc = 0;
		}
	}

	bool ResourceManager::loadGroup( const string &id )
	{
		ResGroup::iterator it = mResGroup.find( id );
		if( it == mResGroup.end() )
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Cannot find ResourcesGroup : " + id );
			return false;
		}

		EdgeEngine *engine = EdgeEngine::getSingletonPtr();
		ArchiveManager *am = engine->getArchiveManager();
		ImageFactory *ifac = engine->getImageFactory();
		SoundFactory *sfac = engine->getSoundFactory();
		AnimSpriteFactory *afac = AnimSpriteFactory::getSingletonPtr();
		FontFactory *ffac = engine->getFontFactory();

		if( afac == 0 )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "AnimSpriteFactory is not created. So ResourceManager will not read Anim resources!" );
		}

		string archive_name;
		string archive_type;
		string res_id;
		string res_name;
		string res_type;
		const char *zip_psd = 0;

		TiXmlElement *group = it->second;
		TiXmlElement *archive = group->FirstChildElement();
		TiXmlElement *res;

		while( archive != 0 )
		{
			/// create archive
			archive_name = archive->Attribute( "path" );
			archive_type = archive->Attribute( "type" );
			if( archive_type == "Zip" )
			{
				zip_psd = archive->Attribute( "password" ); 
			}
			am->createArchive( archive_name, archive_type, (void*)zip_psd );

			/// load resources
			res = archive->FirstChildElement();
			while( res != 0 )
			{
				res_type = res->Attribute( "type" );
				res_id = res->Attribute( "id" );
				res_name = archive_name + ":" + res->Attribute( "path" );

				if( res_type == "Image" )
				{
					/// load images
					ifac->createImage( res_name );
					/// save id
					mImgNameList[res_id] = res_name;
				}
				else if( res_type == "CopyImage" )
				{
					/// load images
					double x, y, w, h;
					res->Attribute( "x", &x );
					res->Attribute( "y", &y );
					res->Attribute( "w", &w );
					res->Attribute( "h", &h );
					ifac->copyImage( res_id, res_name, (float)x, (float)y, (float)w, (float)h );
					/// save id
					mImgNameList[res_id] = res_id;
				}
				else if( res_type == "Sound" )
				{
					/// load sounds
					sfac->createSound( res_name );
					/// save id
					mSndNameList[res_id] = res_name;
				}
				else if( res_type == "Anim" && afac != 0 )
				{
					/// load animations
					afac->createAnimSprite( res_name );
					/// save id
					mAnimNameList[res_id] = res_name;
				}
				else if( res_type == "Font" )
				{
					/// load fonts
					ffac->createFont( res_id, res_name );
					/// save id
					mFontNameList[res_id] = res_id;
				}
				
				/// get next resource
				res = res->NextSiblingElement(); 
			}

			/// next archive
			archive = archive->NextSiblingElement(); 
		}

		return true;
	}

	bool ResourceManager::loadAllGroups()
	{
		for( ResGroup::iterator it = mResGroup.begin(); it != mResGroup.end(); ++ it )
		{
			if( !loadGroup( it->first ) )
			{
				return false;
			}
		}

		return true;
	}

	Image *ResourceManager::getImage( const string &id )
	{
		NameList::iterator it = mImgNameList.find( id );
		if( it != mImgNameList.end() )
		{
			ImageFactory *ifac = EdgeEngine::getSingleton().getImageFactory();
			return ifac->getImage( it->second );
		}
		else
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Cannot get image from ResourceManager : " + id );
			return 0;
		}
	}

	Sound *ResourceManager::getSound( const string &id )
	{
		NameList::iterator it = mSndNameList.find( id );
		if( it != mSndNameList.end() )
		{
			SoundFactory *sfac = EdgeEngine::getSingleton().getSoundFactory();
			return sfac->getSound( it->second );
		}
		else
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Cannot get sound from ResourceManager : " + id );
			return 0;
		}
	}

	AnimSprite *ResourceManager::getAnim( const string &id )
	{
		if( AnimSpriteFactory::getSingletonPtr() == 0 )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "AnimSpriteFactory is not created, so ResourceManager::getAnim willnot work!" );
			return 0;
		}

		NameList::iterator it = mAnimNameList.find( id );
		if( it != mAnimNameList.end() )
		{
			AnimSpriteFactory *afac = AnimSpriteFactory::getSingletonPtr();
			return afac->getAnimSprite( it->second );
		}
		else
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Cannot get animation from ResourceManager : " + id );
			return 0;
		}
	}

	Font *ResourceManager::getFont( const string &id )
	{
		NameList::iterator it = mFontNameList.find( id );
		if( it != mFontNameList.end() )
		{
			FontFactory *ffac = EdgeEngine::getSingleton().getFontFactory();
			return ffac->getFont( it->second );
		}
		else
		{
			LogManager::getSingleton().logMessage( LL_ERROR, "Cannot get font from ResourceManager : " + id );
			return 0;
		}
	}

}