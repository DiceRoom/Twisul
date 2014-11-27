/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_System.h"
#include "../../Core/TWI_Oauth.h"

#ifndef WIN32
#include "TWI_ObjCEncoding.h"
#else
#include <windows.h>
#endif

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		typedef std::map<int , EHttpState>			CHttpStateMap;
		typedef std::map<std::string , std::string> CTransCharMap;
	}
}

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		CHttpStateMap HttpStateMap = boost::assign::map_list_of(200 , HTTP_STATE_OK)
															   (304 , HTTP_STATE_NOT_MODIFIED)
															   (400 , HTTP_STATE_BAD_REQUEST)
															   (401 , HTTP_STATE_NOT_AUTHORIZED)
															   (403 , HTTP_STATE_FORBIDDEN)
															   (404 , HTTP_STATE_NOT_FOUND)
															   (500 , HTTP_STATE_INTERNAL_SERVER_ERROR)
															   (502 , HTTP_STATE_BAD_GATEWAY)
															   (503 , HTTP_STATE_SERVICE_UNAVALIABLE);
		CTransCharMap TransCharMap = boost::assign::map_list_of("&lt;","<")("&gt;",">")("&quot;","\"")("&amp;","&");
	}

	//CSystem
	const char* CSystem::sEndPointUri = "http://api.twitter.com/1.1/";
	const char* CSystem::sConsumerkey = "ftIp9als6FVlEKLTEjm0AA";
	const char* CSystem::sConsumerSecret = "Emug3Y3ciP5IG0Avq70so1syC1CDxXR1qEaNLW1s";
	const char* CSystem::sRequestTokenUri = "http://api.twitter.com/oauth/request_token";
	const char* CSystem::sAuthorizeUri = "http://api.twitter.com/oauth/authorize";
	const char* CSystem::sAccessTokenUri = "http://api.twitter.com/oauth/access_token?oauth_verifier=";

#if 1
	SRateLimit CSystem::sOldRateLimit;
	bool CSystem::sRateLimitGetAPIFlg = false;
