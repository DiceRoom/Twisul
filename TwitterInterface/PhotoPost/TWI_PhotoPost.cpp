/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_PhotoPost.h"
#include "TWI_System.h"
#include "../../../liboauth/oauth.h"
#ifdef WIN32
#include "../../../curl/curl.h"
#else
#include <curl/curl.h>
#endif
#include <boost/regex.hpp>

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
    
    static int PhotoPostProgressCallback(void *clientp,
                               double dltotal,
                               double dlnow,
                               double ultotal,
                               double ulnow){
        IPostProgressInvocation *progress = (IPostProgressInvocation *)clientp;
        if(ultotal && progress)    progress->progress(ulnow/ultotal);
        return 0;
    }
    
    
    static std::string Replace( std::string String1, std::string String2, std::string String3 )
    {
        std::string::size_type  Pos( String1.find( String2 ) );
        
        while( Pos != std::string::npos )
        {
            String1.replace( Pos, String2.length(), String3 );
            Pos = String1.find( String2, Pos + String3.length() );
        }
        
        return String1;
    }
    
    static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
        std::string *core = (std::string *)userp;
        *core += (const char *)buffer;
        int segsize = size * nmemb;	
        return segsize;
    }
    
    static struct curl_slist *makeHeaderListForOauthEcho(const std::string &iKey, const std::string &iSecret){
        std::string authorizationUrl;
        authorizationUrl = oauth_sign_url2("https://api.twitter.com/1.1/account/verify_credentials.json", 0, OA_HMAC, 0, CSystem::GetConsumerKey(), CSystem::GetConsumerSecret(), iKey.c_str(), iSecret.c_str());
        authorizationUrl = Replace(authorizationUrl,"https://api.twitter.com/1.1/account/verify_credentials.json?", "");
        authorizationUrl = Replace(authorizationUrl, "=", "=\"");
        authorizationUrl = Replace(authorizationUrl, "&", "\",");
        authorizationUrl = ("OAuth realm=\"http%3A%2F%2Fapi.twitter.com%2F\"," + authorizationUrl + "\"");

        
        struct curl_slist *headerlist = NULL;
        headerlist = curl_slist_append(headerlist, "X-Auth-Service-Provider: https://api.twitter.com/1.1/account/verify_credentials.json");
        headerlist = curl_slist_append(headerlist, ("X-Verify-Credentials-Authorization: "+authorizationUrl).c_str());
        return headerlist;
    }
    
    static CURL *prepareCURL(){
        CURL *curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data );
        return curl;
    }

	/************************************************************************/
	/* twipple																*/
	/************************************************************************/
	
    
    bool CPostPhoto::PostPhotoTwipple(std::string& oUrl, const std::string& iFileName , const char* iBuffer , int iBufferSize, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress)
	{
        /*
        -success
         <?xml version="1.0" encoding="UTF-8"?>
         <rsp stat="ok">
         <mediaid>oz66d</mediaid>
         <mediaurl>http://p.twipple.jp/oz66d</mediaurl>
         </rsp>
         
        -error
         <?xml version="1.0" encoding="UTF-8"?>
         <rsp stat="fail">
         <err code="1001" msg="Invalid twitter username or password" />
         </rsp>
         
         */
        
        
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        
        curl_formadd(&formpost, 
                     &lastptr,
                     CURLFORM_COPYNAME, "media",
                     CURLFORM_FILENAME, iFileName.c_str(),
                     CURLFORM_BUFFER, "data",
                     CURLFORM_BUFFERPTR, iBuffer,
                     CURLFORM_BUFFERLENGTH, iBufferSize,
                     CURLFORM_END);
        
        struct curl_slist *headerlist = makeHeaderListForOauthEcho(iKey, iSecret);
        CURL *curl = prepareCURL();

        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, PhotoPostProgressCallback);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_URL, "http://p.twipple.jp/api/upload2");
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        std::string data;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data );
        
        curl_easy_perform(curl);
        
        curl_slist_free_all(headerlist);
        curl_formfree(formpost);
        curl_easy_cleanup(curl);
        if(progress)    delete progress;

        boost::regex r("<rsp stat=\"(.*?)\">");
        boost::smatch m;
        
        if(boost::regex_search(data, m, r)){            
            std::string stat = m.str(1);
            if(stat != "ok")  return false;
        }
        
        r = boost::regex("<mediaurl>(.*?)</mediaurl>");
        if(boost::regex_search(data, m, r)){            
            oUrl = m.str(1);
        }
        return true;
	}

	/************************************************************************/
	/* TwitPic																*/
	/************************************************************************/

	bool CPostPhoto::PostPhotoTwitPic(std::string& oUrl, const std::string& iFileName , const char* iBuffer , int iBufferSize, const std::string &iComment, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress)
	{
        /*
         
         -success
         {"id":"b3pnv8","text":"","url":"http:\/\/twitpic.com\/b3pnv8","width":"48","height":"48","size":"1791","type":"png","timestamp":"Sat, 13 Oct 2012 11:13:48 +0000","user":{"id":754979576,"screen_name":"DiceRoomLabo"}}
         
         -error
         {"errors":[{"code":401,"message":"Could not authenticate you (header rejected by twitter)."}]}
         

         
         */
        
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        
        curl_formadd(&formpost, 
                     &lastptr,
                     CURLFORM_COPYNAME, "key",
                     CURLFORM_COPYCONTENTS, "7c40a58dcb70796f42feac027c17d556",
                     CURLFORM_END);
        
        curl_formadd(&formpost, 
                     &lastptr,
                     CURLFORM_COPYNAME, "message",
                     CURLFORM_COPYCONTENTS, iComment.c_str(),
                     CURLFORM_END);
        
        curl_formadd(&formpost, 
                     &lastptr,
                     CURLFORM_COPYNAME, "media",
                     CURLFORM_FILENAME, iFileName.c_str(),
                     CURLFORM_BUFFER, "data",
                     CURLFORM_BUFFERPTR, iBuffer,
                     CURLFORM_BUFFERLENGTH, iBufferSize,
                     CURLFORM_END);
        
        struct curl_slist *headerlist = makeHeaderListForOauthEcho(iKey, iSecret);
        CURL *curl = prepareCURL();
        
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, PhotoPostProgressCallback);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.twitpic.com/2/upload.json");
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        
        std::string data;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data );
        curl_easy_perform(curl);
        
        curl_slist_free_all(headerlist);
        curl_formfree(formpost);
        curl_easy_cleanup(curl);
        if(progress)    delete progress;
        
        boost::regex r;
        boost::smatch m;
        
        r = ("\"errors\":\\[");        
        if(boost::regex_search(data, m, r)){ 
            return false;
        }
        
        r = boost::regex("\"url\":\"(.*?)\"");
        if(boost::regex_search(data, m, r)){
            oUrl = Replace(m.str(1), "\\/", "/");
        }
        return true;
	}

	/************************************************************************/
	/* Yfrog																*/
	/************************************************************************/
	
	bool CPostPhoto::PostPhotoYfrog(std::string& oUrl, const std::string& iFileName , const char* iBuffer , int iBufferSize, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress)
	{
        
        /*
         
         * Connection #0 to host yfrog.com left intact
         {"rsp":{"stat":"ok","mediaid":"nzxqrfp","mediaurl":"http:\/\/yfrog.com\/nzxqrfp"}}
         0
         {"rsp":{"stat":"ok","mediaid":"h0n2ezp","mediaurl":"http:\/\/yfrog.com\/h0n2ezp"}}
         0
         
         ???
         <?xml version="1.0" encoding="UTF-8"?>
         <rsp stat="ok">
         <mediaid>mnomuip</mediaid>
         <mediaurl>http://yfrog.com/mnomuip</mediaurl>
         </rsp>
         */
        
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        
        curl_formadd(&formpost, 
                     &lastptr,
                     CURLFORM_COPYNAME, "media",
                     CURLFORM_FILENAME, iFileName.c_str(),
                     CURLFORM_BUFFER, "data",
                     CURLFORM_BUFFERPTR, iBuffer,
                     CURLFORM_BUFFERLENGTH, iBufferSize,
                     CURLFORM_END);
        
        struct curl_slist *headerlist = makeHeaderListForOauthEcho(iKey, iSecret);
        CURL *curl = prepareCURL();
        
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, PhotoPostProgressCallback);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_URL, "http://yfrog.com/api/xauth_upload");
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        
        std::string data;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data );
        
        curl_easy_perform(curl);
 
        curl_slist_free_all(headerlist);
        curl_formfree(formpost);
        curl_easy_cleanup(curl);
        if(progress)    delete progress;

        boost::regex r("\"stat\":\"(.*?)\"");
        boost::smatch m;
        
        if(boost::regex_search(data, m, r)){ 
            std::string stat = m.str(1);
            if(stat != "ok")  return false;
        }
        
        r = boost::regex("\"mediaurl\":\"(.*?)\"");
        if(boost::regex_search(data, m, r)){
            oUrl = Replace(m.str(1), "\\/", "/");
        }

        return true;
	}
}