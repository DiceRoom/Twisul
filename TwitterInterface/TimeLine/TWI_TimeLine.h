/****************************************************************************/
/* TWI_TimeLine.h															*/
/****************************************************************************/
#ifndef TWI_TIMELINE_H_
#define	TWI_TIMELINE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_TimeLineDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! TimeLine
	class CTimeLine
	{
	public:
		/************************************************************************/
		/* HomeTimeLine															*/
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetHomeTimeLine			(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs = 0);
		
		/************************************************************************/
		/* UserTimeLine				                                            */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetUserTimeLine			(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , ID iUserID , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* MentionsTimeLine			                                            */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetMentionsTimeLine		(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* ListsTimeLine				                                        */
		/************************************************************************/
		static bool GetListsTimeLine		(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , ID iListID , const SGetArgs* iArgs);

		/************************************************************************/
		/* TalkRootID															*/
		/************************************************************************/
		static ID	GetTalkRootID			(ID iStatusID);

		/************************************************************************/
		/* RateLimit                                                            */
		/************************************************************************/
		static bool	GetRateLimit			(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* Connection															*/
		/************************************************************************/
		static void AddStatusConnection		(CStatusPtr iStatus);
		static void DeleteStatusConnection	(ID iStatusID);
		static void ClearConnection			();

	private:
		/************************************************************************/
		/* Inner Decl		                                                    */
		/************************************************************************/
		static bool	_GetRateLimit			(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);
		static bool	_GetTimeLine			(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const std::string& iBaseUri , const SGetArgs* iArgs);
		static void _GetTalkRootIDList		(CIDList& oIDList , ID iStatusID);
		static void _GetTalkStateIDList		(CIDList& oIDList , ID iStatusID);
	private:
		static CConnectionMap	sConnectionMap;
		static const int		MAX_CONNECT_CACHE_VAL = 3000;

		/************************************************************************/
		/* Debug Mode															*/
		/************************************************************************/
	public:
		static void SetUseOldAPIMode		(bool iUseFlg);
	private:
		static bool	_GetOldRateLimit		(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);
		
		typedef bool (*CGetRateLimitFunc)(SResponse& , STimeLineRateLimit& , const std::string& , const std::string&);
		typedef void (*CGetHttpFunc)(SResponse& , const std::string& , const std::string& , const std::string&);
		
		static CGetRateLimitFunc	sGetRateLimitFunc;
		static CGetHttpFunc			sGetHttpFunc;
	};
}

#endif