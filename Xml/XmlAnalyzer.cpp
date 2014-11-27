/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "XmlAnalyzer.h"
#include "XmlNode.h"
#include <fstream>
#include <TwitterInterface/Json/picojson.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/foreach.hpp>

#pragma comment(lib,"libcharset.lib")
#pragma comment(lib,"libiconv.lib")

/*
#ifdef WIN32
#include <Public/libxml/include/libxml/xmlreader.h>
#else
#include <libxml/xmlreader.h>
#endif
*/

#include <libxml/xmlreader.h>
#pragma comment(lib,"libxml2.lib")

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace
{
	typedef boost::char_separator<char> CCharSeparator;
	typedef boost::tokenizer<CCharSeparator> CTokenizer;
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace
{
	static const CCharSeparator SpaceSeparator(" ");
	static const CCharSeparator EqualSeparator("=");
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CXmlAnalyzer::CXmlAnalyzer()
		:mRootNode(0)
	{
#ifdef WIN32
        xmlInitParser();
#endif
	}

	CXmlAnalyzer::~CXmlAnalyzer()
	{
		if(mRootNode != 0)
		{
			ChainDestruction(mRootNode);
            delete mRootNode;
		}
#ifdef WIN32
		xmlCleanupParser();
#endif
	}

	bool CXmlAnalyzer::AnalyzeXmlFile(const std::string& iXmlFilePath)
    {
		xmlTextReaderPtr ptr = xmlNewTextReaderFilename(iXmlFilePath.c_str());
		if(xmlTextReaderRead(ptr) == 0)
		{
			return false;
		}

		bool ret_Flg = _Analyze(xmlTextReaderExpand(ptr));
		xmlFreeTextReader(ptr);
		return ret_Flg;
    }

	bool CXmlAnalyzer::AnalyzeJsonFile(const std::string& iJsonFilePath)
	{
		std::string buf = "";
		std::ifstream ifs(iJsonFilePath.c_str());
		if(ifs.fail())
		{
			return false;
		}

		while(!ifs.eof())
		{
			std::string get;
			getline(ifs , get);
			buf += get + "\n";
		}

		return AnalyzeJsonBuffer(buf);
	}

	bool CXmlAnalyzer::AnalyzeBuffer(const std::string& iBuffer)
    {
#if 1
		xmlDocPtr ptr = xmlParseMemory(iBuffer.c_str(), iBuffer.length());
		if(ptr == 0)
		{
			return false;
		}

		bool ret_Flg = _Analyze(ptr -> children);
		xmlFreeDoc(ptr);
		return ret_Flg;
#else
		xmlTextReaderPtr ptr = xmlReaderForMemory(iBuffer.c_str(),
												 iBuffer.size(),
												 0,
												 0,
												 XML_PARSE_NOENT    |
												 XML_PARSE_NOBLANKS |
												 XML_PARSE_NSCLEAN  |
												 XML_PARSE_NOCDATA  |
												 XML_PARSE_COMPACT);
		if(xmlTextReaderRead(ptr) == 0)
		{
			return false;
		}

		bool ret_Flg = _Analyze(xmlTextReaderExpand(ptr));
		xmlFreeTextReader(ptr);
		return ret_Flg;
#endif
	}

	bool CXmlAnalyzer::AnalyzeJsonBuffer(const std::string& iBuffer)
	{
		if(iBuffer.length() == 0)
		{
			return false;
		}

		picojson::value v;
		const char* s = iBuffer.c_str();
		std::string err;
		picojson::parse(v, s, s + strlen(s) , &err);
		if(err.length() != 0)
		{
			return false;
		}

		_Analyze(v);
		return true;
	}

	CXmlNode* CXmlAnalyzer::GetRootNode() const
    {
        return mRootNode;
    }

	bool CXmlAnalyzer::_Analyze(_xmlNode* iRootNode)
	{
		bool success_Flg = true;
		try
		{
			mRootNode = new CXmlNode("__SYS_ROOT__");
			mCurrentNode = mRootNode;
			mAnalyzeRootNode = iRootNode;
			_LoopAnaluzeNode(mAnalyzeRootNode);
		}
		catch(...)
		{
			success_Flg = false;
		}

		return success_Flg;
	}

	bool CXmlAnalyzer::_Analyze(const picojson::value& iAnalyzeValue)
	{
		bool success_Flg = true;
		try
		{
			mRootNode = new CXmlNode("__SYS_ROOT__");
			mCurrentNode = mRootNode;
			_LoopAnaluzeValue(iAnalyzeValue);
		}
		catch(...)
		{
			success_Flg = false;
		}

		return success_Flg;
	}

	void CXmlAnalyzer::ChainDestruction(CXmlNode* iNode)
	{
		BOOST_FOREACH(CXmlNode* iChildNode , iNode -> GetChildNode())
		{
			ChainDestruction(iChildNode);
            delete iChildNode;
		}
	}

	void CXmlAnalyzer::_LoopAnaluzeNode(_xmlNode* iAnalyzeNode)
	{
		if(iAnalyzeNode == 0)
		{
			return;
		}

		CXmlNode* new_Node = new CXmlNode();
		SXmlNodeInfo info;
		_GetProperties(info , iAnalyzeNode);
		mCurrentNode -> AddChildNode(new_Node);
		new_Node -> SetParentNode(mCurrentNode);

		CXmlNode* prev_Node = mCurrentNode;
		mCurrentNode = new_Node;

		_xmlNode* child_Node = iAnalyzeNode -> children;
		while(child_Node != 0)
		{
			if(child_Node -> type == XML_TEXT_NODE)
			{
				std::string text = reinterpret_cast<const char*>(child_Node -> content);
				boost::trim(text);
				if(text.length() != 0)
				{
					info.mValue = text;
				}
			}
			else if(child_Node -> type == XML_ELEMENT_NODE)
			{
				_LoopAnaluzeNode(child_Node);
			}

			child_Node = child_Node -> next;
		}

		new_Node -> SetNodeInfo(info);
		mCurrentNode = prev_Node;
	}

	void CXmlAnalyzer::_LoopAnaluzeValue(const picojson::value& iAnalyzeValue)
	{
		if(iAnalyzeValue.is<picojson::array>())
		{
			const picojson::array& array = iAnalyzeValue.get<picojson::array>();
			picojson::array::const_iterator itr , end_Itr = array.end();
			for (itr = array.begin(); itr != end_Itr ; ++ itr)
			{
				CXmlNode* new_Node = new CXmlNode();
				new_Node -> SetParentNode(mCurrentNode);
				mCurrentNode -> AddChildNode(new_Node);
				CXmlNode* prev_Node = mCurrentNode;
				mCurrentNode = new_Node;
				_LoopAnaluzeValue((*itr));
				mCurrentNode = prev_Node;
			}
		}
		else if(iAnalyzeValue.is<picojson::object>())
		{
			const picojson::object& object = iAnalyzeValue.get<picojson::object>();
			picojson::object::const_iterator itr , end_Itr = object.end();
			for(itr = object.begin() ; itr != object.end() ; ++ itr)
			{
				if(itr -> second.is<picojson::object>() ||
				   itr -> second.is<picojson::array>())
				{
					CXmlNode* new_Node = new CXmlNode(itr -> first);
					new_Node -> SetParentNode(mCurrentNode);
					mCurrentNode -> AddChildNode(new_Node);
					CXmlNode* prev_Node = mCurrentNode;
					mCurrentNode = new_Node;
					_LoopAnaluzeValue(itr -> second);
					mCurrentNode = prev_Node;

				}
				else
				{
					CXmlNode* set_Node = new CXmlNode(itr -> first , itr -> second.to_str());
					set_Node -> SetParentNode(mCurrentNode);
					mCurrentNode -> AddChildNode(set_Node);
				}
			}
		}
		else
		{
			SXmlNodeInfo info = mCurrentNode -> GetNodeInfo();
			info.mValue = iAnalyzeValue.to_str();
			mCurrentNode -> SetNodeInfo(info);
		}
	}

	void CXmlAnalyzer::_GetProperties(SXmlNodeInfo& oInfo , _xmlNode* iAnalyzeNode) const
	{
		oInfo.mTagName = reinterpret_cast<const char*>(iAnalyzeNode -> name);
		
		_xmlAttr* properties_Attr = iAnalyzeNode -> properties; 
		while(properties_Attr != 0)
		{
			if(properties_Attr -> type == XML_ATTRIBUTE_NODE)
			{
				std::string attr_Name = reinterpret_cast<const char*>(properties_Attr -> name);
				boost::trim(attr_Name);

				std::string attr_Val = "";
				_xmlNode* attr_Node = properties_Attr -> children;
				while(attr_Node != 0)
				{
					if(attr_Node -> type == XML_TEXT_NODE)
					{
						attr_Val = reinterpret_cast<const char*>(attr_Node -> content);
						boost::trim(attr_Val);
						break;
					}
					attr_Node = attr_Node -> next;
				}
			
				oInfo.mAttributeMap.insert(std::make_pair(attr_Name , attr_Val));
			}

			properties_Attr = properties_Attr -> next;
		}
	}
}
