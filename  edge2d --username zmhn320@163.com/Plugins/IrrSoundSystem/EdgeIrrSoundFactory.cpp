/**
 *
 *
 */
#include "EdgeIrrSoundFactory.h"
#include "EdgeException.h"
#include "EdgeLogManager.h"
#include "EdgeDataStream.h"
#include "EdgeArchive.h"
#include "EdgeArchiveManager.h"
#include "EdgeIrrSound.h"
#include "irrKlang/include/irrKlang.h"

#pragma comment( lib, "../plugins/IrrSoundSystem/irrKlang/lib/irrKlang.lib" )

namespace irr
{
	namespace io
	{
		class EdgeReadFile : public IReadFile
		{
		public:
			EdgeReadFile( Edge::DataStreamPtr &stream, const c8 *fileName ) :
			  mFileName( fileName ), mStream( stream )
			{
			}
		    ~EdgeReadFile()
			{
			}

			s32 read( void *buffer, u32 sizeToRead )
			{
				return (s32)mStream->read( buffer, sizeToRead );
			}

			bool seek( s32 finalPos, bool relativeMovement )
			{
				if( relativeMovement )
				{
					mStream->seek( mStream->getPos() + finalPos );
				}
				else
				{
					mStream->seek( finalPos );
				}

				return true;
			}

			s32 getSize()
			{
				return static_cast<s32>( mStream->getSize() );
			}

			s32 getPos()
			{
				return static_cast<s32>( mStream->getPos() );
			}

			const c8 *getFileName()
			{
				return mFileName.c_str();
			}

		private:
			Edge::DataStreamPtr mStream;
			string mFileName;
		};

		class EdgeFileFactory : public IFileFactory
		{
		public:
			IReadFile *createAndOpenFile( const c8 *file )
			{
				string pathName( file );
				string archiveName, fileName;

				size_t i = pathName.rfind( ':' );
				size_t size = pathName.size();
				archiveName.assign( pathName, 0, i );
				fileName.assign( pathName, i + 1, size - i );
			
				/// log result
				Edge::LogManager::getSingleton().logMessage( Edge::LL_INFORMATION, 
					"Load Sound...: " + pathName );
			
				/// create a data stream
				Edge::Archive *archive = Edge::ArchiveManager::getSingletonPtr()->getArchive( archiveName );
				Edge::DataStreamPtr stream = archive->open( fileName );
			
				return new EdgeReadFile( stream, file );
			}
		};
	}
}


namespace Edge
{
	IrrSoundFactory::IrrSoundFactory()
	{
		mSoundEngine = 0;
	}

	IrrSoundFactory::~IrrSoundFactory()
	{
		destroyAllSounds();
		release();
	}

	bool IrrSoundFactory::initiate()
	{
		mSoundEngine = irr::audio::createIrrKlangDevice();

		if( mSoundEngine == 0 )
		{
			EDGE_EXCEPT( Exception::EC_OTHER_ERROR, "Cannot create IrrKlang device",
				"IrrSoundFactory::initiate" );
		}

		irr::io::IFileFactory *ff = new irr::io::EdgeFileFactory();
		mSoundEngine->addFileFactory( ff );
		ff->drop();

		return true;
	}

	void IrrSoundFactory::release()
	{
		mSoundEngine->drop();
	}

	void IrrSoundFactory::setVolume( float volume )
	{
		mSoundEngine->setSoundVolume( volume );
	}

	float IrrSoundFactory::getVolume()
	{
		return mSoundEngine->getSoundVolume();
	}

	void IrrSoundFactory::pause( bool bPause )
	{
		mSoundEngine->setAllSoundsPaused( bPause );
	}

	void IrrSoundFactory::stop()
	{
		mSoundEngine->stopAllSounds();
	}

	Sound *IrrSoundFactory::createSound( const string &file )
	{
		string cfile( file );
		_modifyName( cfile );

		/// check whether the sound is already created
		Sound *sound = _selfGetSound( cfile );
		if( sound != NULL )
		{
			LogManager::getSingleton().logMessage( LL_WARNING, "The sound : " + cfile + " is already created" );
			return sound;
		}

		sound = new IrrSound( cfile, mSoundEngine );
		sound->create();

		_saveSound( sound );
		return sound;
	}
}