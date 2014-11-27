/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "XmlNode.h"
#include <boost/foreach.hpp>

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CXmlNode::CXmlNode()
	{
	}

	CXmlNode::CXmlNode(const std::string& iTagName)
	{
		mNodeInfo.mTagName = iTagName;
	}

	CXmlNode::CXmlNode(const std::string& iTagName , const std::string& iValue)
	{
		mNodeInfo.mTagName = iTagName;
		mNodeInfo.mValue = iValue;
	}

	void CXmlNode::GetXmlBuffer(std::string& oBuffer) const
	{
		_GetXmlBuffer(oBuffer , "");
	}

	void CXmlNode::SetParentNode(CXmlNode* iParentNode)
	{
		mParentNode = iParentNode;
	}

	CXmlNode* CXmlNode::GetParentNode() const
	{
		return mParentNode;
	}

	void CXmlNode::SetNodeInfo(const SXmlNodeInfo& iNodeInfo)
	{
		mNodeInfo = iNodeInfo;
	}

	SXmlNodeInfo CXmlNode::GetNodeInfo() const
	{
		return mNodeInfo;
	}

	void CXmlNode::AddChildNode(CXmlNode* iChildNode)
	{
		if (this == iChildNode)
		{
			return;
		}

		mChildNodeList.push_back(iChildNode);
	}

	int CXmlNode::GetChildNodeValue() const
	{
		return static_cast<int>(mChildNodeList.size());
	}

	CXmlNode* CXmlNode::GetChildNode(int iIndex) const
	{
		return mChildNodeList[iIndex];
	}

	CNodeList CXmlNode::GetChildNode() const
	{
		return mChildNodeList;
	}

	CNodeList CXmlNode::GetChildNode(const std::string& iTagName) const
	{
		CNodeList ret_List;
        CNodeList::const_iterator itr = mChildNodeList.begin();
        CNodeList::const_iterator lst = mChildNodeList.end();
        for(;itr != lst; ++itr)
		{
            CXmlNode* node = *itr;
			if (node->GetNodeInfo().mTagName == iTagName)
			{
				ret_List.push_back((*itr));
			}
		}
		return ret_List;
	}

	void CXmlNode::_GetXmlBuffer(std::string& oBuffer , const std::string& iTab) const
	{
		oBuffer += (iTab + "<" + mNodeInfo.mTagName + ">");
		if(mChildNodeList.size() != 0)
		{
			oBuffer += "\n";
			BOOST_FOREACH(CXmlNode* iNode , mChildNodeList)
			{
				iNode -> _GetXmlBuffer(oBuffer , iTab + "\t");
			}
			oBuffer += (iTab + "</" + mNodeInfo.mTagName + ">\n");
		}
		else
		{
			oBuffer += mNodeInfo.mValue;
			oBuffer += ("</" + mNodeInfo.mTagName + ">\n");
		}
	}
}
