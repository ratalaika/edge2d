/**
*
*
*
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