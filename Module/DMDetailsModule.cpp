/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "DMDetailsModule.h"
#include <Module/Cache/CacheWriter.h>
#include <Module/Cache/CacheReader.h>

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CDMDetailsModule::CDMDetailsModule(const std::string& iKey , const std::string& iSecret)
		:mKey(iKey)
		,mSecret(iSecret)
		,mCurrentOldSendID(UNKNOWN_ID)
		,mCurrentUpdateSendID(0)
		,mCurrentOldRecvID(UNKNOWN_ID)
		,mCurrentUpdateRecvID(0)
	{
	}

	/************************************************************************/
	/* Update                                                               */
	/************************************************************************/

	bool CDMDetailsModule::UpdateSendMessage(CUpdateMap& oUpdateMap)
	{
		//latest check
		if(mCurrentUpdateSendID == 0)
		{
			return false;
		}

		//delete direct add
		mDirectAddSendMessageMap.clear();

		//get message
		CDirectMessageList dm_List;	
		if(!_GetUpdateMessage(dm_List , mCurrentUpdateSendID , CDirectMessage::GetSentDirectMessage))
		{
			return false;
		}

		//insert
		BOOST_FOREACH(CDirectMessagePtr iDM , dm_List)
		{
			//system map add
			SMessageInfo info;
			info.mMyselfSenderFlg = true;
			info.mTargetID = iDM -> mRecipientID;
			{
				boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);
				mMessageManagementMap.insert(std::make_pair(iDM -> mMessageID , CMessageInfoPtr(new SMessageInfo(info))));
			}

			//out add
			ID user_ID = info.mTargetID;
			CUpdateMap::iterator find_Itr = oUpdateMap.find(user_ID);
			if(find_Itr != oUpdateMap.end())
			{
				++ (*find_Itr).second;
			}
			else
			{
				oUpdateMap.insert(std::make_pair(user_ID , 1));
			}
		}

		if(dm_List.size() != 0)
		{
			mCurrentUpdateSendID = dm_List[0] -> mMessageID;
		}

		return true;
	}

	bool CDMDetailsModule::UpdateRecvMessage(CUpdateMap& oUpdateMap)
	{
		//latest check
		if(mCurrentUpdateRecvID == 0)
		{
			return false;
		}

		//get message
		CDirectMessageList dm_List;	
		if(!_GetUpdateMessage(dm_List , mCurrentUpdateRecvID , CDirectMessage::GetReceiveDirectMessage))
		{
			return false;
		}

		//insert
		BOOST_FOREACH(CDirectMessagePtr iDM , dm_List)
		{
			//system map add
			SMessageInfo info;
			info.mMyselfSenderFlg = false;
			info.mTargetID = iDM -> mSenderID;
            {
                boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);
                mMessageManagementMap.insert(std::make_pair(iDM -> mMessageID , CMessageInfoPtr(new SMessageInfo(info))));
            }
                
			//out add
			ID user_ID = info.mTargetID;
			CUpdateMap::iterator find_Itr = oUpdateMap.find(user_ID);
			if(find_Itr != oUpdateMap.end())
			{
				++ (*find_Itr).second;
			}
			else
			{
				oUpdateMap.insert(std::make_pair(user_ID , 1));
			}
		}

		if(dm_List.size() != 0)
		{
			mCurrentUpdateRecvID = dm_List[0] -> mMessageID;
		}

		return true;
	}

	bool CDMDetailsModule::UpdateOldSendMessage()
	{
		//get end
		if(IsCompleteOldSend())
		{
			return false;
		}

		//get message
		CDirectMessageList dm_List;	
		if(!_GetMessage(dm_List , mCurrentOldSendID , CDirectMessage::GetSentDirectMessage))
		{
			return false;
		}

		//insert
		BOOST_FOREACH(CDirectMessagePtr iDM , dm_List)
		{
			SMessageInfo info;
			info.mMyselfSenderFlg = (iDM -> mSenderID == iDM -> mRecipientID) ? false : true;
			info.mTargetID = iDM -> mRecipientID;

			//lock insert
            {
                boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);
                mMessageManagementMap.insert(std::make_pair(iDM -> mMessageID , CMessageInfoPtr(new SMessageInfo(info))));
            }
		}

		//get latest
		if(mCurrentUpdateSendID == 0)
		{
			mCurrentUpdateSendID = dm_List[0] -> mMessageID;
		}

		//update
		size_t dm_Val = dm_List.size();
		mCurrentOldSendID = (dm_Val == 0) ? 0 : dm_List[dm_Val - 1] -> mMessageID;

		return true;
	}

	bool CDMDetailsModule::UpdateOldRecvMessage()
	{
		//get end
		if(IsCompleteOldRecv())
		{
			return false;
		}

		//get message
		CDirectMessageList dm_List;	
		if(!_GetMessage(dm_List , mCurrentOldRecvID , CDirectMessage::GetReceiveDirectMessage))
		{
			return false;
		}

		//insert
		BOOST_FOREACH(CDirectMessagePtr iDM , dm_List)
		{
			SMessageInfo info;
			info.mMyselfSenderFlg = false;
			info.mTargetID = iDM -> mSenderID;

			//find
            {
                boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);
                CMessageManageMap::iterator find_Itr = mMessageManagementMap.find(iDM -> mMessageID);
                if(find_Itr == mMessageManagementMap.end())
                {
                    mMessageManagementMap.insert(std::make_pair(iDM -> mMessageID , CMessageInfoPtr(new SMessageInfo(info))));
                }
            }
		}

		//get latest
		if(mCurrentUpdateRecvID == 0)
		{
			mCurrentUpdateRecvID = dm_List[0] -> mMessageID;
		}

		//update
		size_t dm_Val = dm_List.size();
		mCurrentOldRecvID = (dm_Val == 0) ? 0 : dm_List[dm_Val - 1] -> mMessageID;

		return true;
	}

	/************************************************************************/
	/* Accessor                                                             */
	/************************************************************************/

	bool CDMDetailsModule::IsCompleteOldSend() const
	{
		return (mCurrentOldSendID == 0);
	}

	bool CDMDetailsModule::IsCompleteOldRecv() const
	{
		return (mCurrentOldRecvID == 0);
	}

	void CDMDetailsModule::GetUserIDList(CIDList& oUserList) const
	{
        boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);

		//direct in
		typedef std::pair<ID , CDMMessageInfoList> CDirectPair;
		BOOST_FOREACH(const CDirectPair& iPair , mDirectAddSendMessageMap)
		{
			oUserList.push_back(iPair.first);
		}
		
		//get normal
		CMessageManageMap::const_reverse_iterator itr , end_Itr = mMessageManagementMap.rend();
		for(itr = mMessageManagementMap.rbegin() ; itr != end_Itr ; ++ itr)
		{
			if(std::find(oUserList.begin() , oUserList.end() , (*itr).second -> mTargetID) == oUserList.end())
			{
				oUserList.push_back((*itr).second -> mTargetID);
			}
		}
	}

	bool CDMDetailsModule::GetUserInfo(SUserInfo& oUserInfo , ID iUserID) const
	{
        boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);

		oUserInfo.mDMContactValue = 0;
		CMessageManageMap::const_reverse_iterator itr = mMessageManagementMap.rbegin();
		CMessageManageMap::const_reverse_iterator end_Itr = mMessageManagementMap.rend();

		CDirectAddMessageMap::const_iterator find_Itr = mDirectAddSendMessageMap.find(iUserID);
		if(find_Itr != mDirectAddSendMessageMap.end())
		{
			CDMMessageInfoList::const_reverse_iterator begin = (*find_Itr).second.rbegin();
			ID msg_ID = (*begin) -> mMessageID;
			SResponse response;
			CDirectMessagePtr dm;
			if(CDirectMessage::GetDirectMessage(response , dm , mKey , mSecret , msg_ID))
			{
				oUserInfo.mLastUpdate = dm -> mLogData;
				oUserInfo.mLastDMText = dm -> mText;
				oUserInfo.mDMContactValue = (*find_Itr).second.size() - 1;
			}
			else
			{
				//API Limit Over?
				return false;
			}
		}
		else
		{
			//first
			for( ; itr != end_Itr ; ++ itr)
			{
				//id cmp
				if(iUserID == (*itr).second -> mTargetID)
				{
					ID msg_ID = (*itr).first;
					SResponse response;
					CDirectMessagePtr dm;
					if(CDirectMessage::GetDirectMessage(response , dm , mKey , mSecret , msg_ID))
					{
						oUserInfo.mLastUpdate = dm -> mLogData;
						oUserInfo.mLastDMText = dm -> mText;
						oUserInfo.mDMContactValue = 1;
						++ itr;
						break;
					}
					else
					{
						//API Limit Over?
						return false;
					}
				}
			}
		}

		//after
		for( ; itr != end_Itr ; ++ itr)
		{
			//counting
			if(iUserID == (*itr).second -> mTargetID)
			{
				++ oUserInfo.mDMContactValue;
			}
		}

		return true;
	}

	void CDMDetailsModule::GetMessageList(CMessageList& oMessageList , ID iUserID) const
	{
        boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);

		//direct add
		CDirectAddMessageMap::const_iterator find_Itr = mDirectAddSendMessageMap.find(iUserID);
		if(find_Itr != mDirectAddSendMessageMap.end())
		{
			CDMMessageInfoList::const_reverse_iterator itr , end_Itr = (*find_Itr).second.rend();
			for(itr = (*find_Itr).second.rbegin() ; itr != end_Itr ; ++ itr)
			{
				oMessageList.push_back((*itr));
			}
		}

		//normal add
		{
			CMessageManageMap::const_reverse_iterator itr , end_Itr = mMessageManagementMap.rend();
			for(itr = mMessageManagementMap.rbegin() ; itr != end_Itr ; ++ itr)
			{
				if((*itr).second -> mTargetID == iUserID)
				{
					CDMMessageInfoPtr info(new SDMMessageInfo());
					info -> mMessageID = (*itr).first;
					info -> mMyselfSenderFlg = (*itr).second -> mMyselfSenderFlg;
					oMessageList.push_back(info);
				}
			}
		}
	}

	void CDMDetailsModule::AddSendMessage(CDirectMessagePtr iDirectMessagePtr)
	{
		SDMMessageInfo dm;
		dm.mMessageID = iDirectMessagePtr -> mMessageID;
		dm.mMyselfSenderFlg = true;
		CDMMessageInfoPtr set_Ptr(new SDMMessageInfo(dm));

		ID user_ID = iDirectMessagePtr -> mRecipientID;
		if(mDirectAddSendMessageMap.find(user_ID) == mDirectAddSendMessageMap.end())
		{
			mDirectAddSendMessageMap.insert(std::make_pair(user_ID , CDMMessageInfoList()));
		}

		mDirectAddSendMessageMap[user_ID].push_back(set_Ptr);
	}

	/************************************************************************/
	/* Cache                                                                */
	/************************************************************************/

	bool CDMDetailsModule::SetCache(const char* iData , size_t)
	{
        boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);

		CCacheReader reader(iData);
		
		//version check
		float version;
		reader.ReadMemory(version);
		if(version != CACHE_VERSION)
		{
			return false;
		}

		reader.ReadMemory(mKey);
		reader.ReadMemory(mSecret);
		reader.ReadMemory(mCurrentOldSendID);
		reader.ReadMemory(mCurrentUpdateSendID);
		reader.ReadMemory(mCurrentOldRecvID);
		reader.ReadMemory(mCurrentUpdateRecvID);

		int val = 0;
		reader.ReadMemory(val);
		BOOST_FOREACH(int iIndex , EnamerableRange(0 , val))
		{
			iIndex;

			ID id;
			reader.ReadMemory(id);
			SMessageInfo info;
			reader.ReadMemory(info.mMyselfSenderFlg);
			reader.ReadMemory(info.mTargetID);
			mMessageManagementMap.insert(std::make_pair(id , CMessageInfoPtr(new SMessageInfo(info))));
		}

		return true;
	}

	CCachePtr CDMDetailsModule::GetCache() const
	{
        boost::mutex::scoped_lock lock(mMutexForMessageManangementMap);

		CCacheWriter writer(10000);
		writer.WriteMemory(CACHE_VERSION);
		writer.WriteMemory(mKey);
		writer.WriteMemory(mSecret);
		writer.WriteMemory(mCurrentOldSendID);
		writer.WriteMemory(mCurrentUpdateSendID);
		writer.WriteMemory(mCurrentOldRecvID);
		writer.WriteMemory(mCurrentUpdateRecvID);

		int val = mMessageManagementMap.size();
		writer.WriteMemory(val);
		BOOST_FOREACH(const CMessageManagePair& iPair , mMessageManagementMap)
		{
			writer.WriteMemory(iPair.first);
            writer.WriteMemory(iPair.second->mMyselfSenderFlg);
            writer.WriteMemory(iPair.second->mTargetID);
		}

		CCachePtr cache = writer.GetCachePtr();
		size_t current_Seek = writer.GetCurrentSeek();
		cache -> mMemorySize = current_Seek;
		cache -> mHeadAddress = static_cast<char*>(realloc(cache -> mHeadAddress , current_Seek));
		return cache;
	}

	/************************************************************************/
	/* Inner Decl                                                           */
	/************************************************************************/

	bool CDMDetailsModule::_GetMessage(CDirectMessageList& oDirectMessageList , ID iMaxID , bool (*iGetFunction)(SResponse& , CDirectMessageList& , const std::string& , const std::string& , const SGetArgs*))
	{
		SResponse response;
		SGetArgs args;
		args.mCount = 100;
		args.mMaxID = iMaxID;
		
		bool ret_Flg = iGetFunction(response , oDirectMessageList , mKey , mSecret , &args);
		if(oDirectMessageList.size() != 0 && oDirectMessageList[0] -> mMessageID == iMaxID)
		{
			oDirectMessageList.erase(oDirectMessageList.begin());
		}
		return ret_Flg;
	}

	bool CDMDetailsModule::_GetUpdateMessage(CDirectMessageList& oDirectMessageList , ID iSinceID , bool (*iGetFunction)(SResponse& , CDirectMessageList& , const std::string& , const std::string& , const SGetArgs*))
	{
		SResponse response;
		SGetArgs args;
		args.mCount = 100;
		args.mSinceID = iSinceID;

		return iGetFunction(response , oDirectMessageList , mKey , mSecret , &args);
	}
}