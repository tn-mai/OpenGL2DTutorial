//
//  GameMain.cpp
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2015/05/18.
//
//

#include "GameMain.h"
#include "Lib/InputManager/InputManager.h"
#include <stdlib.h>
#include <time.h>

// AudioEngineインクルード
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

/**
 *	@desc	シーンの生成
 *	@return	CMain レイヤーを内包したシーンクラスインスタンス
 *	@tips	静的メンバ関数
 */
cocos2d::Scene* CGameMain::createScene() {
	
	// シーンの生成
	cocos2d::Scene* pScene = cocos2d::Scene::create();
	
	// GameMain レイヤーの生成
	CGameMain* pLayer = CGameMain::create() ;
	
	// CMain レイヤーをシーンに取り付ける
	pScene->addChild( pLayer ) ;
	
	// 生成したシーンを返す
	return pScene ;
}

/**
 *	@desc	キーボードのキーを押した際のイベント
 *	@param	キーコード
 *	@param	イベント
 */
void CGameMain::onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event ) {

	// 入力管理に入力処理を委託
	CInputManager::getInstance()->onKeyPressed( keyCode ) ;
}

/**
 *	@desc	キーボードのキーを離した際のイベント
 *	@param	キーコード
 *	@param	イベント
 */
void CGameMain::onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event ) {

	// 入力管理に入力処理を委託
	CInputManager::getInstance()->onKeyReleased( keyCode ) ;
}

// デストラクタ
CGameMain::~CGameMain() {

	// BGMの停止
	AudioEngine::stopAll();

	// BGMの解放
	AudioEngine::uncache(SOUND_FILE_BGM);

	// 敵出撃データの配列解放
	SAFE_DELETE_ARRAY(m_pEnemyLaunchData);

	if (this->m_activeFlag == false) {

		this->m_activeFlag = true;

		return;
	}
	// AudioEngineの終了処理
	AudioEngine::end();

}

/**
 *	@desc	初期化
 *	@return	true...成功	false...失敗
 */
bool CGameMain::init() {
	
	// 親レイヤーの初期化
	if ( Layer::init() == false ) {
		return false ;
	}
	
	// 乱数生成
	srand( time( NULL ) ) ;
	
	// キーボード入力イベント受け取り設定
	this->setKeyboardEnabled(true) ;
	
	// update 関数 ( 更新処理関数 ) 呼び出し設定
	// この部分を消したりコメントアウトすると update 関数が呼ばれなくなるので注意
	this->scheduleUpdate() ;
	
	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================
    	
	//=========================================================================
	//	ステージの外部ファイルに関するコードの追加はここから
	//=========================================================================

	// ステージ1の敵出撃データのファイル名
	this->m_enemyLaunchDataFileNameList[0] = ENEMY_LAUNCH_DATA_FILE_1;

	// ステージ2の敵出撃データのファイル名
	this->m_enemyLaunchDataFileNameList[1] = ENEMY_LAUNCH_DATA_FILE_2;

	// ステージ3の敵出撃データのファイル名
	this->m_enemyLaunchDataFileNameList[2] = ENEMY_LAUNCH_DATA_FILE_3;


	//=========================================================================
	//	マップの外部ファイルに関するコードの追加はここから
	//=========================================================================

	// ステージ1の敵出撃データのファイル名
	this->m_mapLaunchDataFileNameList[0] = MAP_FILE_STAGE_1;

	// ステージ2の敵出撃データのファイル名
	this->m_mapLaunchDataFileNameList[1] = MAP_FILE_STAGE_2;

	// ステージ3の敵出撃データのファイル名
	this->m_mapLaunchDataFileNameList[2] = MAP_FILE_STAGE_3;

	//=========================================================================
	//	マップの初期設定に関するコードの追加はここから
	//=========================================================================

	// マップの生成
	this->m_pMap = CMap::create(this->m_mapLaunchDataFileNameList[this->m_mapNo - 1]);

	// 初期位置の設定
	this->m_pMap->m_scrollMove.setPos(0, 0);

	// スクロール速度の設定
	this->m_pMap->m_scrollMove.m_velX = 1.0f;

	// ループの設定（ループありの設定）
	this->m_pMap->m_isLoop = true;

	// レイヤーに取り付け
	this->addChild(this->m_pMap);

	//=========================================================================
	//	プレイヤーの初期化に関するコードの追加はここから
	//=========================================================================

	// プレイヤーキャラクターの生成
	// CPlayerCharacterクラスでオーバーライドしている静的メンバ関数create()が
	// 呼び出され、生成後CPlayerCharacterクラスのinit()メンバ関数が呼び出される
	m_pPlayerChara = CPlayerCharacter::create();

	// プレイヤーキャラクターの取り付け
	this->addChild(this->m_pPlayerChara);
    
	//=========================================================================
	//	プレイヤーバレット ( プレイヤーの弾 ) の初期化に関するコードの追加はここから
	//=========================================================================
	
	// プレイヤーの弾の配列の初期化
	for(int i = 0; i < CBullet::MAX_BULLET; i++) {

		m_pPlayerBullets[i] = NULL;
	}
	
	//=========================================================================
	//	敵の初期化に関するコードの追加はここから
	//=========================================================================

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		m_pEnemyCharas[i] = NULL;
	}
	
	//=========================================================================
	//	敵の弾の初期化に関するコードの追加はここから
	//=========================================================================

	//敵の弾の配列の初期化
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		m_pEnemyBullets[i] = NULL;
	}

	//=========================================================================
	//	敵出撃データの初期化に関するコードの追加はここから
	//=========================================================================

	// 現在参照中の出撃データ番号の初期化
	this->m_enemyLaunchIndex = 0;

	// フレームカウンターの初期化
	this->m_frameCounter = 0;

	//=========================================================================
	//	敵出撃データの読み込みはここから
	//=========================================================================

	this->loadEnemyLaunchData(this->m_enemyLaunchDataFileNameList[this->m_stageNo - 1]);

	//=========================================================================
	//	エフェクトの初期化に関するコードの追加はここから
	//=========================================================================

	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		m_pEffects[i] = NULL;
	}

	//=========================================================================
	//	アイテムの初期化に関するコードの追加はここから
	//=========================================================================

	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		m_pItems[i] = NULL;
	}

	//=========================================================================
	//	BGMの初期設定に関するコードの追加はここから
	//=========================================================================

	// BGMの再生
	this->m_bgmId = AudioEngine::play2d(SOUND_FILE_BGM);
	
	// ループの設定(true...ループする false...ループしない)
	AudioEngine::setLoop(this->m_bgmId, true);

	//=========================================================================
	//	ユーザーインターフェースの初期設定に関するコードの追加はここから
	//=========================================================================

	// HPラベルの生成と初期化
	m_pHpLabel = CHpLabel::create();
	// プレイヤーのステータスを設定
	m_pHpLabel->setStatus(&m_pPlayerChara->m_status);
	// レイヤーに取り付ける
	this->addChild(m_pHpLabel);

	// HPゲージの生成と初期化
	m_pHpGuage = CHpGuage::create();
	// プレイヤーのステータスを設定
	m_pHpGuage->setStatus(&m_pPlayerChara->m_status);
	// レイヤーに取り付ける
	this->addChild(m_pHpGuage);

	// スコアラベルの生成と初期化
	m_pScoreLabel = CScoreLabel::create();
	// スコアのステータスを設定
	m_pScoreLabel->setStatus(&m_pPlayerChara->m_status);
	// レイヤーに取り付ける
	this->addChild(m_pScoreLabel);

	// ステージラベルの生成と初期化
	m_pStageNoLabel = CStageNoLabel::create();
	// ステージを設定
	m_pStageNoLabel->setStage(&this->m_stageNo);
	// レイヤーに取り付ける
	this->addChild(m_pStageNoLabel);

	//=========================================================================
	//	ステージの初期設定に関するコードの追加はここから
	//=========================================================================

	this->m_stageMode = STAGE_MODE::UPDATE;

	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================
	
	return true ;
}

