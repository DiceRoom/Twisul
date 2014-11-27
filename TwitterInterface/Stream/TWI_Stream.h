/****************************************************************************/
/* TWI_Stream.h																*/
/****************************************************************************/
#ifndef TWI_STREAM_H_
#define	TWI_STREAM_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "TWI_StreamDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/

namespace TSUL
{
	//! Stream
	class CStream
	{
	public:
	    /************************************************************************/
		/* maximum length is 60 in each keyword									*/
		/************************************************************************/
		static bool CreateFilterStreamConnection(CStreamConnectionPtr& oConnection , const std::vector<std::string>& iKeywords , const CIDList& iFollowings , const std::string& iKey , const std::string& iSecret);
		static bool CreateFilterStreamConnection(CStreamConnectionPtr& oConnection , const std::vector<std::string>& iKeywords , const CIDList& iFollowings , const SStreamLocationArea* locationArea , const std::string& iKey , const std::string& iSecret);
		static bool CreatePubliIStreamConnection(CStreamConnectionPtr& oConnection , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* Timeline and Event(DM , Favorite , Follow , etc)						*/
		/************************************************************************/
		static bool CreateUserStreamConnection	(CUserStreamConnectionPtr& oConnection , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* still Beta? for Site or Server										*/
		/* maximum user size is 100 in each method								*/
		/* (total maximum size of users is 1000)								*/
		/************************************************************************/
		static bool CreateSiteStreamConnection	(IStreamConnectionPtr& oConnection , const CIDList& iFollowings , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* maximum user size is 100 in each method								*/
		/************************************************************************/
		static bool AddUsersForSiteStream		(const CIDList& iFollowings , const IStreamConnectionPtr& iConnection);

		/************************************************************************/
		/* maximum user size is 100 in each method								*/
		/************************************************************************/
		static bool RemoveUsersForSiteStream	(const CIDList& iFollowings , const IStreamConnectionPtr& iConnection);

		/************************************************************************/
		/* Other																*/
		/************************************************************************/
		static void					SetCertificationFilePath(const std::string& iPath);
		static const std::string&	CertificationFilePath	();

		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		static std::string sCertificationFilePath;
	};
}

#endif