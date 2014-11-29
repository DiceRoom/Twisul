/****************************************************************************/
////////////////////////////////// include ///////////////////////////////////
/****************************************************************************/
#include <curl/curl.h>

#include <TwitterInterface/Core/TWI_Socket.h>
#include <fstream>
#include <TwitterInterface/TwitterInterface/System/TWI_System.h>
#include <TwitterInterface/TwitterInterface/Cache/TWI_Cache.h>
#include <TwitterInterface/TwitterInterface/Account/TWI_Account.h>
#include <TwitterInterface/TwitterInterface/Status/TWI_Status.h>
#include <TwitterInterface/TwitterInterface/DirectMessage/TWI_DirectMessage.h>
#include <TwitterInterface/TwitterInterface/TimeLine/TWI_TimeLine.h>
#include <TwitterInterface/TwitterInterface/Lists/TWI_Lists.h>
#include <TwitterInterface/TwitterInterface/Favorite/TWI_Favorites.h>
#include <TwitterInterface/TwitterInterface/User/TWI_User.h>
#include <TwitterInterface/TwitterInterface/Block/TWI_Blocks.h>
#include <TwitterInterface/TwitterInterface/Trends/TWI_Trends.h>
#include <TwitterInterface/TwitterInterface/Search/TWI_Search.h>
#include <TwitterInterface/TwitterInterface/Stream/TWI_Stream.h>
#include <TwitterInterface/TwitterInterface/Geo/TWI_Geo.h>

#include <Module/DMDetailsModule.h>
#include <Module/FriendCheckModule.h>
#include <Module/NarrowingIDModule.h>

/****************************************************************************/
///////////////////////////// Static Declaration /////////////////////////////
/****************************************************************************/
namespace TSUL
{
	//! �A�J�E���g���
	struct STestAccountInfo
	{
		STestAccountInfo(const std::string& iScreenName , ID iID , const std::string& iKey , const std::string& iSecret)
			:mScreenName(iScreenName)
			,mID(iID)
			,mKey(iKey)
			,mSecret(iSecret)
		{}
		
		std::string mScreenName;
		ID			mID;
		std::string mKey;
		std::string mSecret;
	};

	//�A�J�E���g�ꗗ
	const static STestAccountInfo AccountList[3] = 
	{
		//! DiceRoomLabo
		STestAccountInfo("DiceRoomLabo" , ***, "***" , "***")
	};


	//�e�X�g�A�J�E���g
	static const STestAccountInfo& Account = AccountList[2];
}

/****************************************************************************/
//////////////////////////// Function Declaration ////////////////////////////
/****************************************************************************/
namespace TSUL
{
	void TestWrite(const std::string& iFileName , const SResponse iResponse)
	{
		//json
		{
			std::ofstream ofs(("../Sample/" + iFileName + ".json").c_str());
			ofs << iResponse.mGetData << std::endl;
		}

		//xml
		{
			CXmlAnalyzer analayzer;
			analayzer.AnalyzeJsonBuffer(iResponse.mGetData);
			std::string buf;
			analayzer.GetRootNode() -> GetXmlBuffer(buf);
			std::ofstream ofs(("../Sample/" + iFileName + ".xml").c_str());
			ofs << buf << std::endl;
		}
	}
}

namespace TSUL
{
	void TestAccount();
	void TestStatus();
	void TestDirectMessage();
	void TestTimeLine();
	void TestFavorite();
	void TestLists();
	void TestUser();
	void TestBlock();
	void TestSearch();
	void TestTrends();
	void TestGeo();
	void TestCache();
	void TestStream();
	void TestSystem();

	void TestModule();
	void TestDMDetails();
	void TestFriendCheck();
	void TestNarrowingID();
		
	/*
		���߂�Interface�Ŏg���\���̂͊�{�S��
		TwitterInterface/TwitterInterfaceDef.h�ɒ�`����Ă��܂�

		�C���^�[�t�F�[�X�����Ō��ݗp�ӂ��Ă���̂�
		�uCSystem�v�uCTimeLine�v�uCDirectmessage�v�uCFvorites�v
		�uCLists�v�uCStatus�v�uCUser�v�̂V��
		�����͈ȉ��Ŋe��������Ă����܂�

		�S�ẴC���^�[�t�F�[�X�N���X�Ŏg�p����\���̂Ƃ��Ă�
		�uSGetArgs�v�uSResponse�v�ʂ�(���ɂ����邪�������Ȃ�)
		�O�҂͎擾�n�̊֐��Ŏg�p���܂��A�g�p�o���郁���o�ϐ��͏������ƂɈႢ�܂�
		�g���Ȃ������o�ϐ��Ƀf�[�^����ꍞ�񂾏ꍇ�̓���̕ۏ؂͂��܂���		
		��҂͑�̂��ׂĂ̊֐��Ŏg���܂��ATwitter�ɑ΂���API�𔭍s��������
		���ʂ��擾���邽�߂̍\���̂ɂȂ�܂�

		�����ׂĂ̊֐��ɂ̓A�J�E���g���ɃL�[�Ɣ閧�p�X��n���܂�
	*/
	

	void TestMain()
	{
		setlocale(LC_CTYPE, "");
		TSUL::CSocket::Initialize();
		
		//�V�X�e���e�X�g
		TestAccount();
		TestStatus();
		TestDirectMessage();
		TestTimeLine();
		TestFavorite();
		TestLists();
		TestUser();
		TestBlock();
		TestSearch();
		TestTrends();
		TestGeo();
		TestCache();
		TestStream();
		TestSystem();

		//�e�탂�W���[���e�X�g
		TestModule();
	}

