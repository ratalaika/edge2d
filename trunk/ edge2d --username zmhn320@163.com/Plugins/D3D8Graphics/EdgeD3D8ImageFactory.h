/**
 *
 *
 */
#ifndef EDGE_D3D8IMAGEFACTORY_H
#define EDGE_D3D8IMAGEFACTORY_H

#include "EdgeImageFactory.h"
#include "EdgeSingleton.h"
#include <d3d8.h>
#include <vector>

using std::vector;

namespace Edge
{
	class D3D8Image;

	/**
	 * D3D8ImageFactory will create Images for you, and if you specified, it will
	 * also maintain the Image's pointer, and later you should delete it.
	 *
	*/
	class	D3D8ImageFactory : public Singleton<D3D8ImageFactory>, public ImageFactory
	{
	public:
		/// when an Image which is an IT_TARGET is created, it will be stored here
		/// and later when the device is restore, it will restore the image .
		typedef vector<D3D8Image*> TargetList;

	public:
		/**
		 * Constructor
		 *
		 */
		D3D8ImageFactory();

		/**
		 * Destructor
		 *
		 */
		~D3D8ImageFactory();

		/**
		 *
		 */
		bool		initiate();

		/** 
		 *
		 */
		void		release();

		/**
		 *
		 */
		Image*		createImage( const string &file );

		Image*		createImage( const string &name, int width, int height );

		Image*		copyImage( const string &name, const string &origin, float x, float y, float w, float h );

		/**
		 *
		 */
		Image*		createTarget( const string &name, int width, int height );

		
		/**
		 * notify a child's death, when some children of this factory are destroyed, this function
		 * will be called
		 *
		 */
		void notifyDeath( const Image *image ) ;

		/**
		 * called when the d3d device is lost
		 *
		 */
		static void		OnLostDevice();

		/**
		 * called when the d3d device is reset
		 *
		 */
		static void		OnResetDevice();

		/**
		 * get image infor list
		 *
		 */
		TargetList &getTargetList()
		{
			return mTargets;
		}
	public:
		/// flag to specify the object whether is alive
		static int mAliveCount;
	private:
		/// 
		TargetList mTargets;
	};

}

#endif