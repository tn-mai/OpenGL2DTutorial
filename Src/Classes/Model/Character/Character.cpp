//
//  Character.cpp
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2015/05/18.
//
//

#include "Character.h"
#include "Lib/InputManager/InputManager.h"
#include "Scene/GameMain/GameMain.h"

//=========================================================================
//	プレイヤーキャラクタークラスのメンバ関数の定義はここから書く
//=========================================================================
// コンストラクタ
CPlayerCharacter::CPlayerCharacter() {
}

// デストラクタ
CPlayerCharacter::~CPlayerCharacter() {
}

/**
 *	@desc	初期化処理
 */
bool CPlayerCharacter::init() {
	// cocos2d のクラスを継承したらコンストラクタで
	// スーパークラスの init 関数を処理しなければならないので注意
	if ( Sprite::init() == false ) {
		CCLOG( "CPlayerCharacter() Sprite::init is false !!!" ) ;
		return false ;
	}

	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================
	
	// 画像の設定
	this->setTexture(IMAGE_FILE_CHARACTER);

	// 切り取る矩形
	this->m_chip.setRect(0, 0, 48, 32);

	// 切り取る矩形の設定
	this->setTextureRect(this->m_chip);

	// 移動用データの値の初期化
	this->m_move.setPos(320, 240);
	this->m_move.setVel(0, 0);

	// プレイヤーのアニメーションデータの初期化
	this->m_anime.set(10, 4);

	// 弾発射用カウンターの初期化
	this->m_shotCounter = 0;

	// 弾発射間隔の初期化
	this->m_shotInterval = 20;

	// 衝突判定用データの初期化
	this->m_body.set(-24, 16, 24, -16);

	// ステータスの初期化
	// プレイヤーのスコアはトータルのスコアとして扱う
	this->m_status.set(20, 20, 50, 0, 2);

	// 有効フラグの初期化
	this->m_activeFlag = true;

	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================

	return true ;
}

/**
 *  @desc	プレイヤーの弾の発射
 *  @param	弾タイプ
 *  @param	修正位置X
 *  @param	修正位置Y
 *  @param	速度X
 *  @param	速度Y
 */
CBullet* CPlayerCharacter::shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY) {

	// プレイヤーが死んでいたらNULLを返して飛ばす
	if (this->m_activeFlag == false) {
		return NULL;
	}

	// プレイヤーの弾の生成
	CBullet* pBul = CBullet::create();

	// 画像の設定
	pBul->setTexture(IMAGE_FILE_BULLET);

	// 切り取る矩形
	pBul->m_chip = CChip(0, 2, 8, 8);

	// 切り取る矩形の設定
	pBul->setTextureRect(pBul->m_chip);

	// 移動データに初期位置の設定
	pBul->m_move.setPos(this->m_move.m_posX + fixX, this->m_move.m_posY + fixY);

	// 移動データに初期速度の設定
	pBul->m_move.setVel(velX, velY);

	// 衝突判定用データの初期化
	pBul->m_body.set(-4, 4, 4, -4);

	// ステータスの初期化
	pBul->m_status.set(1, 1, 20, 0);

	// 有効フラグにtrueを入れる
	pBul->m_activeFlag = true;

	return pBul;
}

// 移動に関するメンバ関数の定義
void CPlayerCharacter::moveFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 速度から位置を計算
	this->m_move.moveByVel();
}