/**
 *	@desc	更新処理
 *	@param	１フレーム経過時間
 */
void CGameMain::update( float deltaTime_ ) {

	//=========================================================================
	//	ステージに関するコードの追加はここから
	//=========================================================================

	// esc キーを押したらゲーム終了
	if (inputflag.m_esc == true) {

		cocos2d::Director::getInstance()->end();
	}

	// Sキーを押したら
	if (inputflag.m_s == true) {

		// Sキーが離されていたら
		if (this->m_sKeyDownFlag == false) {
			// Sキーが押された

			this->m_sKeyDownFlag = true;

			// モードを一時停止に変更
			this->m_stageMode = STAGE_MODE::END;

			return;
		}
	}
	else {

		// Sキーが押されていたら
		if (this->m_sKeyDownFlag == true) {
			// Sキーが離された

			this->m_sKeyDownFlag = false;
		}
	}

	// Aキーを押したら
	if (inputflag.m_a == true) {

		// Aキーが離されていたら
		if (this->m_aKeyDownFlag == false) {
			// Aキーが押された

			this->m_aKeyDownFlag = true;


			// どのモードの時に
			if (this->m_stageMode == STAGE_MODE::UPDATE) {

				// モードを一時停止に変更
				this->m_stageMode = STAGE_MODE::PAUSE;
			}
			// どのモードの時に
			else if (this->m_stageMode == STAGE_MODE::PAUSE) {

				// このモードに変更
				this->m_stageMode = STAGE_MODE::UPDATE;
			}


			return;
		}
	}
	else {

		// Aキーが押されていたら
		if (this->m_aKeyDownFlag == true) {
			// Aキーが離された

			this->m_aKeyDownFlag = false;
		}
	}

	// モードによる切り替え
	switch (this->m_stageMode) {

	// ステージ開始処理
	case STAGE_MODE::START:

		this->stageStartFunc();

		break;

	// ステージ更新処理
	case STAGE_MODE::UPDATE:

		this->stageUpdateFunc(deltaTime_);

		break;

	// ステージ終了処理
	case STAGE_MODE::END:

		this->stageEndFunc();

		break;

	// ステージ一時停止処理
	case STAGE_MODE::PAUSE:

		this->stagePauseFunc();

		break;

	default:

		break;
	}


}

//=========================================================================
//	ゲームメインクラスのメンバ関数の定義はここから書く
//=========================================================================

/**
 *  @desc	敵出撃データの読み込み
 *  @param	読み込むファイル名
 *  @param	true...読み込み成功
 */
bool CGameMain::loadEnemyLaunchData(std::string fileName) {

	// ファイルを開く
	fileName = std::string("Resources/") + fileName;
	FILE* fp = fopen(fileName.c_str(), "r");

	if (fp == NULL) {
		CCLOG("ファイルが存在しません。\n");
	}

	// 敵出撃最大数
	int maxCount = 0;

	// 読み込み用の敵出撃データのインスタンス
	CEnemyLaunchData launchData;

	// 行数（敵出撃最大数）を数える為にから回しする
	while (fscanf(fp, "%d, %f, %f, %d",
		&launchData.m_frame,
		&launchData.m_posX, &launchData.m_posY,
		&launchData.m_type) != EOF) {

		maxCount++;
	}

	// 行数分（敵出撃最大数分）の配列を作成する
	this->m_pEnemyLaunchData = new CEnemyLaunchData[maxCount];

	// ファイルポインタを先頭に戻す
	rewind(fp);

	// 生成した敵出撃データの配列に読み込む
	for (int i = 0; i < maxCount; i++) {

		// 1行分ずつテータを読み込む
		fscanf(fp, "%d, %f, %f, %d",
			&this->m_pEnemyLaunchData[i].m_frame,
			&this->m_pEnemyLaunchData[i].m_posX,
			&this->m_pEnemyLaunchData[i].m_posY,
			&this->m_pEnemyLaunchData[i].m_type
		);
	}

	// ファイルを閉じる
	fclose(fp);

	// 敵出撃最大数を設定
	CEnemyLaunchData::MAX_LAUNCH = maxCount;

	return true;
}


/**
 *  @desc	敵の出撃
 *  @param	敵のタイプ
 *  @param	初期位置X
 *  @param	初期位置Y
 */
