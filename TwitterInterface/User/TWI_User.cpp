/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_User.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"
#include "../Account/TWI_Account.h"

namespace TSUL
{
	int CUser::sLastUpdateFollowerTime = 0;
	boost::mutex CUser::sCacheMutex;
	CIDList CUser::sCacheList;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* CreateFollowing														*/
	/************************************************************************/

	bool CUser::CreateFollowing(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		std::string uri = "friendships/create.json";
		GetParameter(uri , "user_id" , iUserID , false);
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			time_t sec;
			time(&sec);

			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CUserPtr user;
			if(!CGetter::GetUser(user , analayzer.GetRootNode()))
			{
				return false;
			}
			CCache::AddUserCache(user , sec);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* DeleteFollowing		                                                */
	/************************************************************************/

	bool CUser::DeleteFollowing(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		std::string uri = "friendships/destroy.json";
		GetParameter(uri , "user_id" , iUserID , false);
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);

		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			time_t sec;
			time(&sec);

			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CUserPtr user;
			if(!CGetter::GetUser(user , analayzer.GetRootNode()))
			{
				return false;
			}
			CCache::AddUserCache(user , sec);
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* GetUserRelations														*/
	/************************************************************************/

	bool CUser::GetUserRelation(SResponse& oResponse , CUserRelationPtr& oRelation , const std::string& iKey , const std::string& iSecret , ID iTargetID)
	{
		CUserRelationList get_List;
		CIDList id_List;
		id_List.push_back(iTargetID);
		if(!GetUserRelation(oResponse , get_List , iKey , iSecret , id_List))
		{
			return false;
		}

		oRelation = get_List[0];
		return true;
	}

	bool CUser::GetUserRelation(SResponse& oResponse , CUserRelationPtr& oRelation , const std::string& iKey , const std::string& iSecret , ID iSourceID , ID iTargetID)
	{
		std::string uri = "friendships/show.json";
		GetParameter(uri , "source_id" , iSourceID , false);
		GetParameter(uri , "target_id" , iTargetID , false);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList get_List = analayzer.GetRootNode() -> GetChildNode("relationship");
			if(get_List.size() == 0)
			{
				return false;
			}

			//get
			CXmlNode* target_Node = get_List[0] -> GetChildNode("target")[0];
			oRelation = CUserRelationPtr(new SUserRelation());

			std::string buf;
			GetFirstNodeValue(buf , "id_str" , target_Node);
			oRelation -> mTargetID = CastParameter<ID>(buf , UNKNOWN_ID);
			GetFirstNodeValue(buf , "followed_by" , target_Node);
			oRelation -> mFollowingFlg = (buf == "true");
			GetFirstNodeValue(buf , "following" , target_Node);
			oRelation -> mFollowedByFlg = (buf == "true");
		}
		return ret_Flg;
	}

	bool CUser::GetUserRelation(SResponse& oResponse , CUserRelationList& oRelationList , const std::string& iKey , const std::string& iSecret , const CIDList& iTargetIDList)
	{
		std::string uri = "friendships/lookup.json";
		GetParameter(uri , "user_id" , iTargetIDList);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CUserRelationPtr relation(new SUserRelation());
				relation -> mTargetID = CastParameter<int>(iNode -> GetChildNode("id")[0] -> GetNodeInfo().mValue , -1);

				CXmlNode* connection_Node = iNode -> GetChildNode("connections")[0];
				std::set<std::string> get_Name;
				BOOST_FOREACH(CXmlNode* iNameNode , connection_Node -> GetChildNode())
				{
					get_Name.insert(iNameNode -> GetNodeInfo().mValue);
				}
				relation -> mFollowingFlg = (get_Name.find("following") != get_Name.end());
				relation -> mFollowedByFlg = (get_Name.find("followed_by") != get_Name.end());
				oRelationList.push_back(relation);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* InconmingID		                                                    */
	/************************************************************************/

	bool CUser::GetIncomingID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret)
	{
		std::string base_Uri = "friendships/incoming.json";
		GetParameter(base_Uri , "stringify_ids" , true);
		SCursor cursor;
		while(cursor.mNextCursor != 0)
		{
			std::string uri = base_Uri;
			GetParameter(uri , "cursor" , cursor.mNextCursor , true);
			
			TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
			
			bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
			if(!ret_Flg)
			{
				return false;
			}

			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			GetIDList(oIDList , analayzer.GetRootNode() -> GetChildNode("ids")[0]);
			GetCursor(cursor , analayzer.GetRootNode());
		}
		return true;
	}

	/************************************************************************/
	/* OutconmingID			                                                */
	/************************************************************************/

