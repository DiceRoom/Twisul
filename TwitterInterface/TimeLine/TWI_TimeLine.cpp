/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_TimeLine.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Status/TWI_Status.h"
#include "../Cache/TWI_Cache.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CConnectionMap CTimeLine::sConnectionMap;
	CTimeLine::CGetRateLimitFunc CTimeLine::sGetRateLimitFunc = CTimeLine::_GetOldRateLimit;
	CTimeLine::CGetHttpFunc CTimeLine::sGetHttpFunc = CSystem::GetHttpOld;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{

	/************************************************************************/
	/* HomeTimeLine		                                                   * /
	/************************************************************************/

	bool CTimeLine::GetHomeTimeLine(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs)
	{
		return _GetTimeLine(oResponse , oStatusList , iKey , iSecret , "statuses/home_timeline.json" , iArgs);
	}

	/************************************************************************/
	/* UserTimeLine								                           * /
	/************************************************************************/

	bool CTimeLine::GetUserTimeLine(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , ID iUserID , const SGetArgs* iArgs)
	{
		std::string uri = "statuses/user_timeline.json";
		GetParameter(uri , "user_id"		, iUserID , false);
		if(iArgs != 0)
		{
			GetParameter(uri , "include_rts"	, iArgs -> mIncludeRTFlg);
		}
		return _GetTimeLine(oResponse , oStatusList , iKey , iSecret , uri , iArgs);
	}

	/************************************************************************/
	/* MentionsTimeLine							                           * /
	/************************************************************************/

	bool CTimeLine::GetMentionsTimeLine(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs)
	{
		//API 1.0
		if(sGetHttpFunc == CSystem::GetHttpOld)
		{
			return _GetTimeLine(oResponse , oStatusList , iKey , iSecret , "statuses/mentions.json" , iArgs);
		}
		else
		{
			return _GetTimeLine(oResponse , oStatusList , iKey , iSecret , "statuses/mentions_timeline.json" , iArgs);
		}
	}

	/************************************************************************/
	/* ListsTimeLine				                                       * /
	/************************************************************************/

	bool CTimeLine::GetListsTimeLine(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , ID iListID , const SGetArgs* iArgs)
	{
		std::string uri = "lists/statuses.json";
		GetParameter(uri , "list_id" , iListID , false);
		if(iArgs != 0)
		{
			GetParameter(uri , "include_rts"	, iArgs -> mIncludeRTFlg);
		}
		return _GetTimeLine(oResponse , oStatusList , iKey , iSecret , uri , iArgs);
	}

	/************************************************************************/
	/* GetTalkTimeLine														*/
	/************************************************************************/
	
	ID CTimeLine::GetTalkRootID(ID iStatusID)
	{
		CIDList root_ID_List;
		_GetTalkRootIDList(root_ID_List , iStatusID);

		//tops
		return (root_ID_List.size() == 0 ? iStatusID : root_ID_List[0]);
	}

	/************************************************************************/
	/* RateLimit                                                            */
	/************************************************************************/

	bool CTimeLine::GetRateLimit(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		return sGetRateLimitFunc(oResponse , oRateLimit , iKey , iSecret);
	}

	/************************************************************************/
	/* Connection															*/
	/************************************************************************/
	
	void CTimeLine::AddStatusConnection(CStatusPtr iStatus)
	{
		//connection
		if(iStatus -> mToReplyUserID != UNKNOWN_ID)
		{
			//check
			if(sConnectionMap.size() >= MAX_CONNECT_CACHE_VAL)
			{
				CConnectionMap::iterator last_Itr = sConnectionMap.end();
				++ last_Itr;
				DeleteStatusConnection((*last_Itr).first);
			}

			//found
			CConnectionMap::iterator find_Itr = sConnectionMap.find(iStatus -> mPostID);
			if(find_Itr != sConnectionMap.end())
			{
				(*find_Itr).second -> mToReplyStateID = iStatus -> mToReplyStateID;
			}
			//not found
			else
			{
				CConnectPtr connection(new SStatusConnection());
				connection -> mToReplyStateID = iStatus -> mToReplyStateID;
				sConnectionMap.insert(std::make_pair(iStatus -> mPostID , connection));
			}

			//next
			find_Itr = sConnectionMap.find(iStatus -> mToReplyStateID);
			//found
			if(find_Itr != sConnectionMap.end())
			{
				(*find_Itr).second -> mFromReplyStatusIDList.push_back(iStatus -> mPostID);
			}
			//not found
			else
			{
				CConnectPtr connection(new SStatusConnection());
				connection -> mFromReplyStatusIDList.push_back(iStatus -> mPostID);
				sConnectionMap.insert(std::make_pair(iStatus -> mToReplyStateID , connection));
			}
		}
	}

	void CTimeLine::DeleteStatusConnection(ID iStatusID)
	{
		CConnectionMap::iterator find_Itr = sConnectionMap.find(iStatusID);
		if(find_Itr != sConnectionMap.end())
		{
			//from -> to erase
			CIDList::iterator itr , end_Itr = (*find_Itr).second -> mFromReplyStatusIDList.end();
			for(itr = (*find_Itr).second -> mFromReplyStatusIDList.begin() ; itr != end_Itr ; ++ itr)
			{
				CConnectionMap::iterator from_Find_Itr = sConnectionMap.find((*itr));
				if(from_Find_Itr != sConnectionMap.end())
				{
					(*from_Find_Itr).second -> mToReplyStateID = UNKNOWN_ID;
				}
			}

			//to -> from erase
			ID to_Reply_ID = (*find_Itr).second -> mToReplyStateID;
			if(to_Reply_ID != UNKNOWN_ID)
			{
				CConnectionMap::iterator to_Find_Itr = sConnectionMap.find(to_Reply_ID);
				if(to_Find_Itr != sConnectionMap.end())
				{
					CIDList::iterator id_Find_Itr = std::find((*to_Find_Itr).second -> mFromReplyStatusIDList.begin() , (*to_Find_Itr).second -> mFromReplyStatusIDList.end() , iStatusID);
					if(id_Find_Itr != (*to_Find_Itr).second -> mFromReplyStatusIDList.end())
					{
						(*to_Find_Itr).second -> mFromReplyStatusIDList.erase(id_Find_Itr);
					}
				}
			}

			//erase
			sConnectionMap.erase(find_Itr);
		}
	}

	void CTimeLine::ClearConnection()
	{
		sConnectionMap.clear();
	}

	/************************************************************************/
	/* Inner Decl		                                                   * /
	/************************************************************************/

	bool CTimeLine::_GetRateLimit(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		CSystem::GetHttp(oResponse , iKey , iSecret , "application/rate_limit_status.json?resources=statuses");
		if(oResponse.mGetHttpState != HTTP_STATE_OK)
		{
			return false;
		}
		SResponse statuses_Response = oResponse;
		CSystem::GetHttp(oResponse , iKey , iSecret , "application/rate_limit_status.json?resources=lists");
		if(oResponse.mGetHttpState != HTTP_STATE_OK)
		{
			return false;
		}

		//statuses
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(statuses_Response.mGetData));
			CNodeList resource_List = analayzer.GetRootNode() -> GetChildNode("resources");
			if(resource_List.size() == 0)
			{
				return false;
			}

			CRateLimitMap rate_Limit_Map;
			if(!CGetter::GetRateLimitMap(rate_Limit_Map , analayzer.GetRootNode() -> GetChildNode("resources")[0]))
			{
				return false;
			}

			oRateLimit.mHome = *(rate_Limit_Map["statuses/home_timeline"]);
			oRateLimit.mMention = *(rate_Limit_Map["statuses/mentions_timeline"]);
			oRateLimit.mUser = *(rate_Limit_Map["statuses/user_timeline"]);
		}

		//list
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList resource_List = analayzer.GetRootNode() -> GetChildNode("resources");
			if(resource_List.size() == 0)
			{
				return false;
			}

			CRateLimitMap rate_Limit_Map;
			if(!CGetter::GetRateLimitMap(rate_Limit_Map , analayzer.GetRootNode() -> GetChildNode("resources")[0]))
			{
				return false;
			}

			oRateLimit.mList = *(rate_Limit_Map["lists/statuses"]);
		}

		return true;
	}

	bool CTimeLine::_GetTimeLine(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const std::string& iBaseUri , const SGetArgs* iArgs)
	{
		std::string uri = iBaseUri;
		
		//Args
		if(iArgs != 0)
		{
			GetParameter(uri , "count"			, iArgs -> mCount	, true);
			GetParameter(uri , "since_id"		, iArgs -> mSinceID	, true);
			GetParameter(uri , "max_id"			, iArgs -> mMaxID	, true);
		}
		GetParameter(uri , "include_entities" , true);

		sGetHttpFunc(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			oStatusList;
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CStatusPtr status;
				if(!CGetter::GetStatus(status , iNode))
				{
					oStatusList.clear();
					return false;
				}
				oStatusList.push_back(status);
				CCache::AddStatusCache(status);
				AddStatusConnection(status);

				//user log
				CUserPtr user;
				if(CGetter::GetUser(user , iNode -> GetChildNode("user")[0]))
				{
					time_t sec;
					time(&sec);
					CCache::AddUserCache(user , sec);
				}
			}
		}
		return ret_Flg;
	}

	void CTimeLine::_GetTalkRootIDList(CIDList& oIDList , ID iStatusID)
	{
		CConnectionMap::iterator find_Itr = sConnectionMap.find(iStatusID);
		
		//found
		if(find_Itr != sConnectionMap.end())
		{
			CIDList::iterator itr = (*find_Itr).second -> mFromReplyStatusIDList.begin();
			CIDList::iterator end_Itr = (*find_Itr).second -> mFromReplyStatusIDList.end();

			//root
			if(itr == end_Itr)
			{
				oIDList.push_back(iStatusID);
			}
			//blanch
			else
			{
				for( ; itr != end_Itr ; ++ itr)
				{
					_GetTalkRootIDList(oIDList , (*itr));
				}
			}
		}
		else
		{
			oIDList.push_back(iStatusID);
		}
	}

	/************************************************************************/
	/* Debug Mode															*/
	/************************************************************************/

	void CTimeLine::SetUseOldAPIMode(bool iUseFlg)
	{
		sGetRateLimitFunc = iUseFlg ? _GetOldRateLimit : _GetRateLimit;	
		sGetHttpFunc = iUseFlg ? CSystem::GetHttpOld : CSystem::GetHttp;	
	}

	bool CTimeLine::_GetOldRateLimit(SResponse& oResponse , STimeLineRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		//API ver 1.0
		oRateLimit.mHome = oRateLimit.mList = oRateLimit.mMention = oRateLimit.mUser = CSystem::GetLimitRate(iKey , iSecret);
		return true;
	}
}