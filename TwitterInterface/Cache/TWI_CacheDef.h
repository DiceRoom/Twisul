/****************************************************************************/
/* TWI_CacheDef.h															*/
/****************************************************************************/
#ifndef TWI_CACHE_DEF_H_
#define	TWI_CACHE_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_Utility.h"
#include "../Status/TWI_StatusDef.h"
#include "../DirectMessage/TWI_DirectMessageDef.h"
#include "../User/TWI_UserDef.h"
#include "../Trends/TWI_TrendsDef.h"
#include <boost/thread.hpp>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//status cache info
	struct SStatusCacheInfo
	{
		typedef std::map<ID , CStatusPtr> CCacheMap;
		CIDList				mCacheIndexList;
		CCacheMap			mCacheMap;

		/************************************************************************/
		/* static variable														*/
		/************************************************************************/
		static boost::mutex sMutex;
		static size_t		sMaxCacheValue;

		/************************************************************************/
		/* static const variable												*/
		/************************************************************************/
		static const size_t	sMaxCacheSize = 1000000;
	};
	
	//dm cache info
	struct SDMCacheInfo
	{
		typedef std::map<ID , CDirectMessagePtr> CCacheMap;
		CCacheMap			mCacheMap;

		/************************************************************************/
		/* static variable														*/
		/************************************************************************/
		static boost::mutex sMutex;
	};

	//user cache info
	struct SUserCacheInfo
	{
		//cache time info
		struct SUserCacheTimeInfo
		{
			CUserPtr	mUserPtr;
			size_t		mSecTime;
		};
		typedef boost::shared_ptr<SUserCacheTimeInfo>	CUserTimeInfoPtr;
		typedef std::vector<CUserTimeInfoPtr> CCacheList;

		CCacheList			mCacheList;

		/************************************************************************/
		/* static variable														*/
		/************************************************************************/
		static boost::mutex	sMutex;
		static size_t		sMaxCacheValue;

		/************************************************************************/
		/* static const variable												*/
		/************************************************************************/
		static const size_t sMaxCacheSize	= 500000;
		static const size_t USER_CACHE_TTL	= 15/*(min)*/ * 60/*(s)*/;	//< user cache Time To Live
	};

	//trend place cache
	struct STrendsPlaceCache
	{
		CPlaceList	mPlaceList;
		size_t		mSecTime;	

		/************************************************************************/
		/* static variable														*/
		/************************************************************************/
		static boost::mutex sMutex;
	};
	
	//chace 
	struct SCacheData
	{
		SCacheData()
			:mHeadAddress(0)
			,mMemorySize(0)
		{}

		~SCacheData()
		{
			if(mMemorySize != 0)
			{
				free(mHeadAddress);
			}
		}

		char*	mHeadAddress;
		size_t	mMemorySize;
	};
	typedef boost::shared_ptr<SCacheData>	CCachePtr;
	static const float						CACHE_VERSION = 1.07f;
}

#endif	//TWI_CACHE_DEF_H_