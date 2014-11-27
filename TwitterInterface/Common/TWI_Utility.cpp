/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Utility.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace
{
	typedef std::vector<std::string>	CMonthList;
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace
{
	CMonthList MonthList = boost::assign::list_of("Jan")("Feb")("Mar")("Apr")("May")("Jun")("Jul")("Aug")("Sep")("Oct")("Nov")("Dec");
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CXmlNode* GetFirstNode(const std::string& iTagName , CXmlNode* iNode)
	{
		CNodeList node_List = iNode -> GetChildNode(iTagName);
		if(node_List.size() == 0)
		{
			return 0;
		}

		return node_List[0];
	}

	bool GetFirstNodeValue(std::string& oValue , std::string iTagName , CXmlNode* iNode)
	{
		CNodeList node_List = iNode -> GetChildNode(iTagName);
		if(node_List.size() == 0)
		{
			return false;
		}

		oValue = node_List[0] -> GetNodeInfo().mValue;
		return true;
	}
 
	void GetIDList(CIDList& oIDList , CXmlNode* iNode)
	{
		CNodeList node_List = iNode -> GetChildNode();
		if(node_List.size() == 0)
		{
			return;
		}

		//str
		if(node_List[0] -> GetNodeInfo().mValue.find('.') != -1)
		{
			BOOST_FOREACH(CXmlNode* iIDNode , node_List)
			{
				oIDList.push_back(CastParameter<int>(iIDNode -> GetNodeInfo().mValue , -1));
			}
		}
		else
		{
			BOOST_FOREACH(CXmlNode* iIDNode , node_List)
			{
				oIDList.push_back(CastParameter<ID>(iIDNode -> GetNodeInfo().mValue , UNKNOWN_ID));
			}
		}
	}

	void GetCursor(SCursor& oCursor , CXmlNode* iNode)
	{
		CXmlNode* next = iNode -> GetChildNode("next_cursor_str")[0];
		CXmlNode* prev = iNode -> GetChildNode("previous_cursor_str")[0];

		oCursor.mNextCursor = CastParameter<ID>(next -> GetNodeInfo().mValue , UNKNOWN_ID);
		oCursor.mPrevCursor = CastParameter<ID>(prev -> GetNodeInfo().mValue , UNKNOWN_ID);
	}
 
	int GetMonthIndex(const std::string& iMonthName)
	{
		CMonthList::iterator find_Itr = std::find(MonthList.begin() , MonthList.end() , iMonthName);
		if(find_Itr == MonthList.end())
		{
			return 0;
		}

		return (static_cast<int>(find_Itr -  MonthList.begin()) + 1);
	}

	//Args
	void GetParameter(std::string& oAddOut , const std::string& iParamName , ID iID , bool iUnknownIDIgnoreFlg)
	{
		if(iID != UNKNOWN_ID)
		{
			oAddOut += ("?" + iParamName + "=" + boost::lexical_cast<std::string>(iID));
		}
		else if(!iUnknownIDIgnoreFlg)
		{
			oAddOut += "?" + iParamName + "=-1";
		}
	}

	void GetParameter(std::string& oAddOut , const std::string& iParamName , const CIDList& iIDList)
	{
		if(iIDList.size() == 0)
		{
			return;
		}

		oAddOut += ("?" + iParamName + "=");

		CIDList::const_iterator end_Itr , itr = iIDList.begin();
		oAddOut += boost::lexical_cast<std::string>((*itr));
		++ itr;
		for(end_Itr = iIDList.end() ; itr != end_Itr ; ++ itr)
		{
			oAddOut += ("," + boost::lexical_cast<std::string>((*itr)));
		}
	}

	void GetParameter(std::string& oAddOut , const std::string& iParamName , int iIntVal , bool iMinusValIgnoreFlg)
	{
		if(iMinusValIgnoreFlg && iIntVal < 0)
		{
			return;
		}

		oAddOut += ("?" + iParamName + "=");
		oAddOut += boost::lexical_cast<std::string>(iIntVal);
	}

	void GetParameter(std::string& oAddOut , const std::string& iParamName , float iFloatVal)
	{
		oAddOut += ("?" + iParamName + "=");
		oAddOut += boost::lexical_cast<std::string>(iFloatVal);
	}

	void GetParameter(std::string& oAddOut , const std::string& iParamName , bool iBoolVal)
	{
		oAddOut += ("?" + iParamName + "=");
		oAddOut += iBoolVal ? "1" : "0";
	}

	void GetParameter(std::string& oAddOut , const std::string& iParamName , const std::string& iStrVal)
	{
		oAddOut += ("?" + iParamName + "=");
		oAddOut += iStrVal;
	}
}
