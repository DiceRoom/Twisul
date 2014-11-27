/****************************************************************************/
/* TWI_Blocks.h																*/
/****************************************************************************/
#ifndef TWI_BLOCKS_H_
#define	TWI_BLOCKS_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_Utility.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! Blocks
	class CBlocks
	{
	public:
		/************************************************************************/
		/* CreateBlocks															*/
		/************************************************************************/
		static bool CreateBlocks	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID);

		/************************************************************************/
		/* DeleteBlocks															*/
		/************************************************************************/
		static bool DeleteBlocks	(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID);

		/************************************************************************/
		/* IsBlocks																*/
		/************************************************************************/
		static bool IsBlocks		(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iBlockUserID);

		/************************************************************************/
		/* BlockUserList																*/
		/************************************************************************/
		static bool GetBlockUserList(SResponse& oResponse , CIDList& oBlockUserIDList , const std::string& iKey , const std::string& iSecret);

		/************************************************************************/
		/* ReportSpam															*/
		/************************************************************************/
		static bool ReportSpam		(SResponse& oResponse , const std::string& iKey , const std::string& iSecret , ID iSpamUserID);
	
	private:
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
		static size_t	sLastUpdateTime;
		static CIDList	sBlockUserList;
	};
}

#endif