CEnemyCharacter* CGameMain::launchEnemy(CEnemyLaunchData* pEneLaunchData) {

	// 敵の生成
	CEnemyCharacter* pEne = CEnemyCharacter::create();

	switch (pEneLaunchData->m_type) {

	// タイプAの敵
	case ENEMY_TYPE::TYPE_A:

		// 画像の設定
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// 切り取る矩形
		pEne->m_chip = CChip(0, 32, 32, 32);

		// 切り取る矩形の設定
		pEne->setTextureRect(pEne->m_chip);

		// 移動データに初期位置の設定
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// 移動データに初期速度の設定（x座標に-4ずつ）
		pEne->m_move.setVel(-4, 0);

		// 敵のアニメーションデータの初期化
		pEne->m_anime.set(10, 6);

		// 衝突判定用データの初期化
		pEne->m_body.set(-16, 16, 16, -16);

		// 発射する弾のタイプの設定
		// 通常弾
		pEne->m_bulletType = BULLET_TYPE::ENEMY_NORMAL;

		// ステータスの初期化
		pEne->m_status.set(1, 1, 1, 100);

		// 行動タイプの設定
		pEne->m_pAction = new CActionControllerZigZag;

		break;

	// タイプBの敵
	case ENEMY_TYPE::TYPE_B:

		// 画像の設定
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// 切り取る矩形
		pEne->m_chip = CChip(0, 64, 48, 32);

		// 切り取る矩形の設定
		pEne->setTextureRect(pEne->m_chip);

		// 移動データに初期位置の設定
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// 移動データに初期速度の設定（x座標に-4ずつ）
		pEne->m_move.setVel(-4, 0);

		// 敵のアニメーションデータの初期化
		pEne->m_anime.set(10, 6);

		// 衝突判定用データの初期化
		pEne->m_body.set(-24, 16, 24, -16);

		// 発射する弾のタイプの設定
		// ホーミング弾
		pEne->m_bulletType = BULLET_TYPE::ENEMY_AIMING;

		// ステータスの初期化
		pEne->m_status.set(2, 2, 2, 500);

		// 行動タイプの設定
		pEne->m_pAction = new CActionControllerConvex();

		break;

	// タイプCの敵
	case ENEMY_TYPE::TYPE_C:

		// 画像の設定
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// 切り取る矩形
		pEne->m_chip = CChip(0, 96, 48, 32);

		// 切り取る矩形の設定
		pEne->setTextureRect(pEne->m_chip);

		// 移動データに初期位置の設定
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// 移動データに初期速度の設定（x座標に-4ずつ）
		pEne->m_move.setVel(-4, 0);

		// 敵のアニメーションデータの初期化
		pEne->m_anime.set(10, 6);

		// 衝突判定用データの初期化
		pEne->m_body.set(-24, 16, 24, -16);

		// 発射する弾のタイプ
		// 7Way弾
		pEne->m_bulletType = BULLET_TYPE::ENEMY_FAN;

		// ステータスの初期化
		pEne->m_status.set(5, 5, 5, 1000);

		// 行動タイプの設定
		pEne->m_pAction = new CActionControllerStraight();

		break;

	default:

		break;

	}

	// タイプ
	pEne->m_enemyType = pEneLaunchData->m_type;

	// 有効フラグにtrueを入れる
	pEne->m_activeFlag = true;

	return pEne;
}

/**
 *  @desc	エフェクトの出現
 *  @param	エフェクトのタイプ
 *  @param	出現位置X
 *  @param	出現位置Y
 *  @return	エフェクトクラスのポインタ
 */
CEffect* CGameMain::launchEffect(EFFECT_TYPE type, float posX, float posY) {

	// エフェクトの生成
	CEffect* pEff = CEffect::create();

	switch (type) {

	// プレイヤーの爆発タイプ
	case EFFECT_TYPE::PLAYER_EXPLOSION:

		// 画像の設定
		pEff->setTexture(IMAGE_FILE_EXPLOSION);

		// 切り取る矩形
		pEff->m_chip = CChip(0, 0, 48, 48);

		// 切り取る矩形の設定
		pEff->setTextureRect(pEff->m_chip);

		// 移動データに初期位置の設定
		pEff->m_move.setPos(posX, posY);

		// 移動データに初期速度の設定
		pEff->m_move.setVel(0, 0);

		AudioEngine::play2d(SOUND_FILE_PLAYER_EXPLOSION);

		break;

	// 敵の爆発タイプ
	case EFFECT_TYPE::ENEMY_EXPLOSION:

		// 画像の設定
		pEff->setTexture(IMAGE_FILE_EXPLOSION);

		// 切り取る矩形
		pEff->m_chip = CChip(0, 0, 48, 48);

		// 切り取る矩形の設定
		pEff->setTextureRect(pEff->m_chip);

		// 初期位置の設定
		pEff->m_move.setPos(posX, posY);

		// 初期速度の設定
		pEff->m_move.setVel(0, 0);

		AudioEngine::play2d(SOUND_FILE_ENEMY_EXPLOSION);

		break;

	default:

		break;
	}

	// 有効フラグにtrueを入れる
	pEff->m_activeFlag = true;

	return pEff;
}

/**
 *  @desc	アイテムの出現
 *  @param	アイテムのタイプ
 *  @param	出現位置X
 *  @param	出現位置Y
 */
CItem * CGameMain::launchItem(ITEM_TYPE type, float posX, float posY) {

	// アイテムの生成
	CItem* pItem = CItem::create();
	
	switch (type) {

	// ポーションタイプ
	case ITEM_TYPE::PORTION:

		// 画像の設定
		pItem->setTexture(IMAGE_FILE_ITEM);

		// 切り取る矩形
		pItem->m_chip = CChip(0, 0, 24, 24);

		// 切り取る矩形の設定
		pItem->setTextureRect(pItem->m_chip);

		// 移動データに初期位置の設定
		pItem->m_move.setPos(posX, posY);

		// 移動データに初期速度の設定
		pItem->m_move.setVel(-1, 0);

		// 衝突判定用データの初期化
		pItem->m_body.set(-12, 12, 12, -12);

		// アイテムのアニメーションデータの初期化
		pItem->m_anime.set(40, 2);

		//アイテムタイプ
		pItem->m_itemType = ITEM_TYPE::PORTION;

		break;

	// ブーストタイプ
	case ITEM_TYPE::BOOST:

		// 画像の設定
		pItem->setTexture(IMAGE_FILE_ITEM);

		// 切り取る矩形
		pItem->m_chip = CChip(24, 0, 24, 24);

		// 切り取る矩形の設定
		pItem->setTextureRect(pItem->m_chip);

		// 移動データに初期位置の設定
		pItem->m_move.setPos(posX, posY);

		// 移動データに初期速度の設定
		pItem->m_move.setVel(-1, 0);

		// 衝突判定用データの初期化
		pItem->m_body.set(-12, 12, 12, -12);

		// 弾のアニメーションデータの初期化
		pItem->m_anime.set(40, 2);

		//アイテムタイプ
		pItem->m_itemType = ITEM_TYPE::BOOST;

		break;

	default:

		break;
	}

	// 有効フラグにtrueを入れる
	pItem->m_activeFlag = true;

	return pItem;
}

