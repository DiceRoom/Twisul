/****************************************************************************/
/* TWI_StreamDef.h															*/
/****************************************************************************/
#ifndef TWI_STREAM_DEF_H_
#define	TWI_STREAM_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../Status/TWI_StatusDef.h"
#include "TWI_UserStreamDef.h"
#include <curl/curl.h>
#include <boost/thread.hpp>
/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
    
	//SStreamLocationArea
    struct SStreamLocationArea
	{
        double mLongitudeSouthWest;
        double mLatitudeSouthWest;
        double mLongitudeNorthEast;
        double mLatitudeNorthEast;
    };

	//virtual class for notification by streaming
    class IUserStreamInvocation
	{
	public:
		IUserStreamInvocation			(){};
		virtual ~IUserStreamInvocation	(){};
		virtual void Call               (const CUserStreamEventList& iEvent) = 0;
	};
    
	class IStreamInvocation
	{
	public:
		IStreamInvocation			(){};
		virtual ~IStreamInvocation	(){};
		virtual void Call			(const CStatusList& iStatus) = 0;
	};

	//for C++ class
	template<typename Type>
	class CStraemClassInvocation : public IStreamInvocation
	{
	public:
		CStraemClassInvocation(Type* target, void (Type::*method)(const CStatusList& iStatus))
			:mTarget(target)
			,mMethod(method)
		{}
		virtual ~CStraemClassInvocation	(){};
		virtual void Call(const CStatusList& iStatus)
		{ 
			mTarget->*mMethod(iStatus);
		}

		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		Type* mTarget;
		void (Type::*mMethod)(const CStatusList& iStatus);
	};

#ifdef __OBJC__
	class CStreamBlocksInvocation : public IStreamInvocation
	{
		void (^block_)(const CStatusList& iStatus);
	public:
		CStreamBlocksInvocation(void (^b)(const CStatusList& iStatus))
		{
			block_ = (__bridge void (^)(const CStatusList& iStatus))_Block_copy((const void*)b);
		};
		virtual void Call(const CStatusList& iStatus){ block_(iStatus);};
		virtual ~CStreamBlocksInvocation(){Block_release(block_);};
	};

	class CUserStreamBlocksInvocation : public IUserStreamInvocation
	{
		void (^block_)(const CUserStreamEventList& iStatus);
	public:
		CUserStreamBlocksInvocation(void (^b)(const CUserStreamEventList& iStatus))
		{
			block_ = (__bridge void (^)(const CUserStreamEventList& iStatus))_Block_copy((const void*)b);
		};
		virtual void Call(const CUserStreamEventList& iStatus){ block_(iStatus);};
		virtual ~CUserStreamBlocksInvocation(){Block_release(block_);};
	};
#endif

    class IStreamConnection
	{
	public:
		IStreamConnection				(CURL* curl, const char* postData = 0);
		virtual ~IStreamConnection		();
		int         Connect				();
		void		Kill				();
        bool        IsConnecting        () const;
		bool		Killed				() const;
		CURL*		Curl				();
		const char* ResponseData		();
        void        WaitUntilDone       ();
		void		ClearResponseData	();
		virtual void	AddResponseData		(const char* iData , int iSize);
        bool        UseCache            () const{return mUseCache;};
        void        SetUseCache         (bool useCache){mUseCache = useCache;};
        
	private:
		static size_t _StreamWriteData(void* iBuffer , size_t iSize , size_t iNmemb, void* iUserp);
        
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	protected:
		mutable boost::mutex	mMutex;
		std::string				mResponseData;
		CURL*					mCurl;
		std::string				mPostData;
        bool                    mConnecting;
		bool					mKilled;
        bool                    mUseCache;
	};

	class CStreamConnection : public IStreamConnection
	{
	public:
		CStreamConnection				(CURL* curl, const char* postData = 0);
		virtual ~CStreamConnection		();
		void		SetInvocation		(IStreamInvocation* iInvocation){mInvocation = iInvocation;};
		void		AddResponseData		(const char* iData , int iSize);
        
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		IStreamInvocation*		mInvocation;
	};
    
	class CUserStreamConnection : public IStreamConnection
	{
	public:
		CUserStreamConnection			(CURL* curl, const char* postData = 0);
		virtual ~CUserStreamConnection	();
		void SetInvocation              (IUserStreamInvocation* iInvocation){mInvocation = iInvocation;};
		void		AddResponseData		(const char* iData , int iSize);
        
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		IUserStreamInvocation*		mInvocation;
	};
    
	typedef boost::shared_ptr<IStreamConnection>	 IStreamConnectionPtr;
	typedef boost::shared_ptr<CStreamConnection>	 CStreamConnectionPtr;
	typedef boost::shared_ptr<CUserStreamConnection> CUserStreamConnectionPtr;
}

#endif	//TWI_USER_DEF_H_