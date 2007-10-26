// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "irrKlang" library.
// For conditions of distribution and use, see copyright notice in irrKlang.h

#ifndef __I_VIRTUAL_UNKNOWN_H_INCLUDED__
#define __I_VIRTUAL_UNKNOWN_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{

	//! Reference counting base class for objects in the Irrlicht Engine similar to IUnknown.
	/** See IUnknown for the basics of this class.
	The difference to IUnknown is that the class has to implement reference counting
	for itself. 
	*/
	class IVirtualUnknown
	{
	public:

		//! Destructor.
		virtual ~IVirtualUnknown()
		{
		}

		//! Grabs the object. Increments the reference counter by one.
		/** To be implemented by the derived class. If you don't want to
		implement this, use the class IUnknown instead. See IUnknown::grab() for details
		of this method. */
		virtual void grab() = 0;

		//! Drops the object. Decrements the reference counter by one.
		/** To be implemented by the derived class. If you don't want to
		implement this, use the class IUnknown instead. See IUnknown::grab() for details
		of this method. */
		virtual bool drop() = 0;
	};



} // end namespace 

#endif