// 敵を出撃させるかどうかのチェックに関するメンバ関数の定義
void CGameMain::checkLaunchEnemy() {

	// 参照する敵出撃データの番号が出撃最大数以上になったら出撃判定を行わない
	if (this->m_enemyLaunchIndex >= CEnemyLaunchData::MAX_LAUNCH) {
		return;
	}

	// 現在参照中の出撃データ番号の出撃テータのフレームが
	// フレームカウンターと同じなら出撃を行う
	while (this->m_frameCounter == this->m_pEnemyLaunchData[this->m_enemyLaunchIndex].m_frame) {

		// 配列のNULLが入っている添字番号を取得
		int index = getFreeIndex(m_pEnemyCharas, CEnemyCharacter::MAX_ENEMY);

		// 空いている要素が存在すれば出撃行う
		if (index != -1) {

			// 敵を出撃
			CEnemyCharacter* pEne = this->launchEnemy(
				&this->m_pEnemyLaunchData[this->m_enemyLaunchIndex]
			);

			// レイヤーに取り付ける
			this->addChild(pEne);

			// 配列にアドレスを取り付ける
			this->m_pEnemyCharas[index] = pEne;

			// 出撃を行ったら
			// 参照している出撃データの番号を次の出撃データの番号に変更する
			this->m_enemyLaunchIndex++;
		}
	}
}

/**
 *  @desc	 エフェクトの呼び出し
 *  @param	 エフェクトのタイプ
 *  @param	 出現位置X
 *  @param	 出現位置Y
 */
void CGameMain::checkLaunchEffect(EFFECT_TYPE type, float posX, float posY) {

	// 配列のNULLが入っている添字番号を取得
	int index = getFreeIndex(m_pEffects, CEffect::MAX_EFFECT);

	// 空いている要素が存在すれば出撃行う
	if (index != -1) {

		// エフェクト
		CEffect* pEff = launchEffect(type, posX, posY);

		// レイヤーに取り付ける
		this->addChild(pEff);

		// 配列にアドレスを取り付ける
		this->m_pEffects[index] = pEff;
	}
}

/**
 *  @desc	アイテムの呼び出し
 *  @param  出現位置X
 *  @param  出現位置Y
 */
void CGameMain::checkLaunchItem(ITEM_TYPE type, float posX, float posY) {

	// 配列のNULLが入っている添字番号を取得
	int index = getFreeIndex(m_pItems, CItem::MAX_ITEM);

	// 空いている要素が存在すれば出撃行う
	if (index != -1) {

		// アイテム
		CItem* pItem = launchItem(type, posX, posY);

		// レイヤーに取り付ける
		this->addChild(pItem);

		// 配列にアドレスを取り付ける
		this->m_pItems[index] = pItem;
	}
}

// 入力に関するメンバ関数の定義
void CGameMain::inputFunc() {

	// デバック用、プレイヤーが死んだ場合の救済処理
	// 死んでいるプレイヤーを生き返らせる
	if (m_pPlayerChara->m_activeFlag == false) {

		if (inputflag.m_space == true) {

			// HPを最大まで戻す
			m_pPlayerChara->m_status.recoveryHp();

			// 有効フラグをtrueにする（生き返らせる）
			m_pPlayerChara->m_activeFlag = true;
			// 表示させる
			m_pPlayerChara->setVisible(true);
		}
	}

	// プレイヤーが死んでいたら飛ばす
	if (m_pPlayerChara->m_activeFlag == false) {

		return;
	}

	// 右移動
	if (inputflag.m_right == true) {
		// 右矢印が押された

		// 速度に値を設定する
		m_pPlayerChara->m_move.m_velX = m_pPlayerChara->m_status.getSpeed();
	}
	// 左移動
	if (inputflag.m_left == true) {
		// 左矢印が押された

		// 速度に値を設定
		m_pPlayerChara->m_move.m_velX = -m_pPlayerChara->m_status.getSpeed();
	}
	// 右も左も押してない
	if (inputflag.m_right == false && inputflag.m_left == false) {
		// 右矢印、左矢印ともに離されている状態なら

		// 速度をリセットする
		m_pPlayerChara->m_move.m_velX = 0;
	}
	// 上移動
	if (inputflag.m_up == true) {
		// 上矢印が押された

		// 速度に値を設定
		m_pPlayerChara->m_move.m_velY = m_pPlayerChara->m_status.getSpeed();
	}
	// 下移動
	if (inputflag.m_down == true) {
		// 下矢印が押された

		// 速度に値を設定
		m_pPlayerChara->m_move.m_velY = -m_pPlayerChara->m_status.getSpeed();
	}
	// 上も下も押してない
	if (inputflag.m_up == false && inputflag.m_down == false) {
		// 上矢印、下矢印ともに離された状態なら

		// 速度をリセットする
		m_pPlayerChara->m_move.m_velY = 0;
	}

	// Zキー
	if (inputflag.m_z == true) {
		// zキーが押された

		// プレイヤーの弾の発射呼び出し
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_NORMAL);		
	}

	// Xキー
	if (inputflag.m_x == true) {
		// xキーが押された

		// 2Way弾の呼び出し
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_2WAY);
	}

	// Cキー
	if (inputflag.m_c == true) {
		// cキーが押された

		// 3Way弾の呼び出し
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_3WAY);
	}
}

// 移動に関するメンバ関数の定義
void CGameMain::moveFunc() {

	// マップの移動処理
	m_pMap->moveFunc();

	// プレイヤーの移動処理
	m_pPlayerChara->moveFunc();

	// プレイヤーの弾の移動処理（こっちのほうが楽）
	for (CBullet* pBul : this->m_pPlayerBullets) {

		if (pBul == NULL) {
			continue;
		}

		pBul->moveFunc();
	}

	// 敵の移動処理
	for (CEnemyCharacter* pEne : this->m_pEnemyCharas) {

		if (pEne == NULL) {
			continue;
		}

		pEne->moveFunc();
	}

	// 敵の弾の移動処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100個の移動処理を行う
		m_pEnemyBullets[i]->moveFunc();
	}

	// エフェクトの移動処理
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100個の移動処理を行う
		m_pEffects[i]->moveFunc();
	}

	// アイテムの移動処理
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100個の移動処理を行う
		m_pItems[i]->moveFunc();
	}
}

/**
 *
 *  @desc	プレイヤーと敵の弾の衝突判定
 *
 */
void CGameMain::collisionPlayerAndEnemyBullets() {

	// プレイヤーが死んでいたら飛ばす
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// プレイヤーの「衝突判定を行う際の位置」を取得
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// 敵の弾は配列なので敵の弾、１つ１つと衝突判定を取る
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 有効フラグがfalseのインスタンスも飛ばす
		if (m_pEnemyBullets[i]->m_activeFlag == false) {
			continue;
		}

		// 敵の弾1つ分の「衝突判定を行う際の位置」を取得
		CBody eneBulCollisionBody = m_pEnemyBullets[i]->m_body.getApplyPositionBody(
			m_pEnemyBullets[i]->m_move.m_posX,
			m_pEnemyBullets[i]->m_move.m_posY
		);

		// 衝突判定を行う
		if (playerCollisionBody.collision(&eneBulCollisionBody) == true) {		
			// 衝突している
			
			// プレイヤーの衝突判定後の計算を行う
			m_pPlayerChara->hits(&m_pEnemyBullets[i]->m_status);

			// 敵の弾の有効フラグをfalseにする
			m_pEnemyBullets[i]->m_activeFlag = false;
			
			// エフェクトの呼び出し
			this->checkLaunchEffect(
				EFFECT_TYPE::PLAYER_EXPLOSION,
				m_pPlayerChara->m_move.m_posX,
				m_pPlayerChara->m_move.m_posY
			);
		}
	}
}

