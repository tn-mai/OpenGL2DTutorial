//
//  Character.h
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2015/05/18.
//
//

#ifndef __ShootingGame__Character__
#define __ShootingGame__Character__

#include "cocos2d.h"
#include "Constants.h"
#include "Data/Status/Status.h"
#include "Data/ActionController/ActionController.h"

//=========================================================================
//	キャラクターに関するタイプなどの列挙はここから書いていく
//=========================================================================

// 敵のタイプ
enum class ENEMY_TYPE : int {
	TYPE_A,
	TYPE_B,
	TYPE_C,
};

// 弾のタイプ
enum class BULLET_TYPE {

	/*
	 *  プレイヤーの弾のタイプ
	 */
	// 通常弾
	PLAYER_NORMAL = 1000,

	// 2Way弾
	PLAYER_2WAY,

	// 3Way弾
	PLAYER_3WAY,

	/*
	 *  敵の弾のタイプ
	 */
	// 通常弾
	ENEMY_NORMAL = 2000,

	// ホーミング弾
	ENEMY_AIMING,

	// 7Way弾
	ENEMY_FAN,
};

// 爆発のタイプ
enum class EFFECT_TYPE {

	// プレイヤーの爆発
	PLAYER_EXPLOSION,

	// 敵の爆発
	ENEMY_EXPLOSION,
};

/*
 *	チップクラス
 *	cocos2d::Rect を typedef して使用する
 *	typedef とは「型」を「別名の型」でも使用出来るようにする指定子
 *
 *	構文
 *		typedef 型 別名の型 ;
 *
 *	今まで通りの型でも宣言できるし、別名の型でも宣言する事ができる。
 *
 *		typedef cocos2d::Rect CRect ;
 *
 *	とすれば、
 *
 *		cocos2d::Rect body ;
 *		CRect body ;
 *
 *	の２通りで宣言する事ができる。
 */
typedef cocos2d::Rect CChip ;

//=========================================================================
//	移動用クラスのコードはここから書く
//=========================================================================

/*
 *　移動用データクラス
 */
class CMove {
public:
	float m_posX;	// 位置X座標
	float m_posY;	// 位置Y座標
	float m_velX;	// 速度X
	float m_velY;	// 速度Y

	/**
	 *  @desc	位置の設定
	 *  @param	X座標
	 *  @param	Y座標
	 */
	void setPos(float posX, float posY) {
		m_posX = posX;
		m_posY = posY;
	}

	/**
	 *  @desc	速度の設定
	 *  @param	速度X
	 *  @param	速度Y
	 */
	void setVel(float velX, float velY) {
		m_velX = velX;
		m_velY = velY;
	}

	/**
	 *  @desc	速度による移動
	 */
	void moveByVel() {
		// 次の位置 = 現在の位置 + 移動する量
		m_posX = m_posX + m_velX;
		m_posY = m_posY + m_velY;
	}
};

//=========================================================================
//	アニメーション用クラスのコードはここから書く
//=========================================================================
/*
 *  アニメーション用クラス
 *　  アニメーションする画像が横に並んでいる場合のみに限る
 */
class CAnimation {
public:
	int m_counter;	// カウンター
	int m_interval; // 切り替え間隔（パラパラ漫画の速度になる）
	int m_number;	// アニメーション枚数
	int m_frame;	// 現在のフレーム確認用

	// ループするかどうかのフラグ
	// true...ループする
	bool m_isLoop;

	// アニメーションが終了したかどうかのフラグ
	// true...終了した
	bool m_isEnd;

	// コンストラクタ
	CAnimation() {
		// アニメーションが終了したかどうかのフラグの初期化
		this->m_isEnd = false;
	}

	/**
	 *  @desc	設定用
	 *  @param	切り替え間隔
	 *  @param	アニメーション枚数
	 *  @param	true...ループする	false...ループしない
	 */
	void set(int interval, int number, bool isLoop = true) {
		this->m_counter = 0;
		this->m_interval = interval;
		this->m_number = number;
		this->m_isLoop = isLoop;
	}

	/**
	 *  @desc	アニメーションの更新処理
	 *  @return	表示するフレーム数
	 */
	int update() {

		// アニメーションが終了していなければカウンターの更新を行う
		if (this->m_isEnd == false) {

			this->m_counter++;

			// カウンターの数がアニメーションさせる枚数以上になったらカウンターをリセットする
			if (this->m_counter >= this->m_interval * this->m_number) {

				if (this->m_isLoop == true) {
					// ループするアニメーションはカウンターをリセットする
					this->m_counter = 0;
				}
				else {
					// アニメーションが終了したかどうかのフラグに対してtrueを入れる
					this->m_isEnd = true;

					// ループしないアニメーションに対しては飛び出たカウンターをデクリメントする
					this->m_counter--;
				}
			}
		}

		// 表示するフレーム数
		m_frame = this->m_counter / this->m_interval;

		return m_frame;
	}
};

