/****************************************************************************/
/* TWI_UserDef.h															*/
/****************************************************************************/
#ifndef TWI_USER_DEF_H_
#define	TWI_USER_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//SUserRelation
	struct SUserRelation
	{
		SUserRelation()
			:mFollowingFlg(false)
			,mFollowedByFlg(false)
		{}

		ID		mTargetID;
		bool	mFollowingFlg;
		bool	mFollowedByFlg;
	};
	typedef boost::shared_ptr<SUserRelation>	CUserRelationPtr;
	typedef std::vector<CUserRelationPtr>		CUserRelationList;

	//User
	struct SUser
	{
		SUser()
			:mUserID(UNKNOWN_ID)
			,mStatusCount(0)
			,mFollowCount(0)
			,mFollowersCount(0)
		{}

		ID			mUserID;			//< id
		std::string	mUserName;			//< name;
		std::string	mUserScreenName;	//< screen_name
		std::string	mUserIconURL;		//< profile_image_url_https
		std::string	mLocation;			//< location
		std::string	mURL;				//< url
		std::string	mDescription;		//< discription
		int			mStatusCount;		//< statuses_count
		int			mFollowCount;		//< friends_count
		int			mFollowersCount;	//< followers_count
		bool		mProtectedFlg;		//< protected
		bool		mFollowingFlg;		//< following
	};
	typedef boost::shared_ptr<SUser>	CUserPtr;
	typedef std::vector<CUserPtr>		CUserList;
}

#endif	//TWI_USER_DEF_H_