/**
 *
 *  @desc	プレイヤーと敵の衝突判定
 *
 */
void CGameMain::collisionPlayerAndEnemies() {

	// プレイヤーが死んでいたら飛ばす
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// プレイヤーの「衝突判定を行う際の位置」を取得
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// 敵は配列なので、敵１つ１つと衝突判定を取る
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 有効フラグがfalseのインスタンスも飛ばす
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		// 敵一体分の「衝突判定を行う際の位置」を取得
		CBody eneCollisionBody = m_pEnemyCharas[i]->m_body.getApplyPositionBody(
			m_pEnemyCharas[i]->m_move.m_posX,
			m_pEnemyCharas[i]->m_move.m_posY
		);

		// 衝突判定を行う
		if (playerCollisionBody.collision(&eneCollisionBody) == true) {
			// 衝突している

			// プレイヤーの衝突判定後の計算を行う
			m_pPlayerChara->hits(&m_pEnemyCharas[i]->m_status);

			// 敵の衝突判定後の計算を行う
			m_pEnemyCharas[i]->hits(&m_pPlayerChara->m_status);

			// 敵を倒せていたらスコアの計算を行う
			if (m_pEnemyCharas[i]->m_activeFlag == false) {
				m_pPlayerChara->m_status.addScore(m_pEnemyCharas[i]->m_status.getScore());
			}

			// エフェクトの呼び出し
			this->checkLaunchEffect(
				EFFECT_TYPE::PLAYER_EXPLOSION,
				m_pPlayerChara->m_move.m_posX,
				m_pPlayerChara->m_move.m_posY
			);

			// エフェクトの呼び出し
			this->checkLaunchEffect(
				EFFECT_TYPE::ENEMY_EXPLOSION,
				m_pEnemyCharas[i]->m_move.m_posX,
				m_pEnemyCharas[i]->m_move.m_posY
			);

			// タイプCの敵のみ
			if (m_pEnemyCharas[i]->m_enemyType == ENEMY_TYPE::TYPE_C) {

				int num = rand() % 2;

				// アイテムの呼び出し
				this->checkLaunchItem(
					(ITEM_TYPE)num,
					m_pEnemyCharas[i]->m_move.m_posX,
					m_pEnemyCharas[i]->m_move.m_posY
				);
			}
		}
	}
}

/**
 *
 *  @desc	プレイヤーの弾と敵の衝突判定
 *
 */
void CGameMain::collisionEnemiesAndPlayerBullets() {

	// 敵は配列なので、敵１つ１つと衝突判定を取る
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 有効フラグがfalseのインスタンスも飛ばす
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		// 敵一体分の「衝突判定を行う際の位置」を取得
		CBody eneCollisionBody = m_pEnemyCharas[i]->m_body.getApplyPositionBody(
			m_pEnemyCharas[i]->m_move.m_posX,
			m_pEnemyCharas[i]->m_move.m_posY
		);

		// 弾は配列なので、弾１つ１つと衝突判定を取る
		for (int j = 0; j < CBullet::MAX_BULLET; j++) {

			// 要素がNULLなら飛ばす
			if (m_pPlayerBullets[j] == NULL) {
				continue;
			}

			// 有効フラグがfalseのインスタンスも飛ばす
			if (m_pPlayerBullets[j]->m_activeFlag == false) {
				continue;
			}

			// プレイヤーの弾の「衝突判定を行う際の位置」を取得
			CBody playerCollisionBullet = m_pPlayerBullets[j]->m_body.getApplyPositionBody(
				m_pPlayerBullets[j]->m_move.m_posX,
				m_pPlayerBullets[j]->m_move.m_posY
			);

			// 衝突判定を行う
			if (eneCollisionBody.collision(&playerCollisionBullet) == true) {
				// 衝突している

				// 弾の衝突判定後の計算を行う
				m_pEnemyCharas[i]->hits(&m_pPlayerBullets[j]->m_status);

				// プレイヤーの弾の有効フラグをfalseにする
				m_pPlayerBullets[j]->m_activeFlag = false;

				// 敵を倒せていたらスコアの計算を行う
				if (m_pEnemyCharas[i]->m_activeFlag == false) {
					m_pPlayerChara->m_status.addScore(m_pEnemyCharas[i]->m_status.getScore());
				}

				// エフェクトの呼び出し
				this->checkLaunchEffect(
					EFFECT_TYPE::ENEMY_EXPLOSION,
					m_pEnemyCharas[i]->m_move.m_posX,
					m_pEnemyCharas[i]->m_move.m_posY
				);

				// タイプCの敵のみ
				if (m_pEnemyCharas[i]->m_enemyType == ENEMY_TYPE::TYPE_C ) {

					int num = rand() % 2;

					// アイテムの呼び出し
					this->checkLaunchItem(
						(ITEM_TYPE)num,
						m_pEnemyCharas[i]->m_move.m_posX,
						m_pEnemyCharas[i]->m_move.m_posY
					);					
				}
			}
		}
	}
}

/**
*
*  @desc	アイテムとプレイヤーの衝突判定
*
*/
void CGameMain::collisionPlayerAndItems() {

	// プレイヤーが死んでいたら飛ばす
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// プレイヤーの「衝突判定を行う際の位置」を取得
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// アイテムは配列なので、アイテム１つ１つと衝突判定を取る
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// 要素がNULLなら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 有効フラグがfalseのインスタンスも飛ばす
		if (m_pItems[i]->m_activeFlag == false) {
			continue;
		}

		// 敵一体分の「衝突判定を行う際の位置」を取得
		CBody itemCollisionBody = m_pItems[i]->m_body.getApplyPositionBody(
			m_pItems[i]->m_move.m_posX,
			m_pItems[i]->m_move.m_posY
		);

		// 衝突判定を行う
		if (playerCollisionBody.collision(&itemCollisionBody) == true) {
			// 衝突している

			// プレイヤーの衝突判定後の計算を行う
			m_pItems[i]->hits(&m_pPlayerChara->m_status);
			
			// アイテムの有効フラグを下げる
			m_pItems[i]->m_activeFlag = false;
		}
	}
}

