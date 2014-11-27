/****************************************************************************/
/* TWI_UserStreamDef.h														*/
/****************************************************************************/
#ifndef TWI_USERSTREAM_DEF_H_
#define	TWI_USERSTREAM_DEF_H_

/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include "../Common/TWI_CommonDef.h"
#include "../User/TWI_UserDef.h"
#include "../Status/TWI_StatusDef.h"
#include "../Lists/TWI_ListsDef.h"

/****************************************************************************/
////////////////////////////// Type Declaration //////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! USES_STATUS_DELETE , USES_DM_DELETE
	struct SUsesDelete
	{
		ID mID;
		ID mUserID;
	};

	//! USES_FAVORITE_CREATE , USES_FAVORITE_DELETE
	struct SUsesFavorite
	{
		CUserPtr	mFavUser;
		CStatusPtr	mFavTargetStatus;
	};

	//! USES_FOLLOW
	struct SUsesFollow
	{
		CUserPtr mSourceUser;
		CUserPtr mTargetUser;
	};

	//! USES_LIST_MEMBER_ADD , USES_LIST_MEMBER_DELETE
	struct SUsesListMember
	{
		CUserPtr mSourceUser;
		CUserPtr mTargetUser;
		CListPtr mTargetList;
	};

	//! USES_LIST_FOLLOW_CREATE , USES_LIST_FOLLOW_DELETE
	struct SUsesListFollow
	{
		CUserPtr mSourceUser;
		CListPtr mTargetList;
	};

	//UserStreamEventState
	enum EUserStreamEventState
	{
		/************************************************************************/
		/* Status																*/
		/************************************************************************/
		USES_STATUS_CREATE,			//< Cast -> CStatusPtr
		USES_STATUS_DELETE,			//< Cast -> SUsesDelete

		/************************************************************************/
		/* Favorite																*/
		/************************************************************************/
		USES_FAVORITE_CREATE,		//< Cast -> CUsesFavoritePair
		USES_FAVORITE_DELETE,		//< Cast -> CUsesFavoritePair
		
		/************************************************************************/
		/* Direct Message														*/
		/************************************************************************/
		USES_DM_CREATE,				//< Cast -> CDirectMessagePtr
		USES_DM_DELETE,				//< Cast -> SUsesDelete

		/************************************************************************/
		/* Follow																*/
		/************************************************************************/
		USES_FOLLOW,				//< Cast -> SUsesFollow

		/************************************************************************/
		/* Lists																*/
		/************************************************************************/
		USES_LIST_CREATE,			//< Cast -> CListPtr
		USES_LIST_UPDATE,			//< Cast -> CListPtr
		USES_LIST_DELETE,			//< Cast -> CListPtr
		USES_LIST_MEMBER_ADD,		//< Cast -> SUsesListMember
		USES_LIST_MEMBER_DELETE,	//< Cast -> SUsesListMember
		USES_LIST_FOLLOW_CREATE,	//< Cast -> SUsesListFollow
		USES_LIST_FOLLOW_DELETE,	//< Cast -> SUsesListFollow

		/************************************************************************/
		/* Other																*/
		/************************************************************************/
		USES_PROFILE,				//< Cast -> CUserPtr
	};

    template <typename CastType> class TUserStreamEvent;

	//! EventBase
	class IEventBase
	{
	public:
		template<typename CastType>
		void GetVariable(CastType& oVariable)
		{
			static_cast< TUserStreamEvent<CastType>* >(this) -> GetEventVariable(oVariable);
		}
	};
	
	//UserStreamEvent
	struct SUserStreamEvent
	{
		EUserStreamEventState	mEventState;
		IEventBase*				mEvent;
	};
	typedef boost::shared_ptr<SUserStreamEvent> CUserStreamEvent;
	typedef std::vector<CUserStreamEvent>		CUserStreamEventList;

	//! Event
	template <typename CastType>
	class TUserStreamEvent : public IEventBase
	{
	public:
		TUserStreamEvent		(CastType oVariable) : mEventVariable(oVariable) {}
		void GetEventVariable	(CastType& oVariable) const {oVariable = mEventVariable;};

	private:
		CastType	mEventVariable;
	};

	//main func
	void GetUserStreamEvent(CUserStreamEvent& oEvent , CXmlNode* iNode);
}

#endif	//TWI_USERSTREAM_DEF_H_