#endif
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* Encode/Decode														*/
	/************************************************************************/
	
	//============================================================================
	//! From Shiftjis : To url
	std::string CSystem::Encode(const std::string& iSource)
	{
#ifdef WIN32
		std::string ret_Code = iSource;
		int length;
		wchar_t ucs2[1000];
		char utf8[1000];
		length = MultiByteToWideChar(CP_ACP , 0 , ret_Code.c_str() , ret_Code.size() , ucs2 , 1000);
		length = WideCharToMultiByte(CP_UTF8 , 0 , ucs2 , length , utf8 , 1000 , 0 , 0);
		ret_Code.assign(utf8 , length);
		return oauth_url_escape(ret_Code.c_str());
#else
        return encodeEscapeFromUtf8(iSource);
#endif
	}

	//============================================================================
	//! From UTF-8 : To Shifjis
	std::string CSystem::Decode(const std::string& iSource)
	{
#ifdef WIN32
		//To Unicode
		int unicode_Length = MultiByteToWideChar(CP_UTF8, 0, iSource.c_str(), iSource.length()+1, NULL, 0);
		wchar_t* unicode_Buf = new wchar_t[unicode_Length + 1];
		MultiByteToWideChar(CP_UTF8 , 0 , iSource.c_str() , iSource.length() + 1 , unicode_Buf , unicode_Length + 1);

		//To ShiftJis
		int utf8_Length = WideCharToMultiByte(CP_ACP, 0, unicode_Buf , unicode_Length , NULL , 0 , NULL , NULL);
		char* shiftjis_Buf = new char[utf8_Length + 1];
		WideCharToMultiByte(CP_ACP , 0 , unicode_Buf, unicode_Length , shiftjis_Buf , utf8_Length , NULL , NULL);
		std::string ret_Str = shiftjis_Buf;
		delete[] shiftjis_Buf;

#else
        std::string ret_Str = iSource;
//        std::string ret_Str = decodeEscapeFromUtf8(iSource);
#endif

		//",<,>,&
		CTransCharMap::iterator itr , end_Itr = TransCharMap.end();
		for(itr = TransCharMap.begin() ; itr != end_Itr ; ++ itr)
		{
			ret_Str = ReplaceString(ret_Str , (*itr).first , (*itr).second);
		}

		return ret_Str;
	} 

	std::string CSystem::TransSpecialChar(const std::string& iSource)
	{
		std::string ret_Str = iSource;

		//",<,>,&
		CTransCharMap::iterator itr , end_Itr = TransCharMap.end();
		for(itr = TransCharMap.begin() ; itr != end_Itr ; ++ itr)
		{
			ret_Str = ReplaceString(ret_Str , (*itr).second , (*itr).first);
		}

		return ret_Str;
	}

	/************************************************************************/
	/* Get/Post																*/
	/************************************************************************/

	//============================================================================
	//! HttpGet
	void CSystem::GetHttp(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri)
	{
		std::string uri = sEndPointUri + iUri;
		std::string req_url = oauth_sign_url2(uri.c_str() , 0 , OA_HMAC , 0 , sConsumerkey , sConsumerSecret , iKey.c_str() , iSecret.c_str());

		std::vector<std::string> header_List;
		oResponse.mGetData = oauth_http_get(req_url.c_str() , &header_List);
		_GetResponseData(oResponse , header_List);
	}

	//============================================================================
	//! HttpPost
	void CSystem::PostHttp(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri , const std::string& iContentType , char* iArgBuffer , int iArgBufferSize)
	{
		std::string uri = sEndPointUri + iUri;
		std::string post_Args;
		std::string req_url = oauth_sign_url2(uri.c_str() , &post_Args , OA_HMAC , 0 , sConsumerkey , sConsumerSecret , iKey.c_str() , iSecret.c_str());

		std::string content_Type = (iContentType.length() == 0) ? "application/x-www-form-urlencoded" : iContentType;
		std::vector<std::string> header_List;
		if(iArgBuffer != 0)
		{
			oResponse.mGetData = oauth_http_post(req_url.c_str() , content_Type , iArgBuffer , iArgBufferSize + 1, post_Args.c_str() , false , &header_List);
		}
		else
		{
			oResponse.mGetData = oauth_http_post(req_url.c_str() , content_Type , post_Args.c_str() , post_Args.length(), "" , false , &header_List);
		}
		_GetResponseData(oResponse , header_List);
	}

	/************************************************************************/
	/* BitLy                                                                */
	/************************************************************************/

	bool CSystem::GetShortUrl(std::string& oUrl , const std::string& iUserName , const std::string& iPass , const std::string& iUrl)
	{
		std::string uri = "http://api.bitly.com/v3/shorten";
		uri += ("?longUrl=" + iUrl);
		uri += ("&login=" + iUserName);
		uri += ("&apiKey=" + iPass);
		uri += "&format=json";

		std::vector<std::string> header_List;
		std::string get_Data = oauth_http_get(uri.c_str() , &header_List);
		CXmlAnalyzer analyzer;
		assert(analyzer.AnalyzeJsonBuffer(get_Data));
		CXmlNode* root_Node = analyzer.GetRootNode();

		//success check
		CXmlNode* code_Node = root_Node -> GetChildNode("status_code")[0];
		if(CastParameter<int>(code_Node -> GetNodeInfo().mValue , -1) != 200)
		{
			return false;
		}

		//url
		CXmlNode* url_Node = root_Node -> GetChildNode("data")[0];
		url_Node = url_Node -> GetChildNode("url")[0];
		oUrl = url_Node -> GetNodeInfo().mValue;
		return true;
	}

	bool CSystem::GetLongUrl(std::string& oUrl , const std::string& iUserName , const std::string& iPass , const std::string& iShortUrl)
	{
		//check
		int pos = iShortUrl.find("http://bit.ly/");
		if(pos == -1)
		{
			return false;
		}

		//decode
		std::string uri = "http://api.bitly.com/v3/expand";
		uri += ("?shortUrl=" + iShortUrl);
		uri += ("&login=" + iUserName);
		uri += ("&apiKey=" + iPass);
		uri += "&format=json";

		std::vector<std::string> header_List;
		std::string get_Data = oauth_http_get(uri.c_str() , &header_List);
		CXmlAnalyzer analyzer;
		assert(analyzer.AnalyzeJsonBuffer(get_Data));
		CXmlNode* root_Node = analyzer.GetRootNode();

		//success check
		CXmlNode* code_Node = root_Node -> GetChildNode("status_code")[0];
		if(CastParameter<int>(code_Node -> GetNodeInfo().mValue , -1) != 200)
		{
			return false;
		}

		//url
		CXmlNode* url_Node = root_Node -> GetChildNode("data")[0];
		url_Node = url_Node -> GetChildNode("expand")[0] -> GetChildNode()[0];
		url_Node = url_Node -> GetChildNode("long_url")[0];
		oUrl = Decode(url_Node -> GetNodeInfo().mValue);
		return true;
	}

	/************************************************************************/
	/* Inner Decl															*/
	/************************************************************************/

	void CSystem::_GetResponseData(SResponse& oResponse , const std::vector<std::string>& iHeaderList)
	{
		//zero header list
		if(iHeaderList.size() == 0 || oResponse.mGetData.length() == 0)
		{
			oResponse.mGetHttpState = HTTP_STATE_BAD_HEADER_LIST;
			return;
		}

		//code check
		int version1, version2 , code;
		sscanf(iHeaderList[0].c_str() , "HTTP/%u.%u %u", &version1 , &version2 , &code);
		CHttpStateMap::const_iterator find_Itr = HttpStateMap.find(code);
		if(find_Itr == HttpStateMap.end())
		{
			oResponse.mGetHttpState = HTTP_STATE_BAD_HEADER_LIST;
			return;
		}
		oResponse.mGetHttpState = (*find_Itr).second;
		
		//LimitAPI
		oResponse.mRateLimit.mLimit = -1;
		int limit_API_Index = _GetHeaderIndex("X-Rate-Limit-Limit: " , iHeaderList);
		if(limit_API_Index != -1)
		{
			sscanf(iHeaderList[limit_API_Index].c_str() , "X-Rate-Limit-Limit: %u" , &oResponse.mRateLimit.mLimit);
		}

		//Remaining
		oResponse.mRateLimit.mRemaining = -1;
		int result_API_Index = _GetHeaderIndex("X-Rate-Limit-Remaining" , iHeaderList);
		if(result_API_Index != -1)
		{
			sscanf(iHeaderList[result_API_Index].c_str() , "X-Rate-Limit-Remaining: %u" , &oResponse.mRateLimit.mRemaining);
		}

		//ResetAPITime
		oResponse.mRateLimit.mResetTime = -1;
		int time_API_Index = _GetHeaderIndex("X-Rate-Limit-Reset" , iHeaderList);
		if(time_API_Index != -1)
		{
			sscanf(iHeaderList[time_API_Index].c_str() , "X-Rate-Limit-Reset: %u" , &oResponse.mRateLimit.mResetTime);
		}
	}

	int CSystem::_GetHeaderIndex(const std::string& iSearchHeaderName , const std::vector<std::string>& iHeaderList)
	{
		size_t length = iSearchHeaderName.length();
		std::vector<std::string>::const_iterator itr , end_Itr = iHeaderList.end();
		for(itr = iHeaderList.begin() ; itr != end_Itr ; ++ itr)
		{
			if(iSearchHeaderName == (*itr).substr(0 , length))
			{
				return (itr - iHeaderList.begin());
			}
		}

		return -1;
	}

	//API 1.0 ONLY-----------------------------------------------------------------
	//============================================================================
	//! HttpGet API 1.0
	void CSystem::GetHttpOld(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri)
	{
		std::string uri = "http://api.twitter.com/1/" + iUri;
		std::string req_url = oauth_sign_url2(uri.c_str() , 0 , OA_HMAC , 0 , sConsumerkey , sConsumerSecret , iKey.c_str() , iSecret.c_str());

		std::vector<std::string> header_List;
		oResponse.mGetData = oauth_http_get(req_url.c_str() , &header_List);
		
		//zero header list
		if(header_List.size() == 0 || oResponse.mGetData.length() == 0)
		{
			oResponse.mGetHttpState = HTTP_STATE_BAD_HEADER_LIST;
			return;
		}

		//code check
		int version1, version2 , code;
		sscanf(header_List[0].c_str() , "HTTP/%u.%u %u", &version1 , &version2 , &code);
		CHttpStateMap::const_iterator find_Itr = HttpStateMap.find(code);
		if(find_Itr == HttpStateMap.end())
		{
			oResponse.mGetHttpState = HTTP_STATE_BAD_HEADER_LIST;
			return;
		}
		oResponse.mGetHttpState = (*find_Itr).second;
		
		//LimitAPI
		oResponse.mRateLimit.mLimit = -1;
		int limit_API_Index = _GetHeaderIndex("X-RateLimit-Limit: " , header_List);
		if(limit_API_Index != -1)
		{
			sscanf(header_List[limit_API_Index].c_str() , "X-RateLimit-Limit: %u" , &oResponse.mRateLimit.mLimit);
		}

		//Remaining
		oResponse.mRateLimit.mRemaining = -1;
		int result_API_Index = _GetHeaderIndex("X-RateLimit-Remaining" , header_List);
		if(result_API_Index != -1)
		{
			sscanf(header_List[result_API_Index].c_str() , "X-RateLimit-Remaining: %u" , &oResponse.mRateLimit.mRemaining);
		}

		//ResetAPITime
		oResponse.mRateLimit.mResetTime = -1;
		int time_API_Index = _GetHeaderIndex("X-RateLimit-Reset" , header_List);
		if(time_API_Index != -1)
		{
			sscanf(header_List[time_API_Index].c_str() , "X-RateLimit-Reset: %u" , &oResponse.mRateLimit.mResetTime);
		}

#if 1
		//set
		sOldRateLimit = oResponse.mRateLimit;
#endif
	}
     
	SRateLimit CSystem::GetLimitRate(const std::string& iKey , const std::string& iSecret)
	{
		//use api
		static bool first_Flg = (sOldRateLimit.mLimit == sOldRateLimit.mRemaining);
		if(first_Flg && !sRateLimitGetAPIFlg)
		{
			sOldRateLimit.mRemaining = sOldRateLimit.mLimit = 350;
			sOldRateLimit.mResetTime = 0;
		}
		first_Flg = false;

		if(sRateLimitGetAPIFlg)
		{
			//API ver 1.0
			std::string uri = "account/rate_limit_status.json";
			SResponse response;
			GetHttpOld(response , iKey , iSecret , uri);
			bool ret_Flg = (response.mGetHttpState == HTTP_STATE_OK);
			if(ret_Flg)
			{
				CXmlAnalyzer analayzer;
				assert(analayzer.AnalyzeJsonBuffer(response.mGetData));
				CXmlNode* root_Node = analayzer.GetRootNode();

				std::string buf;
				GetFirstNodeValue(buf , "hourly_limit" , root_Node);
				sOldRateLimit.mLimit = CastParameter<int>(buf , 0);
				GetFirstNodeValue(buf , "remaining_hits" , root_Node);
				sOldRateLimit.mRemaining = CastParameter<int>(buf , 0);
				GetFirstNodeValue(buf , "reset_time_in_seconds" , root_Node);
				sOldRateLimit.mResetTime = CastParameter<int>(buf , 0);
			}
		}

		return sOldRateLimit;
	}
    
    void CSystem::MakeOAuthURL(std::string &oUrl, const std::string &iUrl, const std::string& iKey , const std::string& iSecret)
	{
		oUrl = oauth_sign_url2(iUrl.c_str() , 0 , OA_HMAC , 0 , sConsumerkey , sConsumerSecret , iKey.c_str() , iSecret.c_str());        
    }
}