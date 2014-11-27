/****************************************************************************/
/* TWI_Utility.h															*/
/****************************************************************************/
#ifndef TWI_UTILITY_H_
#define	TWI_UTILITY_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_CommonDef.h"

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//============================================================================
	//! Boost Foreach
	template<typename Type>
	std::pair< boost::counting_iterator<Type> , boost::counting_iterator<Type> >
	EnamerableRange(const Type& iBegin , const Type& iValue)
	{
		return std::make_pair(boost::counting_iterator<Type>(iBegin), boost::counting_iterator<Type>(iBegin + iValue));
	}

	template<class E , class T , class A>
	std::basic_string<E , T , A>
	ReplaceString(const std::basic_string<E , T , A>& iSource,
				  const std::basic_string<E , T , A>& iPattern,
				  const std::basic_string<E , T , A>& iPlacement)
	{
		std::basic_string<E , T , A> result;
        typename std::basic_string<E , T , A>::size_type pos_before = 0;
		typename std::basic_string<E , T , A>::size_type pos = 0;
		typename std::basic_string<E , T , A>::size_type len = iPattern.size();
		while((pos = iSource.find(iPattern , pos)) != std::string::npos)
		{
			result.append(iSource, pos_before, pos - pos_before);
			result.append(iPlacement);
			pos += len;
			pos_before = pos;
		}
		result.append(iSource , pos_before , iSource.size() - pos_before);
		return result;
	}

	template<typename CastType>
	inline CastType CastParameter(const std::string& iSource , const CastType& iThrowReturnVal)
	{
		if(iSource.length() == 0)
		{
			return iThrowReturnVal;
		}

		try
		{
			return boost::lexical_cast<CastType>(iSource);
		}
		catch(...)
		{
			return iThrowReturnVal;
		}
	}

	template<>
	inline ID CastParameter<ID>(const std::string& iSource , const ID& iThrowReturnVal)
	{
		if(iSource.length() == 0)
		{
			return iThrowReturnVal;
		}

		try
		{
			int pos = iSource.find('.');
			return (pos != -1) ? boost::lexical_cast<ID>(iSource.substr(0 , pos)) : boost::lexical_cast<ID>(iSource);
		}
		catch(...)
		{
			return iThrowReturnVal;
		}
	}

	template<>
	inline int CastParameter<int>(const std::string& iSource , const int& iThrowReturnVal)
	{
		if(iSource.length() == 0)
		{
			return iThrowReturnVal;
		}

		try
		{
			int pos = iSource.find('.');
			return (pos != -1) ? boost::lexical_cast<int>(iSource.substr(0 , pos)) : boost::lexical_cast<int>(iSource);
		}
		catch(...)
		{
			return iThrowReturnVal;
		}
	}

	CXmlNode*	GetFirstNode		(const std::string& iTagName , CXmlNode* iNode);
	bool		GetFirstNodeValue	(std::string& oValue , std::string iTagName , CXmlNode* iNode);
	void		GetIDList			(CIDList& oIDList , CXmlNode* iNode);
	void		GetCursor			(SCursor& oCursor , CXmlNode* iNode);
	int			GetMonthIndex		(const std::string& iMonthName);

	//Args
	void GetParameter(std::string& oAddOut , const std::string& iParamName , ID iID , bool iUnknownIDIgnoreFlg);
	void GetParameter(std::string& oAddOut , const std::string& iParamName , const CIDList& iIDList);
	void GetParameter(std::string& oAddOut , const std::string& iParamName , int iIntVal , bool iMinusValIgnoreFlg);
	void GetParameter(std::string& oAddOut , const std::string& iParamName , float iFloatVal);
	void GetParameter(std::string& oAddOut , const std::string& iParamName , bool iBoolVal);
	void GetParameter(std::string& oAddOut , const std::string& iParamName , const std::string& iStrVal);
}

#endif