// アニメーションに関するメンバ関数の定義
void CPlayerCharacter::animationFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// アニメーションの更新を行う
	int frame = this->m_anime.update();

	// フレーム数から表示するべきチップデータを計算する
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// 画面端判定に関するメンバ関数の定義
void CPlayerCharacter::endOfScreen() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 右端
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// プレイヤーの右端がウィンドウ幅を飛び出た（画面の右端に到達した）

		// 飛び出た分を計算（プレイヤーの右端 - ウィンドウ幅）
		float boundary = (this->m_move.m_posX + this->m_chip.size.width * 0.5f) - WINDOW_RIGHT;

		// 現在の位置から飛び出た分を引いた位置を計算
		this->m_move.m_posX = this->m_move.m_posX - boundary;
	}
	// 左端
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// プレイヤーの左端がウィンドウ幅を飛び出た（画面の左端に到達した）

		// 飛び出た分を計算（プレイヤーの左端 - ウィンドウ幅）
		float boundary = (this->m_move.m_posX - this->m_chip.size.width * 0.5f) - WINDOW_LEFT;

		// 現在の位置から飛び出た分を引いた位置を計算
		this->m_move.m_posX = this->m_move.m_posX - boundary;
	}
	// 上端
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// プレイヤーの上端がウィンドウ幅を飛び出た（画面の上端に到達した）

		// 飛び出た分を計算（プレイヤーの上端 - ウィンドウ幅）
		float boundary = (this->m_move.m_posY + this->m_chip.size.height * 0.5f) - WINDOW_TOP;

		// 現在の位置から飛び出た分を引いた位置を計算
		this->m_move.m_posY = this->m_move.m_posY - boundary;
	}
	// 下端
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// プレイヤーの下端がウィンドウ幅を飛び出た（画面の下端に到達した）

		// 飛び出た分を計算（プレイヤーの下端 - ウィンドウ幅）
		float boundary = (this->m_move.m_posY - this->m_chip.size.height * 0.5f) - WINDOW_BOTTOM;

		// 現在の位置から飛び出た分を引いた位置を計算
		this->m_move.m_posY = this->m_move.m_posY - boundary;
	}
}

// 値の反映に関するメンバ関数の定義
void CPlayerCharacter::applyFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// プレイヤーの移動用データをプレイヤーの実際の位置に反映
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// 切り取る部分の値を反映
	this->setTextureRect(this->m_chip);
}

/**
 *  @desc	相手の攻撃が自身にあたった
 *  @param	衝突してきた相手のステータスのアドレス
 */
void CPlayerCharacter::hits(CStatus* pStatus) {

	// HPを減少させる
	this->m_status.decreaseHp(pStatus->getAttackPt());

	// HPが0以下だった場合有効フラグを下げる
	if (this->m_status.getHp() <= 0) {
		this->m_activeFlag = false;
	}
}

//=========================================================================
//	バレット ( 弾 ) のクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CBullet::CBullet() {
}

// デストラクタ
CBullet::~CBullet() {
}

/**
 *	@desc	初期化処理
 */
bool CBullet::init() {

	// cocos2d のクラスを継承したらコンストラクタで
	// スーパークラスの init 関数を処理しなければならないので注意
	if (Sprite::init() == false) {
		CCLOG("CBullet() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================

	// 弾を大きくする（アニメーション見るため）
	//this->setScale(5.0f);

	// 弾のアニメーションデータの初期化
	this->m_anime.set(5, 4);

	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================

	return true;
}

// 移動に関するメンバ関数の定義
void CBullet::moveFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 移動処理を行う
	this->m_move.moveByVel();
}

// アニメーションに関するメンバ関数の定義
void CBullet::animationFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// アニメーションの更新を行う
	int frame = this->m_anime.update();

	// フレーム数から表示するべきチップデータを計算する
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// 画面外判定に関するメンバ関数の定義
void CBullet::endOfScreen() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 弾画像の左端の座標が画面右端の座標より大きくなったら
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 弾画像の右端の座標が画面左端の座標より大きくなったら
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 弾画像の下端の座標が画面上端の座標より大きくなったら
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 弾画像の上端の座標が画面下端の座標より大きくなったら
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}
}

// 値の反映に関するメンバ関数の定義
void CBullet::applyFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// 切り取る部分の値を反映
	this->setTextureRect(this->m_chip);
}

//=========================================================================
//	敵のクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CEnemyCharacter::CEnemyCharacter() {
}

// デストラクタ
CEnemyCharacter::~CEnemyCharacter() {
}

/**
 *	@desc	初期化処理
 */
