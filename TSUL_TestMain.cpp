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
	//! アカウント情報
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

	//アカウント一覧
	const static STestAccountInfo AccountList[3] = 
	{
		//! DiceRoomLabo
		STestAccountInfo("DiceRoomLabo" , ***, "***" , "***")
	};


	//テストアカウント
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
		初めにInterfaceで使う構造体は基本全て
		TwitterInterface/TwitterInterfaceDef.hに定義されています

		インターフェース部分で現在用意してあるのは
		「CSystem」「CTimeLine」「CDirectmessage」「CFvorites」
		「CLists」「CStatus」「CUser」の７つ
		これらは以下で各種説明していきます

		全てのインターフェースクラスで使用する構造体としては
		「SGetArgs」「SResponse」位で(他にもあるが説明を省く)
		前者は取得系の関数で使用します、使用出来るメンバ変数は処理ごとに違います
		使えないメンバ変数にデータを入れ込んだ場合の動作の保証はしません		
		後者は大体すべての関数で使います、Twitterに対してAPIを発行した時の
		結果を取得するための構造体になります

		又すべての関数にはアカウント毎にキーと秘密パスを渡します
	*/
	

	void TestMain()
	{
		setlocale(LC_CTYPE, "");
		TSUL::CSocket::Initialize();
		
		//システムテスト
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

		//各種モジュールテスト
		TestModule();
	}

	//============================================================================
	//! アカウント関連テスト
	void TestAccount()
	{
		//認証ユーザーが有効なユーザーかどうか調べる
		{
			SResponse response;
			//CAccount::IsVerifyCredentials(response , Account.mKey , Account.mSecret);
		}

		//認証ユーザーのプロフィールの情報を更新
		{
			SResponse response;
			SAccountProfile profile;
			profile.mName = "TwiSUL開発テストアカウント";
			profile.mLocation = "日本とカリフォルニア";
			profile.mDescription = "基本的にはAPI Version1.1で開発\n11/2なう";
			//CAccount::UpdateProfile(response , AccountList[2].mKey , AccountList[2].mSecret , profile);
		}

		//認証ユーザーのプロフィールアイコン変更
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

		/*アカウントの認証方法について
		アカウントの認証情報を取得するには3種類の手順を踏む必要があります
		①:システムから認証用ページのURLと仮キーを取得します
		②:①で取得したURLで認証を行いpin番号を取得します
		③:②で取得したpin番号をシステムに渡しアカウントを取得します
		*/
		{
#if 0
			//①
			std::string url , key;
			CAccount::GetAuthorizeUrl(url , key);

			//②
			std::string pin;
#ifdef WIN32
			//WIN32での取りあえずの対処法
			ShellExecute(0 , 0 , url.c_str() , 0 , 0 , SW_SHOW);

			//ここをブレークポイントで止めてファイルに番号を書き込む
			FILE *fp;
			fp = fopen("Pin.txt", "r");
			if(fp)
			{
				pin.resize(256);
				fscanf(fp , "%s" , pin.c_str());
				fclose(fp);
			}
#endif
			//③
			if(pin.length() != 0)
			{
				SAccount account;
				CAccount::GetAccount(account , Account.mKey , pin);
			}
#endif
		}
	}

	//============================================================================
	//! ツイート関連のテスト
	void TestStatus()
	{
		/*CStatusについての説明
		ステータスに関する操作を行いします
		「指定IDのステータスの情報取得」「ステータスの作成」「ステータスの削除」
		「リツイートのステータス作成」「指定したステータスIDをリツイートしたユーザー情報の取得」
		*/
		{
			/*GetStatus
			指定したIDのステータスの情報を取得します
			指定したIDが存在しない場合はレスポンスのステータスに
			「GET_HTTP_NOT_FOUND」が返ります
			*/
			{
				SResponse response;
				CStatusPtr status;
				CStatus::GetStatus(response , status , Account.mKey , Account.mSecret , 262877763894120448);
			}

			/*GetRetweetStatus
			指定したIDのステータスに認証ユーザーがリツイートしていた場合
			そのリツイートした時のツイートＩＤを取得します
			リツイートされていない場合はIDにUNKNOWN_ID
			指定したIDが存在しない場合はレスポンスのステータスに
			「GET_HTTP_NOT_FOUND」が返ります
			*/
			{
				SResponse response;
				/*ID retweet_ID;
				CStatus::GetRetweetStatusID(response , retweet_ID , Account.mKey , Account.mSecret , 250970820665548800);*/
			}

			/*CreateStatus
			新規ステータスを投稿します
			最終引数には投稿パラメータを付与します
			最終引数の「SPostStatusArgs」のmReplyStatusIDは返信する場合に
			その返信を行う先のステータスIDになります
			尚、これを設定する場合はmStatusの頭にその相手ステータスを

			同じ内容の物を連続投稿しようとすると
			「GET_HTTP_FORBIDDEN」がレスポンスのステータスに返ります
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
			指定したIDのステータスを削除します
			指定したIDが存在しない場合はレスポンスのステータスに
			「GET_HTTP_NOT_FOUND」がレスポンスのステータスに返ります
			*/
			{
				SResponse response;
				//CStatus::DeleteStatus(response , Account.mKey , Account.mSecret , 255164497407397888);
			}

			/*RetweetStatus
			指定したIDのステータスを公式リツイートします
			指定したIDが存在しない場合はレスポンスのステータスに
			「GET_HTTP_NOT_FOUND」が
			既に公式リツイート済みの場合は
			「GET_HTTP_FORBIDDEN」がレスポンスのステータスに返ります
			*/
			{
				SResponse response;
				CStatusPtr status;
				//CStatus::RetweetStatus(response , status , Account.mKey , Account.mSecret , 255163857893482499);
			}

			/*GetRetweetStatusUser
			指定したIDのステータスをリツイートしているユーザーIDを取得します
			指定したIDが存在しない場合はレスポンスのステータスに
			「GET_HTTP_NOT_FOUND」が返ります
			これに関してはサーバーの負荷によって取得できる数が違うっぽい(最大100件)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CStatus::GetRetweetStatusUserID(response , id_List , Account.mKey , Account.mSecret , 254976932263886848);
			}
		}
	}

	//============================================================================
	//! ダイレクトメッセージ関連のテスト
	void TestDirectMessage()
	{
		/*CDirectmessageについての説明
		ダイレクトメッセージの操作を行います
		「受信メッセージの取得」「送信メッセージの取得」
		「ダイレクトメッセージの送信」「ダイレクトメッセージの削除」

		又、ダイレクトメッセージの取得処理の第一引数には
		「SDirectMessage」情報を引き渡します
		こいつにはPost及びGetを行った際の結果が入っており
		他にはダイレクトメッセージの情報が格納されています
		*/
		{
			/*ReceiveDirectMessage
			受信メッセージを取得します
			*/
			{
				SResponse response;
				CDirectMessageList dm_List;
				//CDirectMessage::GetReceiveDirectMessage(response , dm_List , Account.mKey , Account.mSecret);
			}

			/*SentDirectMessage
			送信メッセージを取得します
			*/
			{
				SResponse response;
				CDirectMessageList dm_List;
				//CDirectMessage::GetSentDirectMessage(response , dm_List , Account.mKey , Account.mSecret);
			}

			/*CreateDirectMessage
			メッセージを作成します
			第一引数には作成したダイレクトメッセージ情報の出力先を指定します
			第四引数には空いてユーザーの名前またはIDを指定します
			*/
			{
				SResponse response;
				CDirectMessagePtr dm;
				//CDirectMessage::CreateDirectMessage(response , dm , Account.mKey , Account.mSecret , "___DAISUL" , "API 1.1 DM Test Del");
			}

			/*GetDirectMessge
			メッセージの取得
			*/
			{
				SResponse response;
				CDirectMessagePtr dm;
				//CDirectMessage::GetDirectMessage(response , dm , Account.mKey , Account.mSecret , 256371392503742464);
			}

			/*DeleteDirectMessage
			メッセージを削除します
			第一引数には作成の結果情報構造体の出力先
			第四引数にはメッセージIDを指定します
			*/
			{
				SResponse response;
				//CDirectMessage::DeleteDirectMessage(response , Account.mKey , Account.mSecret , 255509158336532480);
			}
		}
	}

	//============================================================================
	//! タイムライン関連のテスト
	void TestTimeLine()
	{
		/*CTimeLineについての説明
		各種タイムラインの取得になります
		全ての関数の第一引数にはタイムライン情報構造体「STimeLine」を
		参照渡しします、「STimeLine」構造体には各種タイムラインを取得(失敗含む)した時に
		結果情報をこのメンバに返します
		更にメンバはSStatusLogが存在していますがこちらにはタイムラインとして取得した
		ステータス(呟きの事）の詳細が入っています、メンバは大体名前で分かるはず
		但し、このメンバの構造体のメンバに「mRetweetStatus」と言う構造体のメンバあり
		その中の「mValidFlg」がtrueの時この呟きはリツイートの情報となります
		この時「SStatusLog」のmUserNameはリツイートした人物の名前が入りますが
		mTextにはリツイートされたステータスの本文が入っています
		又「SRetweetStatus」内のユーザー情報はリツイート元の呟いた本人の物となります
		
		最終引数にはオプション情報を渡すことになりますがデフォルトでＯＫなら
		最終引数を入れない事も可能です

		又どの引数を使えるかは
		http://watcher.moe-nifty.com/memo/docs/twitterAPI50.txt
		http://phpjavascriptroom.com/?t=topic
		の各種APIへ渡せる引数を参照の上「SGetArgs」を生成してください
		*/
		{
			/*HomeTimeLine
			指定した、キーと秘密パスのアカウントからタイムラインを取得します*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 3;
				CTimeLine::GetHomeTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
			}

			/*UserTimeLine
			指定した、キーと秘密パスのアカウントからタイムラインを取得します
			この関数に限り「SGetArgs」が必須となりmTargetメンバ変数には
			ユーザーIDを入れてください
			又「SGetArgs」のmRetweetFlgをtrueにしない限り
			リツイートのタイムラインログは取得されません*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs args;
				args.mIncludeRTFlg = true;
				//CTimeLine::GetUserTimeLine(response , status_List , Account.mKey , Account.mSecret , AccountList[1].mID , &args);
			}

			/*MentionsTimeLine
			Twitter上で全てのユーザーを含めた最新のつぶやきを取得します*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs arg;
				arg.mCount = 10;
				//CTimeLine::GetMentionsTimeLine(response , status_List , Account.mKey , Account.mSecret , &arg);
			}

			/*ListsTimeLine
			指定した、キーと秘密パスのアカウントからタイムラインを取得します
			又「SGetArgs」のmRetweetFlgをtrueにしない限り
			*/
			{
				SResponse response;
				CStatusList status_List;
				SGetArgs args;
				args.mIncludeRTFlg = true;
				//CTimeLine::GetListsTimeLine(response , status_List , Account.mKey , Account.mSecret , 78393023 , &args);
			}

			/*GetTalkRootID
			指定したステータスIDの会話としての一番最新のIDを返す
			このステータスに対する返信が無い場合は指定したIDと同じIDが返ってくる
			*/
			{
				ID root_ID;
				//root_ID = CTimeLine::GetTalkRootID(256425712574140417);
				root_ID = 0;
#if 0
				//実際に会話を取得したい場合は以下のようにする
				/*最初のツイートを取得(普通は取得しなくても最初から持ってる？*/
				SResponse response;
				CStatusPtr status;
				CStatus::GetStatus(response , status , Account.mKey , Account.mSecret , root_ID);
				
				//次の返信が存在しなくなるまでループする感じ
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
	//! お気に入り関連のテスト
	void TestFavorite()
	{
		/*CFavoritesについての説明
		特定のツイートに対するお気に入りの操作を行います
		「お気に入りの取得」「お気に入りの登録」「お気に入りの登録解除」
		*/
		{
			/*GetFavorities
			お気に入り情報の取得を行います
			取得数やその他情報は上記したURLから確認してください
			「SGetArgs」を指定しない場合は指定したキーと秘密パスのアカウントから
			指定した場合はそのユーザIDのアカウントから取得します
			*/
			{
				SResponse response;
				CStatusList status_List;
				//CFavorites::GetFavorites(response , status_List , Account.mKey , Account.mSecret , AccountList[1].mID);
			}

			/*CreateFavorites
			指定されたステータスに対してお気に入り状態に登録します
			既に登録済みの場合はレスポンスのステータスに対して
			「GET_HTTP_FORBIDDEN」が返ってきます
			*/
			{
				SResponse response;
				CStatusPtr status;
				//CFavorites::CreateFavorites(response , status , Account.mKey , Account.mSecret , 255502567990255616);
			}

			/*DeleteFavoritesMessage
			指定されたステータスに対してお気に入り状態を解除します
			お気に入り状態でない場合でもステータスが有効な場合は
			レスポンスは処理の正常終了を返します
			*/
			{
				SResponse response;
				//CFavorites::DeleteFavorites(response , Account.mKey , Account.mSecret , 255151387200090112);
			}
		}
	}

	//============================================================================
	//! リスト関連テスト
	void TestLists()
	{
		/*CLists
		リストの操作
		*/
		{
			/*CreateList
			指定した、キーと秘密パスのアカウントに新しくリストを作成する
			*/
			{
				SResponse response;
				ID create_List_ID;
				create_List_ID;
				//CLists::CreateLists(response , create_List_ID , Account.mKey , Account.mSecret , "API ver1.1" , "Api 1.1 ver Test" , true);
			}

			/*UpdateLists
			指定した、キーと秘密パスのアカウントが所持しているリスト内に
			指定されたリストIDが存在している場合そのリストに対して
			リスト名、リストの説明、公開状態の設定が行えます
			*/
			{
				SResponse response;
				CListPtr list;
				//CLists::UpdateLists(response , list , Account.mKey , Account.mSecret , 78693262 , "API ver1.1 Edit" , "API Edit" , false);
			}

			/*DeleteList
			指定した、キーと秘密パスのアカウントに新しくリストを作成する
			既に同じ名前がリスト内に存在しても失敗にならないがリストは作られない
			指定したリストIDが存在しない場合はレスポンスに「GET_HTTP_NOT_FOUND」が返る
			*/
			{
				SResponse response;
				//CLists::DeleteLists(response , Account.mKey , Account.mSecret , 78693262);
			}

			/*Lists
			指定した、キーと秘密パスのアカウントが所有しているリスト情報を取得する
			20リストずつの取得となる
			*/
			{
				SResponse response;
				CListList lists;
				//CLists::GetLists(response , lists , Account.mKey , Account.mSecret , Account.mID);
			}

			/*ListsInfo
			指定したリストIDのリスト情報を所有する
			*/
			{
				SResponse response;
				CListPtr lists;
				//CLists::GetListsInfo(response , lists , Account.mKey , Account.mSecret , 77730821);
			}

			/*BelongLists
			指定したユーザーIDが含まれるリストの情報を取得する
			20リストずつの取得となる
			*/
			{
				SResponse response;
				CListList lists;
				SCursor cursor;
				//CLists::GetBelongLists(response , lists , cursor , Account.mKey , Account.mSecret , Account.mID , UNKNOWN_ID);
			}

			/*CreateFollowLists
			指定したリストに対してフォローをする
			*/
			{
				SResponse response;
				//CLists::CreateFollowLists(response , Account.mKey , Account.mSecret , 77730821);
			}

			/*DeleteFollowLists
			指定したリストに対してフォローを解除する
			*/
			{
				SResponse response;
				//CLists::DeleteFollowLists(response , Account.mKey , Account.mSecret , 77730821);
			}

			/*ListsMember
			指定したリストIDが、キーと秘密パスのアカウントが
			所有しているリスト内にある場合はメンバーリストを取得します
			*/
			{
				SResponse response;
				CUserList user_List;
				SCursor cursor;
				//CLists::GetMemberList(response , user_List , cursor , Account.mKey , Account.mSecret , 78693262 , UNKNOWN_ID);
			}

			/*AddMemeber
			指定した、キーと秘密パスのアカウントが所有しているリストへ
			指定したユーザーを追加する
			既に追加されているユーザーでもレスポンスには成功状態が返る
			但し存在しないユーザーIDの場合は「GET_HTTP_NOT_FOUND」が返る
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
			指定した、キーと秘密パスのアカウントが所有しているリスト内に
			指定したユーザーが存在するかを調べます
			存在しないユーザーIDの場合は「GET_HTTP_NOT_FOUND」が返る
			*/
			{
				SResponse response;
				//CLists::IsMember(response , Account.mKey , Account.mSecret , 78693262 , AccountList[1].mID);
			}

			/*RemoveMember
			指定した、キーと秘密パスのアカウントが所有しているリストへ
			指定したユーザーを削除する
			リスト内に最初から存在しないユーザーIDの場合は「GET_HTTP_BAD_REQUEST」が返る
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
	//! ユーザー関連テスト
	void TestUser()
	{
		/*CUserについて
		各種ユーザーの情報を取得します
		「フォロー」「フォロー解除」「フォロー状態取得」
		「フォロー許可受信リストの取得」「フォロー許可送信リストの取得」
		「自分がフォローしているユーザーリストの取得」
		「自分をフォローしているユーザーリストの取得」
		「指定ユーザーIDから情報を取得」
		*/
		{
			/*CreateFollowing
			指定した、キーと秘密パスのアカウントに対して
			指定したユーザーIDの相手をフォローします
			既にフォロー済みの相手でもレスポンスステータスは正常値を返します
			ユーザーが存在しない場合は「GET_HTTP_NOT_FOUND」を返します
			*/
			{
				SResponse response;
				//CUser::CreateFollowing(response , Account.mKey , Account.mSecret , AccountList[1].mID);
			}

			/*DeleteFollowing
			指定した、キーと秘密パスのアカウントがフォローしている
			指定したユーザーIDの相手をフォローを解除します
			ユーザーが存在しない場合は「GET_HTTP_NOT_FOUND」を返します
			*/
			{
				SResponse response;
				//CUser::DeleteFollowing(response , Account.mKey , Account.mSecret , id1);
			}

			/*GetUserRelations
			認証されているユーザーと指定したIDのユーザーの関係情報を取得する
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

				//認証ユーザー以外のユーザー同士も取得が可能（こちらは別のAPI消費カウントとなる)
				{
					SResponse response;
					CUserRelationPtr relation;
					//CUser::GetUserRelation(response , relation , Account.mKey , Account.mSecret , AccountList[0].mID, AccountList[2].mID);
				}
			}

			/*InconmingID
			指定した、キーと秘密パスのアカウントに対して
			フォロー許可待ち中のユーザー情報を取得する(自分が許可する側)
			第五引数のフラグにtrueを入れるとユーザーIDだけの状態の結果が返ります(処理軽減)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetIncomingID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*OutconmingID
			指定した、キーと秘密パスのアカウントが
			フォロー許可待ち中のユーザー情報を取得する(自分が許可される側)
			第五引数のフラグにtrueを入れるとユーザーIDだけの状態の結果が返ります(処理軽減)
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetOutgoingID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*FollowID
			指定した、キーと秘密パスのアカウントでフォローしているユーザーのIDを取得します
			取得件数は固定で1ページにつき5000ユーザーとなります
			最終引数のカーソルはページ番号ではありません
			何も指定しない場合一番最初からの取得となります
			その際に第二引数への出力は前後ページへのカーソルとなります(-1の時先頭)
			*/
			{
				SResponse response;
				CIDList id_List;
				SCursor cursor;
				//CUser::GetFollowID(response , id_List , cursor , Account.mKey , Account.mSecret , Account.mID);
			}

			/*AllFollowID
			指定した、キーと秘密パスのアカウントでフォローしているユーザーのIDを全て取得します
			GetFollowIDでは1回で5000件取得出来るので余程の量でなければこちらを使用しても問題はありません
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetAllFollowID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*FollowerID
			指定した、キーと秘密パスのアカウントでフォローしているユーザーのIDを取得します
			取得件数は固定で1ページにつき5000ユーザーとなります
			最終引数のカーソルはページ番号ではありません
			何も指定しない場合一番最初からの取得となります
			その際に第二引数への出力は前後ページへのカーソルとなります(-1の時先頭)
			*/
			{
				SResponse response;
				CIDList id_List;
				SCursor cursor;
				//CUser::GetFollowerID(response , id_List , cursor , Account.mKey , Account.mSecret , Account.mID);
			}

			/*AllFollowerID
			指定した、キーと秘密パスのアカウントでフォローしているユーザーのIDを全て取得します
			GetFollowIDでは1回で5000件取得出来るので余程の量でなければこちらを使用しても問題はありません
			*/
			{
				SResponse response;
				CIDList id_List;
				//CUser::GetAllFollowerID(response , id_List , Account.mKey , Account.mSecret);
			}

			/*GetUser
			指定したユーザーID(単数)からユーザー情報を取得する
			*/
			{
				//自分自身の情報取得
				{
					SResponse response;
					CUserPtr user;
					//CUser::GetUser(response , user , Account.mKey , Account.mSecret);
				}

				//IDとスクリーンネーム(@～)を指定して取得
				{
					//単数
					{
						SResponse response;
						CUserPtr user;
						//CUser::GetUser(response , user , Account.mKey , Account.mSecret , AccountList[1].mID);
					}

					//複数
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

					//文字列
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
	//! ブロック関連テスト
	void TestBlock()
	{
		/*CBlockについて
		各種ユーザーについてのブロック操作を行います
		「指定したユーザーをブロックする」「指定したユーザーのブロックを解除する」
		「指定ユーザーをブロックしているかを取得」
		「ブロックしているユーザーの情報一覧を取得する」「スパム報告を行う」
		*/
		{
			/*CreateBlocks
			指定した、キーと秘密パスのアカウントが
			指定したユーザーIDの相手をブロックします
			既にブロック済みの相手でもレスポンスステータスは正常値を返します
			ユーザーが存在しない場合は「GET_HTTP_NOT_FOUND」を返します
			*/
			{
				SResponse response;
				//CBlocks::CreateBlocks(response , Account.mKey , Account.mSecret , 139239126);
			}

			/*DeleteFollowing
			指定した、キーと秘密パスのアカウントがフォローしている
			指定したユーザーIDの相手をフォローを解除します
			ブロックしていないの相手でもレスポンスステータスは正常値を返します
			ユーザーが存在しない場合は「GET_HTTP_NOT_FOUND」を返します
			*/
			{
				SResponse response;
				//CBlocks::DeleteBlocks(response , Account.mKey , Account.mSecret , 139239126);
			}

			/*Blocking
			指定した、キーと秘密パスのアカウントがブロックしているユーザーIDを取得する
			*/
			{
				SResponse response;
				CIDList id_List;
				//CBlocks::GetBlocking(response , id_List , Account.mKey , Account.mSecret);
			}

			/*ReportSpam
			指定したIDのユーザーをスパムとして報告する
			*/
			{
				SResponse response;
				//CBlocks::ReportSpam(response , Account.mKey , Account.mSecret , 139239126);
			}
		}
	}

	//============================================================================
	//! 検索関連テスト
	void TestSearch()
	{
		/*
		指定した検索条件にマッチした物を取得する
		ここで取得したツイートは一部保管されていないものがある(RT数とか諸々)
		なのでタイムライン上で表示する程度の情報なら取得できるが
		詳細を取得する場合は、CStatusで再取得する事
		*/
		{
			/*SearchStatus
			キーワードにマッチしたツイートのリストを取得する
			*/
			{
				SResponse response;
				CStatusList status_List;				
				SGetArgs arg;
				arg.mLanguage = "ja";
				arg.mCount = 20;
				//CSearch::SearchStatus(response , status_List , Account.mKey , Account.mSecret , "ダイスル" , &arg);
			}

			/*SearchUser
			指定したユーザー名にマッチしたユーザーリストを取得する
			*/
			{
				SResponse response;
				CUserList user_List;				
				SGetArgs arg;
				arg.mCount = 20;
				//CSearch::SearchUser(response , user_List , Account.mKey , Account.mSecret , "ダイスル" , &arg);
			}

			/*CreateSearchSaveInfo
			検索条件の保存を行う
			*/
			{
				SResponse response;
				CSearchSaveInfoPtr search;
				//CSearch::CreateSearchSaveInfo(response , search , Account.mKey , Account.mSecret , "ダイスル");
			}

			/*DeleteSearchSaveInfo
			作成済みの検索条件を削除する
			*/
			{
				SResponse response;
				//CSearch::DeleteSearchSaveInfo(response , Account.mKey , Account.mSecret , 180797145);
			}

			/*GetSearchSaveInfo
			指定した検索条件番号から検索条件を取得する
			*/
			{
				SResponse response;
				CSearchSaveInfoPtr search;
				//CSearch::GetSearchSaveInfo(response , search , Account.mKey , Account.mSecret , 180797145);
			}

			/*GetAllSearchSaveInfo
			保存されている全ての検索条件を取得する
			*/
			{
				SResponse response;
				CSearchSaveInfoList search_List;
				//CSearch::GetAllSearchSaveInfo(response , search_List , Account.mKey , Account.mSecret);
			}
		}
	}

	//============================================================================
	//! トレンド関連テスト
	void TestTrends()
	{
		//指定したWOEIDの位置のトレンドを取得する(WOEIDはGetClosestPlaceで緯度、経度指定して取得できる)
		{
			SResponse response;
			CTrendsPtr trend;
			//CTrends::GetPlaceTrend(response , trend , Account.mKey , Account.mSecret , 1117099);
		}

		//地域リスト？を取得
		{
			SResponse response;
			CPlaceList place_List;
			//CTrends::GetAvailablePlace(response , place_List , Account.mKey , Account.mSecret);
		}

		//指定した緯度、経度を指定して地域を取得
		{
			//使用不可能
			SResponse response;
			CPlacePtr place;
			//CTrends::GetClosestPlace(response , place , Account.mKey , Account.mSecret , 37.781157f , 122.400612831116f);
		}
	}

	//============================================================================
	//! ジオテスト
	void TestGeo()
	{
		//場所IDから情報を取得する
		{
			SResponse response;
			CGeoPtr geo;
			//CGeo::GetGeoPlace(response , geo , Account.mKey , Account.mSecret , "df51dec6f4ee2b2c");
		}

		//経度緯度から場所リストを取得？
		{
			SResponse response;
			CGeoList geo_List;
			SGeoArgs args;
			args.mLat = 37.76893497f;
			args.mLong = -122.42284884f;
			//CGeo::GetReverseGeocode(response , geo_List , Account.mKey , Account.mSecret , args);
		}

		//指定された名前？の経度緯度から指定された経度緯度で近い所を求める？良くわからん
		{
			SResponse response;
			CGeoList geo_List;
			SGeoArgs args;
			args.mLat = 37;
			args.mLong = -122;
			args.mName = "Twitter%20HQ";
			//CGeo::GetSimilarPlaces(response , geo_List , Account.mKey , Account.mSecret , args);
		}

		//検索用の引数から一致する場所を取得？
		{
			SResponse response;
			CGeoList geo_List;
			
			//緯度、経度、クエリ(mName)、IPの何れかが入力されていないとエラーになる
			SGeoArgs args;
			//args.mLat = 37.76893497f;
			//args.mLong = -122.42284884f;
			//args.mName = "Toronto";
			args.mIP = "74.125.19.104";
			
			//CGeo::GetSearchPlaces(response , geo_List , Account.mKey , Account.mSecret , args);
		}
	}

	//============================================================================
	//! キャッシュテスト
	void TestCache()
	{
		//ツイートキャッシュ
		{
			//取得
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

			//設定
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

		//ダイレクトメッセージキャッシュ
		{
			//取得
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

			//設定
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

		//ユーザーキャッシュ
		{
			//取得
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

			//設定
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

			//ユーザーキャッシュが生存時間かどうかを調べる
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
	//! ストリームテスト
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
	//! システムテスト
	void TestSystem()
	{
		//タイムライン系統のレートリミット情報取得
		{
			/*
			こいつをfalseにしてやるとAPI1.1モードで残り回数を取得する
			尚、タイムラインの各種取得もAPI1.1モードになる
			CTimeLine::SetUseOldAPIMode(false);

			又、以下のように
			CSystem::SetRateLimitAPIFlg(true);
			でtrueを設定してやると残り回数をAPIによって取得するようになる
			*/
			SResponse response;
			STimeLineRateLimit rate_Limit;
			//CTimeLine::GetRateLimit(response , rate_Limit , Account.mKey , Account.mSecret);
		}

		//ダイレクトメッセージのレートリミット情報取得
		{
			/*
			こいつをfalseにしてやるとAPI1.1モードで残り回数を取得する
			尚、ダイレクトメッセージの各種取得もAPI1.1モードになる
			CDirectMessage::SetUseOldAPIMode(false);

			又、以下のように
			CSystem::SetRateLimitAPIFlg(true);
			でtrueを設定してやると残り回数をAPIによって取得するようになる
			*/
			SResponse response;
			SDirectMessageRateLimit rate_Limit;
			//CDirectMessage::GetRateLimit(response , rate_Limit , Account.mKey , Account.mSecret);
		}

		//短縮URLの取得
		std::string get_Url;
		{
			std::string long_Url = "http://live.nicovideo.jp/search/%E3%83%96%E3%83%AC%E3%82%A4%E3%83%96%E3%83%AB%E3%83%BC?kind=content&track=nicouni_search_keyword";
			std::string short_Url;
			//CSystem::GetShortUrl(short_Url , "o_1ao6r87ov8" , "R_df24031c9192445f69580da8aa61bb9f" , CSystem::Decode(long_Url));

			get_Url = short_Url;
		}

		//短縮URLのデコード
		{
			std::string short_Url = get_Url;
			std::string long_Url;
			//CSystem::GetLongUrl(long_Url , "o_1ao6r87ov8" , "R_df24031c9192445f69580da8aa61bb9f" , short_Url);
		}
	}

	//============================================================================
	//! 各種モジュールのテスト
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
		初期化で全取得
		*/
		CDMDetailsModule module(Account.mKey , Account.mSecret);

		//キャッシュ読み込み
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


		//APIが続く間は受信及び送信メッセージのログを取得していく
		CDMDetailsModule::CUpdateMap update_Map;
		module.UpdateOldSendMessage();
		module.UpdateOldRecvMessage();
		
		/*
		このモジュールが走り出してから最新の送受信メッセージがあればupdate_Mapに渡される
		当然APIはSend,Recv共に１回ずつ消費される(別々の消費枠)
		*/
		//module.UpdateSendMessage(update_Map);
		//module.UpdateRecvMessage(update_Map);

		//次にやり取りしたユーザーで新しい順に並んでいるユーザー情報を取得
		CIDList id_List;
		module.GetUserIDList(id_List);

		//ユーザー情報取得
		CDMDetailsModule::SUserInfo user_Info;
		module.GetUserInfo(user_Info , id_List[0]);

		/*
		ユーザーIDからＤＭのやり取りのリストを取得する
		これはやり取りの順番が新しい物から入っている
		又、構造体の「mMyselfSenderFlg」はtrueの時は自分が送ったもの
		falseの時は相手から送ってきた物になっている
		*/
		CDMDetailsModule::CMessageList msg_List;
		module.GetMessageList(msg_List , id_List[0]);

		/*
		取得テスト
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

		//キャッシュ書き出し
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
		//フォロー状態チェックモジュール
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
		module.AddFollowMember("ペルソナ" , true);
		module.AddFollowerMember("ブレイブルー" , true);
		//module.AddListMember("ブレイブルー" , true , 76223053);

		//絞り込み
		module.NarrowMember("ポケモン" , true);

		//取得
		CUserList user_List;
		module.GetMemberList(user_List);
#endif
	}
}