/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TSUL_StdAfx.h"
#include "TWI_Stream.h"
#include "TWI_UserStreamDef.h"
#include "../System/TWI_System.h"
#include "../Getter/TWI_Getter.h"
#include "../Cache/TWI_Cache.h"
#include "../TimeLine/TWI_TimeLine.h"
#include "../Account/TWI_Account.h"

#ifdef WIN32
#include "../../Core/TWI_Oauth.h"
#define CURLOPT_ACCEPT_ENCODING CURLOPT_ENCODING
#else
#include "../../../liboauth/oauth.h"
#endif

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	std::string CStream::sCertificationFilePath;
}


/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		template<typename T>
		std::string MakeParameters(std::string parameterName, const std::vector<T>& data)
		{
			std::string result = "";
			if(!data.empty()){
				result = parameterName+"=";
	            
				BOOST_FOREACH(const T& currentData, data)
				{
					result += CSystem::Encode(boost::lexical_cast<std::string>(currentData))+",";
				}
				result = std::string(result, 0, result.size()-1);
			}
			return result;
		}

		static std::string Replace(std::string iString1 , std::string iString2 , std::string iString3)
		{
			std::string::size_type pos(iString1.find(iString2));
		    
			while(pos != std::string::npos)
			{
				iString1.replace(pos , iString2.length() , iString3);
				pos = iString1.find(iString2 , pos + iString3.length());
			}
		    
			return iString1;
		}

#ifndef WIN32
		template<typename Stream = CStreamConnection>
        Stream *CreateStreamWithOAuthConnection(const std::string& iUrl , const std::string& iKey , const std::string& iSecret)
		{
			std::string url2 = iUrl;
			url2 = oauth_sign_url2(iUrl.c_str(), 0, OA_HMAC, 0, CSystem::GetConsumerKey(), CSystem::GetConsumerSecret(), iKey.c_str(), iSecret.c_str());
			curl_global_init(CURL_GLOBAL_ALL);
			CURL* handle = curl_easy_init();
			curl_easy_setopt(handle, CURLOPT_URL, url2.c_str());
		    
			//output debug info
			curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
		    
			//use custom header
			//struct curl_slist* header = makeHeaderListForOauthEcho(url.c_str(), key, secret);
			//curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);        
		    curl_easy_setopt(handle, CURLOPT_SSLVERSION, 3);

			//use ssl certification
			//curl_easy_setopt(handle, CURLOPT_SSLCERT, CStream::CertificationFilePath().c_str());
			//ignore verify
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, false);
		    
			curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "gzip,deflate");
		    
			return new Stream(handle, NULL);
		}
#endif 
		static struct curl_slist* makeHeaderListForOauthEcho(const char* iUrl, const std::string& iKey, const std::string& iSecret)
		{
			std::string authorizationUrl;
			authorizationUrl = oauth_sign_url2(iUrl, 0, OA_HMAC, 0, CSystem::GetConsumerKey(), CSystem::GetConsumerSecret(), iKey.c_str(), iSecret.c_str());
			authorizationUrl = Replace(authorizationUrl, iUrl, "");
			authorizationUrl = Replace(authorizationUrl, "=", "=\"");
			authorizationUrl = Replace(authorizationUrl, "&", "\",");
		    
		    
			struct curl_slist* headerlist = NULL;
			headerlist = curl_slist_append(headerlist, ("Authorization: OAuth "+authorizationUrl).c_str());
			//headerlist = curl_slist_append(headerlist, "Expect: ");
			return headerlist;
		}

		bool GetJsonData(std::string& oResultString , std::string& oJsonData , const std::string& iString)
		{
			//begin
			size_t current_Pos = iString.find("{");
			if(current_Pos == -1)
			{
				oJsonData = "";
				oResultString = iString;
				return false;
			}
			++ current_Pos;

			int check_Val = 1;
			while(true)
			{
				//check
				size_t begin_Pos = iString.find("{" , current_Pos);
				size_t end_Pos = iString.find("}" , current_Pos);
				
				//finish
				if(begin_Pos == -1 && end_Pos == -1)
				{
					oJsonData = "";
					oResultString = iString;
					return false;
				}

				//end
				if(begin_Pos == -1 || begin_Pos > end_Pos)
				{
					-- check_Val;
					current_Pos = end_Pos + 1;
					if(check_Val == 0)
					{
						break;
					}
				}
				//begin
				else
				{
					++ check_Val;
					current_Pos = begin_Pos + 1;
				}
			}
			
			//next
			oJsonData = iString.substr(0 , current_Pos);
			oResultString = iString.substr(current_Pos);
			return true;
		}

		void ParseStatus(CStatusList& oStatusList , std::string& oResultString , const std::string& iString , bool iCacheFlg)
		{
			oResultString = iString;

			std::string json_Data;
			while(GetJsonData(oResultString , json_Data , oResultString))
			{
				//add list
				CXmlAnalyzer analayzer;
				assert(analayzer.AnalyzeJsonBuffer(json_Data));

				CStatusPtr status;
				if(CGetter::GetStatus(status , analayzer.GetRootNode()))
				{
					oStatusList.push_back(status);
				}
			}
		}

		//parse
		void ParseUserStream(CUserStreamEventList& oEventList , std::string& oResultString , const std::string& iString , bool iCacheFlg)
		{
			oResultString = iString;

			std::string json_Data;
			while(GetJsonData(oResultString , json_Data , oResultString))
			{
				CXmlAnalyzer analayzer;
				assert(analayzer.AnalyzeJsonBuffer(json_Data));

				//add event
				CUserStreamEvent new_Event;
				GetUserStreamEvent(new_Event , analayzer.GetRootNode());
				if(new_Event != 0)
				{
					/*CUserPtr ptr;
					new_Event -> mEvent -> GetVariable(ptr);*/
					oEventList.push_back(new_Event);
				}
			}
		}
	}
}

