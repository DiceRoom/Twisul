/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_PhotoPost.h"


/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	/************************************************************************/
	/* twipple																*/
	/************************************************************************/
	
	bool CPostPhoto::PostPhototwipple(const std::string& iFileName , const char* iBuffer , int iBufferSize)
	{
		iFileName , iBuffer , iBufferSize;
        return false;
	}

	/************************************************************************/
	/* TwitPic																*/
	/************************************************************************/
	
	bool CPostPhoto::PostPhotoTwitPic(const std::string& iFileName , const char* iBuffer , int iBufferSize)
	{
		//api key
		//-> 7c40a58dcb70796f42feac027c17d556
		iFileName , iBuffer , iBufferSize;
        return false;
	}

	/************************************************************************/
	/* Yfrog																*/
	/************************************************************************/
	
	bool CPostPhoto::PostPhotoYfrog(const std::string& iFileName , const char* iBuffer , int iBufferSize)
	{
		iFileName , iBuffer , iBufferSize;
        return false;
	}
}