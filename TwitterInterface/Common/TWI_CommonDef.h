/****************************************************************************/
/* TWI_CommonDef.h															*/
/****************************************************************************/
#ifndef TWI_COMMON_DEF_H_
#define	TWI_COMMON_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <string>
#include <vector>
#include <map>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include "../../Xml/XmlAnalyzer.h"
#include "../../Xml/XmlNode.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* ID																	*/
	/************************************************************************/
	typedef unsigned long long	ID;
	typedef std::vector<ID>	CIDList;
	static const ID UNKNOWN_ID = static_cast<ID>(-1);

	/************************************************************************/
	/* Common																*/
	/************************************************************************/

	//Date
	struct SDate
	{
		int			mYear;
		int			mMonth;
		int			mDay;
		std::string mWeek;
		int			mHour;
		int			mMinute;
		int			mSecond;
		std::string	mTimeZone;
	};

	//Cursor
	struct SCursor
	{
		SCursor()
			:mPrevCursor(UNKNOWN_ID)
			,mNextCursor(mPrevCursor)
		{}

		ID	mPrevCursor;
		ID	mNextCursor;
	};

	//GetArgs
	struct SGetArgs
	{
		SGetArgs()
			:mCount(-1)
			,mSinceID(UNKNOWN_ID)
			,mMaxID(UNKNOWN_ID)
			,mIncludeRTFlg(false)
			,mPage(-1)
		{};

		int			mCount;			//< count
		ID			mSinceID;		//< since_id
		ID			mMaxID;			//< max_id
		bool		mIncludeRTFlg;	//< include_rts
		std::string	mLanguage;		//< language
		int			mPage;			//< page
	};

	//RateLimit
	struct SRateLimit
	{
		int			mLimit;
		int			mRemaining;
		int			mResetTime;
	};
	typedef boost::shared_ptr<SRateLimit>			CRateLimitPtr;
	typedef std::map<std::string , CRateLimitPtr>	CRateLimitMap;

	/************************************************************************/
	/* Response																*/
	/************************************************************************/

	//HttpState
	enum EHttpState
	{
		HTTP_STATE_OK,
		HTTP_STATE_NOT_MODIFIED,
		HTTP_STATE_BAD_REQUEST,
		HTTP_STATE_NOT_AUTHORIZED,
		HTTP_STATE_FORBIDDEN,
		HTTP_STATE_NOT_FOUND,
		HTTP_STATE_INTERNAL_SERVER_ERROR,
		HTTP_STATE_BAD_GATEWAY,
		HTTP_STATE_SERVICE_UNAVALIABLE,
		HTTP_STATE_BAD_HEADER_LIST,
		HTTP_STATE_ERROR_UNDEFIINED,
	};

	//Response
	struct SResponse
	{
		EHttpState	mGetHttpState;
		SRateLimit	mRateLimit;
		std::string	mGetData;
	};
}

#endif	//TWI_COMMON_DEF_H_