/****************************************************************************/
/* TWI_Buffer.h																*/
/****************************************************************************/
#ifndef TWI_BUFFER_H_
#define	TWI_BUFFER_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/

namespace TSUL
{
	class CBufferManager
	{
	private:
		const static int BUFFER_VAL = 12;
		const static int BUFFER_SIZE = 4096;
		struct SBuffer
		{
			SBuffer():mUseFlg(false){};
			bool			mUseFlg;
			unsigned char	mBuffer[BUFFER_SIZE];
		};
		typedef SBuffer CBufferList[BUFFER_VAL];

	public:
		class CBuffer
		{
		public:
			CBuffer(int iIndex):mBuffer(0),mIndex(iIndex){}
			~CBuffer()
			{
				if(mBuffer != 0)
				{
				}
			};

			unsigned char* GetBuffer() { return mBuffer; };

		private:
			/************************************************************************/
			/* Inner Decl                                                           */
			/************************************************************************/
			unsigned char*	mBuffer;
			int				mIndex;
		};
		friend CBuffer;
		typedef boost::shared_ptr<CBuffer> CBufferPtr;

	public:
		static CBufferPtr	GetBuffer		(size_t iSize);

	private:
		static void			_ReleaseBuffer	(int iIndex);

	private:
		/************************************************************************/
		/* Inner Decl                                                           */
		/************************************************************************/
		static CBufferList	sBufferList;
		static boost::mutex sCreateMutex;
		static boost::mutex sLockMutex;
	};
}

#endif
