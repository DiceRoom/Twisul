/****************************************************************************/
/* TWI_DirectMessage.h														*/
/****************************************************************************/
#ifndef TWI_DIRECTMESSAGE_H_
#define	TWI_DIRECTMESSAGE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_DirectMessageDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! DirectMessage
	class CDirectMessage
	{
	public:
		/************************************************************************/
		/* ReceiveDirectMessage				                                    */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetReceiveDirectMessage	(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs = 0);
		
		/************************************************************************/
		/* SentDirectMessage	                                                 */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetSentDirectMessage	(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* CreateDirectMessage		                                            */
		/************************************************************************/
		static bool CreateDirectMessage		(SResponse& oResponse , CDirectMessagePtr& oDirectMessage , const std::string& iKey , const std::string& iSecret , const std::string& iTargetUserName , const std::string& iText);
		
		/************************************************************************/
		/* DirectMessage					                                    */
		/************************************************************************/
		static bool GetDirectMessage		(SResponse& oResponse , CDirectMessagePtr& oDirectMessage , const std::string& iKey , const std::string& iSecret ,  ID iDirectMessageID);

		/************************************************************************/
		/* DeleteDirectMessage				                                    */
		/************************************************************************/
		static bool DeleteDirectMessage		(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iMessageID);

		/************************************************************************/
		/* RateLimit                                                            */
		/************************************************************************/
		static bool	GetRateLimit			(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);

	private:
		/************************************************************************/
		/* Inner Decl		                                                    */
		/************************************************************************/
		static bool	_GetDirectMessage		(SResponse& oResponse , CDirectMessageList& oDirectMessageList , const std::string& iKey , const std::string& iSecret , const std::string& iBaseUri , const SGetArgs* iArgs);
		static bool	_GetRateLimit			(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* Debug Mode															*/
		/************************************************************************/
	public:
		static void SetUseOldAPIMode		(bool iUseFlg);
	private:
		static bool	_GetOldRateLimit		(SResponse& oResponse , SDirectMessageRateLimit& oRateLimit , const std::string& iKey , const std::string& iSecret);
		
		typedef bool (*CGetRateLimitFunc)(SResponse& , SDirectMessageRateLimit& , const std::string& , const std::string&);
		typedef void (*CGetHttpFunc)(SResponse& , const std::string& , const std::string& , const std::string&);
		
		static CGetRateLimitFunc	sGetRateLimitFunc;
		static CGetHttpFunc			sGetHttpFunc;
	};
}

#endif