#include "Title.h"
#include "Constants.h"
#include "Scene/GameMain/GameMain.h"

// AudioEngineインクルード
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

// cocos2d::の省略
using namespace cocos2d;

// デストラクタ
CTitle::~CTitle() {

	if (this->m_activeFlag == false) {
		return;
	}
	// AudioEngineの終了処理
	AudioEngine::end();
}

/**
 *  @desc		シーンの生成
 *  @return		CTitleレイヤーを内包したシーンクラスインスタンス
 *  @tips		静的メンバ関数
 */
cocos2d::Scene* CTitle::createScene() {

	// シーンの生成
	cocos2d::Scene* pScene = cocos2d::Scene::create();

	// CTitleレイヤーの生成
	CTitle* pLayer = CTitle::create();

	// CTitleレイヤーをシーンに取り付ける
	pScene->addChild(pLayer);

	// 生成したシーンを返す
	return pScene;
}

/**
*  @desc		初期化
*  @return		true...成功	false...失敗
*/
bool CTitle::init() {

	// 基底レイヤーの初期化
	if (Layer::init() == false) {

		return false;
	}

	// update関数（更新処理関数）呼び出し設定
	this->scheduleUpdate();

	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================

	//=========================================================================
	//	背景に関するコードの追加はここから
	//=========================================================================

	// タイトル背景の生成と取り付け
	cocos2d::Sprite* pBg = cocos2d::Sprite::create(IMAGE_FILE_TITLE_BG);

	// 初期位置の設定
	pBg->setPosition(320, 240);

	// レイヤーに取り付け
	this->addChild(pBg);

	//=========================================================================
	//	タイトルラベルに関するコードの追加はここから
	//=========================================================================

	// タイトルラベルの生成と取り付け
	Label* pTitleLabel = Label::createWithTTF("ShootingGame", FONT_FILE_STARWARS, 62);

	// 初期位置の設定
	pTitleLabel->setPosition(320, 360);

	// 文字色の設定
	pTitleLabel->setColor(Color3B(0, 200, 200));

	// レイヤーに取り付け
	this->addChild(pTitleLabel);

	//=========================================================================
	//	メニューに関するコードの追加はここから
	//=========================================================================

	/*
	 *  @desc		メニューアイテムの生成　スタートボタン
	 *  @param		通常の画像を設定
	 *  @param		押された時の画像を設定
	 *  @param		押された時に呼び出されるメンバ関数の設定
	 */
	MenuItemImage* pStartBtnItem = MenuItemImage::create(
		IMAGE_FILE_START_BUTTON,
		IMAGE_FILE_START_BUTTON_SELECTED,
		CC_CALLBACK_1(CTitle::callbackChangeGameMain, this)
	);

	// 位置の設定
	pStartBtnItem->setPosition(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.4f);

	/*
	 *  @desc		メニューアイテムの生成　終了ボタン
	 *  @param		通常の画像を設定
	 *  @param		押された時の画像を設定
	 *  @param		押された時に呼び出されるメンバ関数の設定
	 */
	MenuItemImage* pEndBtnItem = MenuItemImage::create(
		IMAGE_FILE_END_BUTTON,
		IMAGE_FILE_END_BUTTON_SLECTED,
		CC_CALLBACK_1(CTitle::callbackEndGame, this)
	);

	// 位置の設定
	pEndBtnItem->setPosition(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.2f);

	// メニューの生成とアイテムの登録
	Menu* pMenu = Menu::create(pStartBtnItem, pEndBtnItem, NULL);

	// 位置の初期化
	pMenu->setPosition(0, 0);

	// レイヤーにメニューを登録する
	this->addChild(pMenu);

	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================

	return true;
}

/**
*  @desc		更新処理
*  @param		経過時間
*/
void CTitle::update(float deltaTime) {

	//=========================================================================
	//
	//	ここから更新処理のコードを追加
	//
	//=========================================================================


}

/**
 *  @desc		ゲームメインに遷移
 *  @param		タイトルレイヤーのインスタンス
 *  @tips		スタートボタンが押された時に呼び出される
 */
void CTitle::callbackChangeGameMain(cocos2d::Ref * pSender) {

	OutputDebugString(L"スタートボタンが押されたよ\n");

	// シーンを生成する
	Scene* pScene = CGameMain::createScene();

	this->m_activeFlag = true;

	if (this->m_activeFlag == true) {

		this->m_activeFlag = false;

		// シーンを切り替える
		Director::getInstance()->replaceScene(pScene);
	}
}

/**
 *  @desc		ゲーム終了
 *  @param		タイトルレイヤーのインスタンス
 *  @param		ゲーム終了ボタンが押された時に呼び出される
 */
void CTitle::callbackEndGame(cocos2d::Ref * pSender) {

	OutputDebugString(L"ゲーム終了ボタンが押されたよ\n");

	Director::getInstance()->end();
}