namespace TSUL
{
    
    CStreamConnection::CStreamConnection(CURL* curl, const char* postData):IStreamConnection(curl, postData){
    }
    CStreamConnection::~CStreamConnection(){
        if(mInvocation)
		{
			delete mInvocation;
		}
    }
    
    CUserStreamConnection::CUserStreamConnection(CURL* curl, const char* postData):IStreamConnection(curl, postData){
    }
    CUserStreamConnection::~CUserStreamConnection(){
        if(mInvocation)
		{
			delete mInvocation;
		}
    };
	//IStreamConnection---------------------------------------------------------------
	
	IStreamConnection::IStreamConnection(CURL* curl, const char* postData)
		:mCurl(curl)
        ,mPostData("")
        ,mConnecting(false)
		,mKilled(false)
        ,mUseCache(false)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _StreamWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);
		curl_easy_setopt(mCurl, CURLOPT_NOSIGNAL, true);
		if(postData)
		{
			mPostData = postData;
		}
		if(!mPostData.empty())
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, mPostData.c_str());
		}
	}

	IStreamConnection::~IStreamConnection()
	{
        if(mCurl)
		{
			curl_easy_cleanup(mCurl);
		}
    }
    
	int IStreamConnection::Connect()
	{
        mConnecting = true;
		boost::mutex::scoped_lock lock(mMutex);
		CURLcode code = curl_easy_perform(mCurl);
        mConnecting = false;
		code;
        printf("done code:%d\n", code);
        return !mKilled;
	}

	void IStreamConnection::Kill()
	{
		mKilled = true;
	}

	bool IStreamConnection::Killed() const
	{
		return mKilled;
	}

	CURL* IStreamConnection::Curl()
	{
		return mCurl;
	}

	const char* IStreamConnection::ResponseData()
	{
		return mResponseData.c_str();
	}

    void IStreamConnection::WaitUntilDone()
	{
        boost::mutex::scoped_lock lock(mMutex);
    }

	void IStreamConnection::ClearResponseData()
	{
		mResponseData.clear();
	}
    
	void CStreamConnection::AddResponseData(const char* iData , int iSize){
        IStreamConnection::AddResponseData(iData, iSize);
        CStatusList statusList;
        std::string result;
		ParseStatus(statusList, result , mResponseData , mUseCache);
        mResponseData = result;
        
        if(statusList.empty()){
            result = "";
            mResponseData = "";
        }
        if(mInvocation) mInvocation -> Call(statusList);
    }
	void CUserStreamConnection::AddResponseData(const char* iData , int iSize){
        IStreamConnection::AddResponseData(iData, iSize);
        
        CUserStreamEventList event_List;
        std::string result;
        
        ParseUserStream(event_List , result , mResponseData , mUseCache);

        mResponseData = result;
        
        if(event_List.empty()){
            result = "";
            mResponseData = "";
        }
        if(mInvocation) mInvocation -> Call(event_List);
    }
    

	void IStreamConnection::AddResponseData(const char* iData , int iSize)
	{
		mResponseData += std::string(iData);
	}

	size_t IStreamConnection::_StreamWriteData(void* iBuffer , size_t iSize , size_t iNmemb, void* iUserp)
	{
		IStreamConnection* instance = static_cast<IStreamConnection*>(iUserp);
	    
		if(instance -> Killed()) 
		{
			return 0;
		}

		int segsize = iSize * iNmemb;	
		instance -> AddResponseData(static_cast<const char*>(iBuffer) , segsize);
		return segsize;
	}

	//CStream---------------------------------------------------------------

	/************************************************************************/
	/* maximum length is 60 in each keyword									*/
	/************************************************************************/
	
	bool CStream::CreateFilterStreamConnection(CStreamConnectionPtr& oConnection, const std::vector<std::string>& iKeywords, const CIDList& iFollowings , const std::string& iKey , const std::string& iSecret)
	{
		return CreateFilterStreamConnection(oConnection, iKeywords, iFollowings, NULL, iKey, iSecret);
	}
	
	bool CStream::CreateFilterStreamConnection(CStreamConnectionPtr& oConnection, const std::vector<std::string>& iKeywords , const CIDList& iFollowings , const SStreamLocationArea* locationArea , const std::string& iKey , const std::string& iSecret)
	{
		const std::string end_Point = "https://stream.twitter.com/1/statuses/filter.json";
	    
		std::string follow = MakeParameters("follow" , iFollowings);
		std::string track = MakeParameters("track" , iKeywords);
		std::string locations = "";
		if(locationArea)
		{
			locations = "locations=" +
			boost::lexical_cast<std::string>(locationArea -> mLongitudeSouthWest)+","+
			boost::lexical_cast<std::string>(locationArea -> mLatitudeSouthWest)+","+
			boost::lexical_cast<std::string>(locationArea -> mLongitudeNorthEast)+","+
			boost::lexical_cast<std::string>(locationArea -> mLatitudeNorthEast);
		}
		std::string post = follow;
		
		if(!track.empty())
		{
			post += (post.empty() ? "" : "&") + track;
		}
		if(!locations.empty())
		{
			post += (post.empty() ? "" : "&") + locations;
		}

		std::string url = end_Point;
		if(!post.empty())
		{
			url += ("?" + post);
		}
		
#ifndef WIN32
		oConnection = CStreamConnectionPtr(CreateStreamWithOAuthConnection(url , iKey , iSecret));
#endif
		return true;
	}

	
	static struct curl_slist* makeHeaderListForOauthEcho(const char* iUrl, const std::string& iKey, const std::string& iSecret){
		std::string authorizationUrl;
		authorizationUrl = oauth_sign_url2(iUrl, 0, OA_HMAC, 0, CSystem::GetConsumerKey(), CSystem::GetConsumerSecret(), iKey.c_str(), iSecret.c_str());
		authorizationUrl = Replace(authorizationUrl, iUrl, "");
		authorizationUrl = Replace(authorizationUrl, "=", "=\"");
		authorizationUrl = Replace(authorizationUrl, "&", "\",");
	    
	    
		struct curl_slist* headerlist = NULL;
		headerlist = curl_slist_append(headerlist, ("Authorization: OAuth "+authorizationUrl).c_str());
		//headerlist = curl_slist_append(headerlist, "Expect: ");
		return headerlist;
	}

	bool CStream::CreatePubliIStreamConnection(CStreamConnectionPtr& oConnection , const std::string& iKey , const std::string& iSecret)
	{
		const std::string endPoint = "https://stream.twitter.com/1/statuses/sample.json";
#ifndef WIN32
		oConnection = CStreamConnectionPtr(CreateStreamWithOAuthConnection(endPoint , iKey , iSecret));
#endif
		return true;
	}
	
	/************************************************************************/
	/* Timeline and Event(DM , Favorite , Follow , etc)						*/
	/************************************************************************/
		
	bool CStream::CreateUserStreamConnection(CUserStreamConnectionPtr& oConnection , const std::string& iKey , const std::string& iSecret)
	{
		const std::string end_Point = "https://userstream.twitter.com/2/user.json";
#ifndef WIN32
		oConnection = CUserStreamConnectionPtr(CreateStreamWithOAuthConnection<CUserStreamConnection>(end_Point , iKey , iSecret));
#endif
		return true;
	}

	/************************************************************************/
	/* still Beta? for Site or Server										*/
	/* maximum user size is 100 in each method								*/
	/* (total maximum size of users is 1000)								*/
	/************************************************************************/

	bool CStream::CreateSiteStreamConnection(IStreamConnectionPtr& oConnection , const CIDList& iFollowings , const std::string& iKey , const std::string& iSecret)
	{
		const std::string endPoint = "https://sitestream.twitter.com/2b/site.json";
		std::string follow = MakeParameters("follow" , iFollowings);
		return true;
	}

	/************************************************************************/
	/* maximum user size is 100 in each method								*/
	/************************************************************************/

	bool CStream::AddUsersForSiteStream(const CIDList& iFollowings , const IStreamConnectionPtr& iConnection)
	{
		const std::string endPoint = "https://stream.twitter.com/1/statuses/filter.json";    
		endPoint;
		return true;
	}

	/************************************************************************/
	/* maximum user size is 100 in each method								*/
	/************************************************************************/

	bool CStream::RemoveUsersForSiteStream(const CIDList& iFollowings, const IStreamConnectionPtr& iConnection)
	{
		return true;
	}

	/************************************************************************/
	/* Other																*/
	/************************************************************************/
	
	void CStream::SetCertificationFilePath(const std::string& iPath)
	{
		sCertificationFilePath = iPath;
	}

	const std::string& CStream::CertificationFilePath()
	{
		return sCertificationFilePath;
	}
}
