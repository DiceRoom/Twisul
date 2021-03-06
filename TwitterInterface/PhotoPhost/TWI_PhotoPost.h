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
	//! PostPhoto
	class CPostPhoto
	{
	public:
		/************************************************************************/
		/* twipple																*/
		/************************************************************************/
		static bool PostPhototwipple	(const std::string& iFilePath , const char* iBuffer , int iBufferSize);

		/************************************************************************/
		/* TwitPic																*/
		/************************************************************************/
		static bool PostPhotoTwitPic	(const std::string& iFilePath , const char* iBuffer , int iBufferSize);

		/************************************************************************/
		/* Yfrog																*/
		/************************************************************************/
		static bool PostPhotoYfrog		(const std::string& iFilePath , const char* iBuffer , int iBufferSize);
	};
}

#endif