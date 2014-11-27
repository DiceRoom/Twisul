/****************************************************************************/
/* TWI_Getter.h																*/
/****************************************************************************/
#ifndef TWI_GETTER_H_
#define	TWI_GETTER_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../Status/TWI_StatusDef.h"
#include "../DirectMessage/TWI_DirectMessageDef.h"
#include "../User/TWI_UserDef.h"
#include "../Lists/TWI_ListsDef.h"
#include "../Trends/TWI_TrendsDef.h"
#include "../Search/TWI_SearchDef.h"
#include "../Geo/TWI_GeoDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Getter
	class CGetter
	{
	private:
		typedef std::pair<int , int>		COffsetPair;
		typedef std::vector<COffsetPair>	COffsetList;
		
	public:
		/************************************************************************/
		/* Getter																*/
		/************************************************************************/
		static bool GetStatus			(CStatusPtr& oStatusPtr					, CXmlNode* iStateNode);
		static bool GetDirectMessage	(CDirectMessagePtr& oDirectMessage		, CXmlNode* iNode);
		static bool GetUser				(CUserPtr& oUserPtr						, CXmlNode* iUserNode);
		static bool GetPlace			(CPlacePtr& oPlace						, CXmlNode* iNode);
		static bool GetLists			(CListPtr& oList						, CXmlNode* iNode);
		static bool GetTrendInfo		(CTrendsInfoPtr& oTrendInfo				, CXmlNode* iNode);
		static bool	GetGeo				(CGeoPtr& oGeo							, CXmlNode* iNode);
		static bool GetSearchSaveInfo	(CSearchSaveInfoPtr& oSearchSaveInfo	, CXmlNode* iNode);
		static bool GetRateLimitMap		(CRateLimitMap& oRateLimitMap			, CXmlNode* iNode);

		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		static bool _GetStatus			(SStatus& oStatus , CXmlNode* iStateNode);
		static bool	_GetMedia			(SStatus& oStatus , const std::string& iText , CXmlNode* iBaseNode);
		static bool _GetMention			(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList);
		static bool _GetHashTag			(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList);
		static bool _GetUrl				(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList);
		static bool _GetPhoto			(SStatus& oStatus , CXmlNode* iEntitiesNode , const COffsetList& iOffsetList);
		static bool _GetEntityIndices	(int& oBeginPos , int& oLength , CXmlNode* iNode , const COffsetList& iOffsetList);
		static bool _GetParams			(std::string* oBufferList , const std::string* iTagList , int iBufferSize , CXmlNode* iNode);
	};
}

#endif