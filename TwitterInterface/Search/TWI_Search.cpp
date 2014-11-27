/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Search.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"
#include "../Status/TWI_Status.h"
#include "../TimeLine/TWI_TimeLine.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* SearchStatus                                                         */
	/************************************************************************/
	
	bool CSearch::SearchStatus(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const std::string& iKeyword , const SGetArgs* iArgs)
	{
		std::string uri = "search/tweets.json?";
		GetParameter(uri , "q" , CSystem::Encode(iKeyword));

		//param
		if(iArgs != 0)
		{
			uri += ("&rpp=" + boost::lexical_cast<std::string>(iArgs -> mCount));
			
			if(iArgs -> mLanguage.length() != 0)
			{
				uri += ("&lang=" + iArgs -> mLanguage);
			}
			if(iArgs -> mSinceID != UNKNOWN_ID)
			{
				uri += ("&since_id=" + boost::lexical_cast<std::string>(iArgs -> mSinceID));
			}
			if(iArgs -> mMaxID != UNKNOWN_ID)
			{
				uri += ("&max_id=" + boost::lexical_cast<std::string>(iArgs -> mMaxID));
			}
		}

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);

		oStatusList;
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CXmlNode* base_Node = analayzer.GetRootNode();
			CNodeList result_List = base_Node -> GetChildNode("statuses");
			if(result_List.size() == 0)
			{
				return false;
			}

			BOOST_FOREACH(CXmlNode* iNode , result_List[0] -> GetChildNode())
			{
				CStatusPtr status;
				if(!CGetter::GetStatus(status , iNode))
				{
					return false;
				}
				CCache::AddStatusCache(status);
				CTimeLine::AddStatusConnection(status);
				oStatusList.push_back(status);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* SearchUser															*/
	/************************************************************************/

	bool CSearch::SearchUser(SResponse& oResponse , CUserList& oUserList , const std::string& iKey , const std::string& iSecret , const std::string& iUserName , const SGetArgs* iArgs)
	{
		std::string uri = "users/search.json?";
		GetParameter(uri , "q" , CSystem::Encode(iUserName));

		//param
		if(iArgs != 0)
		{
			uri += ("&rpp=" + boost::lexical_cast<std::string>(iArgs -> mCount));
			if(iArgs -> mPage > 0)
			{
				uri += ("&page=" + boost::lexical_cast<std::string>(iArgs -> mPage));
			}
		}

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CUserPtr user;
				if(!CGetter::GetUser(user , iNode))
				{
					oUserList.clear();
					return false;
				}
				oUserList.push_back(user);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* CreateSearchSaveInfo													*/
	/************************************************************************/

	bool CSearch::CreateSearchSaveInfo(SResponse& oResponse , CSearchSaveInfoPtr& oSearchSaveInfo , const std::string& iKey , const std::string& iSecret , const std::string& iKeyword)
	{
		std::string uri = "saved_searches/create.json";
		GetParameter(uri , "query" , CSystem::Encode(iKeyword));

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetSearchSaveInfo(oSearchSaveInfo , analayzer.GetRootNode()))
			{
				return false;
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DeleteSearchSaveInfo													*/
	/************************************************************************/

	bool CSearch::DeleteSearchSaveInfo(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iSearchSaveID)
	{
		std::string uri = "saved_searches/destroy/" + boost::lexical_cast<std::string>(iSearchSaveID) + ".json";
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* SearchSaveInfo														*/
	/************************************************************************/

	bool CSearch::GetSearchSaveInfo(SResponse& oResponse , CSearchSaveInfoPtr& oSearchSaveInfo , const std::string& iKey , const std::string& iSecret , ID iSearchSaveID)
	{
		std::string uri = "saved_searches/show/" + boost::lexical_cast<std::string>(iSearchSaveID) + ".json";
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetSearchSaveInfo(oSearchSaveInfo , analayzer.GetRootNode()))
			{
				return false;
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* AllSearchSaveInfo													*/
	/************************************************************************/

	bool CSearch::GetAllSearchSaveInfo(SResponse& oResponse , CSearchSaveInfoList& oSearchSaveInfoList , const std::string& iKey , const std::string& iSecret)
	{
		std::string uri = "saved_searches/list.json";

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));

			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CSearchSaveInfoPtr search;
				if(!CGetter::GetSearchSaveInfo(search , iNode))
				{
					oSearchSaveInfoList.clear();
					return false;
				}
				oSearchSaveInfoList.push_back(search);
			}
		}
		return ret_Flg;
	}
}