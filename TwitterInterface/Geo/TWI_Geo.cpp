/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Geo.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* GeoPlace																*/
	/************************************************************************/
	
	bool CGeo::GetGeoPlace(SResponse oResponse , CGeoPtr& oGeo , const std::string& iKey , const std::string& iSecret , const std::string& iID)
	{
		std::string uri = "geo/id/" + iID + ".json";
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CGeoPtr geo(new SGeo());
			if(!CGetter::GetGeo(geo , analayzer.GetRootNode()))
			{
				return false;
			}
			oGeo = geo;
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* ReverseGeocod														*/
	/************************************************************************/
	
	bool CGeo::GetReverseGeocode(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs)
	{
		std::string uri = "geo/reverse_geocode.json";
		GetParameter(uri , "lat"	, iGeoArgs.mLat);
		uri += ("&long=" + boost::lexical_cast<std::string>(iGeoArgs.mLong));
		if(iGeoArgs.mAccuracy.length() != 0)
		{
			uri += ("&accuracy=" + iGeoArgs.mAccuracy);
		}
		if(iGeoArgs.mGranularity.length() != 0)
		{
			uri += ("&granularity=" + iGeoArgs.mGranularity);
		}
		if(iGeoArgs.mMaxResults > 0)
		{
			uri += ("&max_results=" + boost::lexical_cast<std::string>(iGeoArgs.mMaxResults));
		}

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList result_List = analayzer.GetRootNode() -> GetChildNode("result");
			if(result_List.size() == 0)
			{
				return false;
			}
			CNodeList place_List = result_List[0] -> GetChildNode("places");
			if(place_List.size() == 0)
			{
				return false;
			}

			BOOST_FOREACH(CXmlNode* iNode , place_List[0] -> GetChildNode())
			{
				CGeoPtr geo(new SGeo());
				if(!CGetter::GetGeo(geo , iNode))
				{
					return false;
				}
				oGeoList.push_back(geo);
			}
		}
		return ret_Flg;
	}
	
	/************************************************************************/
	/* SimilarPlaces														*/
	/************************************************************************/
	
	bool CGeo::GetSimilarPlaces(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs)
	{
		std::string uri = "geo/reverse_geocode.json";
		GetParameter(uri , "lat"	, iGeoArgs.mLat);
		uri += ("&long=" + boost::lexical_cast<std::string>(iGeoArgs.mLong));
		uri += ("&name=" + iGeoArgs.mName);

		if(iGeoArgs.mContainedWithin.length() != 0)
		{
			uri += ("&contained_within=" + iGeoArgs.mContainedWithin);
		}
		if(iGeoArgs.mAttributestreetAddress.length() != 0)
		{
			uri += ("&attribute:street_address=" + iGeoArgs.mAttributestreetAddress);
		}

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList result_List = analayzer.GetRootNode() -> GetChildNode("result");
			if(result_List.size() == 0)
			{
				return false;
			}
			CNodeList place_List = result_List[0] -> GetChildNode("places");
			if(place_List.size() == 0)
			{
				return false;
			}

			BOOST_FOREACH(CXmlNode* iNode , place_List[0] -> GetChildNode())
			{
				CGeoPtr geo(new SGeo());
				if(!CGetter::GetGeo(geo , iNode))
				{
					return false;
				}
				oGeoList.push_back(geo);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* SimilarPlaces														*/
	/************************************************************************/
	 
	bool CGeo::GetSearchPlaces(SResponse oResponse , CGeoList& oGeoList , const std::string& iKey , const std::string& iSecret , const SGeoArgs& iGeoArgs)
	{
		//get param
		std::string uri = "geo/search.json";
		std::vector<std::string> keyword_List;
		if(iGeoArgs.mLat != 360)
		{
			keyword_List.push_back("lat=" + boost::lexical_cast<std::string>(iGeoArgs.mLat));
		}
		if(iGeoArgs.mLong != 360)
		{
			keyword_List.push_back("long=" + boost::lexical_cast<std::string>(iGeoArgs.mLong));
		}
		if(iGeoArgs.mName.length() != 0)
		{
			keyword_List.push_back("query=" + iGeoArgs.mName);
		}
		if(iGeoArgs.mIP.length() != 0)
		{
			keyword_List.push_back("ip=" + iGeoArgs.mIP);
		}

		size_t keyword_Val = keyword_List.size();
		if(keyword_Val == 0)
		{
			return false;
		}
		uri += ("?" + keyword_List[0]);
		for(size_t iIndex = 1 ; iIndex < keyword_Val ; ++ iIndex)
		{
			uri += ("&" + keyword_List[iIndex]);
		}

		//sub param
		if(iGeoArgs.mGranularity.length() != 0)
		{
			uri += ("&granularity=" + iGeoArgs.mGranularity);
		}
		if(iGeoArgs.mAccuracy.length() != 0)
		{
			uri += ("&accuracy=" + iGeoArgs.mAccuracy);
		}
		if(iGeoArgs.mMaxResults > 0)
		{
			uri += ("&max_results=" + boost::lexical_cast<std::string>(iGeoArgs.mMaxResults));
		}
		if(iGeoArgs.mContainedWithin.length() != 0)
		{
			uri += ("&contained_within=" + iGeoArgs.mContainedWithin);
		}
		if(iGeoArgs.mAttributestreetAddress.length() != 0)
		{
			uri += ("&attribute:street_address=" + iGeoArgs.mAttributestreetAddress);
		}

		//get
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CNodeList result_List = analayzer.GetRootNode() -> GetChildNode("result");
			if(result_List.size() == 0)
			{
				return false;
			}
			CNodeList place_List = result_List[0] -> GetChildNode("places");
			if(place_List.size() == 0)
			{
				return false;
			}

			BOOST_FOREACH(CXmlNode* iNode , place_List[0] -> GetChildNode())
			{
				CGeoPtr geo(new SGeo());
				if(!CGetter::GetGeo(geo , iNode))
				{
					return false;
				}
				oGeoList.push_back(geo);
			}
		}
		return ret_Flg;
	}
}