/**
* 
*
*
*
*/
#ifndef _EDGE_SINGLETON_H
#define _EDGE_SINGLETON_H

#ifdef _MSC_VER
#pragma warning( disable:4311 )
#pragma warning( disable:4312 )
#endif

namespace Edge 
{
	/** Template class for creating single-instance global classes.
	*/
	template <typename T> 
	class Singleton
	{
	public:
		Singleton()
		{
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
#else
			ms_Singleton = static_cast< T* >( this );
#endif
		}

		~Singleton()
		{  
			ms_Singleton = 0;  
		}

		static T& getSingleton()
		{	
			return ( *ms_Singleton ); 
		}

		static T* getSingletonPtr()
		{ 
			return ms_Singleton; 
		}

	protected:
		static T* ms_Singleton;
	};

	template <typename T> T* Singleton<T>::ms_Singleton = 0;
} // namespace Edge

#endif 