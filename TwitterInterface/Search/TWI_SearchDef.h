/****************************************************************************/
/* TWI_SearchDef.h															*/
/****************************************************************************/
#ifndef TWI_SEARCH_DEF_H_
#define	TWI_SEARCH_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../User/TWI_UserDef.h"
#include "../Status/TWI_StatusDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//SearchSaveInfo
	struct SSearchSaveInfo
	{
		ID			mID;
		std::string	mName;
		std::string	mQuery;
		SDate		mCreateDate;
	};

	typedef boost::shared_ptr<SSearchSaveInfo>	CSearchSaveInfoPtr;
	typedef std::vector<CSearchSaveInfoPtr>		CSearchSaveInfoList;
}

#endif	//TWI_SEARCH_DEF_H_