/****************************************************************************/
/* TWI_User.h																*/
/****************************************************************************/
#ifndef TWI_USER_H_
#define	TWI_USER_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_UserDef.h"
#include <boost/thread/mutex.hpp>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! User
	class CUser
	{
	public:
		/************************************************************************/
		/* CreateFollowing		                                                */
		/************************************************************************/
		static bool CreateFollowing	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iUserID);

		/************************************************************************/
		/* DeleteFollowing														*/
		/************************************************************************/
		static bool DeleteFollowing	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iUserID);

		/************************************************************************/
		/* UserRelations										                */
		/************************************************************************/
		static bool	GetUserRelation	(SResponse& oResponse , CUserRelationPtr& oRelation , const std::string& iKey , const std::string& iSecret , ID iTargetID);
		static bool	GetUserRelation	(SResponse& oResponse , CUserRelationPtr& oRelation , const std::string& iKey , const std::string& iSecret , ID iSourceID , ID iTargetID);
		static bool	GetUserRelation	(SResponse& oResponse , CUserRelationList& oRelationList , const std::string& iKey , const std::string& iSecret , const CIDList& iTargetIDList);

		/************************************************************************/
		/* InconmingID		                                                    */
		/************************************************************************/
		static bool	GetIncomingID	(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* OutconmingID															*/
		/************************************************************************/
		static bool	GetOutgoingID	(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* Follow																*/
		/************************************************************************/
		static bool GetFollowID		(SResponse& oResponse , CIDList& oIDList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID = UNKNOWN_ID , ID iCursor = UNKNOWN_ID);
		static bool GetAllFollowID	(SResponse& oResponse , CIDList& oIDList , const std::string& iKey  , const std::string& iSecret , ID iUserID = UNKNOWN_ID);
		
		/************************************************************************/
		/* Follower																*/
		/************************************************************************/
		static bool GetFollowerID	(SResponse& oResponse , CIDList& oIDList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID = UNKNOWN_ID , ID iCursor = UNKNOWN_ID);
		static bool GetAllFollowerID(SResponse& oResponse , CIDList& oIDList , const std::string& iKey  , const std::string& iSecret , ID iUserID = UNKNOWN_ID);
		static bool IsFollowOwner	(SResponse& oResponse , const std::string& iKey  , const std::string& iSecret , ID iUserID);

		/************************************************************************/
		/* User																	*/
		/************************************************************************/
		static bool GetUser			(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret);
		static bool GetUser			(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret , ID iUserID);
		static bool GetUser			(SResponse& oResponse , CUserList& oUserList , const std::string& iKey , const std::string& iSecret , const CIDList& iUserIDList);
		static bool GetUser			(SResponse& oResponse , CUserPtr& oUser , const std::string& iKey , const std::string& iSecret , const std::string& iUserName);

		/************************************************************************/
		/* Inner Decl                                                           */
		/************************************************************************/
	private:
		static int			sLastUpdateFollowerTime;
		static boost::mutex	sCacheMutex;
		static CIDList		sCacheList;
	};
}

#endif