	//============================================================================
	//! �A�J�E���g�֘A�e�X�g
	void TestAccount()
	{
		//�F�؃��[�U�[���L���ȃ��[�U�[���ǂ������ׂ�
		{
			SResponse response;
			//CAccount::IsVerifyCredentials(response , Account.mKey , Account.mSecret);
		}

		//�F�؃��[�U�[�̃v���t�B�[���̏����X�V
		{
			SResponse response;
			SAccountProfile profile;
			profile.mName = "TwiSUL�J���e�X�g�A�J�E���g";
			profile.mLocation = "���{�ƃJ���t�H���j�A";
			profile.mDescription = "��{�I�ɂ�API Version1.1�ŊJ��\n11/2�Ȃ�";
			//CAccount::UpdateProfile(response , AccountList[2].mKey , AccountList[2].mSecret , profile);
		}

		//�F�؃��[�U�[�̃v���t�B�[���A�C�R���ύX
		{
			SResponse response;

			/*
			FILE* file = fopen("Icon.png" , "rb");
			fseek(file , 0 , SEEK_END);
			int file_Size = ftell(file);
			fseek(file , 0 , SEEK_SET);
			char* buf = static_cast<char*>(malloc(file_Size));
			

			for(int iIndex = 0 ; iIndex < file_Size ; ++ iIndex)
			{
				buf[iIndex] = getc(file);
			}

			CAccount::UpdateProfileImage(response , Account.mKey , Account.mSecret , "Icon.png" , buf , file_Size);*/
		}

		/*�A�J�E���g�̔F�ؕ��@�ɂ���
		�A�J�E���g�̔F�؏����擾����ɂ�3��ނ̎菇�𓥂ޕK�v������܂�
		�@:�V�X�e������F�ؗp�y�[�W��URL�Ɖ��L�[���擾���܂�
		�A:�@�Ŏ擾����URL�ŔF�؂��s��pin�ԍ����擾���܂�
		�B:�A�Ŏ擾����pin�ԍ����V�X�e���ɓn���A�J�E���g���擾���܂�
		*/
		{
#if 0
			//�@
			std::string url , key;
			CAccount::GetAuthorizeUrl(url , key);

			//�A
			std::string pin;
#ifdef WIN32
			//WIN32�ł̎�肠�����̑Ώ��@
			ShellExecute(0 , 0 , url.c_str() , 0 , 0 , SW_SHOW);

			//�������u���[�N�|�C���g�Ŏ~�߂ăt�@�C���ɔԍ�����������
			FILE *fp;
			fp = fopen("Pin.txt", "r");
			if(fp)
			{
				pin.resize(256);
				fscanf(fp , "%s" , pin.c_str());
				fclose(fp);
			}
#endif
			//�B
			if(pin.length() != 0)
			{
				SAccount account;
				CAccount::GetAccount(account , Account.mKey , pin);
			}
#endif
		}
	}