// アニメーションに関するメンバ関数の定義
void CGameMain::animationFunc() {

	// プレイヤーのアニメーション更新処理
	m_pPlayerChara->animationFunc();

	// プレイヤーの弾のアニメーション
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100個のアニメーション
		m_pPlayerBullets[i]->animationFunc();
	}

	// 敵のアニメーション
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100個のアニメーション
		m_pEnemyCharas[i]->animationFunc();
	}

	// 敵の弾のアニメーション
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100個のアニメーション
		m_pEnemyBullets[i]->animationFunc();
	}

	// エフェクトのアニメーション
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100個のアニメーション
		m_pEffects[i]->animationFunc();
	}

	// アイテムのアニメーション
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100個のアニメーション
		m_pItems[i]->animationFunc();
	}
}

// 画面端判定に関するメンバ関数の定義
void CGameMain::endOfScreen() {

	// プレイヤーの画面端の処理
	m_pPlayerChara->endOfScreen();

	// プレイヤーの弾の画面外判定処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100個の画面外判定
		m_pPlayerBullets[i]->endOfScreen();
	}

	// 敵の画面外判定処理
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100個の画面外判定
		m_pEnemyCharas[i]->endOfScreen();
	}

	// 敵の弾の画面外判定処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100個の画面外判定
		m_pEnemyBullets[i]->endOfScreen();
	}

	// アイテムの画面外判定処理
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100個の画面外判定
		m_pItems[i]->endOfScreen();
	}
}

// 値の反映に関するメンバ関数の定義
void CGameMain::applyFunc() {

	// マップの値の反映処理
	m_pMap->applyFunc();

	// プレイヤーの値の反映処理
	m_pPlayerChara->applyFunc();

	// プレイヤーの弾の値の反映処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {
		//NULLが代入されていたら飛ばす
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100個の値の反映を行う
		m_pPlayerBullets[i]->applyFunc();
	}

	// 敵の値の反映処理
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {
		// NULLが代入されていたら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100個の値の反映を行う
		m_pEnemyCharas[i]->applyFunc();
	}

	// 敵の弾の値の反映処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {
		// NULLが代入されていたら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100個の値の反映を行う
		m_pEnemyBullets[i]->applyFunc();
	}

	// エフェクトの値の反映処理
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100個の値の反映を行う
		m_pEffects[i]->applyFunc();
	}

	// アイテムの値の反映処理
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULLが代入されていたら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100個の値の反映を行う
		m_pItems[i]->applyFunc();
	}
}

// 取り外し処理に関するメンバ関数の定義
void CGameMain::checkActiveFlagAndRemove() {

	// プレイヤーの弾のレイヤーからの取り外し処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		if (m_pPlayerBullets[i]->m_activeFlag == false) {

			// レイヤーから取り外す
			m_pPlayerBullets[i]->removeFromParent();

			// 配列情報にNULLを代入
			m_pPlayerBullets[i] = NULL;
		}
	}

	// 敵のレイヤーからの取り外し処理
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		if (m_pEnemyCharas[i]->m_activeFlag == false) {

			// レイヤーから取り外す
			m_pEnemyCharas[i]->removeFromParent();

			// 配列情報にNULLを代入
			m_pEnemyCharas[i] = NULL;
		}
	}

	// 敵の弾のレイヤーからの取り外し処理
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		if (m_pEnemyBullets[i]->m_activeFlag == false) {
			
			// レイヤーから取り外す
			m_pEnemyBullets[i]->removeFromParent();

			// 配列情報にNULLを代入
			m_pEnemyBullets[i] = NULL;
		}
	}

	// エフェクトのレイヤーからの取り外し処理
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEffects[i] == NULL) {
			continue;
		}
		
		if (m_pEffects[i]->m_activeFlag == false) {
			
			// レイヤーから取り外す
			m_pEffects[i]->removeFromParent();

			// 配列情報にNULLを代入
			m_pEffects[i] = NULL;
		}
	}

	// アイテムのレイヤーからの取り外し処理
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// 要素がNULLなら飛ばす
		if (m_pItems[i] == NULL) {
			continue;
		}

		if (m_pItems[i]->m_activeFlag == false) {

			// レイヤーから取り外す
			m_pItems[i]->removeFromParent();

			// 配列情報にNULLを代入
			m_pItems[i] = NULL;
		}
	}
}

/**
 *  @desc	プレイヤーの弾の発射
 *  @param	弾タイプ
 */
void CGameMain::shotPlayerBullet(BULLET_TYPE type) {
	
	if (m_pPlayerChara->m_shotCounter > 0) {

		// 弾発射カウンターが0より上なら
		// 弾発射用カウンターをデクリメント
		m_pPlayerChara->m_shotCounter--;
	}

	// 弾発射用カウンターが0以下なら弾の発射を行う
	if (m_pPlayerChara->m_shotCounter <= 0) {

		switch (type) {

			// 通常弾
		case BULLET_TYPE::PLAYER_NORMAL:
		{

			// 配列のNULLが入っている添字番号を取得
			int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

			// 空いている要素が存在しなければ弾の発射処理は行わない
			if (index == -1) {
				return;
			}

			// プレイヤーの弾の発射
			CBullet* pBul = m_pPlayerChara->shotBullet(
				BULLET_TYPE::PLAYER_NORMAL,
				24.0f, 0.0f, 6.0f, 0.0f
			);

			// 要素がNULLなら飛ばす
			if (pBul == NULL) {
				return;
			}

			// レイヤーに取り付ける
			this->addChild(pBul);

			// 配列にアドレスを取り付ける
			this->m_pPlayerBullets[index] = pBul;
		}

		break;

		// 2Way弾
		case BULLET_TYPE::PLAYER_2WAY:
		{

			for (int i = 0; i < 2; i++) {

				// 配列のNULLが入っている添字番号を取得
				int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

				// 空いている要素が存在しなければ弾の発射処理は行わない
				if (index == -1) {
					return;
				}

				// 発射修正位置の計算
				float fixX = (i == 0) ? 24 : -24;

				// 速度の計算
				float velX = (i == 0) ? 6 : -6;

				// プレイヤーの弾の発射
				CBullet* pBul = m_pPlayerChara->shotBullet(
					BULLET_TYPE::PLAYER_NORMAL,
					fixX, 0.0f, velX, 0.0f
				);

				// 要素がNULLなら飛ばす
				if (pBul == NULL) {
					return;
				}

				// レイヤーに取り付ける
				this->addChild(pBul);

				// 配列にアドレスを取り付ける
				this->m_pPlayerBullets[index] = pBul;
			}
		}

		break;

		// 3Way弾
		case BULLET_TYPE::PLAYER_3WAY:
		{

			for (int i = 0; i < 3; i++) {

				// 配列のNULLが入っている添字番号を取得
				int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

				// 空いている要素が存在しなければ弾の発射処理は行わない
				if (index == -1) {
					return;
				}

				// 角度
				float angle = i * -25.0f + 25.0f;

				// 角度分の2次元ベクトルを取得
				float velX = cos(angle * M_PI / 180.0f) * 6.0f;
				float velY = sin(angle * M_PI / 180.0f) * 6.0f;

				// プレイヤーの弾の発射
				CBullet* pBul = m_pPlayerChara->shotBullet(
					BULLET_TYPE::PLAYER_3WAY,
					24.0f, 0.0f, velX, velY
				);

				// 要素がNULLなら飛ばす
				if (pBul == NULL) {
					return;
				}

				// レイヤーに取り付ける
				this->addChild(pBul);

				// 配列にアドレスを取り付ける
				this->m_pPlayerBullets[index] = pBul;
			}
		}

		break;

		default:

			break;
		}

		// 弾発射効果音の再生
		AudioEngine::play2d(SOUND_FILE_LAUNCH_BULLET);

		// 弾が発射されたらカウンターをインターバルでリセットする
		m_pPlayerChara->m_shotCounter = m_pPlayerChara->m_shotInterval;
	}
}

