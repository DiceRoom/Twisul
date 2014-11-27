/****************************************************************************/
/* TWI_StatusDef.h															*/
/****************************************************************************/
#ifndef TWI_STATUS_DEF_H_
#define	TWI_STATUS_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//PostStatusArgs
	struct SPostStatusArgs
	{
		SPostStatusArgs()
			:mReplyStatusID(UNKNOWN_ID)
		{}

		std::string	mStatus;			//< text
		ID			mReplyStatusID;		//< in_reply_to_status_id
	};

	//RetweetStatus
	struct SRetweetStatus
	{
		SRetweetStatus():mValidFlg(false){}
		bool		mValidFlg;		//< ValidFlg
		ID			mUserID;		//< POST UserID
		std::string	mUserName;		//< POST UserName
		std::string	mScreenName;	//< POST ScreenName
		ID			mPostID;		//< id
		std::string	mUserIconURL;	//< profile_image_url_https
	};

	//Memtion
	struct SMention
	{
		ID			mUserID;
		std::string mScreenName;
		std::string mUserName;
		int			mBeginPos;
		int			mLength;
	};

	//HashTag
	struct SHashTag
	{
		std::string mTagName;
		int			mBeginPos;
		int			mLength;
	};

	//Url
	struct SUrl
	{
		std::string mExpandedURL;
		int			mBeginPos;
		int			mLength;
	};

	//Photo
	struct SPhoto
	{
		std::string mExpandedURL;
		std::string mPhotoURL;
		std::string mThumbURL;
		int			mBeginPos;
		int			mLength;

		static bool sSmallPhotoFlg;
	};

	//Movie
	struct SMovie
	{
		std::string mMovieURL;
		std::string mThumbURL;
		int			mBeginPos;
		int			mLength;
	};

	//Status
	struct SStatus
	{
		SStatus()
			:mUserID(UNKNOWN_ID)
			,mPostID(UNKNOWN_ID)
			,mToReplyStateID(UNKNOWN_ID)
			,mToReplyUserID(UNKNOWN_ID)
			,mRetweetCount(0)
			,mRetweetFlg(false)
			,mFavoritesFlg(false)
			,mUpdateTimeOfUserInfo(0)
			,mUserInfoIndex(0)
			,mCacheNonDeleteFlg(false)
		{}
        ~SStatus(){};

		std::string				mUserName;			//< POST(or RT)UserName
		ID						mUserID;			//< user_id
		std::string				mScreenName;		//< screen_name
		std::string				mUserIconURL;		//< profile_image_url_https
		ID						mPostID;			//< id
		std::string				mText;				//< text
		ID						mToReplyStateID;	//< in_reply_to_status_id
		ID						mToReplyUserID;		//< in_reply_to_user_id
		SDate					mLogDate;			//< created_at
		std::string				mClientSource;		//< PostClientURL
		std::string				mClientName;		//< PostClientName
		int						mRetweetCount;		//< RetweetCount
		bool					mRetweetFlg;		//< MyRetweet
		bool					mFavoritesFlg;		//< MyFavrite
		SRetweetStatus			mRetweetStatus;		//< RetweetSourceStatus
		
		//text option
		typedef boost::shared_ptr<SMention>	CMentionPtr;
		typedef boost::shared_ptr<SHashTag>	CHashPtr;
		typedef boost::shared_ptr<SUrl>		CUrlPtr;
		typedef boost::shared_ptr<SPhoto>	CPhotoPtr;
		typedef boost::shared_ptr<SMovie>	CMoviePtr;

		std::vector<CMentionPtr>	mMemtionList;
		std::vector<CHashPtr>		mHashTagList;
		std::vector<CUrlPtr>		mURLList;
		std::vector<CPhotoPtr>		mPhotoList;
		std::vector<CMoviePtr>		mMovieList;

		//out side
		mutable unsigned long long		mUpdateTimeOfUserInfo;
		mutable unsigned int			mUserInfoIndex;
		mutable bool					mCacheNonDeleteFlg;
	};

	typedef boost::shared_ptr<const SStatus>	CStatusPtr;
	typedef std::vector<CStatusPtr>				CStatusList;
}

#endif	//TWI_STATUS_DEF_H_