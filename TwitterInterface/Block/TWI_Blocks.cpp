/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Blocks.h"
#include "../System/TWI_System.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CIDList CBlocks::sBlockUserList;
	size_t CBlocks::sLastUpdateTime = 0;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* CreateBlocks															*/
	/************************************************************************/
	
	bool CBlocks::CreateBlocks(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID)
	{
		std::string uri = "blocks/create.json";
		GetParameter(uri , "user_id" , iBlockUserID , false);
		GetParameter(uri , "skip_status" , true);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* DeleteBlocksMessage													*/
	/************************************************************************/

	bool CBlocks::DeleteBlocks(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID)
	{
		std::string uri = "blocks/destroy.json";
		GetParameter(uri , "user_id" , iBlockUserID , false);
		GetParameter(uri , "skip_status" , true);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* IsBlocks																*/
	/************************************************************************/
	
	bool CBlocks::IsBlocks(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID)
	{
		CIDList id_List;
		if(GetBlockUserList(oResponse , id_List , iKey , iSecret))
		{
			return (std::find(id_List.begin() , id_List.end() , iBlockUserID) != id_List.end());
		}
		return false;
	}

	/************************************************************************/
	/* BlockUserList																*/
	/************************************************************************/

	bool CBlocks::GetBlockUserList(SResponse& oResponse , CIDList& oBlockUserIDList , const std::string& iKey , const std::string& iSecret)
	{
		//time in
		time_t sec;
		time(&sec);
		int between_Time = sec - sLastUpdateTime;
		sLastUpdateTime = sec;
		if(between_Time < 120)
		{
			oBlockUserIDList = sBlockUserList;
			return true;
		}

		std::string uri = "blocks/ids.json";
		GetParameter(uri , "stringify_ids" , true);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList id_List = analayzer.GetRootNode() -> GetChildNode("ids");
			if(id_List.size() == 0)
			{
				return false;
			}

			CXmlNode* id_Node = id_List[0];
			BOOST_FOREACH(CXmlNode* iNode , id_Node -> GetChildNode())
			{
				oBlockUserIDList.push_back(CastParameter<ID>(iNode -> GetNodeInfo().mValue , UNKNOWN_ID));
			}
		}
		sBlockUserList = oBlockUserIDList;
		return ret_Flg;
	}

	/************************************************************************/
	/* ReportSpam															*/
	/************************************************************************/
	
	bool CBlocks::ReportSpam(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iSpamUserID)
	{
		std::string uri = "users/report_spam.json";
		GetParameter(uri , "user_id" , iSpamUserID , false);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}
}