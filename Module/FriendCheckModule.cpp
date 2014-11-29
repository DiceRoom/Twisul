/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "../TwitterInterface/TwitterInterface/System/TWI_System.h"
#include "../TwitterInterface/TwitterInterface/User/TWI_User.h"
#include "FriendCheckModule.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{	 
	CFriendCheckModule::CFriendCheckModule(const std::string& iKey , const std::string& iSecret , ID iUserID)
		:mKey(iKey)
		,mSecret(iSecret)
		,mUserID(iUserID)
		,mGetFollowFlg(false)
		,mGetFollowerFlg(false)
		,mGetRelationFlg(false)
	{
	}

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/

	bool CFriendCheckModule::GetFollowList(CIDList& oIDList)
	{
		if(!mGetFollowFlg && !_CreateFollowList())
		{
			return false;
		}

		oIDList.clear();
		oIDList.resize(mFollowList.size());
		std::copy(mFollowList.begin() , mFollowList.end() , oIDList.begin());
		return true;
	}

	bool CFriendCheckModule::GetFollowerList(CIDList& oIDList)
	{
		if(!mGetFollowerFlg && !_CreateFollowerList())
		{
			return false;
		}

		oIDList.clear();
		oIDList.resize(mFollowerList.size());
		std::copy(mFollowerList.begin() , mFollowerList.end() , oIDList.begin());
		return true;
	}

	bool CFriendCheckModule::GetFunList(CIDList& oIDList)
	{
		if(!mGetRelationFlg && !_CreateRelationList())
		{
			return false;
		}

		oIDList.clear();
		oIDList.resize(mFunList.size());
		std::copy(mFunList.begin() , mFunList.end() , oIDList.begin());
		return true;
	}

	bool CFriendCheckModule::GetYourFunList(CIDList& oIDList)
	{
		if(!mGetRelationFlg && !_CreateRelationList())
		{
			return false;
		}

		oIDList.clear();
		oIDList.resize(mYourFunList.size());
		std::copy(mYourFunList.begin() , mYourFunList.end() , oIDList.begin());
		return true;
	}

	bool CFriendCheckModule::GetFriendList(CIDList& oIDList)
	{
		if(!mGetRelationFlg && !_CreateRelationList())
		{
			return false;
		}

		oIDList.clear();
		oIDList.resize(mFriendList.size());
		std::copy(mFriendList.begin() , mFriendList.end() , oIDList.begin());
		return true;
	}

	/************************************************************************/
	/* Inner Decl                                                           */
	/************************************************************************/

	bool CFriendCheckModule::_CreateFollowList()
	{
		SResponse response;
		if(!CUser::GetAllFollowID(response , mFollowList , mKey , mSecret , mUserID))
		{
			return false;
		}

		mGetFollowFlg = true;
		return true;
	}

	bool CFriendCheckModule::_CreateFollowerList()
	{
		SResponse response;
		if(!CUser::GetAllFollowerID(response , mFollowerList , mKey , mSecret , mUserID))
		{
			return false;
		}

		mGetFollowerFlg = true;
		return true;
	}

	bool CFriendCheckModule::_CreateRelationList()
	{
		if(!mGetFollowFlg && !_CreateFollowList())
		{
			return false;
		}

		if(!mGetFollowerFlg && !_CreateFollowerList())
		{
			return false;
		}
		
		//fun
		mFunList.clear();
		mFunList.resize(mFollowList.size());
		std::copy(mFollowList.begin(), mFollowList.end(), mFunList.begin());

		//your fun or friend
		mYourFunList.clear();
		mFriendList.clear();
		BOOST_FOREACH(ID iID , mFollowerList)
		{
			//fun?
			CIDList::iterator end_Itr = mFunList.end();
			CIDList::iterator find_Itr = std::find(mFunList.begin() , end_Itr , iID);

			//friend
			if(find_Itr != end_Itr)
			{
				mFunList.erase(find_Itr);
				mFriendList.push_back(iID);
			}
			//your fun
			else
			{
				mYourFunList.push_back(iID);
			}
		}

		mGetRelationFlg = true;
		return true;
	}
}
