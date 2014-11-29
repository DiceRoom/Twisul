/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "NarrowingIDModule.h"
#include "../TwitterInterface/TwitterInterface/Common/TWI_Utility.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CNarrowingIDModule::CNarrowingIDModule(const std::string& iKey , const std::string& iSecret)
		:mKey(iKey)
		,mSecret(iSecret)
	{}

	bool CNarrowingIDModule::AddFollowMember(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo)
	{
		if(mFollowList.size() == 0)
		{
			CIDList id_List;
			SResponse response;
			if(!CUser::GetAllFollowID(response , id_List , mKey , mSecret))
			{
				return false;
			}

			if(!_GetUserList(mFollowList , id_List))
			{
				return false;
			}
		}

		//narrows
		CConditionPtr condition(new SConditionInfo());
		condition -> mAddFlg = true;
		condition -> mNarrowKeyword = iNarrowKeyword;
		condition -> mIncludeFlg = iIncludeFlg;
		_NarrowUser(condition -> mUserList , mFollowList , iNarrowKeyword , iIncludeFlg);
		if(iIndexNo == -1)
		{
			mConditionList.push_back(condition);
		}
		else
		{
			mConditionList[iIndexNo] = condition;
		}
		return true;
	}

	bool CNarrowingIDModule::AddFollowerMember(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo)
	{
		if(mFollowerList.size() == 0)
		{
			CIDList id_List;
			SResponse response;
			if(!CUser::GetAllFollowerID(response , id_List , mKey , mSecret))
			{
				return false;
			}

			if(!_GetUserList(mFollowerList , id_List))
			{
				return false;
			}
		}

		//narrows
		CConditionPtr condition(new SConditionInfo());
		condition -> mAddFlg = true;
		condition -> mNarrowKeyword = iNarrowKeyword;
		condition -> mIncludeFlg = iIncludeFlg;
		_NarrowUser(condition -> mUserList , mFollowerList , iNarrowKeyword , iIncludeFlg);
		if(iIndexNo == -1)
		{
			mConditionList.push_back(condition);
		}
		else
		{
			mConditionList[iIndexNo] = condition;
		}
		return true;
	}
	
    bool CNarrowingIDModule::AddFollowAndFollowerMember	(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo){
		if(mFollowerList.size() == 0)
		{
			CIDList id_List;
			SResponse response;
			if(!CUser::GetAllFollowerID(response , id_List , mKey , mSecret))
			{
				return false;
			}
            
			if(!_GetUserList(mFollowerList , id_List))
			{
				return false;
			}
		}
		if(mFollowerList.size() == 0)
		{
			CIDList id_List;
			SResponse response;
			if(!CUser::GetAllFollowerID(response , id_List , mKey , mSecret))
			{
				return false;
			}
            
			if(!_GetUserList(mFollowerList , id_List))
			{
				return false;
			}
		}

		//narrows
		CConditionPtr condition(new SConditionInfo());
		condition -> mAddFlg = true;
		condition -> mNarrowKeyword = iNarrowKeyword;
		condition -> mIncludeFlg = iIncludeFlg;
        CUserList buffer;
		_NarrowUser(buffer , mFollowList , iNarrowKeyword , iIncludeFlg);
        condition -> mUserList.insert(condition -> mUserList.end(), buffer.begin(), buffer.end());
		_NarrowUser(buffer , mFollowerList , iNarrowKeyword , iIncludeFlg);
        condition -> mUserList.insert(condition -> mUserList.end(), buffer.begin(), buffer.end());
		if(iIndexNo == -1)
		{
			mConditionList.push_back(condition);
		}
		else
		{
			mConditionList[iIndexNo] = condition;
		}
		return true;
        
    }
    
	bool CNarrowingIDModule::AddListMember(const std::string& iNarrowKeyword , bool iIncludeFlg , ID iListID , int iIndexNo)
	{
		CListCacheMap::const_iterator itr = mListMap.find(iListID);
		if(itr == mListMap.end())
		{
			CUserList user_List;

			//create cache
			SResponse response;
			SCursor cursor;
			while(cursor.mNextCursor != 0)
			{
				if(!CLists::GetMemberList(response , user_List , cursor , mKey , mSecret , iListID , cursor.mNextCursor))
				{
					return false;
				}
			}

			//insert and get
			mListMap.insert(std::make_pair(iListID , CListCacheList(new CUserList(user_List))));
			itr = mListMap.find(iListID);
		}

		//narrows
		CConditionPtr condition(new SConditionInfo());
		condition -> mAddFlg = true;
		condition -> mNarrowKeyword = iNarrowKeyword;
		condition -> mIncludeFlg = iIncludeFlg;
		_NarrowUser(condition -> mUserList , (*(*itr).second) , iNarrowKeyword , iIncludeFlg);
		if(iIndexNo == -1)
		{
			mConditionList.push_back(condition);
		}
		else
		{
			mConditionList[iIndexNo] = condition;
		}
		return true;
	}

	void CNarrowingIDModule::NarrowMember(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo)
	{
		CConditionPtr condition(new SConditionInfo());
		condition -> mAddFlg = false;
		condition -> mNarrowKeyword = iNarrowKeyword;
		condition -> mIncludeFlg = iIncludeFlg;
		if(iIndexNo == -1)
		{
			mConditionList.push_back(condition);
		}
		else
		{
			mConditionList[iIndexNo] = condition;
		}
	}

	void CNarrowingIDModule::RemoveCondition(int iIndex)
	{
		if(static_cast<int>(mConditionList.size()) > iIndex)
		{
			CConditionList::iterator itr = mConditionList.begin();
			BOOST_FOREACH(int iDummy , EnamerableRange(0 , iIndex))
			{
				iDummy;
				++ itr;
			}
			mConditionList.erase(itr);
		}
	}

    int CNarrowingIDModule::GetConditionCount() const
	{
        return mConditionList.size();
    }

	int CNarrowingIDModule::GetMemberVal() const
	{
		CUserList get_List;
		GetMemberList(get_List);
		return get_List.size();
	}
 
	void CNarrowingIDModule::GetMemberList(CUserList& oUserList) const
	{
		std::set<ID> check_Id_List;
		BOOST_FOREACH(CConditionPtr iCondition , mConditionList)
		{
			//add
			if(iCondition -> mAddFlg)
			{
				BOOST_FOREACH(CUserPtr iUser , iCondition -> mUserList)
				{
					//not found only
					if(check_Id_List.find(iUser -> mUserID) == check_Id_List.end())
					{
						check_Id_List.insert(iUser -> mUserID);
						oUserList.push_back(iUser);
					}
				}
			}
			else
			{
				_NarrowUser(oUserList , oUserList , iCondition -> mNarrowKeyword , iCondition -> mIncludeFlg);
			}
		}
	}

	/************************************************************************/
	/* Inner Decl															*/
	/************************************************************************/

	bool CNarrowingIDModule::_GetUserList(CUserList& oUserList , const CIDList& iIDList)
	{
		int val = 0;
		CIDList id_List;
		CUserList user_List;
		BOOST_FOREACH(ID iID , iIDList)
		{
			//in cache
			CUserCacheMap::const_iterator find_Itr = mCacheMap.find(iID);
			if(find_Itr != mCacheMap.end())
			{
				oUserList.push_back((*find_Itr).second);
			}
			else
			{
				id_List.push_back(iID);
				++ val;
				if(val == 100)
				{
					SResponse response;
					if(!CUser::GetUser(response , oUserList , mKey , mSecret , id_List))
					{
						return false;
					}
					id_List.clear();
					val = 0;
				}
			}
		}

		//last
		if(val != 0)
		{
			SResponse response;
			if(!CUser::GetUser(response , oUserList , mKey , mSecret , id_List))
			{
				return false;
			}
		}

		//cache
		BOOST_FOREACH(CUserPtr iUser , oUserList)
		{
			CUserCacheMap::const_iterator find_Itr = mCacheMap.find(iUser -> mUserID);
			if(find_Itr == mCacheMap.end())
			{
				mCacheMap.insert(std::make_pair(iUser -> mUserID , iUser));
			}
		}

		return true;
	}
 
	void CNarrowingIDModule::_NarrowUser(CUserList& oUserList , const CUserList& iUserList , const std::string& iNarrowKeyword , bool iIncludeFlg) const
	{
		CUserList get_List;
		BOOST_FOREACH(CUserPtr iUser , iUserList)
		{
			bool find_Flg = (iUser -> mDescription.find(iNarrowKeyword) != -1);
			if(find_Flg == iIncludeFlg)
			{
				get_List.push_back(iUser);
			}
		}
		
		oUserList = get_List;
	}
}
