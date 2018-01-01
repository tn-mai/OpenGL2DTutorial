//
//  GameMain.h
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2015/05/18.
//
//

#ifndef __ShootingGame__Main__
#define __ShootingGame__Main__

#include "cocos2d.h"

//=========================================================================
//	追加のインクルードはここから
//=========================================================================

#include "Constants.h"
#include "Model/Character/Character.h"
#include "Data/EnemyLaunchData/EnemyLaunchData.h"
#include "Model/UserInterface/UserInterface.h"
#include "Model/Item/Item.h"
#include "Model/Map/Map.h"

//=========================================================================
//	ステージに関するタイプなどの列挙はここから書いていく
//=========================================================================

// ステージモード
enum class STAGE_MODE {

	// ステージモード無し
	NONE = -1,

	// ステージ開始処理
	START,

	// ステージ更新処理中
	UPDATE,

	// ステージ終了処理
	END,

	// ステージ一時停止中
	PAUSE,
};

/*
 *	ゲームメインレイヤー
 *
 *		ゲームのメインとなるレイヤー
 *		初めはこの部分に全てを書いていく
 *
 */
class CGameMain : public cocos2d::Layer {
public:
	//=========================================================================
	//	ここからは理解出来るまでは変更禁止
	//=========================================================================

	// デストラクタ
	~CGameMain() ;
	
	/**
	 *	@desc	シーンの生成
	 *	@return	CMain レイヤーを内包したシーンクラスインスタンス
	 *	@tips	静的メンバ関数
	 */
    static cocos2d::Scene* createScene() ;
	
	/*
	 *	@desc	シーン生成時に初期化関数を処理
	 *			初期化関数失敗時は NULL を返す
	 */
    CREATE_FUNC( CGameMain );
	
	/**
	 *	@desc	キーボードのキーを押した際のイベント
	 *	@param	キーコード
	 *	@param	イベント
	 */
	virtual void onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event ) override ;
	
	/**
	 *	@desc	キーボードのキーを離した際のイベント
	 *	@param	キーコード
	 *	@param	イベント
	 */
	virtual void onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event ) override ;
	
	/**
	 *	@desc	初期化
	 *	@return	true...成功	false...失敗
	 */
    virtual bool init() override ;

	/**
	 *	@desc	更新処理
	 *	@param	経過時間
	 */
	virtual void update( float deltaTime_ ) override ;
	
	//=========================================================================
	//	ここまでは理解出来るまでは変更禁止
	//=========================================================================
	
