/****************************************************************************/
/* TWI_Status.h																*/
/****************************************************************************/
#ifndef TWI_STATUS_H_
#define	TWI_STATUS_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_StatusDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Status
	class CStatus
	{
	public:
		/************************************************************************/
		/* Status							                                    */
		/************************************************************************/
		static bool GetStatus				(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret ,  ID iStatusID);

		/************************************************************************/
		/* RetweetStatusID														*/
		/************************************************************************/
		static bool GetRetweetStatusID		(SResponse& oResponse , ID& oRetweetID , const std::string& iKey , const std::string& iSecret ,  ID iStatusID);

		/************************************************************************/
		/* CreateStatus							                                */
		/************************************************************************/
		static bool CreateStatus			(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret , const SPostStatusArgs& iStatusArgs);

		/************************************************************************/
		/* DeleteStatus															*/
		/************************************************************************/
		static bool DeleteStatus			(SResponse& oResponse , const std::string& iKey , const std::string& iSecret ,  ID iStatusID);

		/************************************************************************/
		/* RetweetStatus	                                                    */
		/************************************************************************/
		static bool RetweetStatus			(SResponse& oResponse , CStatusPtr& oStatus , const std::string& iKey , const std::string& iSecret , ID iStatusID);

		/************************************************************************/
		/* RetweetStatusUserID													*/
		/* # UseGetArgs #														*/
		/* -> mCount															*/
		/************************************************************************/
		static bool GetRetweetStatusUserID	(SResponse& oResponse , CIDList& oIDList , const std::string& iKey , const std::string& iSecret , ID iStatusID , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* RateLimit                                                            */
		/************************************************************************/
		static bool GetRateLimit			(SResponse& oResponse , CRateLimitMap& oRateLimitMap , const std::string& iKey , const std::string& iSecret);
	};
}

#endif