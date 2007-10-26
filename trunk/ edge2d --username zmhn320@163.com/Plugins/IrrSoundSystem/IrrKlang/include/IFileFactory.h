// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "irrKlang" library.
// For conditions of distribution and use, see copyright notice in irrKlang.h

#ifndef __I_FILE_FACTORY_H_INCLUDED__
#define __I_FILE_FACTORY_H_INCLUDED__

#include "IUnknown.h"

namespace irr
{
namespace io
{
	class IReadFile;

	//! Interface to overwrite opening files
	/** Derive your own class from IFileFactory, overwrite the createAndOpenFile()
		method and return your own implemented IReadFile to overwrite file access of irrKlang.
		Use ISoundEngine::addFileFactory() to let irrKlang know about your class.
		Example code can be found in the tutorial 04.OverrideFileAccess.
	 */
	class IFileFactory : public virtual IUnknown
	{
	public:

		virtual ~IFileFactory() {};

		//! Opens a file for read access.
		/** Derive your own class from IFileFactory, overwrite this
		method and return your own implemented IReadFile to overwrite file access of irrKlang.
		Use ISoundEngine::addFileFactory() to let irrKlang know about your class.
		Example code can be found in the tutorial 04.OverrideFileAccess.
		\param filename Name of file to open.
		\return Returns a pointer to the created file interface.
		The returned pointer should be dropped when no longer needed.
		See IUnknown::drop() for more information. Returns 0 if file cannot be opened. */
		virtual IReadFile* createAndOpenFile(const c8* filename) = 0;		
	};

} // end namespace irr
} // end namespace io

#endif

