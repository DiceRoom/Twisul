/****************************************************************************/
/* FollowerCheckModule.h													*/
/****************************************************************************/
#ifndef TSUL_FRIENDERCHECK_MODULE_H_
#define	TSUL_FRIENDERCHECK_MODULE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../TwitterInterface/TwitterInterface/Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Friend Check Module
	class CFriendCheckModule
	{
	private:
		CFriendCheckModule(){}
	public:
		CFriendCheckModule(const std::string& iKey , const std::string& iSecret , ID iUserID);

	public:
		/************************************************************************/
		/* Getter                                                               */
		/************************************************************************/
		bool GetFollowList		(CIDList& oIDList);
		bool GetFollowerList	(CIDList& oIDList);
		bool GetFunList			(CIDList& oIDList);
		bool GetYourFunList		(CIDList& oIDList);
		bool GetFriendList		(CIDList& oIDList);
	
	private:
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
		bool _CreateFollowList	();
		bool _CreateFollowerList();
		bool _CreateRelationList();

	private:
		std::string	mKey;
		std::string mSecret;
		ID			mUserID;
		CIDList		mFollowList;
		CIDList		mFollowerList;
		CIDList		mFunList;		//< my --> target
		CIDList		mYourFunList;	//< my <-- target
		CIDList		mFriendList;	//< my <-> target

		//GetFlg
		bool		mGetFollowFlg;
		bool		mGetFollowerFlg;
		bool		mGetRelationFlg;
	};
}

#endif