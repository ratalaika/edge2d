/**
 *
 *
 *
 */
#ifndef EDGE_RESOURCEMANAGER_H
#define EDGE_RESOURCEMANAGER_H

#include "EdgeSingleton.h"

#include <string>
#include <map>

using std::string;
using std::map;

class TiXmlElement;
class TiXmlDocument;

namespace Edge
{
	class Image;
	class Sound;
	class AnimSprite;
	class Font;

	/**
	 * ResourceManager will manage resources for you. Currently supported resources include :
	 * images, sounds, animations
	 *
	 */
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		/**
		 * ResourceGroup
		 *
		 *
		 */
		typedef map<string, TiXmlElement*> ResGroup;
		
		/**
		 * resource name list
		 *
		 */
		typedef map<string, string> NameList;

	public:
		/**
		 * Constructor
		 *
		 */
		ResourceManager();

		/**
		 * Destructor
		 *
		 */
		~ResourceManager();

		/**
		 * initiate , it will load a xml resource config file.
		 *
		 */
		bool initiate( const string &file );

		/**
		 * release, called by destructor
		 *
		 */
		void release();

		/**
		 * load a group of resource
		 *
		 */
		bool loadGroup( const string &id );

		/**
		 * load all groups 
		 *
		 */
		bool loadAllGroups();

		/**
		 * get an image
		 *
		 */
		Image *getImage( const string &id );

		/**
		 * get a sound
		 *
		 */
		Sound *getSound( const string &id );

		/**
		 * get an animation
		 *
		 */
		AnimSprite *getAnim( const string &id );

		/**
		 * get a font
		 *
		 */
		Font *getFont( const string &id );

	protected:
		/// resource group
		ResGroup mResGroup;
		/// image name list
		NameList mImgNameList;
		/// sound name list
		NameList mSndNameList;
		/// animation name list
		NameList mAnimNameList;
		/// Font name list
		NameList mFontNameList;
		/// TinyXml document
		TiXmlDocument *mXmlDoc;
	};
}

#endif