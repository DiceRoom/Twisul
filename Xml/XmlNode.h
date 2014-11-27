/****************************************************************************/
/* XmlNode.h																*/
/****************************************************************************/
#ifndef XMLNODE_H_
#define	XMLNODE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "XmlDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	class CXmlNode
	{
	public:
		CXmlNode();
		CXmlNode(const std::string& iTagName);
		CXmlNode(const std::string& iTagName , const std::string& iValue);

		void GetXmlBuffer					(std::string& oBuffer) const;

	public:
		void			SetParentNode		(CXmlNode* iParentNode);
		CXmlNode*		GetParentNode		() const;
		void			SetNodeInfo			(const SXmlNodeInfo& iNodeInfo);
		SXmlNodeInfo	GetNodeInfo			() const;
		void			AddChildNode		(CXmlNode* iChildNode);
		int				GetChildNodeValue	() const;
		CXmlNode*		GetChildNode		(int iIndex) const;
		CNodeList		GetChildNode		() const;
		CNodeList		GetChildNode		(const std::string& iTagName) const;

	private:
		void			_GetXmlBuffer		(std::string& oBuffer , const std::string& iTab) const;

	private:	
		CXmlNode*		mParentNode;
		CNodeList		mChildNodeList;
		SXmlNodeInfo	mNodeInfo;
	};
}

#endif //BINWRITER_H_