/****************************************************************************/
/* TWI_Account.h															*/
/****************************************************************************/
#ifndef TWI_ACCOUNT_H_
#define	TWI_ACCOUNT_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_AccountDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Account
	class CAccount
	{
	public:
		/************************************************************************/
		/* GetAuthorizeUrl														*/
		/************************************************************************/	
		static bool	GetAuthorizeUrl		(std::string& oUrl , std::string& oKey);

		/************************************************************************/
		/* GetAccount															*/
		/************************************************************************/
		static bool	GetAccount			(SAccount& oAccount , const std::string& iKey , const std::string& iPinNo);

		/************************************************************************/
		/* VerifyCredentials													*/
		/************************************************************************/
		static bool IsVerifyCredentials	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* UpdateProfile														*/
		/************************************************************************/
		static bool UpdateProfile		(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const SAccountProfile& iProfile);

		/************************************************************************/
		/* UpdateProfileImage													*/
		/************************************************************************/
		static bool UpdateProfileImage	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iFilePath , const char* iBuffer , int iBufferSize);
	};
}

#endif