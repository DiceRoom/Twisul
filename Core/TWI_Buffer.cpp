/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TSUL_StdAfx.h>
#include "TWI_Buffer.h"

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CBufferList CBufferManager::sBufferList;
	boost::mutex CBufferManager::sLockMutex;
}

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	CBufferManager::CBufferPtr CBufferManager::GetBuffer(size_t iSize)
	{
		assert(iSize < BUFFER_SIZE);

		sLockMutex.
		int index = 0;
		for( ; index < BUFFER_VAL ; ++ 
	}
}