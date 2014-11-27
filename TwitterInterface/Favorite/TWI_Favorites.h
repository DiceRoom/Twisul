/****************************************************************************/
/* TWI_Favorites.h															*/
/****************************************************************************/
#ifndef TWI_FAVORITES_H_
#define	TWI_FAVORITES_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Status/TWI_StatusDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Favorites
	class CFavorites
	{
	public:
		/************************************************************************/
		/* Favorites						                                    */
		/* # UseGetArgs #														*/
		/* -> mCount , mSinceID , mMaxID										*/
		/************************************************************************/
		static bool GetFavorites		(SResponse& oResponse , CStatusList& oFavoriteStatusList , const std::string& iKey , const std::string& iSecret , ID iUserID = UNKNOWN_ID , const SGetArgs* iArgs = 0);

		/************************************************************************/
		/* CreateFavorites						                                */
		/************************************************************************/
		static bool CreateFavorites		(SResponse& oResponse , CStatusPtr& oFavoriteStatus , const std::string& iKey , const std::string& iSecret , ID iStatusID);

		/************************************************************************/
		/* DeleteFavorites														*/
		/************************************************************************/
		static bool DeleteFavorites		(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iStatusID);
	};
}

#endif