/****************************************************************************/
/* DMDetailsModule.h														*/
/****************************************************************************/
#ifndef TSUL_DMDETAILS_MODULE_H_
#define	TSUL_DMDETAILS_MODULE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../TwitterInterface/TwitterInterface/DirectMessage/TWI_DirectMessage.h"
#include "../TwitterInterface/TwitterInterface/Cache/TWI_CacheDef.h"
#include <boost/thread.hpp>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! DMDetailsModule
	class CDMDetailsModule
	{
	private:
		//! MessageInfo
		struct SMessageInfo
		{
			bool	mMyselfSenderFlg;	//< sending is mine
			ID		mTargetID;			//< target id
		};
		typedef boost::shared_ptr<const SMessageInfo>		CMessageInfoPtr;
		typedef std::pair</*Message*/ID , CMessageInfoPtr>	CMessageManagePair;
		typedef std::map</*Message*/ID , CMessageInfoPtr>	CMessageManageMap;
		
	public:
		//! UserInfo
		struct SUserInfo
		{
			SDate		mLastUpdate;		//< last update
			std::string	mLastDMText;		//< last text
			int			mDMContactValue;	//< contact value
		};
		//! direct message info
		struct SDMMessageInfo
		{
			ID		mMessageID;			//< msg id
			bool	mMyselfSenderFlg;	//< sending is mine
		};

		typedef boost::shared_ptr<SDMMessageInfo>			CDMMessageInfoPtr;
		typedef std::vector<CDMMessageInfoPtr>				CMessageList;
		typedef std::map</*User*/ID , int>					CUpdateMap;

		typedef std::vector<CDMMessageInfoPtr>				CDMMessageInfoList;
		typedef std::map</*User*/ID , CDMMessageInfoList>	CDirectAddMessageMap;

	private:
		CDMDetailsModule(){}
	public:
		CDMDetailsModule			(const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* Update                                                               */
		/************************************************************************/
		bool	UpdateSendMessage	(CUpdateMap& oUpdateMap);
		bool	UpdateRecvMessage	(CUpdateMap& oUpdateMap);
		bool	UpdateOldSendMessage();
		bool	UpdateOldRecvMessage();

		/************************************************************************/
		/* Accessor                                                             */
		/************************************************************************/
		bool	IsCompleteOldSend	() const;
		bool	IsCompleteOldRecv	() const;
		void	GetUserIDList		(CIDList& oUserIDList) const;
		bool	GetUserInfo			(SUserInfo& oUserInfo , ID iUserID) const;
		void	GetMessageList		(CMessageList& oMessageList , ID iUserID) const;
		void	AddSendMessage		(CDirectMessagePtr iDirectMessagePtr);

		/************************************************************************/
		/* Cache                                                                */
		/************************************************************************/
		bool		SetCache			(const char* iData , size_t iDataSize);
		CCachePtr	GetCache			() const;
		
	private:
		/************************************************************************/
		/* Inner Decl                                                           */
		/************************************************************************/
		bool		_GetMessage			(CDirectMessageList& oDirectMessageList , ID iMaxID , bool (*iGetFunction)(SResponse& , CDirectMessageList& , const std::string& , const std::string& , const SGetArgs*));
		bool		_GetUpdateMessage	(CDirectMessageList& oDirectMessageList , ID iSinceID , bool (*iGetFunction)(SResponse& , CDirectMessageList& , const std::string& , const std::string& , const SGetArgs*));
	private:
        mutable boost::mutex    mMutexForMessageManangementMap;
		CMessageManageMap		mMessageManagementMap;
		std::string				mKey;
		std::string				mSecret;
		ID						mCurrentOldSendID;
		ID						mCurrentUpdateSendID;
		ID						mCurrentOldRecvID;
		ID						mCurrentUpdateRecvID;
		CDirectAddMessageMap	mDirectAddSendMessageMap;
	};
}

#endif