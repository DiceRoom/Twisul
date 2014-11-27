/****************************************************************************/
/* TWI_ListsDef.h															*/
/****************************************************************************/
#ifndef TWI_LISTS_DEF_H_
#define	TWI_LISTS_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../User/TWI_UserDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//ListInfo
	struct SListInfo
	{
		ID			mID;				//< id
		std::string	mListName;			//< name
		std::string	mFullName;			//< full_name
		std::string	mDescripttion;		//< description
		int			mMemberCount;		//< member_count
		bool		mPublicMode;		//< mode
		SUser		mUser;				//< user
		bool		mFollowFlg;			//< follow
		int			mFollowCount;		//< follow_count
	};
	typedef boost::shared_ptr<SListInfo>	CListPtr;
	typedef std::vector<CListPtr>			CListList;
}

#endif	//TWI_LISTS_DEF_H_