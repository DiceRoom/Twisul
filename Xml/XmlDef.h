/****************************************************************************/
/* XmlDef.h																	*/
/****************************************************************************/
#ifndef XMLDEF_H_
#define	XMLDEF_H_

#include <map>
#include <string>
#include <vector>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
struct _xmlNode;

namespace TSUL
{
	struct SXmlNodeInfo
	{
	public:
		typedef std::map<std::string , std::string> CAttributeMap;

		SXmlNodeInfo()
			:mTagName("")
			,mValue("")
		{}

		SXmlNodeInfo(const std::string& iTagName)
			:mTagName(iTagName)
			,mValue("")
		{}

		SXmlNodeInfo(const std::string& iTagName , const std::string& iValue)
			:mTagName(iTagName)
			,mValue(iValue)
		{}

	public:
		std::string		mTagName;
		std::string		mValue;
		CAttributeMap	mAttributeMap;
	};

	typedef std::vector<class CXmlNode*> CNodeList;
}

#endif //BINWRITER_H_