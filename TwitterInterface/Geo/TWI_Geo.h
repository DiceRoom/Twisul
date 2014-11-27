/****************************************************************************/
/* TWI_Geo.h																*/
/****************************************************************************/
#ifndef TWI_GEO_H_
#define	TWI_GEO_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_GeoDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Geo
	class CGeo
	{
	public:
		/************************************************************************/
		/* GeoPlace																*/
		/************************************************************************/
		static bool GetGeoPlace			(SResponse oResponse , CGeoPtr& oGeo , const std::string& iKey , const std::string& iSecret , const std::string& iID);

		/************************************************************************/
		/* ReverseGeocode														*/
		/************************************************************************/
		static bool GetReverseGeocode	(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs);

		/************************************************************************/
		/* SimilarPlaces														*/
		/************************************************************************/
		static bool GetSimilarPlaces	(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs);

		/************************************************************************/
		/* SimilarPlaces														*/
		/************************************************************************/
		static bool GetSearchPlaces		(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs);
	};
}

#endif