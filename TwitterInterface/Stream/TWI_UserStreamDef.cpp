/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_UserStreamDef.h"
#include "../Getter/TWI_Getter.h"
#include "../Common/TWI_Utility.h"
#include <boost/assign.hpp>

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		IEventBase* GetDeleteEventBase(CXmlNode* iNode)
		{
			CXmlNode* user_ID_Node = GetFirstNode("user_id" , iNode);
			CXmlNode* target_ID_Node = GetFirstNode("id" , iNode);
			if(user_ID_Node == 0|| target_ID_Node == 0)
			{
				return 0;
			}

			SUsesDelete del;
			del.mUserID = CastParameter(user_ID_Node -> GetNodeInfo().mValue , UNKNOWN_ID);
			del.mID = CastParameter(target_ID_Node -> GetNodeInfo().mValue , UNKNOWN_ID);
			if(del.mUserID == UNKNOWN_ID || del.mID == UNKNOWN_ID)
			{
				return 0;
			}

			//success
			return (new TUserStreamEvent<SUsesDelete>(del));
		}

		/************************************************************************/
		/* Status																*/
		/************************************************************************/
		
		bool GetStatus(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CStatusPtr status;
			if(CGetter::GetStatus(status , iNode))
			{
				oEvent.mEventState = USES_STATUS_CREATE;
				oEvent.mEvent = new TUserStreamEvent<CStatusPtr>(status);
				return true;
			}

			return false;
		}

		bool GetDeleteState(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			oEvent.mEventState = USES_STATUS_DELETE;
			oEvent.mEvent = GetDeleteEventBase(iNode);
			return true;
		}

		/************************************************************************/
		/* Favorite																*/
		/************************************************************************/
		
		bool GetFavorite(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* user_Node = GetFirstNode("source" , iNode);
			CXmlNode* status_Node = GetFirstNode("target_object" , iNode);
			if(user_Node == 0 || status_Node == 0)
			{
				return false;
			}

			SUsesFavorite fav;
			if(!CGetter::GetUser(fav.mFavUser , user_Node) || !CGetter::GetStatus(fav.mFavTargetStatus , status_Node))
			{
				return false;
			}

			oEvent.mEventState = USES_FAVORITE_CREATE;
			oEvent.mEvent = new TUserStreamEvent<SUsesFavorite>(fav);
			return true;
		}

		bool GetDeleteFavorite(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* user_Node = GetFirstNode("source" , iNode);
			CXmlNode* status_Node = GetFirstNode("target_object" , iNode);
			if(user_Node == 0 || status_Node == 0)
			{
				return false;
			}

			SUsesFavorite fav;
			if(!CGetter::GetUser(fav.mFavUser , user_Node) || !CGetter::GetStatus(fav.mFavTargetStatus , status_Node))
			{
				return false;
			}

			oEvent.mEventState = USES_FAVORITE_DELETE;
			oEvent.mEvent = new TUserStreamEvent<SUsesFavorite>(fav);
			return true;
		}

		/************************************************************************/
		/* Direct Message														*/
		/************************************************************************/
		
		bool GetDirectMessage(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CDirectMessagePtr dm;
			if(CGetter::GetDirectMessage(dm , iNode))
			{
				oEvent.mEventState = USES_DM_CREATE;
				oEvent.mEvent = new TUserStreamEvent<CDirectMessagePtr>(dm);
				return true;
			}

			return false;
		}

		bool GetDeleteDirectMessage(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			oEvent.mEventState = USES_DM_DELETE;
			oEvent.mEvent = GetDeleteEventBase(iNode);
			return true;
		}

		/************************************************************************/
		/* Follow																*/
		/************************************************************************/
		
		bool GetFollow(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* src_Node = GetFirstNode("source" , iNode);
			CXmlNode* tgt_Node = GetFirstNode("target" , iNode);
			if(src_Node == 0 || tgt_Node == 0)
			{
				return false;
			}

			SUsesFollow follow;
			if(!CGetter::GetUser(follow.mSourceUser , src_Node) || !CGetter::GetUser(follow.mTargetUser , tgt_Node))
			{
				return false;
			}

			oEvent.mEventState = USES_FOLLOW;
			oEvent.mEvent = new TUserStreamEvent<SUsesFollow>(follow);
			return true;
		}

		/************************************************************************/
		/* Lists																*/
		/************************************************************************/
		
		bool GetLists(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			CListPtr ptr;
			if(list_Node != 0 && CGetter::GetLists(ptr , list_Node))
			{
				oEvent.mEventState = USES_LIST_CREATE;
				oEvent.mEvent = new TUserStreamEvent<CListPtr>(ptr);
				return true;
			}

			return false;
		}

		bool GetUpdateLists(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			CListPtr ptr;
			if(list_Node != 0 && CGetter::GetLists(ptr , list_Node))
			{
				oEvent.mEventState = USES_LIST_UPDATE;
				oEvent.mEvent = new TUserStreamEvent<CListPtr>(ptr);
				return true;
			}

			return false;
		}

		bool GetDeleteLists(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			CListPtr ptr;
			if(list_Node != 0 && CGetter::GetLists(ptr , list_Node))
			{
				oEvent.mEventState = USES_LIST_DELETE;
				oEvent.mEvent = new TUserStreamEvent<CListPtr>(ptr);
				return true;
			}

			return false;
		}

		bool GetAddListsMember(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* src_Node = GetFirstNode("source" , iNode);
			CXmlNode* tgt_Node = GetFirstNode("target" , iNode);
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			if(src_Node == 0 || tgt_Node == 0 || list_Node == 0)
			{
				return false;
			}
			
			SUsesListMember mem;
			if(CGetter::GetUser(mem.mSourceUser , src_Node) &&
			   CGetter::GetUser(mem.mTargetUser , tgt_Node)	&&
			   CGetter::GetLists(mem.mTargetList , list_Node))
			{
				oEvent.mEventState = USES_LIST_MEMBER_ADD;
				oEvent.mEvent = new TUserStreamEvent<SUsesListMember>(mem);
				return true;
			}

			return false;
		}

		bool GetDeleteListsMember(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* src_Node = GetFirstNode("source" , iNode);
			CXmlNode* tgt_Node = GetFirstNode("target" , iNode);
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			if(src_Node == 0 || tgt_Node == 0 || list_Node == 0)
			{
				return false;
			}
			
			SUsesListMember mem;
			if(CGetter::GetUser(mem.mSourceUser , src_Node) &&
			   CGetter::GetUser(mem.mTargetUser , tgt_Node)	&&
			   CGetter::GetLists(mem.mTargetList , list_Node))
			{
				oEvent.mEventState = USES_LIST_MEMBER_DELETE;
				oEvent.mEvent = new TUserStreamEvent<SUsesListMember>(mem);
				return true;
			}

			return false;
		}

		bool GetListFollow(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* user_Node = GetFirstNode("source" , iNode);
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			if(user_Node == 0 || list_Node == 0)
			{
				return false;
			}

			SUsesListFollow follow;
			if(!CGetter::GetUser(follow.mSourceUser , user_Node) || !CGetter::GetLists(follow.mTargetList , list_Node))
			{
				return false;
			}

			oEvent.mEventState = USES_LIST_FOLLOW_CREATE;
			oEvent.mEvent = new TUserStreamEvent<SUsesListFollow>(follow);
			return true;
		}

		
		bool GetDeleteListFollow(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* user_Node = GetFirstNode("source" , iNode);
			CXmlNode* list_Node = GetFirstNode("target_object" , iNode);
			if(user_Node == 0 || list_Node == 0)
			{
				return false;
			}

			SUsesListFollow follow;
			if(!CGetter::GetUser(follow.mSourceUser , user_Node) || !CGetter::GetLists(follow.mTargetList , list_Node))
			{
				return false;
			}

			oEvent.mEventState = USES_LIST_FOLLOW_DELETE;
			oEvent.mEvent = new TUserStreamEvent<SUsesListFollow>(follow);
			return true;
		}

		/************************************************************************/
		/* Other																*/
		/************************************************************************/
		
		bool GetProfile(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CXmlNode* user_Node = GetFirstNode("source" , iNode);
			CUserPtr user_Ptr;
			if(user_Node != 0 && CGetter::GetUser(user_Ptr , user_Node))
			{
				oEvent.mEventState = USES_LIST_DELETE;
				oEvent.mEvent = new TUserStreamEvent<CUserPtr>(user_Ptr);
				return true;
			}

			return false;
		}
	}
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		typedef bool (*CEventVariableGetFunc)(SUserStreamEvent& , CXmlNode*);
		typedef std::map<std::string , CEventVariableGetFunc>	CEventVariableGetMap;

		//event
		static const CEventVariableGetMap EVENT_GET_MAP = boost::assign::map_list_of("favorite"					, &GetFavorite)
																					("unfavorite"				, &GetDeleteFavorite)
																					("follow"					, &GetFollow)
																					("list_member_added"		, &GetAddListsMember)
																					("list_member_removed"		, &GetDeleteListsMember)
																					("list_created"				, &GetLists)
																					("list_updated"				, &GetUpdateLists)																					
																					("list_destroyed"			, &GetDeleteLists)
																					("list_user_subscribed"		, &GetListFollow)
																					("list_user_unsubscribed"	, &GetDeleteListFollow)
																					("user_update"				, &GetProfile);

		//delete
		static const CEventVariableGetMap DELETE_GET_MAP = boost::assign::map_list_of("status" , &GetDeleteState)
																					 ("direct_message" , &GetDeleteDirectMessage);
	}
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		void GetUserStreamEventFromEvent(SUserStreamEvent& oEvent , const std::string& iEventName , CXmlNode* iNode)
		{
			CEventVariableGetMap::const_iterator find_Itr = EVENT_GET_MAP.find(iEventName);
			if(find_Itr != EVENT_GET_MAP.end())
			{
				(*find_Itr).second(oEvent , iNode);
			}
		}

		void GetUserStreamEventFromDelete(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			CNodeList child_List = iNode -> GetChildNode();
			if(child_List.size() != 1)
			{
				return;
			}

			CXmlNode* base_Node = child_List[0];
			CEventVariableGetMap::const_iterator find_Itr = DELETE_GET_MAP.find(base_Node -> GetNodeInfo().mTagName);
			if(find_Itr != DELETE_GET_MAP.end())
			{
				(*find_Itr).second(oEvent , base_Node);
			}
		}

		void GetUserStreamEventMain(SUserStreamEvent& oEvent , CXmlNode* iNode)
		{
			/*
			[Frequency]
			status > favrite > delete > follow > lists > direct message > other
			*/

			//type state
			CXmlNode* state_Node = GetFirstNode("entities" , iNode);
			
			oEvent.mEvent = 0;
			if(state_Node != 0)
			{
				GetStatus(oEvent , iNode);
				return;
			}

			//type event
			CXmlNode* event_Node = GetFirstNode("event" , iNode);
			oEvent.mEvent = 0;
			if(event_Node != 0)
			{
				GetUserStreamEventFromEvent(oEvent , event_Node -> GetNodeInfo().mValue , iNode);
				return;
			}

			//type delete
			CXmlNode* delete_Node = GetFirstNode("delete" , iNode);
			if(delete_Node != 0)
			{
				GetUserStreamEventFromDelete(oEvent , delete_Node);
				return;
			}

			//type direct_message
			CXmlNode* dm_Node = GetFirstNode("direct_message" , iNode);
			if(dm_Node != 0)
			{
				GetDirectMessage(oEvent , dm_Node);
				return;
			}	
		}
	}
}

namespace TSUL
{
	void GetUserStreamEvent(CUserStreamEvent& oEvent , CXmlNode* iNode)
	{
		SUserStreamEvent new_Event;
		GetUserStreamEventMain(new_Event , iNode);

		//create
		if(new_Event.mEvent != 0)
		{
			oEvent = CUserStreamEvent(new SUserStreamEvent(new_Event));
		}
	}
}