public:

	//=========================================================================
	//	メンバ宣言
	//		メイン内で使用するメンバはここ以降に記述していく
	//=========================================================================
	
	// プレイヤーキャラクター
	CPlayerCharacter* m_pPlayerChara;
	
	// 敵キャラクター
	CEnemyCharacter* m_pEnemyCharas[CEnemyCharacter::MAX_ENEMY];

	// プレイヤーバレット ( プレイヤーの弾 )
	CBullet* m_pPlayerBullets[CBullet::MAX_BULLET];

	// エネミーバレット ( 敵の弾 )
	CBullet* m_pEnemyBullets[CBullet::MAX_BULLET];

	// 敵出撃データ
	CEnemyLaunchData* m_pEnemyLaunchData;

	// エフェクト
	CEffect* m_pEffects[CEffect::MAX_EFFECT];

	// アイテム
	CItem* m_pItems[CItem::MAX_ITEM];

	// HPラベル
	CHpLabel* m_pHpLabel;

	// HPゲージ
	CHpGuage* m_pHpGuage;

	// スコアラベル
	CScoreLabel* m_pScoreLabel;

	// ステージラベル
	CStageNoLabel* m_pStageNoLabel;

	// マップ
	CMap* m_pMap = NULL;

	// ステータス
	CStatus m_status;
	
	// ステージモード
	STAGE_MODE m_stageMode = STAGE_MODE::NONE;

	// ステージ最大数
	static const int MAX_STAGE_COUNT = 3;

	// マップ最大数
	static const int MAX_MAP_COUNT = 3;

	// 現在ステージ
	int m_stageNo = 1;

	// 現在のマップ
	int m_mapNo = 1;

	// 現在何番目の出撃データを調べているのかの番号
	int m_enemyLaunchIndex;

	// フレームカウンター
	int m_frameCounter;

	// BGMの識別ID
	int m_bgmId;

	// ステージ分の敵出撃データのファイル名の配列（ステージ最大数分）
	std::string m_enemyLaunchDataFileNameList[CGameMain::MAX_STAGE_COUNT];

	// ステージ分のマップデータのファイル名の配列（マップ最大数分）
	std::string m_mapLaunchDataFileNameList[CGameMain::MAX_MAP_COUNT];

	/**
	 *  @desc	敵出撃データの読み込み
	 *  @param	読み込むファイル名
	 *  @param	true...読み込み成功
	 */
	bool loadEnemyLaunchData(std::string fileName);

	// Aキー多重判定防止フラグ（押されていたらtrue）
	bool m_aKeyDownFlag = false;

	// Sキー多重判定防止フラグ
	bool m_sKeyDownFlag = false;

	// 有効フラグ
	bool m_activeFlag;

	/**
	 *  @desc	敵の出撃
	 *  @param	参照する敵管理データのアドレス
	 */
	CEnemyCharacter* launchEnemy(CEnemyLaunchData* pEneLaunchData);

	/**
	 *  @desc	 エフェクトの出現
	 *  @param	 エフェクトのタイプ
	 *  @param	 出現位置X
	 *  @param	 出現位置Y
	 *  @return	 エフェクトクラスのポインタ
	 */
	CEffect* launchEffect(EFFECT_TYPE type, float posX, float posY);

	/**
	 *  @desc	アイテムの出現
	 *  @param	アイテムのタイプ
	 *  @param	出現位置X
	 *  @param	出現位置Y
	 */
	CItem* launchItem(ITEM_TYPE type, float posX, float posY);

	// 敵を出撃させるかどうかのチェックに関するメンバ関数
	void checkLaunchEnemy();

	/**
	 *  @desc	 エフェクトの呼び出し
	 *  @param	 エフェクトのタイプ
	 *  @param	 出現位置X
	 *  @param	 出現位置Y
	 */
	void checkLaunchEffect(EFFECT_TYPE type, float posX, float posY);

	/**
	 *  @desc	アイテムの呼び出し
	 *  @param	アイテムのタイプ
	 *  @param  出現位置X
	 *  @param  出現位置Y
	 */
	void checkLaunchItem(ITEM_TYPE type, float posX, float posY);

	// 入力に関するメンバ関数
	void inputFunc();
		
	// 移動に関するメンバ関数
	void moveFunc();
	
	// プレイヤーと敵の弾の衝突
	void collisionPlayerAndEnemyBullets();

	// プレイヤーと敵の衝突
	void collisionPlayerAndEnemies();

	// 敵とプレイヤーの弾の衝突
	void collisionEnemiesAndPlayerBullets();

	//プレイヤーとアイテムの衝突
	void collisionPlayerAndItems();
	
	// アニメーションに関するメンバ関数
	void animationFunc();
	
	// 画面端判定に関するメンバ関数
	void endOfScreen();

	// 値の反映に関するメンバ関数
	void applyFunc();

	// 取り外し処理に関するメンバ関数
	void checkActiveFlagAndRemove();

	/**
	 *  @desc	プレイヤーの弾の発射
	 *  @param	弾タイプ
	 */
	void shotPlayerBullet(BULLET_TYPE type);

	// 敵の弾の発射に関する宣言
	void shotEnemyBullet();

	// 行動管理に関する宣言
	void actionFunc();

	// ステージ開始処理
	void stageStartFunc();

	/**
	 *  @desc	ステージ更新処理
	 *  @param	経過時間
	 */
	void stageUpdateFunc(float deltaTime);

	// ステージ終了処理
	void stageEndFunc();

	// ステージ一時停止処理
	void stagePauseFunc();
} ;

#endif /* defined(__ShootingGame__Main__) */