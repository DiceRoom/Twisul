/****************************************************************************/
/* TWI_AccountDef.h															*/
/****************************************************************************/
#ifndef TWI_ACCOUNT_DEF_H_
#define	TWI_ACCOUNT_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//Account
	struct SAccount
	{
		std::string	mUserName;
		ID			mUserID;
		std::string	mKey;
		std::string	mSecret;
	};

	//AccountProfile
	struct SAccountProfile
	{
		std::string	mName;			//< max 40char
		std::string	mURL;			//< max 100char
		std::string mLocation;		//< max 30char
		std::string mDescription;	//< max 160char
	};
}

#endif	//TWI_ACCOUNT_DEF_H_