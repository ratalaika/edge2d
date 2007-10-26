/**
 *
 *
 *
 */
#ifndef EDGE_IMAGEFACTORY_H
#define EDGE_IMAGEFACTORY_H

#include "EdgeCompile.h"
#include <map>
#include <string>
#include "EdgeImage.h"

using std::map;
using std::string;

namespace Edge
{
	/**
	 * ImageFactory will create Images for you, and if you specified, it will
	 * also maintain the Image's pointer, and later you should delete it.
	 *
	*/
	class EDGE_EXPORT ImageFactory 
	{
	public:
		/**
		 * Constructor
		 *
		 */
		ImageFactory();

		/**
		 * Destructor
		 *
		 */
		virtual ~ImageFactory();

		/**
		 * create the factory
		 *
		 */
		virtual bool initiate() { return true; }

		/**
		 * release the factory, it will be called automatically by the constructor
		 *
		 */
		virtual void release() {}

		/**
		 * createImage
		 * this function will not save the pointer, and when the returned value
		 * is destroyed, the Image object also will be destroyed. 
		 */
		//virtual ImagePtr	createImage( const string &archiveName, const string &file ) = 0;
		//virtual ImagePtr	createImage( int width, int height ) = 0;

		/**
		 * createImage
		 * this function will save the pointer, and you can get the created image by its full path name.
		 * If there is an image which has the same archive name and file name in the image list, this 
		 * function will not create the image, and only return the created image in the list.
		 *
		 * Note : file parameter contains the archive name and the file name, they are separated by ':'
		 * i.e : ./data:image.png 
		 */
		virtual Image*		createImage( const string &file ) = 0;
		virtual Image*		createImage( const string &name, int width, int height ) = 0;

		/**
		 * copy an image from an exist image
		 *
		 */
		virtual Image*		copyImage( const string &name, const string &origin, float x, float y, float w, float h ) = 0;

		//virtual Image*		createImage( const string &name, int width, int height ) = 0;

		//virtual ImagePtr	createTarget( const string &archiveName, const string &file ) = 0;
		//virtual ImagePtr	createTarget( int width, int height ) = 0;

		//virtual Image*		createTarget( const string &name, const string &archiveName, const string &file ) = 0;
		virtual Image*		createTarget( const string &name, int width, int height ) = 0;
		
		/**
		 * getImage
		 *
		 */
		Image*		getImage( const string &name );

		/**
		 * removeImage
		 * this function will remove the Image in the list and also will destroy the 
		 * Image object from memory
		 * 
		 */
		void		removeImage( const string &name );
		void		removeImage( Image *image );

		/**
		 * removeAllImages
		 *
		 */
		void		removeAllImages();

		
		/**
		 * notify a child's death, when some children of this factory are destroyed, this function
		 * will be called
		 *
		 */
		virtual void notifyDeath( const Image *image ) {}

	protected:
		/**
		 * saveImage, save the image object in the list
		 *
		 */
		void saveImage( const string &name, Image *image );

		/**
		 * modifyName, modify the image's name so that it's the correct format. 
		 * the correct format is like : ./data/image.jpg
		 *
		 */
		void modifyName( string &name );

	protected:
		/// 
		typedef map<string, Image*>	ImageMap;

	protected:
		ImageMap	mImages;
	};
}


#endif