// 敵の弾の発射に関する定義
void CGameMain::shotEnemyBullet() {

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// NULLが代入されたらいたら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 弾発射フラグがfalseなら飛ばす
		if (m_pEnemyCharas[i]->m_shotFlag == false) {
			continue;
		}

		// 敵が持っている敵の弾のタイプを判定して弾を発射させる
		switch (m_pEnemyCharas[i]->m_bulletType) {

			// 通常弾
			case BULLET_TYPE::ENEMY_NORMAL:
			{
				
				// 配列のNULLが入っている添字番号を取得
				int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

				// 空いている要素が存在しなければ弾の発射処理は行わない
				if (index == -1) {
					return;
				}

				// 敵の弾の発射
				CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
					BULLET_TYPE::ENEMY_NORMAL,
					-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f),0, -8, 0
				);

				// 要素がNULLなら飛ばす
				if (pBul == NULL) {
					continue;
				}

				// レイヤーに取り付ける
				this->addChild(pBul);

				// 配列にアドレスを取り付ける
				this->m_pEnemyBullets[index] = pBul;
			}

				break;

			// ホーミング弾
			case BULLET_TYPE::ENEMY_AIMING:
			{

				// 配列のNULLが入っている添字番号を取得
				int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

				// 空いている要素が存在しなければ弾の発射処理は行わない
				if (index == -1) {
					return;
				}

				// プレイヤーの位置を取得
				float playerPosX = m_pPlayerChara->m_move.m_posX;
				float playerPosY = m_pPlayerChara->m_move.m_posY;

				// 敵の位置を取得
				float enePosX = m_pEnemyCharas[i]->m_move.m_posX;
				float enePosY = m_pEnemyCharas[i]->m_move.m_posY;

				// 敵からプレイヤーまでのベクトルを計算
				float vecX = playerPosX - enePosX;
				float vecY = playerPosY - enePosY;

				// ベクトルの長さを求める
				float len = sqrt(vecX * vecX + vecY * vecY);

				// 1フレーム単位のベクトルを計算する、つまり速度が1の時の速度を求める。
				float velX = vecX / len;
				float velY = vecY / len;

				// 早さを計算する
				velX = velX * 5;
				velY = velY * 5;

				// 敵の弾の発射
				CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
					BULLET_TYPE::ENEMY_NORMAL,
					-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f),0, velX, velY
				);

				// 要素がNULLなら飛ばす
				if (pBul == NULL) {
					continue;
				}

				// レイヤーに取り付ける
				this->addChild(pBul);

				// 配列にアドレスを取り付ける
				this->m_pEnemyBullets[index] = pBul;
			}

				break;

			// 7Way弾
			case BULLET_TYPE::ENEMY_FAN:
			{

				for (int j = 0; j < 7; j++) {

					// 配列のNULLが入っている添字番号を取得
					int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

					// 空いている要素が存在しなければ弾の発射処理は行わない
					if (index == -1) {
						return;
					}

					// 角度
					float angle = j * 30.0f + 90.0f;

					// 角度分の2次元ベクトルを取得
					float velX = cos(angle * M_PI / 180.0f) * 6.0f;
					float velY = sin(angle * M_PI / 180.0f) * 6.0f;

					// 敵の弾の発射
					CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
						BULLET_TYPE::ENEMY_FAN,
						-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f), 0, velX, velY
					);

					// 要素がNULLなら飛ばす
					if (pBul == NULL) {
						return;
					}

					// レイヤーに取り付ける
					this->addChild(pBul);

					// 配列にアドレスを取り付ける
					this->m_pEnemyBullets[index] = pBul;
				}
			}

				break;

			default:

				break;
		}

		// 弾発射フラグを下げる
		m_pEnemyCharas[i]->m_shotFlag = false;
	}
}

// 行動管理に関する定義
void CGameMain::actionFunc() {

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// 要素がNULLなら飛ばす
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 有効フラグがfalseなら飛ばす
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		m_pEnemyCharas[i]->actionFunc();
	}
}

// ステージ開始処理に関する定義
void CGameMain::stageStartFunc() {

	//=========================================================================
	//	BGMのステージ切り替えに関するコードの追加はここから
	//=========================================================================

	// BGMの読み込み
	AudioEngine::play2d(SOUND_FILE_BGM);

	//=========================================================================
	//	敵出撃のステージ切り替えに関するコードの追加はここから
	//=========================================================================

	// 次のステージの敵出撃データ読み込む
	this->loadEnemyLaunchData(this->m_enemyLaunchDataFileNameList[this->m_stageNo - 1]);

	//=========================================================================
	//	マップのステージ切り替えに関するコードの追加はここから
	//=========================================================================

	// 次のステージのマップの生成
	// マップの生成
	this->m_pMap = CMap::create(this->m_mapLaunchDataFileNameList[this->m_mapNo - 1]);

	// 初期位置の設定
	this->m_pMap->m_scrollMove.setPos(0, 0);

	// スクロール速度の設定
	this->m_pMap->m_scrollMove.m_velX = 1.0f;

	// ループ設定（ループあり設定）
	this->m_pMap->m_isLoop = true;

	// レイヤーに取り付ける
	this->addChild(this->m_pMap, -1);

	//=========================================================================
	//	プレイヤーのステージ切り替えに関するコードの追加はここから
	//=========================================================================

	// プレイヤーの有効フラグを上げる
	m_pPlayerChara->m_activeFlag = true;

	// プレイヤーを表示する
	m_pPlayerChara->setVisible(true);

	// プレイヤーの初期位置の設定
	m_pPlayerChara->m_move.setPos(100.0f, 240.0f);

	// 初期位置の反映
	m_pPlayerChara->setPosition(m_pPlayerChara->m_move.m_posX, m_pPlayerChara->m_move.m_posY);

	//=========================================================================
	//	UIのステージ切り替えに関するコードの追加はここから
	//=========================================================================

	// HPラベルの表示
	m_pHpLabel->setVisible(true);

	// HPゲージの表示
	m_pHpGuage->setVisible(true);

	// スコアラベルの表示
	m_pScoreLabel->setVisible(true);

	// ステージラベルの表示
	m_pStageNoLabel->setVisible(true);

	// モードをステージ更新処理に切り替える
	this->m_stageMode = STAGE_MODE::UPDATE;
}