	bool CUser::GetOutgoingID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret)
	{
		std::string base_Uri = "friendships/outgoing.json";
		GetParameter(base_Uri , "stringify_ids" , true);
		SCursor cursor;
		while(cursor.mNextCursor != 0)
		{
			std::string uri = base_Uri;
			GetParameter(uri , "cursor" , cursor.mNextCursor , true);
			
			TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
			
			bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
			if(!ret_Flg)
			{
				return false;
			}
			
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			GetIDList(oIDList , analayzer.GetRootNode() -> GetChildNode("ids")[0]);
			GetCursor(cursor , analayzer.GetRootNode());
		}
		return true;
	}

	/************************************************************************/
	/* Follow																*/
	/************************************************************************/

	bool CUser::GetFollowID(SResponse& oResponse , CIDList& oIDList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID , ID iCursor)
	{
		std::string uri = "friends/ids.json";
		GetParameter(uri , "user_id" , iUserID , true);
		GetParameter(uri , "cursor" , iCursor , false);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			GetIDList(oIDList , analayzer.GetRootNode() -> GetChildNode("ids")[0]);
			GetCursor(oCursor , analayzer.GetRootNode());
		}
		return ret_Flg;
	}
 
	bool CUser::GetAllFollowID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		SCursor cursor;
		while(cursor.mNextCursor != 0)
		{
			if(!GetFollowID(oResponse , oIDList , cursor , iKey , iSecret , iUserID , cursor.mNextCursor))
			{
				return false;
			}
		}
		return true;
	}

	/************************************************************************/
	/* Follower																*/
	/************************************************************************/

	bool CUser::GetFollowerID(SResponse& oResponse , CIDList& oIDList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID , ID iCursor)
	{
		std::string uri = "followers/ids.json";
		GetParameter(uri , "user_id" , iUserID , true);
		GetParameter(uri , "cursor" , iCursor , false);

		TSUL::CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			GetIDList(oIDList , analayzer.GetRootNode() -> GetChildNode("ids")[0]);
			GetCursor(oCursor , analayzer.GetRootNode());
		}
		return ret_Flg;
	}

	bool CUser::GetAllFollowerID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		SCursor cursor;
		while(cursor.mNextCursor != 0)
		{
			if(!GetFollowerID(oResponse , oIDList , cursor , iKey , iSecret , iUserID , cursor.mNextCursor))
			{
				return false;
			}
		}

		return true;
	}

	bool CUser::IsFollowOwner(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		boost::mutex::scoped_lock lock(sCacheMutex);

		//time check getter
		time_t sec;
		time(&sec);
		int between_Time = sec - sLastUpdateFollowerTime;
		sLastUpdateFollowerTime = sec;
		if(between_Time > 120)
		{
			sCacheList.clear();
			if(!GetAllFollowerID(oResponse , sCacheList , iKey , iSecret))
			{
				sLastUpdateFollowerTime = 0;
				return false;
			}
		}

		return (std::find(sCacheList.begin() , sCacheList.end() , iUserID) != sCacheList.end());
	}

	/************************************************************************/
	/* User																	*/
	/************************************************************************/

	bool CUser::GetUser(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret)
	{
		CAccount::IsVerifyCredentials(oResponse , iKey , iSecret);

		if(oResponse.mGetHttpState == HTTP_STATE_OK)
		{
			//XmlAnalyze
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			return CGetter::GetUser(oUser , analayzer.GetRootNode());
		}

		return false;	
	}

	bool CUser::GetUser(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret , ID iUserID)
	{
		time_t sec;
		time(&sec);
		if(CCache::GetUserCache(oUser , iUserID , sec))
		{
			return true;
		}

		std::string uri = "users/lookup.json";
		GetParameter(uri , "user_id" , iUserID , false);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		if(oResponse.mGetHttpState == HTTP_STATE_OK)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(CGetter::GetUser(oUser , analayzer.GetRootNode() -> GetChildNode()[0]))
			{
				CCache::AddUserCache(oUser , sec);
				return true;
			}
		}
		return false;
	}

	bool CUser::GetUser(SResponse& oResponse , CUserList& oUserList , const std::string& iKey , const std::string& iSecret , const CIDList& iUserIDList)
	{
		//cache check
		time_t sec;
		time(&sec);
		
		CUserList get_List;
		CIDList id_List;
		std::vector<CIDList> id_List_List;
		BOOST_FOREACH(ID iID , iUserIDList)
		{
			CUserPtr user;
			
			//hit cache
			if(CCache::GetUserCache(user , iID , sec))
			{
				get_List.push_back(user);
			}
			//insert list
			else
			{
				id_List.push_back(iID);
				if(id_List.size() == 100)
				{
					id_List_List.push_back(id_List);
					id_List.clear();
				}
			}
		}
		if(id_List.size() != 0)
		{
			id_List_List.push_back(id_List);
		}

		//get
		BOOST_FOREACH(const CIDList& iIDList , id_List_List)
		{
			std::string uri = "users/lookup.json";
			GetParameter(uri , "user_id" , iIDList);
			CSystem::GetHttp(oResponse , iKey , iSecret , uri);
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(analayzer.GetRootNode()){
                BOOST_FOREACH(CXmlNode* iUserNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CUserPtr user;
				if(!CGetter::GetUser(user , iUserNode))
				{
					return false;
				}
				get_List.push_back(user);
				CCache::AddUserCache(user , sec);
			}
            }
		}

		//Sort
		std::map<ID , CUserPtr> id_Map;
		BOOST_FOREACH(CUserPtr iUser , get_List)
		{
			id_Map.insert(std::make_pair(iUser -> mUserID , iUser));
		}

		BOOST_FOREACH(ID iID , iUserIDList)
		{
			std::map<ID , CUserPtr>::const_iterator find_Itr = id_Map.find(iID);
			if(find_Itr != id_Map.end())
			{
				oUserList.push_back((*find_Itr).second);
			}
			else
			{
				oUserList.end();
				return false;
			}
		}

		return true;
	}

	bool CUser::GetUser(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret , const std::string& iUserName)
	{
		//cache
		time_t sec;
		time(&sec);
		if(CCache::GetUserCache(oUser , iUserName , sec))
		{
			return true;
		}

		std::string uri = "users/lookup.json";
		GetParameter(uri , "screen_name" , iUserName);
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		if(oResponse.mGetHttpState == HTTP_STATE_OK)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(CGetter::GetUser(oUser , analayzer.GetRootNode() -> GetChildNode()[0]))
			{
				CCache::AddUserCache(oUser , sec);
				return true;
			}
		}
		return false;
	}
}