/****************************************************************************/
/* TWI_TimeLineDef.h														*/
/****************************************************************************/
#ifndef TWI_TIMELINE_DEF_H_
#define	TWI_TIMELINE_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../Status/TWI_StatusDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//status connection
	struct SStatusConnection
	{
		SStatusConnection()
			:mToReplyStateID(UNKNOWN_ID)
		{}

		CIDList mFromReplyStatusIDList;
		ID		mToReplyStateID;

		static const int MAX_CONNECT_CACHE_VAL = 3000;
	};
	typedef boost::shared_ptr<SStatusConnection>	CConnectPtr;
	typedef std::map<ID , CConnectPtr>				CConnectionMap;

	//TimeLineRateLimit
	struct STimeLineRateLimit
	{
		SRateLimit mHome;
		SRateLimit mMention;
		SRateLimit mUser;
		SRateLimit mList;
	};
}

#endif	//TWI_TIMELINE_DEF_H_