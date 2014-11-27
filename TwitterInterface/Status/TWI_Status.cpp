/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Status.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"
#include "../TimeLine/TWI_TimeLine.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	bool SPhoto::sSmallPhotoFlg = true;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* GetStatus														    */
	/************************************************************************/

	bool CStatus::GetStatus(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret , ID iStatusID)
	{
		//cache check
		if(!CCache::GetStatusCache(oStatus , iStatusID))
		{
			std::string uri = "statuses/show.json";
			GetParameter(uri , "id" , iStatusID , false);
			GetParameter(uri , "include_entities" , true);
			CSystem::GetHttp(oResponse , iKey , iSecret , uri);
			bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
			if(ret_Flg)
			{
				CXmlAnalyzer analayzer;
				assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
				if(!CGetter::GetStatus(oStatus , analayzer.GetRootNode()))
				{
					return false;
				}
				CCache::AddStatusCache(oStatus);
				CTimeLine::AddStatusConnection(oStatus);
			}
			return ret_Flg;
		}
		return true;
	}

	/************************************************************************/
	/* RetweetStatus														*/
	/************************************************************************/
 
	bool CStatus::GetRetweetStatusID(SResponse& oResponse , ID& oRetweetID , const std::string& iKey , const std::string& iSecret , ID iStatusID)
	{
		std::string uri = "statuses/show.json";
		GetParameter(uri , "id" , iStatusID , false);
		GetParameter(uri , "include_my_retweet" , true);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList retweet_List = analayzer.GetRootNode() -> GetChildNode("current_user_retweet");
			if(retweet_List.size() == 0)
			{
				return false;
			}

			//id get
			CXmlNode* str_Node = retweet_List[0] -> GetChildNode("id_str")[0];
			std::string id = str_Node -> GetNodeInfo().mValue;
			oRetweetID = CastParameter<ID>(id , UNKNOWN_ID);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* CreateStatus							                                */
	/************************************************************************/
	
	bool CStatus::CreateStatus(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret , const SPostStatusArgs& iStatusArgs)
	{
		if(iStatusArgs.mStatus.length() == 0)
		{
			return false;
		}

		std::string uri = "statuses/update.json";
		GetParameter(uri , "status" , CSystem::Encode(iStatusArgs.mStatus));
		GetParameter(uri , "in_reply_to_status_id" , iStatusArgs.mReplyStatusID , true);
		GetParameter(uri , "source=TwiSUL?include_entities" , true);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetStatus(oStatus , analayzer.GetRootNode()))
			{
				return false;
			}
			CCache::AddStatusCache(oStatus);
			CTimeLine::AddStatusConnection(oStatus);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DeleteStatusMessage		                                            */
	/************************************************************************/

	bool CStatus::DeleteStatus(SResponse& oResponse , const std::string& iKey , const std::string& iSecret ,  ID iStatusID)
	{
		std::string uri = "statuses/destroy/" + boost::lexical_cast<std::string>(iStatusID) + ".json";
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CTimeLine::DeleteStatusConnection(iStatusID);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* RetweetStatus	                                                    */
	/************************************************************************/

	bool CStatus::RetweetStatus(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret ,  ID iStatusID)
	{
		std::string uri = "statuses/retweet/" + boost::lexical_cast<std::string>(iStatusID) + ".json";
		GetParameter(uri , "source" , "TwiSUL");
		GetParameter(uri , "include_entities" , true);
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetStatus(oStatus , analayzer.GetRootNode()))
			{
				return false;
			}
			CCache::AddStatusCache(oStatus);
			CTimeLine::AddStatusConnection(oStatus);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* GetRetweetStatusUser													*/
	/************************************************************************/

	bool CStatus::GetRetweetStatusUserID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret , ID iStatusID , const SGetArgs* iArgs)
	{
		std::string uri = "statuses/retweets/" + boost::lexical_cast<std::string>(iStatusID) + ".json";
		if(iArgs != 0)
		{
			GetParameter(uri , "count" , iArgs -> mCount , true);
		}
		GetParameter(uri , "trim_user" , true);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CXmlNode* user_Node = iNode -> GetChildNode("user")[0];
				CXmlNode* id_Node = user_Node -> GetChildNode("id_str")[0];
				oIDList.push_back(CastParameter<ID>(id_Node -> GetNodeInfo().mValue , UNKNOWN_ID));
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* RateLimit                                                            */
	/************************************************************************/

	bool CStatus::GetRateLimit(SResponse& oResponse , CRateLimitMap& oRateLimitMap , const std::string& iKey , const std::string& iSecret)
	{
		std::string uri = "application/rate_limit_status.json?resources=statuses";

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList resource_List = analayzer.GetRootNode() -> GetChildNode("resources");
			if(resource_List.size() == 0)
			{
				return false;
			}

			return CGetter::GetRateLimitMap(oRateLimitMap , analayzer.GetRootNode() -> GetChildNode("resources")[0]);
		}
		return ret_Flg;
	}
}