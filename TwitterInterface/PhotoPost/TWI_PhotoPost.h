/****************************************************************************/
/* TWI_PhotoPost.h															*/
/****************************************************************************/
#ifndef TWI_ACCOUNT_H_
#define	TWI_ACCOUNT_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
    
    class IPostProgressInvocation{
    public:
        virtual ~IPostProgressInvocation(){};
        virtual void progress(float progress) = 0;
    };
#ifdef __OBJC__
    class CPostProgressBlocksInvocation : public IPostProgressInvocation{
		void (^block_)(float progress);
    public:
        CPostProgressBlocksInvocation(void(^block)(float progress)){
			block_ = (__bridge void (^)(const float))_Block_copy((const void*)block);
        }
		virtual ~CPostProgressBlocksInvocation(){Block_release(block_);};
        void progress(float progress){block_(progress);};
    };
    
#endif
	//! PostPhoto
	class CPostPhoto
	{
	public:
		/************************************************************************/
		/* twipple																*/
		/************************************************************************/
		static bool PostPhotoTwipple	(std::string& oUrl, const std::string& iFilePath , const char* iBuffer , int iBufferSize, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress = NULL);

		/************************************************************************/
		/* TwitPic																*/
		/************************************************************************/
		static bool PostPhotoTwitPic	(std::string& oUrl, const std::string& iFilePath , const char* iBuffer , int iBufferSize, const std::string &iComment, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress = NULL);
		static bool PostPhotoTwitPic	(std::string& oUrl, const std::string& iFilePath , const char* iBuffer , int iBufferSize, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress = NULL){
            return PostPhotoTwitPic(oUrl, iFilePath, iBuffer, iBufferSize, "", iKey, iSecret, progress);
        }

		/************************************************************************/
		/* Yfrog																*/
		/************************************************************************/
		static bool PostPhotoYfrog		(std::string& oUrl, const std::string& iFilePath , const char* iBuffer , int iBufferSize, const std::string &iKey, const std::string &iSecret, IPostProgressInvocation *progress = NULL);
	};
}

#endif