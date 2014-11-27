/****************************************************************************/
/* TWI_TrendsDef.h															*/
/****************************************************************************/
#ifndef TWI_TRENDS_DEF_H_
#define	TWI_TRENDS_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//Place
	struct SPlace
	{
		std::string mCountry;
		std::string mCountryCode;
		std::string mName;
		ID			mParentID;
		std::string	mUrl;
		ID			mWOEID;

		int			mPlaceTypeCode;
		std::string	mPlaceTypeName;
	};

	typedef boost::shared_ptr<SPlace>	CPlacePtr;
	typedef std::vector<CPlacePtr>		CPlaceList;

	//TrendInfo
	struct STrendInfo
	{
		std::string	mEvents;
		std::string mName;
		std::string mPromotedContent;
		std::string mQuery;
		std::string mUrl;
	};

	typedef boost::shared_ptr<STrendInfo>	CTrendsInfoPtr;

	//Trend
	struct STrend
	{
		SDate						mDate;
		std::string					mLocationsName;
		ID							mLocationsWOEID;
		std::vector<CTrendsInfoPtr>	mTrendList;
	};

	typedef boost::shared_ptr<STrend>	CTrendsPtr;
}

#endif	//TWI_TRENDS_DEF_H_