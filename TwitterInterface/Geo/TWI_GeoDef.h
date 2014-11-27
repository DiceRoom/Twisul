/****************************************************************************/
/* TWI_GeoDef.h																*/
/****************************************************************************/
#ifndef TWI_GEO_DEF_H_
#define	TWI_GEO_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//Geo
	struct SGeo
	{
		typedef std::pair<float , float>				CCoordinatePair;	//< first -> lat , second -> long
		typedef std::vector<CCoordinatePair>			CCoordinateList;
		typedef std::pair<std::string , std::string>	CAttributePair;
		typedef std::vector<CAttributePair>				CAttributeList;		

		std::string		mID;
		std::string		mCountry;
		std::string		mCountryCode;
		std::string		mName;
		std::string		mFullName;
		std::string		mPlaceType;
		//std::string		mPolylines;
		std::string		mCoordinateType;
		CAttributeList	mAttributeList;
		CCoordinateList	mBoundingBoxCoordinateList;
	};

	typedef boost::shared_ptr<SGeo>	CGeoPtr;
	typedef std::vector<CGeoPtr>	CGeoList;

	//Geo Args(ref => https://dev.twitter.com/docs/api/1.1/get/geo/reverse_geocode)
	struct SGeoArgs
	{
		SGeoArgs()
			:mLat(360)
			,mLong(360)
			,mAccuracy("")
			,mGranularity("")
			,mMaxResults(-1)
			,mName("")
			,mContainedWithin("")
			,mAttributestreetAddress("")
			,mIP("")
			,mQuery("")
		{}

		//Base
		float		mLat;
		float		mLong;
		std::string	mAccuracy;
		std::string	mGranularity;
		int			mMaxResults;

		//SimilarPlaces
		std::string	mName;
		std::string mContainedWithin;
		std::string mAttributestreetAddress;

		//Search
		std::string	mIP;
		std::string	mQuery;
	};
}

#endif	//TWI_GEO_DEF_H_