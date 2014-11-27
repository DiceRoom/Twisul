/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Lists.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace
{
	

	static const int USER_TAG_VALUE = 11;
	std::string USER_TAG_LIST[USER_TAG_VALUE] =
	{
		"id_str",
		"name",
		"screen_name",
		"profile_image_url_https",
		"location",
		"url",
		"description",
		"statuses_count",
		"friends_count",
		"followers_count",
		"protected"
	};
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{

	/************************************************************************/
	/* CreateLists															*/
	/************************************************************************/

	bool CLists::CreateLists(SResponse& oResponse , ID& oListID , const std::string& iKey , const std::string& iSecret , const std::string& iListName , const std::string& iDescription , bool iPublicMode)
	{
		std::string uri = "lists/create.json";
		GetParameter(uri , "name" , CSystem::Encode(iListName));
		if(!iPublicMode)
		{
			GetParameter(uri , "mode" , "private");
		}
		GetParameter(uri , "description" , CSystem::Encode(iDescription));
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList node_List = analayzer.GetRootNode() -> GetChildNode("id_str");
			if(node_List.size() != 0)
			{
				oListID = CastParameter<ID>(node_List[0] -> GetNodeInfo().mValue , UNKNOWN_ID);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* UpdateLists															*/
	/************************************************************************/

	bool CLists::UpdateLists(SResponse& oResponse , CListPtr oList , const std::string& iKey , const std::string& iSecret , ID iListID , const std::string& iListName , const std::string& iDescription , bool iPublicModeFlg)
	{
		std::string uri = "lists/update.json";
		uri += ("&list_id=" + boost::lexical_cast<std::string>(iListID));
		uri += ("&name=" + CSystem::Encode(iListName));
		uri += ("&description=" + CSystem::Encode(iDescription));
		uri += "&mode=";
		uri += (iPublicModeFlg ? "public" : "private");
	
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CGetter::GetLists(oList , analayzer.GetRootNode());
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DeleteListsMessage													*/
	/************************************************************************/

	bool CLists::DeleteLists(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID)
	{
		std::string uri = "lists/destroy.json";
		GetParameter(uri , "list_id" , iListID , true);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* Lists							                                    */
	/************************************************************************/

	bool CLists::GetLists(SResponse& oResponse , CListList& oListsList , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		std::string uri = "lists/list.json";
		if(iUserID != UNKNOWN_ID)
		{
			uri += ("?user_id=" + boost::lexical_cast<std::string>(iUserID));
		}

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CListPtr list;
				if(!CGetter::GetLists(list , iNode))
				{
					oListsList.clear();
					return false;
				}
				oListsList.push_back(list);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* ListsInfo															*/
	/************************************************************************/

	bool CLists::GetListsInfo(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID)
	{
		std::string uri = "lists/show.json";
		GetParameter(uri , "list_id" , iListID , true);
		
		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetLists(oLists , analayzer.GetRootNode());
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* BelongLists						                                    */
	/************************************************************************/

	bool CLists::GetBelongLists(SResponse& oResponse , CListList& oListsList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID , ID iCursor)
	{
		std::string uri = "lists/memberships.json";
		GetParameter(uri , "user_id" , iUserID , true);
		GetParameter(uri , "cursor" , iCursor , false);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));

			CXmlNode* base_Node = analayzer.GetRootNode();
			CNodeList lists_List = base_Node -> GetChildNode();
			if(lists_List.size() != 0)
			{
				BOOST_FOREACH(CXmlNode* iNode , lists_List[0] -> GetChildNode())
				{
					CListPtr list;
					if(!CGetter::GetLists(list , iNode))
					{
						oListsList.clear();
						return false;
					}
					oListsList.push_back(list);
				}

				//cursor
				std::string get_Val;
				GetFirstNodeValue(get_Val , "next_cursor_str" , base_Node);
				oCursor.mNextCursor = CastParameter<ID>(get_Val , 0);
				GetFirstNodeValue(get_Val , "previous_cursor_str" , base_Node);
				oCursor.mPrevCursor = CastParameter<ID>(get_Val , 0);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* CreateFollowLists	                                                */
	/************************************************************************/

	bool CLists::CreateFollowLists(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID)
	{
		std::string uri = "lists/subscribers/create.json";
		GetParameter(uri , "list_id" , iListID , false);
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* DeleteFollowLists	                                                */
	/************************************************************************/

	bool CLists::DeleteFollowLists(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID)
	{
		std::string uri = "lists/subscribers/destroy.json";
		GetParameter(uri , "list_id" , iListID , false);
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* ListMember			                                                */
	/************************************************************************/

	bool CLists::GetMemberList(SResponse& oResponse , CUserList& oUserList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iListID , ID iCursor)
	{
		std::string uri = "lists/members.json";
		GetParameter(uri , "list_id" , iListID , false);
		GetParameter(uri , "cursor" , iCursor , true);
		GetParameter(uri , "skip_status" , true);

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CXmlNode* base_Node = analayzer.GetRootNode();
			CNodeList user_List = base_Node -> GetChildNode("users");
			if(user_List.size() != 0)
			{
				BOOST_FOREACH(CXmlNode* iNode , user_List[0] -> GetChildNode())
				{
					CUserPtr user;
					if(!CGetter::GetUser(user , iNode))
					{
						oUserList.clear();
						return false;
					}
					oUserList.push_back(user);
				}

				//cursor
				std::string get_Val;
				GetFirstNodeValue(get_Val , "next_cursor_str" , base_Node);
				oCursor.mNextCursor = CastParameter<ID>(get_Val , 0);
				GetFirstNodeValue(get_Val , "previous_cursor_str" , base_Node);
				oCursor.mPrevCursor = CastParameter<ID>(get_Val , 0);
			}
		}
		return ret_Flg;
	}
	
	/************************************************************************/
	/* AddMember			                                                */
	/************************************************************************/

	bool CLists::AddMember(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID)
	{
		std::string uri = "lists/members/create.json";
		GetParameter(uri , "list_id" , iListID , false);
		GetParameter(uri , "user_id" , iUserID , false);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetLists(oLists , analayzer.GetRootNode());
		}
		return ret_Flg;
	}

	bool CLists::AddMember(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , const CIDList& iUserIDList)
	{
		std::string base_Uri = "lists/members/create_all.json";
		GetParameter(base_Uri , "list_id" , iListID , false);
		base_Uri += "?user_id=";

		std::vector<CIDList::const_iterator> begin_Itr_List;
		std::vector<CIDList::const_iterator> end_Itr_List;

		//create iterator list
		int loop_Val = iUserIDList.size() / 100;
		CIDList::const_iterator current_Itr = iUserIDList.begin();
		BOOST_FOREACH(int iIndex , EnamerableRange(0 , loop_Val))
		{
			iIndex;
			begin_Itr_List.push_back(current_Itr);
			current_Itr += 100;
			end_Itr_List.push_back(current_Itr);
		}

		if(current_Itr != iUserIDList.end())
		{
			begin_Itr_List.push_back(current_Itr);
			end_Itr_List.push_back(iUserIDList.end());
		}

		//post loop
		loop_Val = begin_Itr_List.size();
		BOOST_FOREACH(size_t iIndex , EnamerableRange(0 , loop_Val))
		{
			//first
			std::vector<ID>::const_iterator itr = begin_Itr_List[iIndex] , end_Itr;
			std::string uri = base_Uri +  boost::lexical_cast<std::string>((*itr));
			++itr;

			for(end_Itr = end_Itr_List[iIndex] ; itr != end_Itr ; ++ itr)
			{
				uri += ("," + boost::lexical_cast<std::string>((*itr)));
			}

			//post
			CSystem::PostHttp(oResponse , iKey , iSecret , uri);
			if(oResponse.mGetHttpState != HTTP_STATE_OK &&
			   oResponse.mGetHttpState != HTTP_STATE_BAD_GATEWAY)
			{
				return false;
			}
		}

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetLists(oLists , analayzer.GetRootNode());
		}
		return (ret_Flg || (oResponse.mGetHttpState == HTTP_STATE_BAD_GATEWAY));
	}

	/************************************************************************/
	/* IsMember			                                                    */
	/************************************************************************/

	bool CLists::IsMember(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID)
	{
		std::string uri = "lists/members/show.json";
		GetParameter(uri , "list_id" , iListID , false);
		GetParameter(uri , "user_id" , iUserID , false);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* RemoveMember		                                                    */
	/************************************************************************/

	bool CLists::RemoveMember(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID)
	{
		std::string uri = "lists/members/destroy.json";
		GetParameter(uri , "list_id" , iListID , false);
		GetParameter(uri , "user_id" , iUserID , false);

		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetLists(oLists , analayzer.GetRootNode());
		}
		return ret_Flg;
	}

	bool CLists::RemoveMember(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , const CIDList& iUserIDList)
	{
		std::string base_Uri = "lists/members/destroy_all.json";
		GetParameter(base_Uri , "list_id" , iListID , false);
		base_Uri += "?user_id=";

		std::vector<CIDList::const_iterator> begin_Itr_List;
		std::vector<CIDList::const_iterator> end_Itr_List;

		//create iterator list
		int loop_Val = iUserIDList.size() / 100;
		CIDList::const_iterator current_Itr = iUserIDList.begin();
		BOOST_FOREACH(int iIndex , EnamerableRange(0 , loop_Val))
		{
			iIndex;
			begin_Itr_List.push_back(current_Itr);
			current_Itr += 100;
			end_Itr_List.push_back(current_Itr);
		}

		if(current_Itr != iUserIDList.end())
		{
			begin_Itr_List.push_back(current_Itr);
			end_Itr_List.push_back(iUserIDList.end());
		}

		//post loop
		loop_Val = begin_Itr_List.size();
		BOOST_FOREACH(size_t iIndex , EnamerableRange(0 , loop_Val))
		{
			//first
			std::vector<ID>::const_iterator itr = begin_Itr_List[iIndex] , end_Itr;
			std::string uri = base_Uri +  boost::lexical_cast<std::string>((*itr));
			++itr;

			for(end_Itr = end_Itr_List[iIndex] ; itr != end_Itr ; ++ itr)
			{
				uri += ("," + boost::lexical_cast<std::string>((*itr)));
			}

			//post
			CSystem::PostHttp(oResponse , iKey , iSecret , uri);
			if(oResponse.mGetHttpState != HTTP_STATE_OK &&
				oResponse.mGetHttpState != HTTP_STATE_BAD_GATEWAY)
			{
				return false;
			}
		}

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetLists(oLists , analayzer.GetRootNode());
		}
		return (ret_Flg || (oResponse.mGetHttpState == HTTP_STATE_BAD_GATEWAY));
	}
}