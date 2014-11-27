/****************************************************************************/
/* TWI_System.h																*/
/****************************************************************************/
#ifndef TWI_BASE_H_
#define	TWI_BASE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_Utility.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! TwitterInterfaceSystem
	class CSystem
	{
	public:
		/************************************************************************/
		/* Encode/Decode														*/
		/************************************************************************/
		static std::string	Encode				(const std::string& iSource);
		static std::string	Decode				(const std::string& iSource);
		static std::string	TransSpecialChar	(const std::string& iSource);
		
	public:
		/************************************************************************/
		/* Get/Post																*/
		/************************************************************************/
		static void			GetHttp				(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri);
		static void			PostHttp			(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri , const std::string& iContentType = "" , char* iArgBuffer = 0 , int iArgBufferSize = 0);
		
	public:
		/************************************************************************/
		/* BitLy                                                                */
		/************************************************************************/
		static bool			GetShortUrl			(std::string& oUrl , const std::string& iUserName , const std::string& iPass , const std::string& iUrl);
		static bool			GetLongUrl			(std::string& oUrl , const std::string& iUserName , const std::string& iPass , const std::string& iUrl);

	public:
		/************************************************************************/
		/* SystemAccessor														*/
		/************************************************************************/
		static const char*	GetReuestTokenUri	(){return sRequestTokenUri;};
		static const char*	GetAuthorizeUri		(){return sAuthorizeUri;};
		static const char*	GetAccessTokenUri	(){return sAccessTokenUri;};
		static const char*	GetConsumerKey		(){return sConsumerkey;};
		static const char*	GetConsumerSecret	(){return sConsumerSecret;};

	private:
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
		static void			_GetResponseData	(SResponse& oResponse , const std::vector<std::string>& iHeaderList);
		static int			_GetHeaderIndex		(const std::string& iSearchHeaderName , const std::vector<std::string>& iHeaderList);

		/************************************************************************/
		/* Varible																*/
		/************************************************************************/
	private:
		static const char*			sEndPointUri;
		static const char*			sRequestTokenUri;
		static const char*			sAuthorizeUri;
		static const char*			sAccessTokenUri;
		static const char*			sConsumerkey;
		static const char*			sConsumerSecret;


		//API ver 1.0 ONLY-----------------------------------------------------
	public:
		static void			GetHttpOld			(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , const std::string& iUri);
		static void			SetRateLimitAPIFlg	(bool iFlg) { sRateLimitGetAPIFlg = iFlg; };
        static SRateLimit	GetLimitRate		(const std::string& iKey , const std::string& iSecret);
        
	private:
		static bool			sRateLimitGetAPIFlg;
		static SRateLimit	sOldRateLimit;

	public:
        static void MakeOAuthURL	(std::string &oUrl, const std::string &iUrl, const std::string& iKey , const std::string& iSecret);
	};
}

#endif