/**
 *  @desc	ステージ更新処理
 *  @param	経過時間
 */
void CGameMain::stageUpdateFunc(float deltaTime) {

	//=========================================================================
	//
	//	ここに更新処理のコードを追加していく
	//
	//=========================================================================

	//=========================================================================
	//	入力処理に関するコードの追加はここから
	//=========================================================================

	this->inputFunc();

	//=========================================================================
	//	敵の出撃に関するコードの追加はここから
	//=========================================================================

	this->checkLaunchEnemy();

	//=========================================================================
	//	行動処理に関するコードの追加はここから
	//=========================================================================

	this->actionFunc();

	//=========================================================================
	//	移動処理に関するコードの追加はここから
	//=========================================================================

	this->moveFunc();

	//=========================================================================
	//	画面端判定処理に関するコードの追加はここから
	//=========================================================================

	this->endOfScreen();

	//=========================================================================
	//	衝突判定処理に関するコードの追加はここから
	//=========================================================================

	this->collisionPlayerAndEnemyBullets();
	this->collisionPlayerAndEnemies();
	this->collisionEnemiesAndPlayerBullets();
	this->collisionPlayerAndItems();

	//=========================================================================
	//	敵の弾の発射に関するコードの追加はここから
	//=========================================================================

	this->shotEnemyBullet();

	//=========================================================================
	//	アニメーション処理に関するコードの追加はここから
	//=========================================================================

	this->animationFunc();

	//=========================================================================
	//	非表示処理に関するコードの追加はここから
	//=========================================================================

	if (m_pPlayerChara->m_activeFlag == false) {

		//既に非表示の場合は非表示処理は行わない
		if (m_pPlayerChara->isVisible() == true) {

			//非表示にする
			m_pPlayerChara->setVisible(false);
		}
	}

	//=========================================================================
	//	取り外し処理に関するコードの追加はここから
	//=========================================================================

	this->checkActiveFlagAndRemove();

	//=========================================================================
	//	計算用データを反映に関するコードの追加はここから
	//=========================================================================

	this->applyFunc();

	//=========================================================================
	//	その他のコードの追加はここから
	//=========================================================================

	// フレームカウンターの更新
	this->m_frameCounter++;

}

// ステージ終了処理に関する定義
void CGameMain::stageEndFunc() {

	//=========================================================================
	//	BGMに関するコードの追加はここから
	//=========================================================================

	// BGMの停止
	AudioEngine::stopAll();

	//=========================================================================
	//	プレイヤーキャラクターに関するコードの追加はここから
	//=========================================================================

	// プレイヤーの有効フラグを下げて画面から非表示
	if (m_pPlayerChara != NULL) {

		// 有効フラグもゲームの作りによっては下げる必要もない
		m_pPlayerChara->m_activeFlag = false;

		// プレイヤーは使いまわすのでいちいち削除せず非表示にするだけ
		m_pPlayerChara->setVisible(false);
	}

	//=========================================================================
	//	プレイヤーバレットに関するコードの追加はここから
	//=========================================================================

	// プレイヤーバレットをレイヤーから取り外してNULLを代入
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		m_pPlayerBullets[i]->removeFromParent();
		m_pPlayerBullets[i] = NULL;
	}

	//=========================================================================
	//	敵キャラクターに関するコードの追加はここから
	//=========================================================================

	// 敵をレイヤーから取り外してNULLを代入
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		m_pEnemyCharas[i]->removeFromParent();
		m_pEnemyCharas[i] = NULL;
	}

	//=========================================================================
	//	エネミーバレットに関するコードの追加はここから
	//=========================================================================

	// エネミーバレットをレイヤーから取り外してNULLを代入
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		m_pEnemyBullets[i]->removeFromParent();
		m_pEnemyBullets[i] = NULL;
	}

	//=========================================================================
	//	エフェクトに関するコードの追加はここから
	//=========================================================================

	// エフェクトをレイヤーから取り外してNULLを代入
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		if (m_pEffects[i] == NULL) {
			continue;
		}

		m_pEffects[i]->removeFromParent();
		m_pEffects[i] = NULL;
	}

	//=========================================================================
	//	アイテムに関するコードの追加はここから
	//=========================================================================

	// アイテムをレイヤーから取り外してNULLを代入
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		if (m_pItems[i] == NULL) {
			continue;
		}

		m_pItems[i]->removeFromParent();
		m_pItems[i] = NULL;
	}

	//=========================================================================
	//	敵出撃データに関するコードの追加はここから
	//=========================================================================

	// 敵出撃データの解放
	SAFE_DELETE_ARRAY(m_pEnemyLaunchData);

	// 敵出撃インデックスのリセット
	m_enemyLaunchIndex = 0;

	// フレームカウンターのリセット
	m_frameCounter = 0;

	//=========================================================================
	//	マップに関するコードの追加はここから
	//=========================================================================

	// マップレイヤーから取り外してNULLを代入
	if (m_pMap != NULL) {
		
		m_pMap->removeFromParent();
		m_pMap = NULL;
	}

	//=========================================================================
	//	UIに関するコードの追加はここから
	//=========================================================================

	// 全てのUIを一旦非表示にする
	m_pHpLabel->setVisible(false);
	m_pHpGuage->setVisible(false);
	m_pScoreLabel->setVisible(false);
	m_pStageNoLabel->setVisible(false);

	//=========================================================================
	//	ステージの切り替えに関するコードの追加はここから
	//=========================================================================

	if (this->m_stageNo >= CGameMain::MAX_STAGE_COUNT) {

		// TODO:ここで「エンディングシーン」に進む
	}
	else {

		// ステージを更新（インクリメントするだけ）
		this->m_stageNo++;

		this->m_mapNo++;

		this->m_activeFlag = true;

		if (this->m_activeFlag == true) {

			this->m_activeFlag = false;

			// モードをステージ開始に切り替える
			this->m_stageMode = STAGE_MODE::START;
		}
	}
}

// ステージ一時停止処理に関する定義
void CGameMain::stagePauseFunc() {

}