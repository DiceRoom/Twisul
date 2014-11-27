/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
#include <winsock.h>
#include <windows.h>
#include <libzlib/include/zlib.h>
#include <libzlib/include/zconf.h>
#else

#include <zlib.h>
#include <unistd.h>
#include <sys/Socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#endif

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace
{
	static const int GZIP_INBUF_SIZE	= 4096;
	static const int GZIP_OUTBUF_SIZE	= 4096;
}

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace
{
#define USER_AGENT "DiceRoomLabo.jp"
#ifdef WIN32
	typedef SOCKET Socket_t;
#else
	typedef int Socket_t;
	#define INVALID_SOCKET (-1)
	#define CloseSocket(FD) Close(FD)
#endif

	struct SHttpRequestInfo
	{
		SHttpRequestInfo():mPort(0){}
		std::string	mName;
		int		mPort;
		std::string	mQuery;
	};
}

namespace TSUL
{
	struct CSocket::SCore
	{
		SCore()	: mRef(0) , mSocket(INVALID_SOCKET){}
		unsigned int		mRef;
		Socket_t	mSocket;
	};
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace
{
	SHttpRequestInfo ParseHttp(const char* iUri)
	{
		if(strncmp(iUri, "http://", 7) != 0) {
			return SHttpRequestInfo();
		}

		SHttpRequestInfo info;

		const char* p;
		p = strchr(iUri + 7, '/');
		if(p != 0)
		{
			info.mQuery = p;
		}
		else
		{
			p = iUri + strlen(iUri);
			info.mQuery = "/";
		}
		info.mName = std::string(iUri + 7, p);
		info.mPort = 80;
		p = strchr(info.mName.c_str(), ':');
		if(p != 0)
		{
			info.mPort = atoi(p + 1);
			info.mName = std::string(info.mName.c_str(), p);
		}
		return info;
	}

	static inline std::string to_s(size_t n)
	{
		char tmp[100];
		sprintf(tmp, "%u", n);
		return tmp;
	}
}

namespace TSUL
{
	CSocket::CSocket()
		:mCore(0)
	{
		_Assign(new SCore());
	}

	CSocket::CSocket(CSocket const& iCopy)
		:mCore(0)
	{
		iCopy;
		_Assign(iCopy.mCore);
	}

	CSocket::~CSocket()
	{
		_Assign(0);
	}

	void CSocket::Initialize()
	{
#ifdef WIN32
		WSADATA wsaData;
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(1,1);
		WSAStartup(wVersionRequested, &wsaData);
		atexit((void (*)(void))(WSACleanup));
#endif
	}

	void CSocket::_Assign(SCore* iCore)
	{
		if(iCore != 0)
		{
			++ iCore -> mRef;
		}

		if(mCore != 0)
		{
			if(mCore -> mRef > 1)
			{
				-- mCore -> mRef;
			}
			else
			{
				delete mCore;
			}
		}
		mCore = iCore;
	}

	bool CSocket::_ParseHttpResult(CHeader& oHeader, CData& oData, unsigned char const* iBegin, unsigned char const* iEnd, bool iCompressFlg)
	{
		CData get_Data;
		if(iBegin < iEnd)
		{
			unsigned char const* left = iBegin;
			unsigned char const* right = left;
			while(true)
			{
				if(right >= iEnd)
				{
					break;
				}
				if(*right == '\r' || *right == '\n')
				{
					if(left < right)
					{
						oHeader.push_back(std::string(left, right));
					}
					if(right + 1 < iEnd&&* right == '\r' && right[1] == '\n')
					{
						++ right;
					}
					++ right;

					if(*right == '\r' || *right == '\n')
					{
						if(right + 1 < iEnd&& *right == '\r' && right[1] == '\n')
						{
							++ right;
						}
						++ right;
						left = right;
						break;
					}
					left = right;
				}
				else
				{
					++ right;
				}
			}
			if(left < iEnd)
			{
				//uncompress
				oData.clear();

				if(iCompressFlg)
				{
					return _UncompressGZip(oData , left , reinterpret_cast<int>(iEnd) - reinterpret_cast<int>(left));
				}
				else
				{
					oData.assign(left , left + reinterpret_cast<int>(iEnd) - reinterpret_cast<int>(left));
					oData.push_back('\0');
				}
			}
		}

		return false;
	}