	//============================================================================
	//! �c�C�[�g�֘A�̃e�X�g
	void TestStatus()
	{
		/*CStatus�ɂ��Ă̐���
		�X�e�[�^�X�Ɋւ��鑀����s�����܂�
		�u�w��ID�̃X�e�[�^�X�̏��擾�v�u�X�e�[�^�X�̍쐬�v�u�X�e�[�^�X�̍폜�v
		�u���c�C�[�g�̃X�e�[�^�X�쐬�v�u�w�肵���X�e�[�^�XID�����c�C�[�g�������[�U�[���̎擾�v
		*/
		{
			/*GetStatus
			�w�肵��ID�̃X�e�[�^�X�̏����擾���܂�
			�w�肵��ID�����݂��Ȃ��ꍇ�̓��X�|���X�̃X�e�[�^�X��
			�uGET_HTTP_NOT_FOUND�v���Ԃ�܂�
			*/
			{
				SResponse response;
				CStatusPtr status;
				CStatus::GetStatus(response , status , Account.mKey , Account.mSecret , 262877763894120448);
			}

			/*GetRetweetStatus
			�w�肵��ID�̃X�e�[�^�X�ɔF�؃��[�U�[�����c�C�[�g���Ă����ꍇ
			���̃��c�C�[�g�������̃c�C�[�g�h�c���擾���܂�
			���c�C�[�g����Ă��Ȃ��ꍇ��ID��UNKNOWN_ID
			�w�肵��ID�����݂��Ȃ��ꍇ�̓��X�|���X�̃X�e�[�^�X��
			�uGET_HTTP_NOT_FOUND�v���Ԃ�܂�
			*/
			{
				SResponse response;
				/*ID retweet_ID;
				CStatus::GetRetweetStatusID(response , retweet_ID , Account.mKey , Account.mSecret , 250970820665548800);*/
			}

			/*CreateStatus
			�V�K�X�e�[�^�X�𓊍e���܂�
			�ŏI�����ɂ͓��e�p�����[�^��t�^���܂�
			�ŏI�����́uSPostStatusArgs�v��mReplyStatusID�͕ԐM����ꍇ��
			���̕ԐM���s����̃X�e�[�^�XID�ɂȂ�܂�
			���A�����ݒ肷��ꍇ��mStatus�̓��ɂ��̑���X�e�[�^�X��

			�������e�̕���A�����e���悤�Ƃ����
			�uGET_HTTP_FORBIDDEN�v�����X�|���X�̃X�e�[�^�X�ɕԂ�܂�
			*/
			{
				SPostStatusArgs args;
				SResponse response;
				CStatusPtr status;
				args.mStatus = "Error Test3";
				//args.mReplyStatusID = 238513044462919680;
				//CStatus::CreateStatus(response , status , Account.mKey , Account.mSecret , args);
			}

			/*DeleteStatusMessage
			�w�肵��ID�̃X�e�[�^�X���폜���܂�
			�w�肵��ID�����݂��Ȃ��ꍇ�̓��X�|���X�̃X�e�[�^�X��
			�uGET_HTTP_NOT_FOUND�v�����X�|���X�̃X�e�[�^�X�ɕԂ�܂�
			*/
			{
				SResponse response;
				//CStatus::DeleteStatus(response , Account.mKey , Account.mSecret , 255164497407397888);
			}

			/*RetweetStatus
			�w�肵��ID�̃X�e�[�^�X���������c�C�[�g���܂�
			�w�肵��ID�����݂��Ȃ��ꍇ�̓��X�|���X�̃X�e�[�^�X��
			�uGET_HTTP_NOT_FOUND�v��
			���Ɍ������c�C�[�g�ς݂̏ꍇ��
			�uGET_HTTP_FORBIDDEN�v�����X�|���X�̃X�e�[�^�X�ɕԂ�܂�
			*/
			{
				SResponse response;
				CStatusPtr status;
				//CStatus::RetweetStatus(response , status , Account.mKey , Account.mSecret , 255163857893482499);
			}

			/*GetRetweetStatusUser
			�w�肵��ID�̃X�e�[�^�X�����c�C�[�g���Ă��郆�[�U�[ID���擾���܂�
			�w�肵��ID�����݂��Ȃ��ꍇ�̓��X�|���X�̃X�e�[�^�X��
			�uGET_HTTP_NOT_FOUND�v���Ԃ�܂�
			����Ɋւ��Ă̓T�[�o�[�̕��ׂɂ���Ď擾�ł��鐔���Ⴄ���ۂ�(�ő�100��)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CStatus::GetRetweetStatusUserID(response , id_List , Account.mKey , Account.mSecret , 254976932263886848);
			}
		}
	}

	//============================================================================
	//! �_�C���N�g���b�Z�[�W�֘A�̃e�X�g
	void TestDirectMessage()
	{
		/*CDirectmessage�ɂ��Ă̐���
		�_�C���N�g���b�Z�[�W�̑�����s���܂�
		�u��M���b�Z�[�W�̎擾�v�u���M���b�Z�[�W�̎擾�v
		�u�_�C���N�g���b�Z�[�W�̑��M�v�u�_�C���N�g���b�Z�[�W�̍폜�v

		���A�_�C���N�g���b�Z�[�W�̎擾�����̑������ɂ�
		�uSDirectMessage�v���������n���܂�
		�����ɂ�Post�y��Get���s�����ۂ̌��ʂ������Ă���
		���ɂ̓_�C���N�g���b�Z�[�W�̏�񂪊i�[����Ă��܂�
		*/
		{
			/*ReceiveDirectMessage
			��M���b�Z�[�W���擾���܂�
			*/
			{
				SResponse response;
				CDirectMessageList dm_List;
				//CDirectMessage::GetReceiveDirectMessage(response , dm_List , Account.mKey , Account.mSecret);
			}

			/*SentDirectMessage
			���M���b�Z�[�W���擾���܂�
			*/
			{
				SResponse response;
				CDirectMessageList dm_List;
				//CDirectMessage::GetSentDirectMessage(response , dm_List , Account.mKey , Account.mSecret);
			}

			/*CreateDirectMessage
			���b�Z�[�W���쐬���܂�
			�������ɂ͍쐬�����_�C���N�g���b�Z�[�W���̏o�͐���w�肵�܂�
			��l�����ɂ͋󂢂ă��[�U�[�̖��O�܂���ID���w�肵�܂�
			*/
			{
				SResponse response;
				CDirectMessagePtr dm;
				//CDirectMessage::CreateDirectMessage(response , dm , Account.mKey , Account.mSecret , "___DAISUL" , "API 1.1 DM Test Del");
			}

			/*GetDirectMessge
			���b�Z�[�W�̎擾
			*/
			{
				SResponse response;
				CDirectMessagePtr dm;
				//CDirectMessage::GetDirectMessage(response , dm , Account.mKey , Account.mSecret , 256371392503742464);
			}

			/*DeleteDirectMessage
			���b�Z�[�W���폜���܂�
			�������ɂ͍쐬�̌��ʏ��\���̂̏o�͐�
			��l�����ɂ̓��b�Z�[�WID���w�肵�܂�
			*/
			{
				SResponse response;
				//CDirectMessage::DeleteDirectMessage(response , Account.mKey , Account.mSecret , 255509158336532480);
			}
		}
	}

	//============================================================================
	//! �^�C�����C���֘A�̃e�X�g
	void TestTimeLine()
	{
		/*CTimeLine�ɂ��Ă̐���
		�e��^�C�����C���̎擾�ɂȂ�܂�
		�S�Ă̊֐��̑������ɂ̓^�C�����C�����\���́uSTimeLine�v��
		�Q�Ɠn�����܂��A�uSTimeLine�v�\���̂ɂ͊e��^�C�����C�����擾(���s�܂�)��������
		���ʏ������̃����o�ɕԂ��܂�
		�X�Ƀ����o��SStatusLog�����݂��Ă��܂���������ɂ̓^�C�����C���Ƃ��Ď擾����
		�X�e�[�^�X(�ꂫ�̎��j�̏ڍׂ������Ă��܂��A�����o�͑�̖��O�ŕ�����͂�
		�A���A���̃����o�̍\���̂̃����o�ɁumRetweetStatus�v�ƌ����\���̂̃����o����
		���̒��́umValidFlg�v��true�̎����̙ꂫ�̓��c�C�[�g�̏��ƂȂ�܂�
		���̎��uSStatusLog�v��mUserName�̓��c�C�[�g�����l���̖��O������܂���
		mText�ɂ̓��c�C�[�g���ꂽ�X�e�[�^�X�̖{���������Ă��܂�
		���uSRetweetStatus�v���̃��[�U�[���̓��c�C�[�g���̙ꂢ���{�l�̕��ƂȂ�܂�
		
		�ŏI�����ɂ̓I�v�V��������n�����ƂɂȂ�܂����f�t�H���g�łn�j�Ȃ�
		�ŏI���������Ȃ������\�ł�

		���ǂ̈������g���邩��
		http://watcher.moe-nifty.com/memo/docs/twitterAPI50.txt
		http://phpjavascriptroom.com/?t=topic
		�̊e��API�֓n����������Q�Ƃ̏�uSGetArgs�v�𐶐����Ă�������
		*/
		{
			/*HomeTimeLine
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g����^�C�����C�����擾���܂�*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 3;
				CTimeLine::GetHomeTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
			}

			/*UserTimeLine
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g����^�C�����C�����擾���܂�
			���̊֐��Ɍ���uSGetArgs�v���K�{�ƂȂ�mTarget�����o�ϐ��ɂ�
			���[�U�[ID�����Ă�������
			���uSGetArgs�v��mRetweetFlg��true�ɂ��Ȃ�����
			���c�C�[�g�̃^�C�����C�����O�͎擾����܂���*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs args;
				args.mIncludeRTFlg = true;
				//CTimeLine::GetUserTimeLine(response , status_List , Account.mKey , Account.mSecret , AccountList[1].mID , &args);
			}

			/*MentionsTimeLine
			Twitter��őS�Ẵ��[�U�[���܂߂��ŐV�̂Ԃ₫���擾���܂�*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 10;
				//CTimeLine::GetMentionsTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
			}

			/*ListsTimeLine
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g����^�C�����C�����擾���܂�
			���uSGetArgs�v��mRetweetFlg��true�ɂ��Ȃ�����
			*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs args;
				args.mIncludeRTFlg = true;
				//CTimeLine::GetListsTimeLine(response , status_List , Account.mKey , Account.mSecret , 78393023 , &args);
			}

			/*GetTalkRootID
			�w�肵���X�e�[�^�XID�̉�b�Ƃ��Ă̈�ԍŐV��ID��Ԃ�
			���̃X�e�[�^�X�ɑ΂���ԐM�������ꍇ�͎w�肵��ID�Ɠ���ID���Ԃ��Ă���
			*/
			{
				ID root_ID;
				//root_ID = CTimeLine::GetTalkRootID(256425712574140417);
				root_ID = 0;
#if 0
				//���ۂɉ�b���擾�������ꍇ�͈ȉ��̂悤�ɂ���
				/*�ŏ��̃c�C�[�g���擾(���ʂ͎擾���Ȃ��Ă��ŏ����玝���Ă�H*/
				SResponse response;
				CStatusPtr status;
				CStatus::GetStatus(response , status , Account.mKey , Account.mSecret , root_ID);
				
				//���̕ԐM�����݂��Ȃ��Ȃ�܂Ń��[�v���銴��
				CStatusList status_List;
				status_List.push_back(status);
				while(status -> mToReplyStateID != UNKNOWN_ID)
				{
					CStatus::GetStatus(response , status , Account.mKey , Account.mSecret , status -> mToReplyStateID);
					status_List.push_back(status);
				}
#endif
			}
		}
	}

	//============================================================================
	//! ���C�ɓ���֘A�̃e�X�g
	void TestFavorite()
	{
		/*CFavorites�ɂ��Ă̐���
		����̃c�C�[�g�ɑ΂��邨�C�ɓ���̑�����s���܂�
		�u���C�ɓ���̎擾�v�u���C�ɓ���̓o�^�v�u���C�ɓ���̓o�^�����v
		*/
		{
			/*GetFavorities
			���C�ɓ�����̎擾���s���܂�
			�擾���₻�̑����͏�L����URL����m�F���Ă�������
			�uSGetArgs�v���w�肵�Ȃ��ꍇ�͎w�肵���L�[�Ɣ閧�p�X�̃A�J�E���g����
			�w�肵���ꍇ�͂��̃��[�UID�̃A�J�E���g����擾���܂�
			*/
			{
				SResponse response;
				CStatusList status_List;
				//CFavorites::GetFavorites(response , status_List , Account.mKey , Account.mSecret , AccountList[1].mID);
			}

			/*CreateFavorites
			�w�肳�ꂽ�X�e�[�^�X�ɑ΂��Ă��C�ɓ����Ԃɓo�^���܂�
			���ɓo�^�ς݂̏ꍇ�̓��X�|���X�̃X�e�[�^�X�ɑ΂���
			�uGET_HTTP_FORBIDDEN�v���Ԃ��Ă��܂�
			*/
			{
				SResponse response;
				CStatusPtr status;
				//CFavorites::CreateFavorites(response , status , Account.mKey , Account.mSecret , 255502567990255616);
			}

			/*DeleteFavoritesMessage
			�w�肳�ꂽ�X�e�[�^�X�ɑ΂��Ă��C�ɓ����Ԃ��������܂�
			���C�ɓ����ԂłȂ��ꍇ�ł��X�e�[�^�X���L���ȏꍇ��
			���X�|���X�͏����̐���I����Ԃ��܂�
			*/
			{
				SResponse response;
				//CFavorites::DeleteFavorites(response , Account.mKey , Account.mSecret , 255151387200090112);
			}
		}
	}

	//============================================================================
	//! ���X�g�֘A�e�X�g
	void TestLists()
	{
		/*CLists
		���X�g�̑���
		*/
		{
			/*CreateList
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�ɐV�������X�g���쐬����
			*/
			{
				SResponse response;
				ID create_List_ID;
				create_List_ID;
				//CLists::CreateLists(response , create_List_ID , Account.mKey , Account.mSecret , "API ver1.1" , "Api 1.1 ver Test" , true);
			}

			/*UpdateLists
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g���������Ă��郊�X�g����
			�w�肳�ꂽ���X�gID�����݂��Ă���ꍇ���̃��X�g�ɑ΂���
			���X�g���A���X�g�̐����A���J��Ԃ̐ݒ肪�s���܂�
			*/
			{
				SResponse response;
				CListPtr list;
				//CLists::UpdateLists(response , list , Account.mKey , Account.mSecret , 78693262 , "API ver1.1 Edit" , "API Edit" , false);
			}

			/*DeleteList
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�ɐV�������X�g���쐬����
			���ɓ������O�����X�g���ɑ��݂��Ă����s�ɂȂ�Ȃ������X�g�͍���Ȃ�
			�w�肵�����X�gID�����݂��Ȃ��ꍇ�̓��X�|���X�ɁuGET_HTTP_NOT_FOUND�v���Ԃ�
			*/
			{
				SResponse response;
				//CLists::DeleteLists(response , Account.mKey , Account.mSecret , 78693262);
			}

			/*Lists
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�����L���Ă��郊�X�g�����擾����
			20���X�g���̎擾�ƂȂ�
			*/
			{
				SResponse response;
				CListList lists;
				//CLists::GetLists(response , lists , Account.mKey , Account.mSecret , Account.mID);
			}

			/*ListsInfo
			�w�肵�����X�gID�̃��X�g�������L����
			*/
			{
				SResponse response;
				CListPtr lists;
				//CLists::GetListsInfo(response , lists , Account.mKey , Account.mSecret , 77730821);
			}

			/*BelongLists
			�w�肵�����[�U�[ID���܂܂�郊�X�g�̏����擾����
			20���X�g���̎擾�ƂȂ�
			*/
			{
				SResponse response;
				CListList lists;
				SCursor cursor;
				//CLists::GetBelongLists(response , lists , cursor , Account.mKey , Account.mSecret , Account.mID , UNKNOWN_ID);
			}

			/*CreateFollowLists
			�w�肵�����X�g�ɑ΂��ăt�H���[������
			*/
			{
				SResponse response;
				//CLists::CreateFollowLists(response , Account.mKey , Account.mSecret , 77730821);
			}

			/*DeleteFollowLists
			�w�肵�����X�g�ɑ΂��ăt�H���[����������
			*/
			{
				SResponse response;
				//CLists::DeleteFollowLists(response , Account.mKey , Account.mSecret , 77730821);
			}

			/*ListsMember
			�w�肵�����X�gID���A�L�[�Ɣ閧�p�X�̃A�J�E���g��
			���L���Ă��郊�X�g���ɂ���ꍇ�̓����o�[���X�g���擾���܂�
			*/
			{
				SResponse response;
				CUserList user_List;
				SCursor cursor;
				//CLists::GetMemberList(response , user_List , cursor , Account.mKey , Account.mSecret , 78693262 , UNKNOWN_ID);
			}

			/*AddMemeber
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�����L���Ă��郊�X�g��
			�w�肵�����[�U�[��ǉ�����
			���ɒǉ�����Ă��郆�[�U�[�ł����X�|���X�ɂ͐�����Ԃ��Ԃ�
			�A�����݂��Ȃ����[�U�[ID�̏ꍇ�́uGET_HTTP_NOT_FOUND�v���Ԃ�
			*/
			{
				SResponse response;
				CListPtr lists;
				CIDList id_List;
				id_List.push_back(AccountList[0].mID);
				id_List.push_back(AccountList[1].mID);
				//CLists::AddMember(response , lists , Account.mKey , Account.mSecret , 78693262 , AccountList[1].mID);
				//CLists::AddMember(response , lists , Account.mKey , Account.mSecret , 78693262 , id_List);
			}

			/*IsMember
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�����L���Ă��郊�X�g����
			�w�肵�����[�U�[�����݂��邩�𒲂ׂ܂�
			���݂��Ȃ����[�U�[ID�̏ꍇ�́uGET_HTTP_NOT_FOUND�v���Ԃ�
			*/
			{
				SResponse response;
				//CLists::IsMember(response , Account.mKey , Account.mSecret , 78693262 , AccountList[1].mID);
			}

			/*RemoveMember
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�����L���Ă��郊�X�g��
			�w�肵�����[�U�[���폜����
			���X�g���ɍŏ����瑶�݂��Ȃ����[�U�[ID�̏ꍇ�́uGET_HTTP_BAD_REQUEST�v���Ԃ�
			*/
			{
				SResponse response;
				CListPtr lists;
				CIDList id_List;
				id_List.push_back(AccountList[0].mID);
				id_List.push_back(AccountList[1].mID);
				//CLists::RemoveMember(response , lists , Account.mKey , Account.mSecret , 78693262 , AccountList[1].mID);
				//CLists::RemoveMember(response , lists , Account.mKey , Account.mSecret , 78693262 , id_List);
			}
		}
	}

	//============================================================================
	//! ���[�U�[�֘A�e�X�g
	void TestUser()
	{
		/*CUser�ɂ���
		�e�탆�[�U�[�̏����擾���܂�
		�u�t�H���[�v�u�t�H���[�����v�u�t�H���[��Ԏ擾�v
		�u�t�H���[����M���X�g�̎擾�v�u�t�H���[�����M���X�g�̎擾�v
		�u�������t�H���[���Ă��郆�[�U�[���X�g�̎擾�v
		�u�������t�H���[���Ă��郆�[�U�[���X�g�̎擾�v
		�u�w�胆�[�U�[ID��������擾�v
		*/
		{
			/*CreateFollowing
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�ɑ΂���
			�w�肵�����[�U�[ID�̑�����t�H���[���܂�
			���Ƀt�H���[�ς݂̑���ł����X�|���X�X�e�[�^�X�͐���l��Ԃ��܂�
			���[�U�[�����݂��Ȃ��ꍇ�́uGET_HTTP_NOT_FOUND�v��Ԃ��܂�
			*/
			{
				SResponse response;
				//CUser::CreateFollowing(response , Account.mKey , Account.mSecret , AccountList[1].mID);
			}

			/*DeleteFollowing
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g���t�H���[���Ă���
			�w�肵�����[�U�[ID�̑�����t�H���[���������܂�
			���[�U�[�����݂��Ȃ��ꍇ�́uGET_HTTP_NOT_FOUND�v��Ԃ��܂�
			*/
			{
				SResponse response;
				//CUser::DeleteFollowing(response , Account.mKey , Account.mSecret , id1);
			}

			/*GetUserRelations
			�F�؂���Ă��郆�[�U�[�Ǝw�肵��ID�̃��[�U�[�̊֌W�����擾����
			*/
			{
				{
					SResponse response;
					CUserRelationList relation_List;
					CIDList id_List;
					//id_List.push_back(AccountList[0].mID);
					//id_List.push_back(AccountList[1].mID);
					//id_List.push_back(AccountList[2].mID);
					//CUser::GetUserRelation(response , relation_List , Account.mKey , Account.mSecret , id_List);
				}

				//�F�؃��[�U�[�ȊO�̃��[�U�[���m���擾���\�i������͕ʂ�API����J�E���g�ƂȂ�)
				{
					SResponse response;
					CUserRelationPtr relation;
					//CUser::GetUserRelation(response , relation , Account.mKey , Account.mSecret , AccountList[0].mID, AccountList[2].mID);
				}
			}

			/*InconmingID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�ɑ΂���
			�t�H���[���҂����̃��[�U�[�����擾����(�����������鑤)
			��܈����̃t���O��true������ƃ��[�U�[ID�����̏�Ԃ̌��ʂ��Ԃ�܂�(�����y��)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetIncomingID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*OutconmingID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g��
			�t�H���[���҂����̃��[�U�[�����擾����(������������鑤)
			��܈����̃t���O��true������ƃ��[�U�[ID�����̏�Ԃ̌��ʂ��Ԃ�܂�(�����y��)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetOutgoingID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*FollowID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�Ńt�H���[���Ă��郆�[�U�[��ID���擾���܂�
			�擾�����͌Œ��1�y�[�W�ɂ�5000���[�U�[�ƂȂ�܂�
			�ŏI�����̃J�[�\���̓y�[�W�ԍ��ł͂���܂���
			�����w�肵�Ȃ��ꍇ��ԍŏ�����̎擾�ƂȂ�܂�
			���̍ۂɑ������ւ̏o�͂͑O��y�[�W�ւ̃J�[�\���ƂȂ�܂�(-1�̎��擪)
			*/
			{
				SResponse response;
				CIDList id_List;
				SCursor cursor;
				//CUser::GetFollowID(response , id_List , cursor , Account.mKey , Account.mSecret , Account.mID);
			}

			/*AllFollowID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�Ńt�H���[���Ă��郆�[�U�[��ID��S�Ď擾���܂�
			GetFollowID�ł�1���5000���擾�o����̂ŗ]���̗ʂłȂ���΂�������g�p���Ă����͂���܂���
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetAllFollowID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*FollowerID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�Ńt�H���[���Ă��郆�[�U�[��ID���擾���܂�
			�擾�����͌Œ��1�y�[�W�ɂ�5000���[�U�[�ƂȂ�܂�
			�ŏI�����̃J�[�\���̓y�[�W�ԍ��ł͂���܂���
			�����w�肵�Ȃ��ꍇ��ԍŏ�����̎擾�ƂȂ�܂�
			���̍ۂɑ������ւ̏o�͂͑O��y�[�W�ւ̃J�[�\���ƂȂ�܂�(-1�̎��擪)
			*/
			{
				SResponse response;
				CIDList id_List;
				SCursor cursor;
				//CUser::GetFollowerID(response , id_List , cursor , Account.mKey , Account.mSecret , Account.mID);
			}

			/*AllFollowerID
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g�Ńt�H���[���Ă��郆�[�U�[��ID��S�Ď擾���܂�
			GetFollowID�ł�1���5000���擾�o����̂ŗ]���̗ʂłȂ���΂�������g�p���Ă����͂���܂���
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetAllFollowerID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*GetUser
			�w�肵�����[�U�[ID(�P��)���烆�[�U�[�����擾����
			*/
			{
				//�������g�̏��擾
				{
					SResponse response;
					CUserPtr user;
					//CUser::GetUser(response , user , Account.mKey , Account.mSecret);
				}

				//ID�ƃX�N���[���l�[��(@�`)���w�肵�Ď擾
				{
					//�P��
					{
						SResponse response;
						CUserPtr user;
						//CUser::GetUser(response , user , Account.mKey , Account.mSecret , AccountList[1].mID);
					}

					//����
					{
						SResponse response;
						CUserList user_List;
						CIDList id_List;
						
						/*id_List.push_back(AccountList[0].mID);
						id_List.push_back(AccountList[1].mID);
						id_List.push_back(AccountList[2].mID);*/
						//CUser::GetUser(response , user_List , Account.mKey , Account.mSecret , id_List);
						//CUser::GetUser(response , user_List , Account.mKey , Account.mSecret , id_List);
					}

					//������
					{
						SResponse response;
						CUserPtr user;
						//CUser::GetUser(response , user , Account.mKey , Account.mSecret , "DAISUL___");
					}
				}
			}
		}
	}

	//============================================================================
	//! �u���b�N�֘A�e�X�g
	void TestBlock()
	{
		/*CBlock�ɂ���
		�e�탆�[�U�[�ɂ��Ẵu���b�N������s���܂�
		�u�w�肵�����[�U�[���u���b�N����v�u�w�肵�����[�U�[�̃u���b�N����������v
		�u�w�胆�[�U�[���u���b�N���Ă��邩���擾�v
		�u�u���b�N���Ă��郆�[�U�[�̏��ꗗ���擾����v�u�X�p���񍐂��s���v
		*/
		{
			/*CreateBlocks
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g��
			�w�肵�����[�U�[ID�̑�����u���b�N���܂�
			���Ƀu���b�N�ς݂̑���ł����X�|���X�X�e�[�^�X�͐���l��Ԃ��܂�
			���[�U�[�����݂��Ȃ��ꍇ�́uGET_HTTP_NOT_FOUND�v��Ԃ��܂�
			*/
			{
				SResponse response;
				//CBlocks::CreateBlocks(response , Account.mKey , Account.mSecret , 139239126);
			}

			/*DeleteFollowing
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g���t�H���[���Ă���
			�w�肵�����[�U�[ID�̑�����t�H���[���������܂�
			�u���b�N���Ă��Ȃ��̑���ł����X�|���X�X�e�[�^�X�͐���l��Ԃ��܂�
			���[�U�[�����݂��Ȃ��ꍇ�́uGET_HTTP_NOT_FOUND�v��Ԃ��܂�
			*/
			{
				SResponse response;
				//CBlocks::DeleteBlocks(response , Account.mKey , Account.mSecret , 139239126);
			}

			/*Blocking
			�w�肵���A�L�[�Ɣ閧�p�X�̃A�J�E���g���u���b�N���Ă��郆�[�U�[ID���擾����
			*/
			{
				SResponse response;
				CIDList id_List;
				//CBlocks::GetBlocking(response , id_List , Account.mKey , Account.mSecret);
			}

			/*ReportSpam
			�w�肵��ID�̃��[�U�[���X�p���Ƃ��ĕ񍐂���
			*/
			{
				SResponse response;
				//CBlocks::ReportSpam(response , Account.mKey , Account.mSecret , 139239126);
			}
		}
	}

	//============================================================================
	//! �����֘A�e�X�g
	void TestSearch()
	{
		/*
		�w�肵�����������Ƀ}�b�`���������擾����
		�����Ŏ擾�����c�C�[�g�͈ꕔ�ۊǂ���Ă��Ȃ����̂�����(RT���Ƃ����X)
		�Ȃ̂Ń^�C�����C����ŕ\��������x�̏��Ȃ�擾�ł��邪
		�ڍׂ��擾����ꍇ�́ACStatus�ōĎ擾���鎖
		*/
		{
			/*SearchStatus
			�L�[���[�h�Ƀ}�b�`�����c�C�[�g�̃��X�g���擾����
			*/
			{
				SResponse response;
				CStatusList status_List;				
				SGetArgs arg;
				arg.mLanguage = "ja";
				arg.mCount = 20;
				//CSearch::SearchStatus(response , status_List , Account.mKey , Account.mSecret , "�_�C�X��" , &arg);
			}

			/*SearchUser
			�w�肵�����[�U�[���Ƀ}�b�`�������[�U�[���X�g���擾����
			*/
			{
				SResponse response;
				CUserList user_List;				
				SGetArgs arg;
				arg.mCount = 20;
				//CSearch::SearchUser(response , user_List , Account.mKey , Account.mSecret , "�_�C�X��" , &arg);
			}

			/*CreateSearchSaveInfo
			���������̕ۑ����s��
			*/
			{
				SResponse response;
				CSearchSaveInfoPtr search;
				//CSearch::CreateSearchSaveInfo(response , search , Account.mKey , Account.mSecret , "�_�C�X��");
			}

			/*DeleteSearchSaveInfo
			�쐬�ς݂̌����������폜����
			*/
			{
				SResponse response;
				//CSearch::DeleteSearchSaveInfo(response , Account.mKey , Account.mSecret , 180797145);
			}

			/*GetSearchSaveInfo
			�w�肵�����������ԍ����猟���������擾����
			*/
			{
				SResponse response;
				CSearchSaveInfoPtr search;
				//CSearch::GetSearchSaveInfo(response , search , Account.mKey , Account.mSecret , 180797145);
			}

			/*GetAllSearchSaveInfo
			�ۑ�����Ă���S�Ă̌����������擾����
			*/
			{
				SResponse response;
				CSearchSaveInfoList search_List;
				//CSearch::GetAllSearchSaveInfo(response , search_List , Account.mKey , Account.mSecret);
			}
		}
	}

	//============================================================================
	//! �g�����h�֘A�e�X�g
	void TestTrends()
	{
		//�w�肵��WOEID�̈ʒu�̃g�����h���擾����(WOEID��GetClosestPlace�ňܓx�A�o�x�w�肵�Ď擾�ł���)
		{
			SResponse response;
			CTrendsPtr trend;
			//CTrends::GetPlaceTrend(response , trend , Account.mKey , Account.mSecret , 1117099);
		}

		//�n�惊�X�g�H���擾
		{
			SResponse response;
			CPlaceList place_List;
			//CTrends::GetAvailablePlace(response , place_List , Account.mKey , Account.mSecret);
		}

		//�w�肵���ܓx�A�o�x���w�肵�Ēn����擾
		{
			//�g�p�s�\
			SResponse response;
			CPlacePtr place;
			//CTrends::GetClosestPlace(response , place , Account.mKey , Account.mSecret , 37.781157f , 122.400612831116f);
		}
	}

	//============================================================================
	//! �W�I�e�X�g
	void TestGeo()
	{
		//�ꏊID��������擾����
		{
			SResponse response;
			CGeoPtr geo;
			//CGeo::GetGeoPlace(response , geo , Account.mKey , Account.mSecret , "df51dec6f4ee2b2c");
		}

		//�o�x�ܓx����ꏊ���X�g���擾�H
		{
			SResponse response;
			CGeoList geo_List;
			SGeoArgs args;
			args.mLat = 37.76893497f;
			args.mLong = -122.42284884f;
			//CGeo::GetReverseGeocode(response , geo_List , Account.mKey , Account.mSecret , args);
		}

		//�w�肳�ꂽ���O�H�̌o�x�ܓx����w�肳�ꂽ�o�x�ܓx�ŋ߂��������߂�H�ǂ��킩���
		{
			SResponse response;
			CGeoList geo_List;
			SGeoArgs args;
			args.mLat = 37;
			args.mLong = -122;
			args.mName = "Twitter%20HQ";
			//CGeo::GetSimilarPlaces(response , geo_List , Account.mKey , Account.mSecret , args);
		}

		//�����p�̈��������v����ꏊ���擾�H
		{
			SResponse response;
			CGeoList geo_List;
			
			//�ܓx�A�o�x�A�N�G��(mName)�AIP�̉��ꂩ�����͂���Ă��Ȃ��ƃG���[�ɂȂ�
			SGeoArgs args;
			//args.mLat = 37.76893497f;
			//args.mLong = -122.42284884f;
			//args.mName = "Toronto";
			args.mIP = "74.125.19.104";
			
			//CGeo::GetSearchPlaces(response , geo_List , Account.mKey , Account.mSecret , args);
		}
	}

	//============================================================================
	//! �L���b�V���e�X�g
	void TestCache()
	{
		//�c�C�[�g�L���b�V��
		{
			//�擾
			{
#if 0
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 100;
				CTimeLine::GetHomeTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
				
				CCachePtr cache = CCache::GetStatusCacheData();
				FILE* file = fopen("StatusCache.cmp" , "wb");
				fwrite(&cache -> mMemorySize , sizeof(cache -> mMemorySize) , 1 , file);
				fwrite(cache -> mHeadAddress , 1 , cache -> mMemorySize , file);
				fclose(file);
#endif
			}

			//�ݒ�
			{
#if 0
				int cache_Size = 0;
				FILE* file = fopen("StatusCache.cmp" , "rb");
				fread(&cache_Size , sizeof(cache_Size) , 1 , file);
				char* cache = new char[cache_Size];
				fread(cache , 1 , cache_Size , file);
				fclose(file);
				CCache::SetStatusCacheData(cache , cache_Size);
				delete[] cache;
#endif
			}
		}

		//�_�C���N�g���b�Z�[�W�L���b�V��
		{
			//�擾
			{
#if 0
				SResponse response;
				CDirectMessageList dm_List;
				CDirectMessage::GetReceiveDirectMessage(response , dm_List , Account.mKey , Account.mSecret);

				CCachePtr cache = CCache::GetDirectMessageCacheData();
				FILE* file = fopen("DMCache.cmp" , "wb");
				fwrite(&cache -> mMemorySize , sizeof(cache -> mMemorySize) , 1 , file);
				fwrite(cache -> mHeadAddress , 1 , cache -> mMemorySize , file);
				fclose(file);
#endif
			}

			//�ݒ�
			{
#if 0
				int cache_Size = 0;
				FILE* file = fopen("DMCache.cmp" , "rb");
				fread(&cache_Size , sizeof(cache_Size) , 1 , file);
				char* cache = new char[cache_Size];
				fread(cache , 1 , cache_Size , file);
				fclose(file);
				CCache::SetDirectMessageCacheData(cache , cache_Size);
				delete[] cache;
#endif
			}
		}

		//���[�U�[�L���b�V��
		{
			//�擾
			{
#if 0
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 20;
				CTimeLine::GetHomeTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
				
				CCachePtr cache = CCache::GetUserCacheData();
				FILE* file = fopen("UserCache.cmp" , "wb");
				fwrite(&cache -> mMemorySize , sizeof(cache -> mMemorySize) , 1 , file);
				fwrite(cache -> mHeadAddress , 1 , cache -> mMemorySize , file);
				fclose(file);
#endif
			}

			//�ݒ�
			{
#if 0
				int cache_Size = 0;
				FILE* file = fopen("UserCache.cmp" , "rb");
				fread(&cache_Size , sizeof(cache_Size) , 1 , file);
				char* cache = new char[cache_Size];
				fread(cache , 1 , cache_Size , file);
				fclose(file);
				CCache::SetUserCacheData(cache , cache_Size);
				delete[] cache;
#endif
			}

			//���[�U�[�L���b�V�����������Ԃ��ǂ����𒲂ׂ�
			{
#if 0
				time_t sec;
				time(&sec);
				CCache::IsValidTimeUserCache(Account.mID , sec);
#endif
			}
		}
	}

	//============================================================================
	//! �X�g���[���e�X�g
	void TestStream()
	{
#if 0
		FILE* file = fopen("C:\\DL\\DL_PROJECT\\TwiSUL\\Project\\Application\\Public.txt" , "rb");
		fseek(file , 0 , SEEK_END);
		int file_Size = ftell(file);
		fseek(file , 0 , SEEK_SET);
		char* buf = static_cast<char*>(malloc(file_Size + 1));
		memset(buf , 0 , file_Size + 1);
		for(int iIndex = 0 ; iIndex < file_Size ; ++ iIndex)
		{
			buf[iIndex] = getc(file);
		}

		CStatusList status_List;
		std::string result;
		free(buf);
#endif

#if 0
		CStreamConnectionPtr connection;
		CStream::CreatePublicStreamConnection(connection , AccountList[2].mKey , AccountList[2].mSecret);
		connection -> Connect();
#endif

#if 0
		CStreamConnectionPtr connection;
		CStream::CreateUserStreamConnection(connection , AccountList[2].mKey , AccountList[2].mSecret);
		connection -> Connect();
#endif

	}

	//============================================================================
	//! �V�X�e���e�X�g
	void TestSystem()
	{
		//�^�C�����C���n���̃��[�g���~�b�g���擾
		{
			/*
			������false�ɂ��Ă���API1.1���[�h�Ŏc��񐔂��擾����
			���A�^�C�����C���̊e��擾��API1.1���[�h�ɂȂ�
			CTimeLine::SetUseOldAPIMode(false);

			���A�ȉ��̂悤��
			CSystem::SetRateLimitAPIFlg(true);
			��true��ݒ肵�Ă��Ǝc��񐔂�API�ɂ���Ď擾����悤�ɂȂ�
			*/
			SResponse response;
			STimeLineRateLimit rate_Limit;
			//CTimeLine::GetRateLimit(response , rate_Limit , Account.mKey , Account.mSecret);
		}

		//�_�C���N�g���b�Z�[�W�̃��[�g���~�b�g���擾
		{
			/*
			������false�ɂ��Ă���API1.1���[�h�Ŏc��񐔂��擾����
			���A�_�C���N�g���b�Z�[�W�̊e��擾��API1.1���[�h�ɂȂ�
			CDirectMessage::SetUseOldAPIMode(false);

			���A�ȉ��̂悤��
			CSystem::SetRateLimitAPIFlg(true);
			��true��ݒ肵�Ă��Ǝc��񐔂�API�ɂ���Ď擾����悤�ɂȂ�
			*/
			SResponse response;
			SDirectMessageRateLimit rate_Limit;
			//CDirectMessage::GetRateLimit(response , rate_Limit , Account.mKey , Account.mSecret);
		}

		//�Z�kURL�̎擾
		std::string get_Url;
		{
			std::string long_Url = "http://live.nicovideo.jp/search/%E3%83%96%E3%83%AC%E3%82%A4%E3%83%96%E3%83%AB%E3%83%BC?kind=content&track=nicouni_search_keyword";
			std::string short_Url;
			//CSystem::GetShortUrl(short_Url , "o_1ao6r87ov8" , "R_df24031c9192445f69580da8aa61bb9f" , CSystem::Decode(long_Url));

			get_Url = short_Url;
		}

		//�Z�kURL�̃f�R�[�h
		{
			std::string short_Url = get_Url;
			std::string long_Url;
			//CSystem::GetLongUrl(long_Url , "o_1ao6r87ov8" , "R_df24031c9192445f69580da8aa61bb9f" , short_Url);
		}
	}

	//============================================================================
	//! �e�탂�W���[���̃e�X�g
	void TestModule()
	{
		TestDMDetails();
		TestFriendCheck();
		TestNarrowingID();
	}

	void TestDMDetails()
	{
#if 0
		/*
		�������őS�擾
		*/
		CDMDetailsModule module(Account.mKey , Account.mSecret);

		//�L���b�V���ǂݍ���
#if 0
		{
			int cache_Size = 0;
			FILE* file = fopen("DMModuleCache.cmp" , "rb");
			fread(&cache_Size , sizeof(cache_Size) , 1 , file);
			char* cache = new char[cache_Size];
			fread(cache , 1 , cache_Size , file);
			fclose(file);
			module.SetCache(cache , cache_Size);
			delete[] cache;
		}
#endif


		//API�������Ԃ͎�M�y�ё��M���b�Z�[�W�̃��O���擾���Ă���
		CDMDetailsModule::CUpdateMap update_Map;
		module.UpdateOldSendMessage();
		module.UpdateOldRecvMessage();
		
		/*
		���̃��W���[��������o���Ă���ŐV�̑���M���b�Z�[�W�������update_Map�ɓn�����
		���RAPI��Send,Recv���ɂP�񂸂�����(�ʁX�̏���g)
		*/
		//module.UpdateSendMessage(update_Map);
		//module.UpdateRecvMessage(update_Map);

		//���ɂ���肵�����[�U�[�ŐV�������ɕ���ł��郆�[�U�[�����擾
		CIDList id_List;
		module.GetUserIDList(id_List);

		//���[�U�[���擾
		CDMDetailsModule::SUserInfo user_Info;
		module.GetUserInfo(user_Info , id_List[0]);

		/*
		���[�U�[ID����c�l�̂����̃��X�g���擾����
		����͂����̏��Ԃ��V��������������Ă���
		���A�\���̂́umMyselfSenderFlg�v��true�̎��͎���������������
		false�̎��͑��肩�瑗���Ă������ɂȂ��Ă���
		*/
		CDMDetailsModule::CMessageList msg_List;
		module.GetMessageList(msg_List , id_List[0]);

		/*
		�擾�e�X�g
		*/
		/*
		CDMDetailsModule::CMessageList::iterator itr , end_Itr = msg_List.end();
		CDirectMessageList dm_List;
		for(itr = msg_List.begin() ; itr != end_Itr ; ++ itr)
		{
			SResponse response;
			CDirectMessagePtr dm;
			CDirectMessage::GetDirectMessage(response , dm , Account.mKey , Account.mSecret , (*itr) -> mMessageID);
			dm_List.push_back(dm);
		}*/

		//�L���b�V�������o��
#if 0
		{
			CCachePtr cache = module.GetCache();
			FILE* file = fopen("DMModuleCache.cmp" , "wb");
			fwrite(&cache -> mMemorySize , sizeof(cache -> mMemorySize) , 1 , file);
			fwrite(cache -> mHeadAddress , 1 , cache -> mMemorySize , file);
			fclose(file);
		}
#endif
#endif
	}

	void TestFriendCheck()
	{
#if 0
		//�t�H���[��ԃ`�F�b�N���W���[��
		CFriendCheckModule module(Account.mKey , Account.mSecret , Account.mID);
			
		CIDList	follow_List;
		module.GetFollowList(follow_List);
		CIDList	follower_List;
		module.GetFollowerList(follower_List);
		CIDList	fun_List;
		module.GetFunList(fun_List);
		CIDList	you_Fun_List;
		module.GetYourFunList(you_Fun_List);
		CIDList friend_List;
		module.GetFriendList(friend_List);
#endif
	}

	void TestNarrowingID()
	{
#if 0
		CNarrowingIDModule module(Account.mKey , Account.mSecret);
		module.AddFollowMember("�y���\�i" , true);
		module.AddFollowerMember("�u���C�u���[" , true);
		//module.AddListMember("�u���C�u���[" , true , 76223053);

		//�i�荞��
		module.NarrowMember("�|�P����" , true);

		//�擾
		CUserList user_List;
		module.GetMemberList(user_List);
#endif
	}
}