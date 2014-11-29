/****************************************************************************/
/* CacheWriter.h															*/
/****************************************************************************/
#ifndef TSUL_CACHEWRITER_MODULE_H_
#define	TSUL_CACHEWRITER_MODULE_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <TwitterInterface/TwitterInterface/Cache/TWI_Cache.h>

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//Cache Writer
	class CCacheWriter
	{
	private:
		CCacheWriter(){}
	public:
		CCacheWriter(size_t iBlockSize);

	public:
		/************************************************************************/
		/* Accessor																*/
		/************************************************************************/

		template <typename WriteType> 
		void		WriteMemory				(const WriteType& iVariable);
#ifdef WIN32
		template <> 
		void		WriteMemory				(const std::string& iVariable);
#endif
		size_t		GetCurrentSeek			();
		CCachePtr	GetCachePtr				();

		/************************************************************************/
		/* Inner Decl															*/
		/************************************************************************/
	private:
		size_t		mBlockSize;
		CCachePtr	mCache;
		size_t		mCurrentSeek;
		size_t		mCurrentSize;
	};

	template <typename WriteType> 
	void CCacheWriter::WriteMemory(const WriteType& iVariable)
	{
		size_t val_Size = sizeof(iVariable);
		while(mCurrentSeek + val_Size > mCurrentSize)
		{
			mCurrentSize += mBlockSize;
			mCache -> mHeadAddress = static_cast<char*>(realloc(mCache -> mHeadAddress , mCurrentSize));
		}
		memcpy(mCache -> mHeadAddress + mCurrentSeek , &iVariable , val_Size);
		mCurrentSeek += sizeof(iVariable);
	}
    
    template<>
    void inline CCacheWriter::WriteMemory<std::string>(const std::string& iStrVariable)
    {
        size_t length = iStrVariable.length() + 1;
        while(mCurrentSeek + length > mCurrentSize)
        {
            mCurrentSize += mBlockSize;
            mCache -> mHeadAddress = static_cast<char*>(realloc(mCache -> mHeadAddress , mCurrentSize));
        }
        memcpy(mCache -> mHeadAddress + mCurrentSeek , iStrVariable.c_str() , length);
        mCurrentSeek += length;
    }

}

#endif