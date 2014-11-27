/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Cache.h"
#include "../TimeLine/TWI_TimeLine.h"
#include <Module/Cache/CacheWriter.h>
#include <Module/Cache/CacheReader.h>
#include <time.h>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//ID finder
	class CUserCacheIDFinder
	{
	public:
		CUserCacheIDFinder(ID iID):mID(iID){};

		bool operator()(SUserCacheInfo::CUserTimeInfoPtr iPtr)
		{
			return (iPtr -> mUserPtr -> mUserID == mID);
		}

	private:
		ID mID;
	};

	//UserName finder
	class CUserCacheUserNameFinder
	{
	public:
		CUserCacheUserNameFinder(const std::string& iUserName):mUserName(iUserName){};

		bool operator()(SUserCacheInfo::CUserTimeInfoPtr iPtr)
		{
			return (iPtr -> mUserPtr -> mUserScreenName == mUserName);
		}

	private:
		std::string mUserName;
	};
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	SStatusCacheInfo CCache::sStatusCache;
	SDMCacheInfo CCache::sDirectMessageCache;
	SUserCacheInfo CCache::sUserCache;
	STrendsPlaceCache CCache::sTrendPlaceCache;

	size_t SStatusCacheInfo::sMaxCacheValue = SStatusCacheInfo::sMaxCacheSize / sizeof(SStatusCacheInfo);
	boost::mutex SStatusCacheInfo::sMutex;
	boost::mutex SDMCacheInfo::sMutex;
	boost::mutex SUserCacheInfo::sMutex;
	size_t SUserCacheInfo::sMaxCacheValue = SUserCacheInfo::sMaxCacheSize / sizeof(SUserCacheInfo);
	boost::mutex STrendsPlaceCache::sMutex;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* state																*/
	/************************************************************************/
	
	bool CCache::AddStatusCache(CStatusPtr iStatus)
	{
        boost::mutex::scoped_lock lock(SStatusCacheInfo::sMutex);
        
		//cache check(LRU)
		ID id = iStatus -> mPostID;
		CIDList::iterator find_Itr = std::find(sStatusCache.mCacheIndexList.begin() , sStatusCache.mCacheIndexList.end() , id);
		if(find_Itr != sStatusCache.mCacheIndexList.end())
		{
			sStatusCache.mCacheIndexList.erase(find_Itr);
			sStatusCache.mCacheIndexList.push_back(id);
			return true;
		}
        
		//check over
		if(sStatusCache.mCacheIndexList.size() >= SStatusCacheInfo::sMaxCacheValue)
		{
			//before delete
			CIDList::iterator itr , end_Itr = sStatusCache.mCacheIndexList.end();
			for(itr = sStatusCache.mCacheIndexList.begin() ; itr != end_Itr ; ++ itr)
			{
				ID id = (*itr);
				if(!sStatusCache.mCacheMap[id] -> mCacheNonDeleteFlg)
				{
					sStatusCache.mCacheIndexList.erase(itr);
					sStatusCache.mCacheMap.erase(sStatusCache.mCacheMap.find(id));
					break;
				}
			}
            
			//non delete
			if(itr == end_Itr)
			{
				return false;
			}
		}
        
		sStatusCache.mCacheIndexList.push_back(id);
		sStatusCache.mCacheMap.insert(std::make_pair(id , iStatus));
		
		return true;
	}
 
	bool CCache::GetStatusCache(CStatusPtr& oStatus , ID iID)
	{
        boost::mutex::scoped_lock lock(SStatusCacheInfo::sMutex);
		SStatusCacheInfo::CCacheMap::const_iterator find_Itr = sStatusCache.mCacheMap.find(iID);
		bool ret_Flg = (find_Itr != sStatusCache.mCacheMap.end());
		if(ret_Flg)
		{
			oStatus = (*find_Itr).second;
		}
		return ret_Flg;
	}

	bool CCache::SetStatusCacheData(const char* iData , size_t)
	{
		ClearStatusCacheData();

		boost::mutex::scoped_lock lock(SStatusCacheInfo::sMutex);
		CCacheReader reader(iData);
		
		//version check
		float version;
		reader.ReadMemory(version);
		if(version != CACHE_VERSION)
		{
			return false;
		}
	
		//read index
		size_t index_Val; 
		reader.ReadMemory(index_Val);
		for(size_t iIndex = 0 ; iIndex < index_Val ; ++ iIndex)
		{
			ID id;
			reader.ReadMemory(id);
			sStatusCache.mCacheIndexList.push_back(id);
		}

		//get
		for(size_t iIndex = 0 ; iIndex < index_Val ; ++ iIndex)
		{
			SStatus status;

			reader.ReadMemory(status.mUserName);
			reader.ReadMemory(status.mUserID);
			reader.ReadMemory(status.mScreenName);
			reader.ReadMemory(status.mUserIconURL);
			reader.ReadMemory(status.mPostID);
			reader.ReadMemory(status.mText);
			reader.ReadMemory(status.mToReplyStateID);
			reader.ReadMemory(status.mToReplyUserID);

			//Date
			reader.ReadMemory(status.mLogDate.mYear);
			reader.ReadMemory(status.mLogDate.mMonth);
			reader.ReadMemory(status.mLogDate.mDay);
			reader.ReadMemory(status.mLogDate.mWeek);
			reader.ReadMemory(status.mLogDate.mHour);
			reader.ReadMemory(status.mLogDate.mMinute);
			reader.ReadMemory(status.mLogDate.mSecond);
			reader.ReadMemory(status.mLogDate.mTimeZone);

			reader.ReadMemory(status.mClientSource);
			reader.ReadMemory(status.mClientName);
			reader.ReadMemory(status.mRetweetCount);
			reader.ReadMemory(status.mRetweetFlg);
			reader.ReadMemory(status.mFavoritesFlg);
	
			reader.ReadMemory(status.mRetweetStatus.mValidFlg);
			if(status.mRetweetStatus.mValidFlg)
			{
				reader.ReadMemory(status.mRetweetStatus.mUserName);
				reader.ReadMemory(status.mRetweetStatus.mUserID);
				reader.ReadMemory(status.mRetweetStatus.mScreenName);
				reader.ReadMemory(status.mRetweetStatus.mPostID);
				reader.ReadMemory(status.mRetweetStatus.mUserIconURL);
			}

			int m_Val;
			reader.ReadMemory(m_Val);
			while(m_Val > 0)
			{
				SStatus::CMentionPtr memtion(new SMention());
				reader.ReadMemory(memtion -> mUserID);
				reader.ReadMemory(memtion -> mScreenName);
				reader.ReadMemory(memtion -> mUserName);
				reader.ReadMemory(memtion -> mBeginPos);
				reader.ReadMemory(memtion -> mLength);
				status.mMemtionList.push_back(memtion);
				-- m_Val;
			}

			//Hash
			reader.ReadMemory(m_Val);
			while(m_Val > 0)
			{
				SStatus::CHashPtr hash(new SHashTag());
				reader.ReadMemory(hash -> mTagName);
				reader.ReadMemory(hash -> mBeginPos);
				reader.ReadMemory(hash -> mLength);
				status.mHashTagList.push_back(hash);
				-- m_Val;
			}

			//Url
			reader.ReadMemory(m_Val);
			while(m_Val > 0)
			{
				SStatus::CUrlPtr url(new SUrl());
				reader.ReadMemory(url -> mExpandedURL);
				reader.ReadMemory(url -> mBeginPos);
				reader.ReadMemory(url -> mLength);
				status.mURLList.push_back(url);
				-- m_Val;
			}
		
			//Photo
			reader.ReadMemory(m_Val);
			while(m_Val > 0)
			{
				SStatus::CPhotoPtr photo(new SPhoto());
				reader.ReadMemory(photo -> mExpandedURL);
				reader.ReadMemory(photo -> mPhotoURL);
				reader.ReadMemory(photo -> mThumbURL);
				reader.ReadMemory(photo -> mBeginPos);
				reader.ReadMemory(photo -> mLength);
				status.mPhotoList.push_back(photo);
				-- m_Val;
			}

			//Movie
			reader.ReadMemory(m_Val);
			while(m_Val > 0)
			{
				SStatus::CMoviePtr movie(new SMovie());
				reader.ReadMemory(movie -> mMovieURL);
				reader.ReadMemory(movie -> mThumbURL);
				reader.ReadMemory(movie -> mBeginPos);
				reader.ReadMemory(movie -> mLength);
				status.mMovieList.push_back(movie);
				-- m_Val;
			}

			//out side
			reader.ReadMemory(status.mUpdateTimeOfUserInfo);
			reader.ReadMemory(status.mUserInfoIndex);
			reader.ReadMemory(status.mCacheNonDeleteFlg);

			CStatusPtr status_Ptr = CStatusPtr(new SStatus(status));
			sStatusCache.mCacheMap.insert(std::make_pair(status.mPostID , status_Ptr));
			CTimeLine::AddStatusConnection(status_Ptr);
		}

		return true;
	}

	CCachePtr CCache::GetStatusCacheData()
	{
		boost::mutex::scoped_lock lock(SStatusCacheInfo::sMutex);
		CCacheWriter writer(10000);
		writer.WriteMemory(CACHE_VERSION);
		
		//index
		writer.WriteMemory(sStatusCache.mCacheIndexList.size());
		BOOST_FOREACH(ID iID , sStatusCache.mCacheIndexList)
		{
			writer.WriteMemory(iID);
		}

		typedef std::pair<ID , CStatusPtr> CGetPair;
		BOOST_FOREACH(CGetPair iPair , sStatusCache.mCacheMap)
		{
			CStatusPtr status = iPair.second;

			writer.WriteMemory(status -> mUserName);
			writer.WriteMemory(status -> mUserID);
			writer.WriteMemory(status -> mScreenName);
			writer.WriteMemory(status -> mUserIconURL);
			writer.WriteMemory(status -> mPostID);
			writer.WriteMemory(status -> mText);
			writer.WriteMemory(status -> mToReplyStateID);
			writer.WriteMemory(status -> mToReplyUserID);

			writer.WriteMemory(status -> mLogDate.mYear);
			writer.WriteMemory(status -> mLogDate.mMonth);
			writer.WriteMemory(status -> mLogDate.mDay);
			writer.WriteMemory(status -> mLogDate.mWeek);
			writer.WriteMemory(status -> mLogDate.mHour);
			writer.WriteMemory(status -> mLogDate.mMinute);
			writer.WriteMemory(status -> mLogDate.mSecond);
			writer.WriteMemory(status -> mLogDate.mTimeZone);

			writer.WriteMemory(status -> mClientSource);
			writer.WriteMemory(status -> mClientName);
			writer.WriteMemory(status -> mRetweetCount);
			writer.WriteMemory(status -> mRetweetFlg);
			writer.WriteMemory(status -> mFavoritesFlg);

			writer.WriteMemory(status -> mRetweetStatus.mValidFlg);
			if(status -> mRetweetStatus.mValidFlg)
			{
				writer.WriteMemory(status -> mRetweetStatus.mUserName);
				writer.WriteMemory(status -> mRetweetStatus.mUserID);
				writer.WriteMemory(status -> mRetweetStatus.mScreenName);
				writer.WriteMemory(status -> mRetweetStatus.mPostID);
				writer.WriteMemory(status -> mRetweetStatus.mUserIconURL);
			}

			int m_Val = status -> mMemtionList.size();
			writer.WriteMemory(m_Val);
			BOOST_FOREACH(SStatus::CMentionPtr iMention , status -> mMemtionList)
			{
				writer.WriteMemory(iMention -> mUserID);
				writer.WriteMemory(iMention -> mScreenName);
				writer.WriteMemory(iMention -> mUserName);
				writer.WriteMemory(iMention -> mBeginPos);
				writer.WriteMemory(iMention -> mLength);
			}

			//Hash
			m_Val = status -> mHashTagList.size();
			writer.WriteMemory(m_Val);
			BOOST_FOREACH(SStatus::CHashPtr iHash , status -> mHashTagList)
			{
				writer.WriteMemory(iHash -> mTagName);
				writer.WriteMemory(iHash -> mBeginPos);
				writer.WriteMemory(iHash -> mLength);
			}

			//Url
			m_Val = status -> mURLList.size();
			writer.WriteMemory(m_Val);
			BOOST_FOREACH(SStatus::CUrlPtr iUrl , status -> mURLList)
			{
				writer.WriteMemory(iUrl -> mExpandedURL);
				writer.WriteMemory(iUrl -> mBeginPos);
				writer.WriteMemory(iUrl -> mLength);
			}
		
			//Photo
			m_Val = status -> mPhotoList.size();
			writer.WriteMemory(m_Val);
			BOOST_FOREACH(SStatus::CPhotoPtr iPhoto , status -> mPhotoList)
			{
				writer.WriteMemory(iPhoto -> mExpandedURL);
				writer.WriteMemory(iPhoto -> mPhotoURL);
				writer.WriteMemory(iPhoto -> mThumbURL);
				writer.WriteMemory(iPhoto -> mBeginPos);
				writer.WriteMemory(iPhoto -> mLength);
			}

			//Movie
			m_Val = status -> mMovieList.size();
			writer.WriteMemory(m_Val);
			BOOST_FOREACH(SStatus::CMoviePtr iMovie , status -> mMovieList)
			{
				writer.WriteMemory(iMovie -> mMovieURL);
				writer.WriteMemory(iMovie -> mThumbURL);
				writer.WriteMemory(iMovie -> mBeginPos);
				writer.WriteMemory(iMovie -> mLength);
			}

			//out side
			writer.WriteMemory(status -> mUpdateTimeOfUserInfo);
			writer.WriteMemory(status -> mUserInfoIndex);
			writer.WriteMemory(status -> mCacheNonDeleteFlg);
		}

		CCachePtr cache = writer.GetCachePtr();
		size_t current_Seek = writer.GetCurrentSeek();
		cache -> mMemorySize = current_Seek;
		cache -> mHeadAddress = static_cast<char*>(realloc(cache -> mHeadAddress , current_Seek));
		return cache;
	}

	void CCache::ClearStatusCacheData()
	{
        boost::mutex::scoped_lock lock(SStatusCacheInfo::sMutex);
		sStatusCache.mCacheIndexList.clear();
		sStatusCache.mCacheMap.clear();
		CTimeLine::ClearConnection();
	}

	/************************************************************************/
	/* Direct Message														*/
	/************************************************************************/

	void CCache::AddDirectMessageCache(CDirectMessagePtr iDirectMessage)
	{
        boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);
		sDirectMessageCache.mCacheMap.insert(std::make_pair(iDirectMessage -> mMessageID , iDirectMessage));
	}

	bool CCache::GetDirectMessageCache(CDirectMessagePtr& oDirectMessage , ID iID)
	{
        boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);

		SDMCacheInfo::CCacheMap::const_iterator find_Itr = sDirectMessageCache.mCacheMap.find(iID);
		bool ret_Flg = (find_Itr != sDirectMessageCache.mCacheMap.end());
		if(ret_Flg)
		{
			oDirectMessage = (*find_Itr).second;
		}
		return ret_Flg;
	}

	bool CCache::SetDirectMessageCacheData(const char* iData , size_t)
	{
		ClearDirectMessageCacheData();

		boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);
		CCacheReader reader(iData);
		
		//version check
		float version;
		reader.ReadMemory(version);
		if(version != CACHE_VERSION)
		{
			return false;
		}
	
		//read index
		size_t cache_Val; 
		reader.ReadMemory(cache_Val);

		//get
		for(size_t iIndex = 0 ; iIndex < cache_Val ; ++ iIndex)
		{
			SDirectMessage dm;

			//Date
			reader.ReadMemory(dm.mLogData.mYear);
			reader.ReadMemory(dm.mLogData.mMonth);
			reader.ReadMemory(dm.mLogData.mDay);
			reader.ReadMemory(dm.mLogData.mWeek);
			reader.ReadMemory(dm.mLogData.mHour);
			reader.ReadMemory(dm.mLogData.mMinute);
			reader.ReadMemory(dm.mLogData.mSecond);
			reader.ReadMemory(dm.mLogData.mTimeZone);

			//message
			reader.ReadMemory(dm.mMessageID);
			reader.ReadMemory(dm.mText);

			//sender
			reader.ReadMemory(dm.mSenderID);
			reader.ReadMemory(dm.mSenderName);
			reader.ReadMemory(dm.mSenderScreenName);

			//recipient
			reader.ReadMemory(dm.mRecipientID);
			reader.ReadMemory(dm.mRecipientName);
			reader.ReadMemory(dm.mRecipientScreenName);

			CDirectMessagePtr dm_Ptr(new SDirectMessage(dm));
            
			sDirectMessageCache.mCacheMap.insert(std::make_pair(dm.mMessageID , dm_Ptr));
		}
		return true;
	}
		
	CCachePtr CCache::GetDirectMessageCacheData()
	{
        boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);

		CCacheWriter writer(10000);
		writer.WriteMemory(CACHE_VERSION);
		
		typedef std::pair<ID , CDirectMessagePtr> CGetPair;
		writer.WriteMemory(sDirectMessageCache.mCacheMap.size());
		BOOST_FOREACH(CGetPair iPair , sDirectMessageCache.mCacheMap)
		{
			CDirectMessagePtr dm = iPair.second;

			//Date
			writer.WriteMemory(dm -> mLogData.mYear);
			writer.WriteMemory(dm -> mLogData.mMonth);
			writer.WriteMemory(dm -> mLogData.mDay);
			writer.WriteMemory(dm -> mLogData.mWeek);
			writer.WriteMemory(dm -> mLogData.mHour);
			writer.WriteMemory(dm -> mLogData.mMinute);
			writer.WriteMemory(dm -> mLogData.mSecond);
			writer.WriteMemory(dm -> mLogData.mTimeZone);

			//message
			writer.WriteMemory(dm -> mMessageID);
			writer.WriteMemory(dm -> mText);

			//sender
			writer.WriteMemory(dm -> mSenderID);
			writer.WriteMemory(dm -> mSenderName);
			writer.WriteMemory(dm -> mSenderScreenName);

			//recipient
			writer.WriteMemory(dm -> mRecipientID);
			writer.WriteMemory(dm -> mRecipientName);
			writer.WriteMemory(dm -> mRecipientScreenName);
		}

		CCachePtr cache = writer.GetCachePtr();
		size_t current_Seek = writer.GetCurrentSeek();
		cache -> mMemorySize = current_Seek;
		cache -> mHeadAddress = static_cast<char*>(realloc(cache -> mHeadAddress , current_Seek));
		return cache;
	}

	void CCache::ClearDirectMessageCacheData()
	{
        boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);
		sDirectMessageCache.mCacheMap.clear();
	}

	/************************************************************************/
	/* User                                                                 */
	/************************************************************************/

	void CCache::AddUserCache(CUserPtr iUser , int iTime)
	{
        boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
	
		//already delete
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUser -> mUserID));
		if(find_Itr != sUserCache.mCacheList.end())
		{
			sUserCache.mCacheList.erase(find_Itr);
		}

		//cache check(LRU)
		if(sUserCache.mCacheList.size() >= SUserCacheInfo::sMaxCacheValue)
		{
			//delete
			SUserCacheInfo::CCacheList::iterator begin = sUserCache.mCacheList.begin();
			sUserCache.mCacheList.erase(begin);
		}

		//add
		SUserCacheInfo::CUserTimeInfoPtr user(new SUserCacheInfo::SUserCacheTimeInfo());
		user -> mSecTime = iTime;
		user -> mUserPtr = iUser;
		sUserCache.mCacheList.push_back(user);
	}	



	bool CCache::IsValidTimeUserCache(ID iUserID , int iTime)
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);

		//time check
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUserID));
		if(find_Itr != sUserCache.mCacheList.end())
		{
			int delete_Time = (*find_Itr) -> mSecTime + SUserCacheInfo::USER_CACHE_TTL;
			if(delete_Time > iTime)
			{
				return true;
			}
		}

		return false;
	}
    
	bool CCache::IsValidTimeUserCache(ID iUserID)
	{
        time_t value;
        time(&value);
        return IsValidTimeUserCache(iUserID, value);
    }
    
	bool CCache::ExistsUser(ID iUserID)
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUserID));
		return find_Itr != sUserCache.mCacheList.end();
    }

	bool CCache::RemoveUserCache(ID iUserID)
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUserID));
		if(find_Itr != sUserCache.mCacheList.end())
		{
            sUserCache.mCacheList.erase(find_Itr);
            return true;
		}
        return false;
	}
    
    bool CCache::GetUserCache(CUserPtr& oUser , ID iUserID)
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUserID));
		if(find_Itr != sUserCache.mCacheList.end())
		{
            oUser = (*find_Itr) -> mUserPtr;
            return true;
		}
        return false;
    }

	bool CCache::GetUserCache(CUserPtr& oUser , ID iUserID , int iTime)
	{
        boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);

		//time check
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheIDFinder(iUserID));
		if(find_Itr != sUserCache.mCacheList.end())
		{
			int delete_Time = (*find_Itr) -> mSecTime + SUserCacheInfo::USER_CACHE_TTL;
			if(delete_Time < iTime)
			{
				sUserCache.mCacheList.erase(find_Itr);
			}
			else
			{
				oUser = (*find_Itr) -> mUserPtr;
				return true;
			}
		}

		return false;
	}

	bool CCache::GetUserCache(CUserPtr& oUser , const std::string& iUserName , int iTime)
	{
        boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);

		//time check
		SUserCacheInfo::CCacheList::iterator find_Itr = std::find_if(sUserCache.mCacheList.begin() , sUserCache.mCacheList.end() , CUserCacheUserNameFinder(iUserName));
		if(find_Itr != sUserCache.mCacheList.end())
		{
			int delete_Time = (*find_Itr) -> mSecTime + SUserCacheInfo::USER_CACHE_TTL;
			if(delete_Time < iTime)
			{
				sUserCache.mCacheList.erase(find_Itr);
			}
			else
			{
				oUser = (*find_Itr) -> mUserPtr;
				return true;
			}
		}

		return false;
	}

	bool CCache::SetUserCacheData(const char* iData , size_t)
	{
		ClearUserCacheData();

		boost::mutex::scoped_lock lock(SDMCacheInfo::sMutex);
		CCacheReader reader(iData);
		
		//version check
		float version;
		reader.ReadMemory(version);
		if(version != CACHE_VERSION)
		{
			return false;
		}
	
		//read index
		size_t cache_Val; 
		reader.ReadMemory(cache_Val);

		//get
		for(size_t iIndex = 0 ; iIndex < cache_Val ; ++ iIndex)
		{
			SUserCacheInfo::SUserCacheTimeInfo cache_Info;
			reader.ReadMemory(cache_Info.mSecTime);

			SUser user;
			reader.ReadMemory(user.mUserID);
			reader.ReadMemory(user.mUserName);
			reader.ReadMemory(user.mUserScreenName);
			reader.ReadMemory(user.mUserIconURL);
			reader.ReadMemory(user.mLocation);
			reader.ReadMemory(user.mURL);
			reader.ReadMemory(user.mDescription);
			reader.ReadMemory(user.mStatusCount);
			reader.ReadMemory(user.mFollowCount);
			reader.ReadMemory(user.mFollowersCount);
			reader.ReadMemory(user.mProtectedFlg);
			reader.ReadMemory(user.mFollowingFlg);
			cache_Info.mUserPtr = CUserPtr(new SUser(user));

			sUserCache.mCacheList.push_back(SUserCacheInfo::CUserTimeInfoPtr(new SUserCacheInfo::SUserCacheTimeInfo(cache_Info)));
		}

		return true;
	}

	CCachePtr CCache::GetUserCacheData()
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
		CCacheWriter writer(10000);
		writer.WriteMemory(CACHE_VERSION);
		
		//cache
		writer.WriteMemory(sUserCache.mCacheList.size());
		BOOST_FOREACH(SUserCacheInfo::CUserTimeInfoPtr iPtr , sUserCache.mCacheList)
		{
			writer.WriteMemory(iPtr -> mSecTime);

			CUserPtr user = iPtr -> mUserPtr;
			writer.WriteMemory(user -> mUserID);
			writer.WriteMemory(user -> mUserName);
			writer.WriteMemory(user -> mUserScreenName);
			writer.WriteMemory(user -> mUserIconURL);
			writer.WriteMemory(user -> mLocation);
			writer.WriteMemory(user -> mURL);
			writer.WriteMemory(user -> mDescription);
			writer.WriteMemory(user -> mStatusCount);
			writer.WriteMemory(user -> mFollowCount);
			writer.WriteMemory(user -> mFollowersCount);
			writer.WriteMemory(user -> mProtectedFlg);
			writer.WriteMemory(user -> mFollowingFlg);
		}

		CCachePtr cache = writer.GetCachePtr();
		size_t current_Seek = writer.GetCurrentSeek();
		cache -> mMemorySize = current_Seek;
		cache -> mHeadAddress = static_cast<char*>(realloc(cache -> mHeadAddress , current_Seek));
		return cache;
	}

	void CCache::ClearUserCacheData()
	{
		boost::mutex::scoped_lock lock(SUserCacheInfo::sMutex);
		sUserCache.mCacheList.clear();
	}

	/************************************************************************/
	/* Trends Place															*/
	/************************************************************************/
	
	void CCache::SetTrendsPlaceCache(const CPlaceList& iPlaceList , int iTime)
	{
		boost::mutex::scoped_lock lock(STrendsPlaceCache::sMutex);
		sTrendPlaceCache.mPlaceList = iPlaceList;
		sTrendPlaceCache.mSecTime = iTime;
	}
 
	bool CCache::GetTrendsPlaceCache(CPlaceList& oPlaceList)
	{
		if(sTrendPlaceCache.mSecTime == 0)
		{
			return false;
		}

		oPlaceList = sTrendPlaceCache.mPlaceList;
		return true;
	}

	bool CCache::SetTrendsPlaceCacheData(const char* iData , size_t)
	{
		ClearTrendsPlaceCache();

		boost::mutex::scoped_lock lock(STrendsPlaceCache::sMutex);
		CCacheReader reader(iData);
		
		//version check
		float version;
		reader.ReadMemory(version);
		if(version != CACHE_VERSION)
		{
			return false;
		}

		//time
		reader.ReadMemory(sTrendPlaceCache.mSecTime);
	
		//read index
		size_t cache_Val; 
		reader.ReadMemory(cache_Val);

		//get
		for(size_t iIndex = 0 ; iIndex < cache_Val ; ++ iIndex)
		{
			SPlace place;
			reader.ReadMemory(place.mCountry);
			reader.ReadMemory(place.mCountryCode);
			reader.ReadMemory(place.mName);
			reader.ReadMemory(place.mParentID);
			reader.ReadMemory(place.mUrl);
			reader.ReadMemory(place.mWOEID);

			reader.ReadMemory(place.mPlaceTypeCode);
			reader.ReadMemory(place.mPlaceTypeName);
			sTrendPlaceCache.mPlaceList.push_back(CPlacePtr(new SPlace(place)));
		}

		return true;
	}

	CCachePtr CCache::GetTrendsPlaceCacheData()
	{
		boost::mutex::scoped_lock lock(STrendsPlaceCache::sMutex);
		CCacheWriter writer(10000);
		writer.WriteMemory(CACHE_VERSION);

		//cache
		writer.WriteMemory(sTrendPlaceCache.mSecTime);
		writer.WriteMemory(sTrendPlaceCache.mPlaceList.size());
		BOOST_FOREACH(CPlacePtr iPlace , sTrendPlaceCache.mPlaceList)
		{
			writer.WriteMemory(iPlace -> mCountry);
			writer.WriteMemory(iPlace -> mCountryCode);
			writer.WriteMemory(iPlace -> mName);
			writer.WriteMemory(iPlace -> mParentID);
			writer.WriteMemory(iPlace -> mUrl);
			writer.WriteMemory(iPlace -> mWOEID);

			writer.WriteMemory(iPlace -> mPlaceTypeCode);
			writer.WriteMemory(iPlace -> mPlaceTypeName);
		}

		CCachePtr cache = writer.GetCachePtr();
		size_t current_Seek = writer.GetCurrentSeek();
		cache -> mMemorySize = current_Seek;
		cache -> mHeadAddress = static_cast<char*>(realloc(cache -> mHeadAddress , current_Seek));
		return cache;
	}

	void CCache::ClearTrendsPlaceCache()
	{
		boost::mutex::scoped_lock lock(STrendsPlaceCache::sMutex);
		sTrendPlaceCache.mPlaceList.clear();
		sTrendPlaceCache.mSecTime = 0;
	}
}
