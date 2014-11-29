/****************************************************************************/
/* CacheReader.h															*/
/****************************************************************************/
#ifndef TSUL_CACHEREADER_MODULE_H_
#define	TSUL_CACHEREADER_MODULE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TwitterInterface/TwitterInterface/Cache/TWI_Cache.h>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//Cache Reader
	class CCacheReader
	{
	private:
		CCacheReader(){}
	public:
		CCacheReader(const char* iBuffer);

	public:
		/************************************************************************/
		/* Accessor																*/
		/************************************************************************/
		template <typename ReadType>
		void ReadMemory(ReadType& oVariable)
        {
            size_t val_Size = sizeof(oVariable);
            memcpy(&oVariable , mBuffer + mCurrentSeek , val_Size);
            mCurrentSeek += val_Size;
        }
#ifdef WIN32
		template <>
		void ReadMemory				(std::string& oVariable);
#endif
		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		const char*	mBuffer;
		size_t		mCurrentSeek;
	};

    template <>
    void inline CCacheReader::ReadMemory(std::string& oString)
    {
        oString = mBuffer + mCurrentSeek;
        mCurrentSeek += (oString.length() + 1);
    }
}

#endif