	bool CSocket::_UncompressGZip(CData& oData , unsigned char const* iData , int iSize)
	{
		//initialize
		z_stream z_Stream;
		z_Stream.zalloc		= Z_NULL;
		z_Stream.zfree		= Z_NULL;
		z_Stream.opaque		= Z_NULL;
		z_Stream.next_in	= Z_NULL;
		if(inflateInit2(&z_Stream , 16 + MAX_WBITS) != Z_OK)
		{
			return false;
		}

		unsigned char outbuf[GZIP_OUTBUF_SIZE];
		unsigned char inbuf[GZIP_INBUF_SIZE];

		z_Stream.avail_in	= 0;
		z_Stream.next_out	= outbuf;
		z_Stream.avail_out	= GZIP_OUTBUF_SIZE;

		int lest = iSize;
		int status = Z_OK;
		unsigned char* uncmp_Data = NULL;
		unsigned char* outp;
		int uncmp_Size = 0;
		while(status != Z_STREAM_END)
		{
			if(z_Stream.avail_in==0)
			{
				z_Stream.next_in = inbuf;
				if(lest > GZIP_INBUF_SIZE)
				{
					memcpy(inbuf , iData , GZIP_INBUF_SIZE);
					z_Stream.avail_in = GZIP_INBUF_SIZE;
					iData += GZIP_INBUF_SIZE;
					lest -= GZIP_INBUF_SIZE;
				}
				else
				{
					memcpy(inbuf , iData , lest);
					z_Stream.avail_in = lest;
					iData += lest;
					lest -= lest;
				}
			}

			//check
			status = inflate(&z_Stream , Z_NO_FLUSH);
			if(status == Z_STREAM_END)
			{
				break;
			}
			else if(status != Z_OK)
			{
				inflateEnd(&z_Stream);
				return false;
			}

			if(z_Stream.avail_out==0)
			{
				if(uncmp_Data == NULL)
				{
					uncmp_Data= static_cast<unsigned char *>(malloc(GZIP_OUTBUF_SIZE));
					outp = uncmp_Data;
					uncmp_Size = GZIP_OUTBUF_SIZE;
				}
				else
				{
					uncmp_Data = static_cast<unsigned char *>(realloc(uncmp_Data , uncmp_Size + GZIP_OUTBUF_SIZE));
					outp = uncmp_Data+uncmp_Size;
					uncmp_Size += GZIP_OUTBUF_SIZE;
				}
				memcpy(outp , outbuf , GZIP_OUTBUF_SIZE);
				z_Stream.next_out = outbuf;
				z_Stream.avail_out = GZIP_OUTBUF_SIZE;
			}
		}

		int count = GZIP_OUTBUF_SIZE - z_Stream.avail_out;
		if(count != 0)
		{
			if(uncmp_Data == NULL)
			{
				uncmp_Data = static_cast<unsigned char *>(malloc(count));
				outp=uncmp_Data;
				uncmp_Size=count;
			}
			else
			{
				uncmp_Data = static_cast<unsigned char *>(realloc(uncmp_Data , uncmp_Size + count));
				outp = uncmp_Data+uncmp_Size;
				uncmp_Size += count;
			}
			memcpy(outp , outbuf , count);
		}

		if(inflateEnd(&z_Stream) != Z_OK)
		{
			return false;
		}

		//insert
		oData.assign(uncmp_Data , uncmp_Data + uncmp_Size);
		oData.push_back('\0');
		return true;
	}

	void CSocket::Socket(int iAf, int iType, int iProtocol)
	{
		_Assign(new SCore());
		mCore -> mSocket = ::socket(iAf, iType, iProtocol);
	}

	void CSocket::Connect(const char* iName, int iPort)
	{
		struct hostent* host;
		struct sockaddr_in addr;
		host = gethostbyname(iName);
        if(!host)   return;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, host -> h_addr, host -> h_length);
		addr.sin_port = htons(iPort);
		::connect(mCore -> mSocket, (struct sockaddr* )&addr, sizeof(sockaddr_in));
	}

	void CSocket::GetHttp(CHeader& oHeader, CData& oData, const char* iUri, const CHeader* iHeader, bool iKeepAliveFlg, bool iCompressFlg)
	{
		oHeader.clear();
		oData.clear();

		if(strncmp(iUri, "http://", 7) != 0)
		{
			return;
		}

		SHttpRequestInfo http = ParseHttp(iUri);
		Socket(AF_INET, SOCK_STREAM, 0);
		Connect(http.mName.c_str(), http.mPort);

		std::string req;
		req = "GET ";
		req += http.mQuery;
		req += " HTTP/1.1\r\n";

		CHeader preheader;
		preheader.push_back(std::string("User-Agent: ") + USER_AGENT);
		preheader.push_back("Host: " + http.mName);
		preheader.push_back("Accept:* /*");

		//compress
		if(iCompressFlg)
		{
			preheader.push_back("Accept-Encoding: gzip, default");
		}

		if(!iKeepAliveFlg)
		{
			preheader.push_back("Connection: Close");
		}

        CHeader::const_iterator itr = preheader.begin();
        CHeader::const_iterator lst = preheader.end();
		for(; itr != lst; ++itr)
		{
			req += (*itr);
			req += "\r\n";
		}

		if(iHeader != 0)
		{
            CHeader::const_iterator itr = (*iHeader).begin();
            CHeader::const_iterator lst = (*iHeader).end();
            for(; itr != lst; ++itr)
			{
				req += (*itr);
				req += "\r\n";
			}
		}
		req += "\r\n";

		Write(req.c_str(), req.size());

		CData data;
		Read(data);

		if(!data.empty())
		{
			unsigned char const* begin = &data[0];
			unsigned char const* end = begin + data.size();
			if(!_ParseHttpResult(oHeader , oData , begin , end, iCompressFlg))
			{
				GetHttp(oHeader , oData , iUri , iHeader , iKeepAliveFlg, false);
			}
		}
	}

