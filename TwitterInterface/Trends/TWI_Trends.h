/****************************************************************************/
/* TWI_Trends.h																*/
/****************************************************************************/
#ifndef TWI_TRENDS_H_
#define	TWI_TRENDS_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_TrendsDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Trends
	class CTrends
	{
	public:
		/************************************************************************/
		/* PlaceTrend															*/
		/************************************************************************/
		static bool GetPlaceTrend		(SResponse oResponse , CTrendsPtr& oTrend , const std::string& iKey , const std::string& iSecret , ID iWOEID);

		/************************************************************************/
		/* Available															*/
		/************************************************************************/
		static bool GetAvailablePlace	(SResponse oResponse , CPlaceList& oPlaceList , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* ClosestPlace                                                         */
		/************************************************************************/
		static bool GetClosestPlace		(SResponse oResponse , CPlacePtr& oPlacePtr , const std::string& iKey , const std::string& iSecret , float iLat , float iLong);
	};
}

#endif