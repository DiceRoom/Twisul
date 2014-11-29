/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "CacheWriter.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CCacheWriter::CCacheWriter(size_t iBlockSize)
		:mBlockSize(iBlockSize)
		,mCache(new SCacheData())
		,mCurrentSeek(0)
		,mCurrentSize(iBlockSize)
	{
		mCache -> mHeadAddress = static_cast<char*>(malloc(iBlockSize));
		if(mCache -> mHeadAddress == 0)
		{
			throw;
		}
	}
	
	/************************************************************************/
	/* Accessor																*/
	/************************************************************************/

	size_t CCacheWriter::GetCurrentSeek()
	{
		return mCurrentSeek;
	}

	CCachePtr CCacheWriter::GetCachePtr()
	{
		return mCache;
	}
    

}