    std::string Replace(std::string String1 , std::string String2 , std::string String3)
    {
        std::string::size_type  Pos( String1.find( String2 ) );
        
        while( Pos != std::string::npos )
        {
            String1.replace( Pos, String2.length(), String3 );
            Pos = String1.find( String2, Pos + String3.length() );
        }
        
        return String1;
    }

	void CSocket::PostHttp(CHeader& oHeader, CData& oData, const char* iUri, const CHeader* iHeader, const char* iContent, size_t iContentLen, const std::string& iContentType ,const std::string& iAuthorization ,bool iKeepAliveFlg)
	{
		oHeader.clear();
		oData.clear();

		if(strncmp(iUri, "http://", 7) != 0)
		{
			return;
		}

		SHttpRequestInfo http = ParseHttp(iUri);

		Socket(AF_INET, SOCK_STREAM, 0);
		Connect(http.mName.c_str(), http.mPort);

		std::string req;
		req = "POST ";
		req += http.mQuery;
		req += " HTTP/1.1\r\n";

		CHeader preheader;
		preheader.push_back(std::string("User-Agent: ") + USER_AGENT);
		preheader.push_back("Host: " + http.mName);
		preheader.push_back("Accept:* /*");

		//preheader.push_back("Accept-Encoding: gzip, default");

        if(iAuthorization.size()){
            std::string buf = Replace(iAuthorization, "=", "=\"");
            buf = Replace(buf, "&", "\",");
            preheader.push_back("Authorization: OAuth realm=\"http://twitter.com/\"," + buf + "\"");
        }
		preheader.push_back("Content-Length: " + to_s(iContentLen));
		preheader.push_back("Content-Type: " + iContentType);

		if(!iKeepAliveFlg)
		{
			preheader.push_back("Connection: Close");
		}

        CHeader::const_iterator itr = preheader.begin();
        CHeader::const_iterator lst = preheader.end();
		for(; itr != lst; ++itr)
		{
			req += (*itr);
			req += "\r\n";
		}

		if(iHeader != 0)
		{
            CHeader::const_iterator itr = (*iHeader).begin();
            CHeader::const_iterator lst = (*iHeader).end();
			for(; itr != lst; ++itr)
			{
				req += (*itr);
				req += "\r\n";
			}
		}
		req += "\r\n";

//        printf("%s", req.c_str());
		Write(req.c_str(), req.size());
//        printf("%s", iContent);;
		Write(iContent, iContentLen);

		//
		CData data;
		Read(data);

		if(!data.empty())
		{
			unsigned char const* begin = &data[0];
			unsigned char const* end = begin + data.size();
			_ParseHttpResult(oHeader , oData , begin , end , false);
		}
	}

	void CSocket::Close()
	{
#ifdef WIN32
		::closesocket(mCore -> mSocket);
#else
        close(mCore -> mSocket);
#endif
	}

	void CSocket::Clear()
	{
		mCore -> mSocket = INVALID_SOCKET;
	}

	void CSocket::Read(CData& oBuffer)
	{
		oBuffer.clear();

		char tmp[4096];
		while (1)
		{
			int n = Recv(tmp, sizeof(tmp));
			if(n < 1)
			{
				break;
			}
			oBuffer.insert(oBuffer.end(), tmp, tmp + n);
		}
	}

	int CSocket::Recv(void* buf, size_t iLen)
	{
		return ::recv(mCore -> mSocket, static_cast<char*>(buf), iLen, 0);
	}

	void CSocket::Write(const char* iData, int iDataLen)
	{
		while (iDataLen > 0)
		{
			int n = Send(iData, iDataLen);
			if(n < 1 || n > iDataLen)
			{
				break;
			}
			iDataLen -= n;
			iData += n;
		}
	}

	void CSocket::Write(const char* iData)
	{
		Write(iData, strlen(iData));
	}

	int CSocket::Send(const void* iBuffer, size_t iLen)
	{
		return ::send(mCore -> mSocket, static_cast<const char*>(iBuffer), iLen, 0);
	}
}