// 前方宣言
class CBullet;

//=========================================================================
//	衝突判定用クラスのコードはここから書く
//=========================================================================

class CBody {
public:
	float m_left;		// 左の位置
	float m_top;		// 上の位置
	float m_right;		// 右の位置
	float m_bottom;		// 下の位置

	// コンストラクタ
	CBody() {

	}

	/**
	 *  @desc	設定用
	 *  @param	左の設定
	 *  @param	上の設定
	 *  @param	右の設定
	 *  @param	下の設定
	 */
	void set(float left, float top, float right, float bottom) {
		this->m_left = left;
		this->m_top = top;
		this->m_right = right;
		this->m_bottom = bottom;
	}

	/**
	 *  @desc	 衝突判定
	 *  @param	 衝突をしてくる対象のデータ
	 *  @return	 true...衝突している	false...衝突していない
	 *  @tips	 「自分(this)」というのが「衝突される対象」
	 *			 引数の「対象(pBody)」というのが「衝突してくる対象」
	 */
	bool collision(CBody* pBody) {

		// 自分の右側と対象の左側と自分の左側と対象の右側
		if (this->m_right > pBody->m_left && this->m_left < pBody->m_right) {

			// 自分の上側と対象の下側と自分の下側と対象の上側
			if (this->m_top > pBody->m_bottom && this->m_bottom < pBody->m_top) {
				// 重なった（衝突している）
				return true;
			}
		}

		// 衝突していない
		return false;
	}

	/**
	 *  @desc	 位置を反映した衝突判定のデータを取得
	 *  @param	 位置X
	 *  @param	 位置Y
	 *  @return  位置を反映した衝突判定用データ
	 */
	CBody getApplyPositionBody(float posX, float posY) {

		// 新しく衝突判定用データを生成
		CBody body;

		// 「衝突判定を行う大きさ」に位置を反映
		body.set(
			posX + this->m_left,
			posY + this->m_top,
			posX + this->m_right,
			posY + this->m_bottom
		);

		// 「衝突判定を行う際の位置」を返す
		return body;
	}
};

/*
 *
 *	プレイヤーキャラクタークラス
 *
 *		cocos2d::Sprite クラスを継承して使用している
 *		インスタンスを生成する際は 静的メンバ関数 create を使用できるが
 *		引数無しの create しか用意されていないので注意
 *
 */
class CPlayerCharacter : public cocos2d::Sprite {
public:

	//=========================================================================
	//	ここからは理解出来るまでは変更禁止
	//=========================================================================
	
	// コンストラクタの宣言
	CPlayerCharacter() ;
	// デストラクタの宣言
	~CPlayerCharacter() ;
	
	CREATE_FUNC( CPlayerCharacter ) ;
	
	/**
	 *	@desc	初期化処理
	 */
	virtual bool init() override ;

	//=========================================================================
	//	ここまでは理解出来るまでは変更禁止
	//=========================================================================
	
public:

	//=========================================================================
	//	メンバ宣言
	//		メイン内で使用するメンバはここ以降に記述していく
	//=========================================================================
	
	// 移動用データ
	CMove m_move;

	// チップデータ
	CChip m_chip;
	
	// アニメーションデータ
	CAnimation m_anime;

	// 衝突判定用データ
	CBody m_body;

	// ステータス
	CStatus m_status;
	
	// 有効フラグ
	bool m_activeFlag;

	// 弾発射用カウンター
	int m_shotCounter;

	// 弾発射間隔
	int m_shotInterval;

	/**
	 *  @desc	プレイヤーの弾の発射
	 *  @param	弾タイプ
	 *  @param	修正位置X
	 *  @param	修正位置Y
	 *  @param	速度X
	 *  @param	速度Y
	 */
	CBullet* shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY);

	// 移動に関するメンバ関数宣言
	void moveFunc();
	
	// アニメーションに関するメンバ関数宣言
	void animationFunc();
	
	// 画面端判定に関するメンバ関数宣言
	void endOfScreen();
	
	// 値の反映に関するメンバ関数宣言
	void applyFunc();

	/**
	 *  @desc	相手の攻撃が自身にあたった
	 *  @param	衝突してきた相手のステータスのアドレス
	 */
	void hits(CStatus* pStatus);
};

//=========================================================================
//	バレット ( 弾 ) のクラスはここから書く
//=========================================================================

class CBullet : public cocos2d::Sprite {
public:

	//=========================================================================
	//	ここからは理解出来るまでは変更禁止
	//=========================================================================

	// コンストラクタの宣言
	CBullet();
	// デストラクタの宣言
	~CBullet();

