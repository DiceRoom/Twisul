/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_DirectMessage.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CDirectMessage::CGetRateLimitFunc CDirectMessage::sGetRateLimitFunc = CDirectMessage::_GetOldRateLimit;
	CDirectMessage::CGetHttpFunc CDirectMessage::sGetHttpFunc = CSystem::GetHttpOld;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* ReceiveDirectMessage				                                    */
	/************************************************************************/

	bool CDirectMessage::GetReceiveDirectMessage(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs)
	{
		return _GetDirectMessage(oResponse , oDirectMessageList , iKey , iSecret , "direct_messages.json" , iArgs);
	}

	/************************************************************************/
	/* SentDirectMessage	                                                 */
	/************************************************************************/

	bool CDirectMessage::GetSentDirectMessage(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs)
	{
		return _GetDirectMessage(oResponse , oDirectMessageList , iKey , iSecret , "direct_messages/sent.json" , iArgs);
	}

	/************************************************************************/
	/* CreateDirectMessage		                                            */
	/************************************************************************/

	bool CDirectMessage::CreateDirectMessage(SResponse& oResponse , CDirectMessagePtr& oDirectMessage , const std::string& iKey , const std::string& iSecret , const std::string& iTargetUserName , const std::string& iText)
	{
		std::string uri = "direct_messages/new.json";
		GetParameter(uri , "user" , iTargetUserName);
		GetParameter(uri , "text" , CSystem::Encode(iText));

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetDirectMessage(oDirectMessage , analayzer.GetRootNode()))
			{
				return false;
			}
			
			CCache::AddDirectMessageCache(oDirectMessage);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DirectMessage					                                    */
	/************************************************************************/

	bool CDirectMessage::GetDirectMessage(SResponse& oResponse , CDirectMessagePtr& oDirectMessage , const std::string& iKey , const std::string& iSecret , ID iDirectMessageID)
	{
		//cache check
		if(!CCache::GetDirectMessageCache(oDirectMessage , iDirectMessageID))
		{
			std::string uri = "direct_messages/show.json";
			GetParameter(uri , "id" , iDirectMessageID , false);

			sGetHttpFunc(oResponse , iKey , iSecret , uri);
			bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
			if(ret_Flg)
			{
				CXmlAnalyzer analayzer;
				assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
				if(!CGetter::GetDirectMessage(oDirectMessage , analayzer.GetRootNode()))
				{
					return false;
				}

				CCache::AddDirectMessageCache(oDirectMessage);
			}
			return ret_Flg;
		}
		return true;
	}

	/************************************************************************/
	/* DeleteDirectMessage		                                            */
	/************************************************************************/

	bool CDirectMessage::DeleteDirectMessage(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iMessageID)
	{
		std::string uri = "direct_messages/destroy.json";
		GetParameter(uri , "id" , iMessageID , false);
		
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* RateLimit                                                            */
	/************************************************************************/

	bool CDirectMessage::GetRateLimit(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		return sGetRateLimitFunc(oResponse , oRateLimit , iKey , iSecret);
	}

	/************************************************************************/
	/* Inner Decl		                                                    */
	/************************************************************************/

	bool CDirectMessage::_GetDirectMessage(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const std::string& iBaseUri , const SGetArgs* iArgs)
	{
		std::string uri = iBaseUri;
		if(iArgs != 0)
		{
			GetParameter(uri , "count"			, iArgs -> mCount	, true);
			GetParameter(uri , "since_id"		, iArgs -> mSinceID	, true);
			GetParameter(uri , "max_id"			, iArgs -> mMaxID	, true);
		}

		sGetHttpFunc(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CDirectMessagePtr dm;
				if(!CGetter::GetDirectMessage(dm , iNode))
				{
					oDirectMessageList.clear();
					return false;
				}

				oDirectMessageList.push_back(dm);
				CCache::AddDirectMessageCache(dm);
			}
		}
		return ret_Flg;
	}

	bool CDirectMessage::_GetRateLimit(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		std::string uri = "application/rate_limit_status.json?resources=direct_messages";
	
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

			CRateLimitMap rate_Limit_Map;
			if(!CGetter::GetRateLimitMap(rate_Limit_Map , analayzer.GetRootNode() -> GetChildNode("resources")[0]))
			{
				return false;
			}

			oRateLimit.mGetDM = *(rate_Limit_Map["direct_messages/show"]);
			oRateLimit.mGetRecvDM = *(rate_Limit_Map["direct_messages/sent"]);
			oRateLimit.mGetSendDM = *(rate_Limit_Map["direct_messages"]);
		}

		return ret_Flg;
	}

	/************************************************************************/
	/* Debug Mode															*/
	/************************************************************************/

	void CDirectMessage::SetUseOldAPIMode(bool iUseFlg)
	{
		sGetRateLimitFunc = iUseFlg ? _GetOldRateLimit : _GetRateLimit;	
		sGetHttpFunc = iUseFlg ? CSystem::GetHttpOld : CSystem::GetHttp;	
	}

	bool CDirectMessage::_GetOldRateLimit(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret)
	{
		//API ver 1.0
		oRateLimit.mGetDM = oRateLimit.mGetRecvDM = oRateLimit.mGetSendDM = CSystem::GetLimitRate(iKey , iSecret);
		return true;
	}
}
