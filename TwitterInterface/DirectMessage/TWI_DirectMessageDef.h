/****************************************************************************/
/* TWI_DirectMessageDef.h													*/
/****************************************************************************/
#ifndef TWI_DIRECTMESSAGE_DEF_H_
#define	TWI_DIRECTMESSAGE_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* DirectMessage														*/
	/************************************************************************/

	//DirectMessage
	struct SDirectMessage
	{
		SDate		mLogData;				//< created_at
		ID			mMessageID;				//< id
		std::string	mText;					//< text
		ID			mSenderID;				//< sender_id
		std::string mSenderName;			//< sender_name
		std::string	mSenderScreenName;		//< sender_screen_name
		ID			mRecipientID;			//< recipient_id
		std::string mRecipientName;			//< recipient_name
		std::string	mRecipientScreenName;	//< recipient_screen_name
	};

	//DirectMessageRateLimit
	struct SDirectMessageRateLimit
	{
		SRateLimit mGetSendDM;
		SRateLimit mGetRecvDM;
		SRateLimit mGetDM;
	};

	typedef boost::shared_ptr<const SDirectMessage>	CDirectMessagePtr;
	typedef std::vector<CDirectMessagePtr>			CDirectMessageList;
}

#endif	//TWI_DIRECTMESSAGE_DEF_H_