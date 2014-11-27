/****************************************************************************/
/* XmlAnalyzer.h															*/
/****************************************************************************/
#ifndef XMLANALYZER_H_
#define	XMLANALYZER_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "XmlDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace picojson
{
	class value;
}

namespace TSUL
{
	class CXmlNode;

	class CXmlAnalyzer
	{
	public:
		CXmlAnalyzer					();
		~CXmlAnalyzer					();

	public:
		bool		AnalyzeXmlFile		(const std::string& iXmlFilePath);
		bool		AnalyzeJsonFile		(const std::string& ijsonFilePath);
		bool		AnalyzeBuffer		(const std::string& iBuffer);
		bool		AnalyzeJsonBuffer	(const std::string& iBuffer);
		CXmlNode*	GetRootNode			() const;

	public:
		static void	ChainDestruction(CXmlNode* iNode);

	private:
		bool	_Analyze			(_xmlNode* iRootNode);
		bool	_Analyze			(const picojson::value& iRootValue);
		void	_LoopAnaluzeNode	(_xmlNode* iAnalyzeNode);
		void	_LoopAnaluzeValue	(const picojson::value& iAnalyzeValue);
		void	_GetProperties		(SXmlNodeInfo& oInfo , _xmlNode* iAnalyzeNode) const;
		
	private:
		_xmlNode*		mAnalyzeRootNode;
		_xmlNode*		mAnaluzeCurrentNode;
		CXmlNode*		mRootNode;
		CXmlNode*		mCurrentNode;
	};
}

#endif //BINWRITER_H_