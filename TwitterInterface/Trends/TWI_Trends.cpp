/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Trends.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* PlaceTrend															*/
	/************************************************************************/
	 
	bool CTrends::GetPlaceTrend(SResponse oResponse , CTrendsPtr& oTrend , const std::string& iKey , const std::string& iSecret , ID iWOEID)
	{
		std::string uri = "trends/place.json";
		GetParameter(uri , "id" , iWOEID , false);

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			CXmlNode* root_Node = analayzer.GetRootNode() -> GetChildNode()[0];
			CNodeList location_List = root_Node -> GetChildNode("locations");
			if(location_List.size() == 0 || location_List[0] -> GetChildNode().size() == 0)
			{
				return false;
			}

			//base
			CXmlNode* location_Node = location_List[0] -> GetChildNode()[0];
			std::string buf_List[3];
			GetFirstNodeValue(buf_List[0] , "created_at"	, root_Node);
			GetFirstNodeValue(buf_List[1] , "name"			, location_Node);
			GetFirstNodeValue(buf_List[2] , "woeid"			, location_Node);
			
			STrend trend;
			sscanf(buf_List[0].c_str(),
				   "%d-%d-%dT%d:%d:%dZ",
				   &trend.mDate.mYear,
				   &trend.mDate.mMonth,
				   &trend.mDate.mDay,
				   &trend.mDate.mHour,
				   &trend.mDate.mMinute,
				   &trend.mDate.mSecond);
			trend.mLocationsName = CSystem::Decode(buf_List[1]);
			trend.mLocationsWOEID = CastParameter<ID>(buf_List[2].substr(0 , buf_List[2].find('.')) , UNKNOWN_ID);

			BOOST_FOREACH(CXmlNode* iNode , root_Node -> GetChildNode("trends")[0] -> GetChildNode())
			{
				CTrendsInfoPtr info;
				if(!CGetter::GetTrendInfo(info , iNode))
				{
					return false;
				}
				trend.mTrendList.push_back(info);
			}

			oTrend = CTrendsPtr(new STrend(trend));
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* AvailablePlace														*/
	/************************************************************************/
	
	bool CTrends::GetAvailablePlace(SResponse oResponse , CPlaceList& oPlaceList , const std::string& iKey , const std::string& iSecret)
	{
		std::string uri = "trends/available.json";
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));

			BOOST_FOREACH(CXmlNode* iNode , analayzer.GetRootNode() -> GetChildNode())
			{
				CPlacePtr place;
				if(!CGetter::GetPlace(place , iNode))
				{
					return false;
				}
				oPlaceList.push_back(place);
			}
		}
		return ret_Flg;
	}

	/************************************************************************/
	/* ClosestPlace                                                         */
	/************************************************************************/

	bool CTrends::GetClosestPlace(SResponse oResponse , CPlacePtr& oPlacePtr , const std::string& iKey , const std::string& iSecret , float iLat , float iLong)
	{
		std::string uri = "trends/closest.json";
		GetParameter(uri , "lat" , iLat);
		uri += ("&long=" + boost::lexical_cast<std::string>(iLong));

		CSystem::GetHttp(oResponse , iKey , iSecret , uri);		
		bool ret_Flg = (oResponse.mGetHttpState == HTTP_STATE_OK);
		if(ret_Flg)
		{
			CXmlAnalyzer analayzer;
			assert(analayzer.AnalyzeJsonBuffer(oResponse.mGetData));
			if(!CGetter::GetPlace(oPlacePtr , analayzer.GetRootNode() -> GetChildNode()[0]))
			{
				return false;
			}
		}
		return ret_Flg;
	}
}