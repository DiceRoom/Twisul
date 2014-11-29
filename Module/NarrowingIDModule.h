/****************************************************************************/
/* NarrowingIDModule.h															*/
/****************************************************************************/
#ifndef TSUL_NARROWINGID_MODULE_H_
#define	TSUL_NARROWINGID_MODULE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../TwitterInterface/TwitterInterface/User/TWI_User.h"
#include "../TwitterInterface/TwitterInterface/Lists/TWI_Lists.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Follow User Narrowing ID Module
	class CNarrowingIDModule
	{
	private:
		//Condition
		struct SConditionInfo
		{
			bool		mAddFlg;
			std::string	mNarrowKeyword;
			bool		mIncludeFlg;

			//Add Only
			CUserList	mUserList;
		};

		typedef std::map<ID , CUserPtr>				CUserCacheMap;
		typedef std::pair<ID , CUserPtr>			CPair;
		typedef boost::shared_ptr<CUserList>		CListCacheList;
		typedef std::map<ID , CListCacheList>		CListCacheMap;
		typedef boost::shared_ptr<SConditionInfo>	CConditionPtr;
		typedef std::vector<CConditionPtr>			CConditionList;

	public:
		CNarrowingIDModule					(const std::string& iKey , const std::string& iSecret);
		bool	AddFollowMember				(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo = -1);
		bool	AddFollowerMember			(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo = -1);
		bool	AddFollowAndFollowerMember	(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo = -1);
		bool	AddListMember				(const std::string& iNarrowKeyword , bool iIncludeFlg , ID iListID , int iIndexNo = -1);
		void	NarrowMember				(const std::string& iNarrowKeyword , bool iIncludeFlg , int iIndexNo = -1);
		void	RemoveCondition				(int iIndex);
        int     GetConditionCount			() const;
		int		GetMemberVal				() const;
		void	GetMemberList				(CUserList& oUserList) const;

	/************************************************************************/
	/* Inner Decl															*/
	/************************************************************************/
	private:
		bool _GetUserList			(CUserList& oUserList , const CIDList& iIDList);
		void _NarrowUser			(CUserList& oUserList , const CUserList& iUserList , const std::string& iNarrowKeyword , bool iIncludeFlg) const;

	private:
		std::string		mKey;
		std::string		mSecret;
		CUserList		mFollowList;
		CUserList		mFollowerList;
		CUserCacheMap	mCacheMap;
		CListCacheMap	mListMap;
		CConditionList	mConditionList;
	};
}

#endif