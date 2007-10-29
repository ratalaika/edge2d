/*
-----------------------------------------------------------------------------
This source file is part of EDGE
 (A very object-oriented and plugin-based 2d game engine)
For the latest info, see http://edge2d.googlecode.com

Copyright (c) 2007-2008 The EDGE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef EDGE_shared_ptr_H
#define EDGE_shared_ptr_H

namespace Edge 
{

	template<class T> 
	class shared_ptr 
	{
	protected:
		T* pRep;
		unsigned int* pUseCount;
	public:
		shared_ptr() : pRep(0), pUseCount(0)
		{
		}

		explicit shared_ptr(T* rep) : pRep(rep), pUseCount(new unsigned int(1))
		{

		}

		shared_ptr(const shared_ptr& r)
			: pRep(0), pUseCount(0)
		{
			pRep = r.pRep;
			pUseCount = r.pUseCount; 
			if(pUseCount)
			{
				++(*pUseCount); 
			}
		}

		shared_ptr& operator=(const shared_ptr& r) 
		{
			if (pRep == r.pRep)
				return *this;
			release();

			pRep = r.pRep;
			pUseCount = r.pUseCount;
			if (pUseCount)
			{
				++(*pUseCount);
			}

			return *this;
		}
		virtual ~shared_ptr() 
		{
			release();
		}


		inline T& operator*() const { return *pRep; }
		inline T* operator->() const { return pRep; }
		inline T* get() const { return pRep; }

		void bind(T* rep) 
		{
			pUseCount = new unsigned int(1);
			pRep = rep;
		}

		inline bool unique() const 
		{ 
			return *pUseCount == 1; 
		}
		inline unsigned int useCount() const 
		{ 
			return *pUseCount; 
		}
		inline unsigned int* useCountPointer() const 
		{ 
			return pUseCount;
		}

		inline T* getPointer() const
		{ 
			return pRep;
		}

		inline bool isNull(void) const 
		{ 
			return pRep == 0; 
		}

		inline void setNull(void) 
		{ 
			if (pRep)
			{
				release();
				pRep = 0;
				pUseCount = 0;
			}
		}

	protected:

		inline void release(void)
		{
			bool destroyThis = false;

			if (pUseCount)
			{
				if (--(*pUseCount) == 0) 
				{
					destroyThis = true;
				}
			}
			if (destroyThis)
				destroy();

		}

		virtual void destroy(void)
		{
			delete pRep;
			delete pUseCount;
		}
	};

	template<class T, class U> inline bool operator==(shared_ptr<T> const& a, shared_ptr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(shared_ptr<T> const& a, shared_ptr<U> const& b)
	{
		return a.get() != b.get();
	}
}


#endif