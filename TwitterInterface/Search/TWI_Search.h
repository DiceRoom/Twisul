/****************************************************************************/
/* TWI_Search.h																*/
/****************************************************************************/
#ifndef TWI_SEARCH_H_
#define	TWI_SEARCH_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_SearchDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Search
	class CSearch
	{
	public:
		/************************************************************************/
		/* SearchStatus                                                         */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID , mLanguage							*/
		/************************************************************************/
		static bool SearchStatus			(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const std::string& iKeyword , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* SearchToUserReply                                                    */
		/* # UseGetArgs #														*/
		/* -> mCount , mPage													*/
		/************************************************************************/
		static bool SearchUser				(SResponse& oResponse , CUserList& oUserList , const std::string& iKey , const std::string& iSecret , const std::string& iUserName , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* CreateSearchSaveInfo													*/
		/************************************************************************/
		static bool CreateSearchSaveInfo	(SResponse& oResponse , CSearchSaveInfoPtr& oSearchSaveInfo , const std::string& iKey , const std::string& iSecret , const std::string& iKeyword);

		/************************************************************************/
		/* DeleteSearchSaveInfo													*/
		/************************************************************************/
		static bool DeleteSearchSaveInfo	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iSearchSaveID);

		/************************************************************************/
		/* SearchSaveInfo														*/
		/************************************************************************/
		static bool GetSearchSaveInfo		(SResponse& oResponse , CSearchSaveInfoPtr& oSearchSaveInfo , const std::string& iKey , const std::string& iSecret , ID iSearchSaveID);

		/************************************************************************/
		/* AllSearchSaveInfo													*/
		/************************************************************************/
		static bool GetAllSearchSaveInfo	(SResponse& oResponse , CSearchSaveInfoList& oSearchSaveInfoList , const std::string& iKey , const std::string& iSecret);

	private:
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
		static bool _SearchStatus			(SResponse& oResponse , CStatusList& oStatusList , const std::string& iKey , const std::string& iSecret , const std::string& iKeyword , const SGetArgs* iArgs);
		static bool	_GetSearchSaveInfo		(SSearchSaveInfo& oSearchSaveInfo , CXmlNode* iNode);
	};
}

#endif