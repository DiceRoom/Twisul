/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Getter.h"
#include "../System/TWI_System.h"
#include "../Common/TWI_Utility.h"
#include <boost/regex.hpp>

#ifdef WIN32
#else
#include "TWI_ObjCEncoding.h"
#endif

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace
{
	//
	struct SPhotoRegexInfo
	{
		SPhotoRegexInfo(const std::string& iThumbKeyword , const std::string& iSmallPhotoKeyword , const std::string& iLargePhotoKeyword)
			:mThumbKeyword(iThumbKeyword)
			,mSmallPhotoKeyword(iSmallPhotoKeyword)
			,mLargePhotoKeyword(iLargePhotoKeyword)
		{}

		//variable
		std::string mThumbKeyword;
		std::string mSmallPhotoKeyword;
		std::string mLargePhotoKeyword;
	};

	typedef std::pair<boost::regex , SPhotoRegexInfo> CPhotoRegexPair;
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace
{
	//status param list
	static const int STATUS_TAG_VALUE = 9;
	std::string STATUS_TAG_LIST[STATUS_TAG_VALUE] =
	{
		"in_reply_to_status_id_str",
		"in_reply_to_user_id_str",
		"created_at",
		"retweet_count",
		"retweeted",
		"favorited",
		"text",
		"source",
		"id_str",
	};

	//directmessage param list
	static const int DM_TAG_VALUE = 7;
	std::string DM_TAG_LIST[DM_TAG_VALUE] =
	{
		"created_at",
		"id_str",
		"text",
		"sender_id",
		"sender_screen_name",
		"recipient_id",
		"recipient_screen_name",
	};

	//status user param list
	static const int STATUS_USER_TAG_VALUE = 4;
	std::string STATUS_USER_TAG_LIST[STATUS_USER_TAG_VALUE] =
	{
		"name",
		"id_str",
		"screen_name",
		"profile_image_url_https",
	};

	//user param list
	static const int USER_TAG_VALUE = 12;
	std::string USER_TAG_LIST[USER_TAG_VALUE] =
	{
		"id_str",
		"name",
		"screen_name",
		"profile_image_url_https",
		"location",
		"url",
		"description",
		"statuses_count",
		"friends_count",
		"followers_count",
		"protected",
		"following",
	};

	//place param list
	static const int PLACE_TAG_VALUE = 6;
	std::string PLACE_TAG_LIST[PLACE_TAG_VALUE] =
	{
		"country",
		"countryCode",
		"name",
		"parentid",
		"url",
		"woeid",
	};

	//list param list
	static const int LIST_TAG_VALUE = 8;
	std::string LIST_TAG_LIST[LIST_TAG_VALUE] =
	{
		"id_str",
		"name",
		"full_name",
		"description",
		"member_count",
		"mode",
		"following",
		"subscriber_count",
	};

	//trends param list
	static const int TRENDS_TAG_VALUE = 5;
	std::string TRENDS_TAG_LIST[TRENDS_TAG_VALUE] =
	{
		"events",
		"name",
		"promoted_content",
		"query",
		"url"
	};

	//geo param list
	static const int GEO_TAG_VALUE = 6;
	std::string GEO_TAG_LIST[GEO_TAG_VALUE] =
	{
		"id",
		"country",
		"country_code",
		"name",
		"full_name",
		"place_type"
	};

	//search param list
	static const int SEARCH_TAG_VALUE = 4;
	std::string SEARCH_TAG_LIST[SEARCH_TAG_VALUE] =
	{
		"id_str",
		"name",
		"query",
		"created_at",
	};

	//ratelimit param list
	static const int RATELIMIT_TAG_VALUE = 3;
	std::string RATELIMIT_TAG_LIST[RATELIMIT_TAG_VALUE] =
	{
		"limit",
		"remaining",
		"reset",
	};

	//movie regex
	static const boost::regex YOUTUBE_REGEX("(?:www.youtube.com/watch\\?.*v=|youtu.be/)(\\w)");
	static const boost::regex NICONICO_REGEX("(?:www.nicovideo.jp/watch/sm|nico.ms/sm)(\\w)");


	//photo regex
	static const boost::regex TWIPPLE_REGEX("(?:http://p.twipple.jp/)(\\w)");
	static const boost::regex TWITPIC_REGEX("(?:http://twitpic.com/)(\\w)");
	static const boost::regex YFROG_REGEX1("(?:http://yfrog.com/)(\\w)");
	static const boost::regex YFROG_REGEX2("(?:http://twitter.yfrog.com/)(\\w)");
	static const std::map<boost::regex , SPhotoRegexInfo> PHOTO_TRANS_MAP = boost::assign::map_list_of
			(TWIPPLE_REGEX	, SPhotoRegexInfo("http://p.twipple.jp/show/thumb/%s" , "http://p.twipple.jp/show/large/%s" , "http://p.twipple.jp/show/large/%s"))
			(TWITPIC_REGEX	, SPhotoRegexInfo("http://twitpic.com/show/thumb/%s" , "http://twitpic.com/show/large/%s" , "http://twitpic.com/show/full/%s"))
			(YFROG_REGEX1	, SPhotoRegexInfo("http://yfrog.com/%s.th.jpg" , "http://yfrog.com/%s:medium" , "http://yfrog.com/%s:small"))
			(YFROG_REGEX2	, SPhotoRegexInfo("http://yfrog.com/%s.th.jpg" , "http://yfrog.com/%s:medium" , "http://yfrog.com/%s:small"));
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace
{
	void MultiToWide(std::wstring &oWide , const std::string &iMulti)
	{
		wchar_t *wcs = new wchar_t[iMulti.length() + 1];
		mbstowcs(wcs, iMulti.c_str(), iMulti.length() + 1);
		oWide = wcs;
		delete [] wcs;
	}

	void WideToMulti(std::string &oMulti , const std::wstring &iWide)
	{
		char *mbs = new char[iWide.length() * MB_CUR_MAX + 1];
		wcstombs(mbs, iWide.c_str(), iWide.length() * MB_CUR_MAX + 1);
		oMulti = mbs;
		delete [] mbs;
	}
}

namespace TSUL
{
	/************************************************************************/
	/* Getter																*/
	/************************************************************************/

	bool CGetter::GetStatus(CStatusPtr& oStatusPtr , CXmlNode* iStateNode)
	{
		SStatus status;
		if(_GetStatus(status , iStateNode))
		{
			//in cache
			oStatusPtr = CStatusPtr(new SStatus(status));
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CGetter::GetDirectMessage(CDirectMessagePtr& oDirectMessage , CXmlNode* iNode)
	{
		std::string buf_List[DM_TAG_VALUE];
		if(!_GetParams(buf_List , DM_TAG_LIST , DM_TAG_VALUE , iNode))
		{
			return false;
		}

		SDirectMessage dm;
		char get_Buf[3][16];
		sscanf(buf_List[0].c_str(),
			"%s %s %d %d:%d:%d +%s %d",
			&get_Buf[0][0],
			&get_Buf[1][0],
			&dm.mLogData.mDay,
			&dm.mLogData.mHour,
			&dm.mLogData.mMinute,
			&dm.mLogData.mSecond,
			&get_Buf[2][0],
			&dm.mLogData.mYear);
		dm.mLogData.mWeek		= get_Buf[0];
		dm.mLogData.mMonth		= GetMonthIndex(get_Buf[1]);
		dm.mLogData.mTimeZone	= get_Buf[2];
		dm.mMessageID			= CastParameter<ID>(buf_List[1] , UNKNOWN_ID);
		dm.mText				= CSystem::Decode(buf_List[2]);
		dm.mSenderID			= CastParameter<ID>(buf_List[3] , UNKNOWN_ID);
		dm.mSenderScreenName	= buf_List[4];
		dm.mRecipientID			= CastParameter<ID>(buf_List[5] , 0);
		dm.mRecipientScreenName = buf_List[6];

		GetFirstNodeValue(dm.mSenderName , "name" , iNode -> GetChildNode("sender")[0]);
		dm.mSenderName = CSystem::Decode(dm.mSenderName);
		GetFirstNodeValue(dm.mRecipientName , "name" , iNode -> GetChildNode("recipient")[0]);
		dm.mRecipientName = CSystem::Decode(dm.mRecipientName);

		oDirectMessage = CDirectMessagePtr(new SDirectMessage(dm));
		return true;
	}

	bool CGetter::GetUser(CUserPtr& oUserPtr , CXmlNode* iUserNode)
	{
		std::string user_Buf_List[USER_TAG_VALUE];
		if(!_GetParams(user_Buf_List , USER_TAG_LIST , USER_TAG_VALUE , iUserNode) ||
		   user_Buf_List[11].length() == 0)
		{
			return false;
		}

		oUserPtr = CUserPtr(new SUser());
		oUserPtr -> mUserID			= CastParameter<ID>(user_Buf_List[0] , UNKNOWN_ID);
		oUserPtr -> mUserName		= CSystem::Decode(user_Buf_List[1]);
		oUserPtr -> mUserScreenName = user_Buf_List[2];
		oUserPtr -> mUserIconURL	= user_Buf_List[3];
		oUserPtr -> mLocation		= CSystem::Decode(user_Buf_List[4]);
		oUserPtr -> mURL			= (user_Buf_List[5] == "null") ? "" : user_Buf_List[5];
		oUserPtr -> mDescription	= CSystem::Decode(user_Buf_List[6]);
		oUserPtr -> mStatusCount	= CastParameter<int>(user_Buf_List[7] , 0);
		oUserPtr -> mFollowCount	= CastParameter<int>(user_Buf_List[8] , 0);
		oUserPtr -> mFollowersCount = CastParameter<int>(user_Buf_List[9] , 0);
		oUserPtr -> mProtectedFlg	= (user_Buf_List[10] == "true");
		oUserPtr -> mFollowingFlg	= (user_Buf_List[11] == "true");
		return true;
	}

	bool CGetter::GetPlace(CPlacePtr& oPlace , CXmlNode* iNode)
	{
		CNodeList place_List = iNode -> GetChildNode("placeType");
		if(place_List.size() == 0)
		{
			return false;
		}

		std::string buf_List[PLACE_TAG_VALUE + 2];
		if(!_GetParams(buf_List , PLACE_TAG_LIST , PLACE_TAG_VALUE , iNode))
		{
			return false;
		}
	
		//place
		CXmlNode* place_Node = place_List[0];
		if(!GetFirstNodeValue(buf_List[PLACE_TAG_VALUE]		, "code" , place_Node) ||
		   !GetFirstNodeValue(buf_List[PLACE_TAG_VALUE + 1] , "name" , place_Node))
		{
			return false;
		}

		//get
		oPlace = CPlacePtr(new SPlace());
		oPlace -> mCountry			= CSystem::Decode(buf_List[0]);
		oPlace -> mCountryCode		= buf_List[1];
		oPlace -> mName				= CSystem::Decode(buf_List[2]);
		oPlace -> mParentID			= CastParameter<ID>(buf_List[3].substr(0 , buf_List[3].find('.')) , UNKNOWN_ID);
		oPlace -> mUrl				= buf_List[4];
		oPlace -> mWOEID			= CastParameter<ID>(buf_List[5].substr(0 , buf_List[5].find('.')) , UNKNOWN_ID);
		oPlace -> mPlaceTypeCode	= CastParameter<int>(buf_List[6].substr(0 , buf_List[6].find('.')) , 0);
		oPlace -> mPlaceTypeName	= buf_List[7];
		return true;
	}

	bool CGetter::GetLists(CListPtr& oList , CXmlNode* iNode)
	{
		std::string buf_List[LIST_TAG_VALUE];
		if(!_GetParams(buf_List , LIST_TAG_LIST , LIST_TAG_VALUE , iNode))
		{
			return false;
		}

		//User
		CUserPtr user;
		if(!GetUser(user , iNode -> GetChildNode("user")[0]))
		{
			return false;
		}

		//translate
		oList = CListPtr(new SListInfo());
		oList -> mID = CastParameter<ID>(buf_List[0] , UNKNOWN_ID);
		oList -> mListName = CSystem::Decode(buf_List[1]);
		oList -> mFullName = CSystem::Decode(buf_List[2]);
		oList -> mDescripttion = CSystem::Decode(buf_List[3]);
		oList -> mMemberCount = CastParameter<int>(buf_List[4] , 0);
		oList -> mPublicMode = (buf_List[5] == "public");
		oList -> mFollowFlg = (buf_List[6] == "true");
		oList -> mFollowCount = CastParameter<int>(buf_List[7] , 0);
		oList -> mUser = (*user);

		return true;
	}


	bool CGetter::GetTrendInfo(CTrendsInfoPtr& oTrendInfo , CXmlNode* iNode)
	{
		std::string buf_List[TRENDS_TAG_VALUE];
		if(!_GetParams(buf_List , TRENDS_TAG_LIST , TRENDS_TAG_VALUE , iNode))
		{
			return false;
		}

		oTrendInfo = CTrendsInfoPtr(new STrendInfo());
		oTrendInfo -> mEvents = CSystem::Decode(buf_List[0]);
		oTrendInfo -> mName = CSystem::Decode(buf_List[1]);
		oTrendInfo -> mPromotedContent = CSystem::Decode(buf_List[2]);
		oTrendInfo -> mQuery = CSystem::Decode(buf_List[3]);
		oTrendInfo -> mUrl = buf_List[4];
		return true;
	}
 
	bool CGetter::GetGeo(CGeoPtr& oGeo , CXmlNode* iNode)
	{
		std::string buf_List[GEO_TAG_VALUE];
		if(!_GetParams(buf_List , GEO_TAG_LIST , GEO_TAG_VALUE , iNode))
		{
			return false;
		}

		//bounding box
		CNodeList bdb_List = iNode -> GetChildNode("bounding_box");
		if(bdb_List.size() == 0)
		{
			return false;
		}
		CNodeList type_List = bdb_List[0] -> GetChildNode("type");
		if(type_List.size() == 0)
		{
			return false;
		}
		
		//base
		oGeo -> mID = buf_List[0];
		oGeo -> mCountry = CSystem::Decode(buf_List[1]);
		oGeo -> mCountryCode = buf_List[2];
		oGeo -> mName = CSystem::Decode(buf_List[3]);
		oGeo -> mFullName = CSystem::Decode(buf_List[4]);
		oGeo -> mPlaceType = buf_List[5];

		//attribute
		CNodeList attr_List = iNode -> GetChildNode("attributes");
		if(attr_List.size() != 0)
		{
			BOOST_FOREACH(CXmlNode* iAttrNode , attr_List[0] -> GetChildNode())
			{
				const SXmlNodeInfo& info = iAttrNode -> GetNodeInfo();
				oGeo -> mAttributeList.push_back(std::make_pair(info.mTagName , info.mValue));
			}
		}

		//bounding box
		oGeo -> mCoordinateType = type_List[0] -> GetNodeInfo().mValue;
		CNodeList coordinate_List = bdb_List[0] -> GetChildNode("coordinates");
		if(coordinate_List.size() != 0)
		{
			coordinate_List = coordinate_List[0] -> GetChildNode();
			BOOST_FOREACH(CXmlNode* iCoordinateNode , coordinate_List[0] -> GetChildNode())
			{
				CNodeList val_List = iCoordinateNode -> GetChildNode();
				float set_Lat = CastParameter(val_List[0] -> GetNodeInfo().mValue , 360.f);
				float set_Long = CastParameter(val_List[1] -> GetNodeInfo().mValue , 360.f);;
				oGeo -> mBoundingBoxCoordinateList.push_back(std::make_pair(set_Lat , set_Long));
			}
		}

		return true;
	}

	bool CGetter::GetSearchSaveInfo(CSearchSaveInfoPtr& oSearchSaveInfo , CXmlNode* iNode)
	{
		std::string buf_List[SEARCH_TAG_VALUE];
		if(!_GetParams(buf_List , SEARCH_TAG_LIST , SEARCH_TAG_VALUE , iNode))
		{
			return false;
		}
		
		oSearchSaveInfo = CSearchSaveInfoPtr(new SSearchSaveInfo());
		oSearchSaveInfo -> mID = CastParameter<ID>(buf_List[0] , UNKNOWN_ID);
		oSearchSaveInfo -> mName = CSystem::Decode(buf_List[1]);
		oSearchSaveInfo -> mQuery = CSystem::Decode(buf_List[2]);

		//date
		char get_Buf[3][16];
		sscanf(buf_List[3].c_str(),
			"%s %s %d %d:%d:%d +%s %d",
			&get_Buf[0][0],
			&get_Buf[1][0],
			&oSearchSaveInfo -> mCreateDate.mDay,
			&oSearchSaveInfo -> mCreateDate.mHour,
			&oSearchSaveInfo -> mCreateDate.mMinute,
			&oSearchSaveInfo -> mCreateDate.mSecond,
			&get_Buf[2][0],
			&oSearchSaveInfo -> mCreateDate.mYear);
		oSearchSaveInfo -> mCreateDate.mWeek = get_Buf[0];
		oSearchSaveInfo -> mCreateDate.mMonth = GetMonthIndex(get_Buf[1]);
		oSearchSaveInfo -> mCreateDate.mTimeZone = get_Buf[2];

		return true;
	}

	bool CGetter::GetRateLimitMap(CRateLimitMap& oRateLimitMap , CXmlNode* iNode)
	{
		std::string buf_List[RATELIMIT_TAG_VALUE];
		BOOST_FOREACH(CXmlNode* iCategoryNode , iNode -> GetChildNode())
		{
			BOOST_FOREACH(CXmlNode* iLimitNode , iCategoryNode ->GetChildNode())
			{
				if(!_GetParams(buf_List , RATELIMIT_TAG_LIST , RATELIMIT_TAG_VALUE , iLimitNode))
				{
					return false;
				}

				CRateLimitPtr rate_Limit(new SRateLimit());
				rate_Limit -> mLimit		= CastParameter<int>(buf_List[0] , -1);
				rate_Limit -> mRemaining	= CastParameter<int>(buf_List[1] , -1);
				rate_Limit -> mResetTime	= CastParameter<int>(buf_List[2] , -1);
				oRateLimitMap.insert(std::make_pair(iLimitNode -> GetNodeInfo().mTagName.substr(1) , rate_Limit));
			}
		}

		return true;
	}

	/************************************************************************/
	/* Inner Decl															*/
	/************************************************************************/

	bool CGetter::_GetStatus(SStatus& oStatus , CXmlNode* iStateNode)
	{
		//get user
		CNodeList user_List = iStateNode -> GetChildNode("user");
		if(user_List.size() == 0)
		{
			return false;
		}
		CXmlNode* user_Node = user_List[0];
		std::string user_Buf_List[STATUS_USER_TAG_VALUE];
		if(!_GetParams(user_Buf_List , STATUS_USER_TAG_LIST , STATUS_USER_TAG_VALUE , user_Node))
		{
			return false;
		}

		//RT
		CNodeList retweet_List = iStateNode -> GetChildNode("retweeted_status");
		if(retweet_List.size() != 0)
		{
			//set retweet state
			oStatus.mRetweetStatus.mValidFlg = true;
			oStatus.mRetweetStatus.mUserName	= CSystem::Decode(user_Buf_List[0]);
			oStatus.mRetweetStatus.mUserID		= CastParameter<ID>(user_Buf_List[1] , UNKNOWN_ID);
			oStatus.mRetweetStatus.mScreenName	= CSystem::Decode(user_Buf_List[2]);
			oStatus.mRetweetStatus.mUserIconURL	= user_Buf_List[3];
			std::string post_ID;
			if(!GetFirstNodeValue(post_ID , "id_str" , iStateNode))
			{
				return false;
			}
			oStatus.mRetweetStatus.mPostID		= CastParameter<ID>(post_ID , UNKNOWN_ID);

			//get status
			if(!_GetStatus(oStatus , retweet_List[0]))
			{
				return false;
			}
		}
		//Status
		else
		{
			//get buf
			std::string buf_List[STATUS_TAG_VALUE];
			
			if(!_GetParams(buf_List , STATUS_TAG_LIST , STATUS_TAG_VALUE , iStateNode))
			{
				return false;
			}

			//set param
			oStatus.mToReplyStateID =	buf_List[0] == "null" ? UNKNOWN_ID : CastParameter<ID>(buf_List[0] , UNKNOWN_ID);
			oStatus.mToReplyUserID =	buf_List[1] == "null" ? UNKNOWN_ID : CastParameter<ID>(buf_List[1] , UNKNOWN_ID);
			char get_Buf[3][16];
			sscanf(buf_List[2].c_str(),
				   "%s %s %d %d:%d:%d +%s %d",
				   &get_Buf[0][0],
				   &get_Buf[1][0],
				   &oStatus.mLogDate.mDay,
				   &oStatus.mLogDate.mHour,
				   &oStatus.mLogDate.mMinute,
				   &oStatus.mLogDate.mSecond,
				   &get_Buf[2][0],
				   &oStatus.mLogDate.mYear);
			oStatus.mLogDate.mWeek		= get_Buf[0];
			oStatus.mLogDate.mMonth		= GetMonthIndex(get_Buf[1]);
			oStatus.mLogDate.mTimeZone	= get_Buf[2];
			oStatus.mRetweetCount		= CastParameter<int>(buf_List[3] , 0);
			oStatus.mRetweetFlg			= (buf_List[4] == "true");
			oStatus.mFavoritesFlg		= (buf_List[5] == "true");
			oStatus.mText				= CSystem::Decode(buf_List[6]);
			oStatus.mClientSource = oStatus.mClientName = CSystem::Decode(buf_List[7]);
			int find_Pos = oStatus.mClientName.find_last_of("</a>");
			if(find_Pos != -1)
			{
				oStatus.mClientName = oStatus.mClientName.erase(find_Pos - 3);
				find_Pos = oStatus.mClientName.find_last_of(">");
				if(find_Pos != -1)
				{
					oStatus.mClientName = oStatus.mClientName.substr(find_Pos + 1);
				}
			}
			oStatus.mPostID				= CastParameter<ID>(buf_List[8] , UNKNOWN_ID);
			
			//User
			oStatus.mUserName		= CSystem::Decode(user_Buf_List[0]);
			oStatus.mUserID			= CastParameter<ID>(user_Buf_List[1] , UNKNOWN_ID);
			oStatus.mScreenName		= CSystem::Decode(user_Buf_List[2]);
			oStatus.mUserIconURL	= user_Buf_List[3];
			
			//media
			if(!_GetMedia(oStatus , buf_List[6] , iStateNode))
			{
				return false;
			}
		}
		return true;
	}

	bool CGetter::_GetMedia(SStatus& oStatus , const std::string& iText , CXmlNode* iBaseNode)
	{
		CNodeList entities_List = iBaseNode -> GetChildNode("entities");
		if(entities_List.size() != 0)
		{
			std::string check_Str = iText;
			COffsetList offset_List;
#ifdef WIN32
			int pos = 0;
			while(pos != -1)
			{
				pos = check_Str.find("&" , pos);
				int end_Pos = check_Str.find(";" , pos);
				if(pos != -1 && end_Pos != -1)
				{
					offset_List.push_back(std::make_pair(pos , end_Pos - pos));
					pos = end_Pos + 1;
				}
			}
            
#else
            calcEscapeRange(check_Str, offset_List);
#endif

			CXmlNode* entities_Node = entities_List[0];
			
			//get each entities
			if(!_GetMention	(oStatus , entities_Node , offset_List) ||
			   !_GetHashTag	(oStatus , entities_Node , offset_List) ||
			   !_GetUrl		(oStatus , entities_Node , offset_List) ||
			   !_GetPhoto	(oStatus , entities_Node , offset_List))
			{
				return false;
			}
		}

		return true;
	}

	bool CGetter::_GetMention(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList)
	{
		BOOST_FOREACH(CXmlNode* iNode , iEntitiesNode -> GetChildNode("user_mentions")[0] -> GetChildNode())
		{
			SStatus::CMentionPtr mention(new SMention());
			if(!_GetEntityIndices(mention -> mBeginPos , mention -> mLength , iNode , iOffsetList))
			{
				return false;
			}

			mention -> mUserID = boost::lexical_cast<ID>(iNode -> GetChildNode("id_str")[0] -> GetNodeInfo().mValue);
			mention -> mUserName = CSystem::Decode(iNode -> GetChildNode("name")[0] -> GetNodeInfo().mValue);
			mention -> mScreenName = iNode -> GetChildNode("screen_name")[0] -> GetNodeInfo().mValue;
			oStatus.mMemtionList.push_back(mention);
		}

		return true;
	}

	bool CGetter::_GetHashTag(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList)
	{
		BOOST_FOREACH(CXmlNode* iNode , iEntitiesNode -> GetChildNode("hashtags")[0] -> GetChildNode())
		{
			SStatus::CHashPtr tag(new SHashTag());
			if(!_GetEntityIndices(tag -> mBeginPos , tag -> mLength , iNode , iOffsetList))
			{
				return false;
			}

			tag -> mTagName = CSystem::Decode(iNode -> GetChildNode("text")[0] -> GetNodeInfo().mValue);
			oStatus.mHashTagList.push_back(tag);
		}

		return true;
	}

	bool CGetter::_GetUrl(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList)
	{
		BOOST_FOREACH(CXmlNode* iNode , iEntitiesNode -> GetChildNode("urls")[0] -> GetChildNode())
		{
			std::string url_Str = iNode -> GetChildNode("expanded_url")[0] -> GetNodeInfo().mValue;

			//youtube
			boost::smatch result;
			if(boost::regex_search(url_Str , result , YOUTUBE_REGEX))
			{
				//feature erase
				int pos = url_Str.find("&feature=");
				if(pos != -1)
				{
					url_Str = url_Str.substr(0 , pos);
				}

				size_t result_Pos = result.position(1);
				if(url_Str.length() > result_Pos)
				{
					pos = url_Str.find("?" , result_Pos);
					if(pos != -1)
					{
						url_Str = url_Str.substr(0 , pos);
					}
				}

				SStatus::CMoviePtr movie(new SMovie());
				_GetEntityIndices(movie -> mBeginPos , movie -> mLength , iNode , iOffsetList);
				movie -> mThumbURL = "http://i.ytimg.com/vi/" + url_Str.substr(result.position(1)) + "/1.jpg";
				movie -> mMovieURL = url_Str;
				oStatus.mMovieList.push_back(movie);
				continue;
			}

			//niconico
			if(boost::regex_search(url_Str , result , NICONICO_REGEX))
			{
				SStatus::CMoviePtr movie(new SMovie());
				_GetEntityIndices(movie -> mBeginPos , movie -> mLength , iNode , iOffsetList);
				ID id = boost::lexical_cast<ID>(url_Str.substr(result.position(1)));
				std::string host = boost::lexical_cast<std::string>((id % 4) + 1);
				movie -> mThumbURL = "http://tn-skr" + host + ".smilevideo.jp/smile?i=" + boost::lexical_cast<std::string>(id);
				movie -> mMovieURL = url_Str;
				oStatus.mMovieList.push_back(movie);
				continue;
			}

			//photo
			std::string extention = "";
			int pos = url_Str.find_last_of(".");
			if(pos != -1)
			{
				extention = url_Str.substr(pos + 1);
				std::transform(extention.begin() , extention.end() , extention.begin() , tolower);
			}
			if(extention == "png" || extention == "gif" || extention == "jpg" || extention == "bmp")
			{
				SStatus::CPhotoPtr photo(new SPhoto());
				_GetEntityIndices(photo -> mBeginPos , photo -> mLength , iNode , iOffsetList);
				photo -> mPhotoURL = photo -> mThumbURL = photo -> mExpandedURL = url_Str;
				oStatus.mPhotoList.push_back(photo);
			}
			//other url
			else
			{
				//other photo
				bool match_Flg = false;
				BOOST_FOREACH(const CPhotoRegexPair& iPair , PHOTO_TRANS_MAP)
				{
					match_Flg = (boost::regex_search(url_Str , result , iPair.first));
					if(match_Flg)
					{
						SStatus::CPhotoPtr photo(new SPhoto());
						_GetEntityIndices(photo -> mBeginPos , photo -> mLength , iNode , iOffsetList);
						photo -> mPhotoURL = "";
						photo -> mExpandedURL = url_Str;

						char buf[512];
						std::string get_Str = url_Str.substr(result.position(1));
						
						sprintf(buf , iPair.second.mThumbKeyword.c_str() , get_Str.c_str());
						photo -> mThumbURL = buf;

						if(SPhoto::sSmallPhotoFlg)
						{
							sprintf(buf , iPair.second.mSmallPhotoKeyword.c_str() , get_Str.c_str());
						}
						else
						{
							sprintf(buf , iPair.second.mLargePhotoKeyword.c_str() , get_Str.c_str());
						}
						photo -> mPhotoURL = buf;
						oStatus.mPhotoList.push_back(photo);
						
						break;
					}
				}

				if(!match_Flg)
				{
					SStatus::CUrlPtr url(new SUrl());
					_GetEntityIndices(url -> mBeginPos , url -> mLength , iNode , iOffsetList);
					url -> mExpandedURL = url_Str;
					oStatus.mURLList.push_back(url);
				}
			}
		}
		
		return true;
	}

	bool CGetter::_GetPhoto(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList)
	{
		BOOST_FOREACH(CXmlNode* iMediaNode , iEntitiesNode -> GetChildNode("media"))
		{
			BOOST_FOREACH(CXmlNode* iNode , iMediaNode -> GetChildNode())
			{
				if(iNode -> GetChildNode("type")[0] -> GetNodeInfo().mValue == "photo")
				{
					SStatus::CPhotoPtr photo(new SPhoto());
					if(!_GetEntityIndices(photo -> mBeginPos , photo -> mLength , iNode , iOffsetList))
					{
						return false;
					}

					photo -> mPhotoURL = iNode -> GetChildNode("media_url")[0] -> GetNodeInfo().mValue;
					photo -> mThumbURL = photo -> mPhotoURL + ":thumb";
					oStatus.mPhotoList.push_back(photo);
				}
			}
		}

		return true;
	}

	bool CGetter::_GetEntityIndices(int& oBeginPos , int& oLength , CXmlNode* iNode , const COffsetList& iOffsetList)
	{
		CNodeList ind_List = iNode -> GetChildNode("indices");
		if(ind_List.size() == 0)
		{
			return false;
		}
		CNodeList pos_List = ind_List[0] -> GetChildNode();
		if(pos_List.size() < 2)
		{
			return false;
		}

		oBeginPos = CastParameter<int>(pos_List[0] -> GetNodeInfo().mValue , -1);
		oLength = CastParameter<int>(pos_List[1] -> GetNodeInfo().mValue , -1);
		if(oBeginPos == -1 || oLength == -1)
		{
			return false;
		}
		oLength -= oBeginPos;

		//slide offset
		int offset = 0;
		BOOST_FOREACH(const COffsetPair& iPair , iOffsetList)
		{
			if(iPair.first > oBeginPos)
			{
				break;
			}
			offset += iPair.second;
		}
		oBeginPos -= offset;
		return true;
	}

	bool CGetter::_GetParams(std::string* oBufferList , const std::string* iTagList , int iBufferSize , CXmlNode* iNode)
	{
		oBufferList , iTagList , iNode , iBufferSize;
		BOOST_FOREACH(int iIndex , EnamerableRange(0 , iBufferSize))
		{
			if(!GetFirstNodeValue(oBufferList[iIndex] , iTagList[iIndex] , iNode))
			{
				return false;
			}

			if(oBufferList[iIndex] == "null")
			{
				oBufferList[iIndex].clear();
			}
		}

		return true;
	}
}