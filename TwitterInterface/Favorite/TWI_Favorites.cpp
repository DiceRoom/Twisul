/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Favorites.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"
#include "../TimeLine/TWI_TimeLine.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* Favorites						                                    */
	/************************************************************************/

	bool CFavorites::GetFavorites(SResponse& oResponse , CStatusList& oFavoriteStatusList , const std::string& iKey , const std::string& iSecret , ID iUserID , const SGetArgs* iArgs)
	{
		std::string uri = "favorites/list.json";
		GetParameter(uri , "user_id" , iUserID , true);
		if(iArgs != 0)
		{
			GetParameter(uri , "count"			, iArgs -> mCount	, true);
			GetParameter(uri , "since_id"		, iArgs -> mSinceID	, true);
			GetParameter(uri , "max_id"			, iArgs -> mMaxID	, true);
		}
		GetParameter(uri , "include_entities" , true);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CStatusPtr status;
				if(!CGetter::GetStatus(status , iNode))
				{
					oFavoriteStatusList.clear();
					return false;
				}
				CCache::AddStatusCache(status);
				CTimeLine::AddStatusConnection(status);
				oFavoriteStatusList.push_back(status);
			}
		}
		return ret_Flg;
	}


	/************************************************************************/
	/* CreateFavorites												        */
	/************************************************************************/
	bool CFavorites::CreateFavorites(SResponse& oResponse , CStatusPtr& oFavoriteStatus , const std::string& iKey , const std::string& iSecret , ID iStatusID)
	{
		std::string uri = "favorites/create.json";
		GetParameter(uri , "id" , iStatusID , false);
		GetParameter(uri , "include_entities" , true);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetStatus(oFavoriteStatus , analayzer.GetRootNode()))
			{
				return false;
			}
			CCache::AddStatusCache(oFavoriteStatus);
			CTimeLine::AddStatusConnection(oFavoriteStatus);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DeleteFavoritesMessage		                                        */
	/************************************************************************/

	bool CFavorites::DeleteFavorites(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iStatusID)
	{
		std::string uri = "favorites/destroy.json";
		GetParameter(uri , "id" , iStatusID , false);
		GetParameter(uri , "include_entities" , false);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}
}