	CREATE_FUNC(CBullet);

	/**
	 *	@desc	初期化処理
	 */
	virtual bool init() override;

	//=========================================================================
	//	ここまでは理解出来るまでは変更禁止
	//=========================================================================

public:

	//=========================================================================
	//	メンバ宣言
	//		メイン内で使用するメンバはここ以降に記述していく
	//=========================================================================

	// 移動用データ
	CMove m_move;

	// チップデータ
	CChip m_chip;

	// アニメーションデータ
	CAnimation m_anime;

	// 衝突判定用データ
	CBody m_body;

	// ステータス
	CStatus m_status;

	// 有効フラグ（false...レイヤーから取り外される）
	bool m_activeFlag;

	// プレイヤーの弾の発射できる最大数
	static const int MAX_BULLET = 100;

	// 移動に関するメンバ関数宣言
	void moveFunc();

	// アニメーションに関するメンバ関数宣言
	void animationFunc();

	// 画面端判定に関するメンバ関数宣言
	void endOfScreen();

	// 値の反映に関するメンバ関数宣言
	void applyFunc();
};

//=========================================================================
//	敵のクラスはここから書く
//=========================================================================

class CEnemyCharacter : public cocos2d::Sprite {
public:

	//=========================================================================
	//	ここからは理解出来るまでは変更禁止
	//=========================================================================

	// コンストラクタの宣言
	CEnemyCharacter();
	// デストラクタの宣言
	~CEnemyCharacter();

	CREATE_FUNC(CEnemyCharacter);

	/**
	 *	@desc	初期化処理
	 */
	virtual bool init() override;

	//=========================================================================
	//	ここまでは理解出来るまでは変更禁止
	//=========================================================================

public:

	//=========================================================================
	//	メンバ宣言
	//		メイン内で使用するメンバはここ以降に記述していく
	//=========================================================================

	// 移動用データ
	CMove m_move;

	// チップデータ
	CChip m_chip;

	// アニメーションデータ
	CAnimation m_anime;

	// 衝突判定用データ
	CBody m_body;

	// ステータス
	CStatus m_status;

	// 敵のタイプ
	ENEMY_TYPE m_enemyType;

	// 発射する弾のタイプ
	BULLET_TYPE m_bulletType;

	CAction* m_pAction = NULL;

	// 有効フラグ（false...レイヤーから取り外される）
	bool m_activeFlag;

	// 弾発射フラグ
	bool m_shotFlag = false;

	// 敵の最大数
	static const int MAX_ENEMY = 100;

	// 現在の行動ステップ
	int m_actionStep = 0;

	/**
	 *  @desc	敵の弾の発射
	 *  @param	弾タイプ
	 *  @param	修正位置X
	 *  @param	修正位置Y
	 *  @param	速度X
	 *  @param	速度Y
	 */
	CBullet* shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY);

	// 移動に関するメンバ関数宣言
	void moveFunc();

	// アニメーションに関するメンバ関数宣言
	void animationFunc();

	// 画面端判定に関するメンバ関数宣言
	void endOfScreen();

	// 値の反映に関するメンバ関数宣言
	void applyFunc();

	// 行動に関するメンバ関数宣言
	void actionFunc();

	/**
	 *  @desc	攻撃が相手にあたった
	 *  @param	衝突した相手のステータスのアドレス
	 */
	void hits(CStatus* pStatus);
};

//=========================================================================
//	エフェクトのクラスはここから書く
//=========================================================================

class CEffect : public cocos2d::Sprite {
public:

	//=========================================================================
	//	ここからは理解出来るまでは変更禁止
	//=========================================================================

	// コンストラクタの宣言
	CEffect();
	// デストラクタの宣言
	~CEffect();

	CREATE_FUNC(CEffect);

	/**
	 *	@desc	初期化処理
	 */
	virtual bool init() override;

	//=========================================================================
	//	ここまでは理解出来るまでは変更禁止
	//=========================================================================

public:

	//=========================================================================
	//	メンバ宣言
	//		メイン内で使用するメンバはここ以降に記述していく
	//=========================================================================

	// 移動用データ
	CMove m_move;

	// チップデータ
	CChip m_chip;

	// アニメーションデータ
	CAnimation m_anime;

	// エフェクトタイプ
	EFFECT_TYPE m_effectType;

	// 有効フラグ
	bool m_activeFlag;

	// エフェクトの最大数
	static const int MAX_EFFECT = 100;

	// 移動に関するメンバ関数宣言
	void moveFunc();

	// アニメーションに関するメンバ関数宣言
	void animationFunc();

	// 値の反映に関するメンバ関数宣言
	void applyFunc();
};

#endif /* defined(__ShootingGame__Character__) */