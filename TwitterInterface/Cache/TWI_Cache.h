/****************************************************************************/
/* TWI_Cache.h																*/
/****************************************************************************/
#ifndef TWI_CACHE_H_
#define	TWI_CACHE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_CacheDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Cache
	class CCache
	{
	public:
		/************************************************************************/
		/* state																*/
		/************************************************************************/
		static bool			AddStatusCache			(CStatusPtr iStatus);
		static bool			GetStatusCache			(CStatusPtr& oStatus , ID iID);
		static bool			SetStatusCacheData		(const char* iData , size_t iDataSize);
		static CCachePtr	GetStatusCacheData		();
		static void			ClearStatusCacheData	();

	public:
		/************************************************************************/
		/* Direct Messge														*/
		/************************************************************************/
		static void			AddDirectMessageCache		(CDirectMessagePtr iDirectMessage);
		static bool			GetDirectMessageCache		(CDirectMessagePtr& oDirectMessage , ID iID);
		static bool			SetDirectMessageCacheData	(const char* iData , size_t iDataSize);
		static CCachePtr	GetDirectMessageCacheData	();
		static void			ClearDirectMessageCacheData	();

	public:
		/************************************************************************/
		/* User                                                                 */
		/************************************************************************/
		static void			AddUserCache		(CUserPtr iUser , int iTime);
		static bool			IsValidTimeUserCache(ID iUserID , int iTime);
		static bool			IsValidTimeUserCache(ID iUserID);
		static bool			ExistsUser          (ID iUserID);
		static bool			RemoveUserCache		(ID iUserID);
		static bool			GetUserCache		(CUserPtr& oUser , ID iUserID);
		static bool			GetUserCache		(CUserPtr& oUser , ID iUserID , int iTime);
		static bool			GetUserCache		(CUserPtr& oUser , const std::string& iUserName , int iTime);
		static bool			SetUserCacheData	(const char* iData , size_t iDataSize);
		static CCachePtr	GetUserCacheData	();
		static void			ClearUserCacheData	();

	public:
		/************************************************************************/
		/* Trends Place															*/
		/************************************************************************/
		static void			SetTrendsPlaceCache		(const CPlaceList& iPlaceList , int iTime);
		static bool			GetTrendsPlaceCache		(CPlaceList& oPlaceList);
		static bool			SetTrendsPlaceCacheData	(const char* iData , size_t iDataSize);
		static CCachePtr	GetTrendsPlaceCacheData	();
		static void			ClearTrendsPlaceCache	();

	private:
		static SStatusCacheInfo		sStatusCache;			//< status
		static SDMCacheInfo			sDirectMessageCache;	//< direct message
		static SUserCacheInfo		sUserCache;				//< user
		static STrendsPlaceCache	sTrendPlaceCache;		//< trends place
	};
}

#endif