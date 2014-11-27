/****************************************************************************/
/* TSULSocket.h																*/
/****************************************************************************/
#ifndef TWITTERCLIENT_SOCKET_H_
#define	TWITTERCLIENT_SOCKET_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <vector>
#include <string>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/

namespace TSUL
{
	class CSocket
	{
	private:
		struct SCore;
		typedef std::vector<std::string> CHeader;
		typedef std::vector<unsigned char> CData;

	public:
		CSocket	();
		CSocket	(CSocket const& iCopy);
		~CSocket();
		static void Initialize();
	
	public:
		void	Socket	(int iAf , int iType, int iProtocol);
		void	Connect	(const char* iName, int iPort);
		void	GetHttp	(CHeader& oHeader, CData& oData, const char* iUri, const CHeader* iHeader, bool iKeepAliveFlg, bool iCompressFlg);
		void	PostHttp(CHeader& oHeader, CData& oData, const char* iUri, const CHeader* iHeader, const char* iContent, size_t iContentLen, const std::string& iContentType ,const std::string& iAuthorization ,bool iKeepAliveFlg);
		void	Close	();
		void	Clear	();
		void	Read	(CData& oBuffer);
		int		Recv	(void* iBufffer, size_t iLen);
		int		Send	(const void* iBufffer, size_t iLen);
		void	Write	(const char* iData, int iDataLen);
		void	Write	(const char* iData);

	private:
		void _Assign			(SCore* iCore);
		bool _ParseHttpResult	(CHeader& oHeader, CData& oData, unsigned char const* iBegin, unsigned char const* iEnd, bool iCompressFlg);
		bool _UncompressGZip	(CData& oData , unsigned char const* iData , int iSize);
		void operator =			(CSocket const& iCopy){ _Assign(iCopy.mCore); }

		/************************************************************************/
		/* Inner Decl                                                           */
		/************************************************************************/
		private:
			SCore* mCore;
	};
}

#endif
