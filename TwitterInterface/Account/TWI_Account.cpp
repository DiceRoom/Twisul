/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_Account.h"
#include "../System/TWI_System.h"
#include "../../Core/TWI_Oauth.h"
#include <boost/tokenizer.hpp>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace
{
	typedef std::map<std::string , std::string>	CSplitMap;
	typedef boost::char_separator<char> CCharSeparator;
	typedef boost::tokenizer<CCharSeparator> CTokenizer;
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	namespace
	{
		void SplitParameter(CSplitMap& oOutMap, const std::string& iStr)
		{
			CCharSeparator sep("&");
			CTokenizer tokens(iStr, sep);

			CTokenizer::iterator itr , end_Itr = tokens.end();
			for (itr = tokens.begin(); itr != end_Itr ; ++itr)
			{
				std::string get_Token = (*itr);
				int find_Pos = get_Token.find("=");
				if(find_Pos != -1)
				{
					std::string key = get_Token.substr(0, find_Pos);
					std::string val = get_Token.substr(find_Pos + 1);
					oOutMap.insert(std::make_pair(key , val));
				}
			}
		}
	}
}

namespace TSUL
{
	/************************************************************************/
	/* GetAuthorizeUrl														*/
	/************************************************************************/
	
	bool CAccount::GetAuthorizeUrl(std::string& oUrl , std::string& oKey)
	{
		//Getparam
		std::string request_token_uri , postarg;
		std::string req_url = oauth_sign_url2(CSystem::GetReuestTokenUri() , &postarg , OA_HMAC , 0 , CSystem::GetConsumerKey() , CSystem::GetConsumerSecret() , 0 , 0);
		std::string reply = oauth_http_post(req_url.c_str() , "application/x-www-form-urlencoded" , postarg.c_str() , postarg.length(), "" , false);
		CSplitMap split_Map;
		SplitParameter(split_Map , reply);

		//CheckParam
		CSplitMap::const_iterator key = split_Map.find("oauth_token");
		CSplitMap::const_iterator secret = split_Map.find("oauth_token_secret");
		if(key == split_Map.end() || secret == split_Map.end())
		{
			return false;
		}

		//GetKey&Secret
		oKey = (*key).second;
		oUrl = oauth_sign_url2(CSystem::GetAuthorizeUri() , 0 , OA_HMAC , 0 , CSystem::GetConsumerKey() , CSystem::GetConsumerSecret() , oKey.c_str() , (*secret).second.c_str());
		return true;
	}

	/************************************************************************/
	/* GetAccount															*/
	/************************************************************************/
	
	bool CAccount::GetAccount(SAccount& oAccount , const std::string& iKey , const std::string& iPinNo)
	{
		std::string url = CSystem::GetAccessTokenUri() + boost::lexical_cast<std::string>(iPinNo);
		std::string req_url = oauth_sign_url2(url.c_str() , 0, OA_HMAC , 0 , CSystem::GetConsumerKey() , 0 , iKey.c_str() , 0);
		std::string reply = oauth_http_get(req_url.c_str());

		//insert
		CSplitMap split_Map;
		SplitParameter(split_Map , reply);
		CSplitMap::const_iterator name = split_Map.find("screen_name");
		CSplitMap::const_iterator id = split_Map.find("user_id");
		CSplitMap::const_iterator key = split_Map.find("oauth_token");
		CSplitMap::const_iterator secret = split_Map.find("oauth_token_secret");
		if(name == split_Map.end()	||
		  id == split_Map.end()	||
		  key == split_Map.end()	||
		  secret == split_Map.end())
		{
			return false;
		}

		oAccount.mUserName = (*name).second;
		oAccount.mUserID = CastParameter<ID>((*id).second , UNKNOWN_ID);
		oAccount.mKey = (*key).second;
		oAccount.mSecret = (*secret).second;
		return true;
	}

	/************************************************************************/
	/* VerifyCredentials													*/
	/************************************************************************/
	
	bool CAccount::IsVerifyCredentials(SResponse& oResponse , const std::string& iKey , const std::string& iSecret)
	{
		const std::string uri = "account/verify_credentials.json";
		CSystem::GetHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* UpdateProfileBackColor												*/
	/************************************************************************/
	
	bool CAccount::UpdateProfile(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const SAccountProfile& iProfile)
	{
		std::string uri = "account/update_profile.json";
		uri += ("&name=" + CSystem::Encode(iProfile.mName));
		uri += ("&url=" + CSystem::Encode(iProfile.mURL));
		uri += ("&location=" + CSystem::Encode(iProfile.mLocation));
		uri += ("&description=" + CSystem::Encode(iProfile.mDescription));
		CSystem::PostHttp(oResponse , iKey , iSecret , uri);
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}

	/************************************************************************/
	/* UpdateProfileImage													*/
	/************************************************************************/
	
	bool CAccount::UpdateProfileImage(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iFileName , const char* iBuffer , int iBufferSize)
	{
        const char *extPtr = strrchr(iFileName.c_str(), '.');
        if(!extPtr) return false;
        std::string ext = extPtr;
        std::string imageType;
		if(ext == ".jpg")		{imageType = "jpeg";}
		else if(ext == ".png")  {imageType = "png";}
        
		std::string uri = "account/update_profile_image.json";
		std::string extension = iFileName.substr(iFileName.find_last_of('.'));
		std::string content_Boundary_Base = "--2e701b9768ba";
		std::string body_Field[4];
		body_Field[0] = "--" + content_Boundary_Base + "\r\n";
		body_Field[1] = "Content-Disposition: form-data; name=\"image\"; filename=\"" + iFileName + "\"\r\n";
		body_Field[2] = "Content-Type: image/" + imageType + "\r\n\r\n";
		body_Field[3] = "\r\n--" + content_Boundary_Base + "--\r\n";

		int length = iBufferSize + 1;
		length += body_Field[0].length();
		length += body_Field[1].length();
		length += body_Field[2].length();
		length += body_Field[3].length();

		char* buf = new char[length];
		memset(buf , 0 , length);
		int offset = 0;
		memcpy(buf , body_Field[0].c_str() , body_Field[0].length());
		offset += body_Field[0].length();
		memcpy(buf + offset , body_Field[1].c_str() , body_Field[1].length());
		offset += body_Field[1].length();
		memcpy(buf + offset , body_Field[2].c_str() , body_Field[2].length());
		offset += body_Field[2].length();
		memcpy(buf + offset , iBuffer , iBufferSize);
		offset += iBufferSize;
		memcpy(buf + offset , body_Field[3].c_str() , body_Field[3].length());

		std::string content_Type = "multipart/form-data; boundary=" + content_Boundary_Base;
		CSystem::PostHttp(oResponse , iKey , iSecret , uri , content_Type , buf , length);
		delete[] buf;
		return (oResponse.mGetHttpState == HTTP_STATE_OK);
	}
}