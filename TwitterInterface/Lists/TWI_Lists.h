/****************************************************************************/
/* TWI_Lists.h																*/
/****************************************************************************/
#ifndef TWI_LISTS_H_
#define	TWI_LISTS_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_ListsDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	class CXmlNode;

	//! Lists
	class CLists
	{
	public:
		/************************************************************************/
		/* CreateLists															*/
		/************************************************************************/
		static bool CreateLists			(SResponse& oResponse , ID& oListID , const std::string& iKey , const std::string& iSecret , const std::string& iListName , const std::string& iDescription , bool iPublicMode);

		/************************************************************************/
		/* UpdateLists															*/
		/************************************************************************/
		static bool UpdateLists			(SResponse& oResponsee , CListPtr oList , const std::string& iKey , const std::string& iSecret , ID iListID , const std::string& iListName , const std::string& iDescription , bool iPublicModeFlg);

		/************************************************************************/
		/* DeleteLists															*/
		/************************************************************************/
		static bool DeleteLists			(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID);

		/************************************************************************/
		/* Lists																*/
		/************************************************************************/
		static bool GetLists			(SResponse& oResponse , CListList& oListsList , const std::string& iKey , const std::string& iSecret , ID iUserID = UNKNOWN_ID);

		/************************************************************************/
		/* ListsInfo															*/
		/************************************************************************/
		static bool GetListsInfo		(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID);

		/************************************************************************/
		/* BelongLists															*/
		/************************************************************************/
		static bool GetBelongLists		(SResponse& oResponse , CListList& oListsList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iUserID = UNKNOWN_ID , ID iCursor = UNKNOWN_ID);

		/************************************************************************/
		/* CreateFollowLists	                                                */
		/************************************************************************/
		static bool CreateFollowLists	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID);

		/************************************************************************/
		/* DeleteFollowLists	                                                */
		/************************************************************************/
		static bool DeleteFollowLists	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID);

		/************************************************************************/
		/* ListsMember															*/
		/************************************************************************/
		static bool GetMemberList		(SResponse& oResponse , CUserList& oUserList , SCursor& oCursor , const std::string& iKey , const std::string& iSecret , ID iListID , ID iCursor = UNKNOWN_ID);

		/************************************************************************/
		/* AddMember			                                                */
		/************************************************************************/
		static bool AddMember			(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID);
		static bool AddMember			(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , const CIDList& iUserIDList);

		/************************************************************************/
		/* IsMember			                                                    */
		/************************************************************************/
		static bool IsMember			(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID);

		/************************************************************************/
		/* RemoveMember															*/
		/************************************************************************/
		static bool RemoveMember		(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , ID iUserID);
		static bool RemoveMember		(SResponse& oResponse , CListPtr& oLists , const std::string& iKey , const std::string& iSecret , ID iListID , const CIDList& iUserIDList);
	};
}

#endif