bool CEnemyCharacter::init() {

	// cocos2d のクラスを継承したらコンストラクタで
	// スーパークラスの init 関数を処理しなければならないので注意
	if (Sprite::init() == false) {
		CCLOG("CEnemyCharacter() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================


	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================

	return true;
}

/**
 *  @desc	敵の弾の発射
 *  @param	弾タイプ
 *  @param	修正位置X
 *  @param	修正位置Y
 *  @param	速度X
 *  @param	速度Y
 */
CBullet* CEnemyCharacter::shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY) {

	// プレイヤーが死んでいたらNULLを返して飛ばす
	if (this->m_activeFlag == false) {
		return NULL;
	}

	// 敵の弾の生成
	CBullet* pBul = CBullet::create();

	// 画像の設定
	pBul->setTexture(IMAGE_FILE_BULLET);

	// 切り取る矩形（青色を表示）
	pBul->m_chip = CChip(0, 10, 8, 8);

	// 切り取る矩形の設定
	pBul->setTextureRect(pBul->m_chip);

	// 移動データに初期位置の設定（敵の左端から発射されるようにする）
	pBul->m_move.setPos(this->m_move.m_posX + fixX, this->m_move.m_posY + fixY);

	// 移動データに初期速度の設定
	pBul->m_move.setVel(velX, velY);

	// 衝突判定用データの初期化
	pBul->m_body.set(-4, 4, 4, -4);

	// ステータスの初期化
	pBul->m_status.set(1, 1, 1, 0);

	// 有効フラグにtrueを入れる
	pBul->m_activeFlag = true;

	return pBul;
}

// 移動に関するメンバ関数の定義
void CEnemyCharacter::moveFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 移動処理
	this->m_move.moveByVel();
}

// アニメーションに関するメンバ関数の定義
void CEnemyCharacter::animationFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// アニメーションの更新を行う
	int frame = this->m_anime.update();

	// フレーム数から表示するべきチップデータを計算する
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// 画面端判定に関するメンバ関数の定義
void CEnemyCharacter::endOfScreen() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 敵画像の左端の座標が画面右端端の座標より大きくなったら
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 敵画像の右端の座標が画面左端の座標より小さくなったら
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 敵画像の下端の座標が画面上端の座標より大きくなったら
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

	// 敵画像の上端の座標が画面下端の座標より小さくなったら
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// 有効フラグを下げる
		this->m_activeFlag = false;
	}

}

// 値の反映に関するメンバ関数の定義
void CEnemyCharacter::applyFunc() {

	//プ レイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 位置の値の反映
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// 切り取る部分の値を反映
	this->setTextureRect(this->m_chip);
}

void CEnemyCharacter::actionFunc() {

	m_pAction->actionFunc(this);
}

/**
 *  @desc	攻撃が相手にあたった
 *  @param	衝突した相手のステータスのアドレス
 */
void CEnemyCharacter::hits(CStatus * pStatus) {

	// HPを減少させる
	this->m_status.decreaseHp(pStatus->getAttackPt());

	// HPが0以下だった場合有効フラグを下げる
	if (this->m_status.getHp() <= 0) {
		this->m_activeFlag = false;
	}
}

//=========================================================================
//	エフェクトのクラスの定義はここから書く
//=========================================================================

// コンストラクタ
CEffect::CEffect() {
}

// デストラクタ
CEffect::~CEffect() {
}

/**
 *	@desc	初期化処理
 */
bool CEffect::init() {

	// cocos2d のクラスを継承したらコンストラクタで
	// スーパークラスの init 関数を処理しなければならないので注意
	if (Sprite::init() == false) {
		CCLOG("CEffect() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	ここから初期化、初期設定のコードを追加
	//
	//=========================================================================

	// エフェクトのアニメーションデータの初期化
	m_anime.set(5, 8, false);

	//=========================================================================
	//
	//	ここまでに初期化、初期設定のコードを追加
	//
	//=========================================================================

	return true;
}

// 移動に関するメンバ関数の宣言
void CEffect::moveFunc() {

	//プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 移動処理を行う
	this->m_move.moveByVel();
}

// アニメーションに関するメンバ関数の宣言
void CEffect::animationFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// アニメーションの更新を行う
	int frame = this->m_anime.update();

	// フレーム数から表示するべきチップデータを計算する
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);

	// trueならループをせずフラグを下げる
	if (m_anime.m_isEnd == true) {
		this->m_activeFlag = false;
	}
}

// 値の反映に関するメンバ関数の宣言
void CEffect::applyFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 位置の値の反映
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// 切り取る部分の値を反映
	this->setTextureRect(this->m_chip);
}