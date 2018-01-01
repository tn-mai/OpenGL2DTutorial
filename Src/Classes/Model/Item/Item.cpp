#include "Item.h"

//=========================================================================
//	エフェクトのクラスの定義はここから書く
//=========================================================================

// コンストラクタ
CItem::CItem() {
}

// デストラクタ
CItem::~CItem() {
}

/**
*	@desc	初期化処理
*/
bool CItem::init() {

	// cocos2d のクラスを継承したらコンストラクタで
	// スーパークラスの init 関数を処理しなければならないので注意
	if (Sprite::init() == false) {
		CCLOG("CItem() Sprite::init is false !!!");
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

// 移動に関するメンバ関数の宣言
void CItem::moveFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 移動処理を行う
	this->m_move.moveByVel();
}

// アニメーションに関するメンバ関数の宣言
void CItem::animationFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// アニメーションの更新
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
void CItem::endOfScreen() {

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

// 値の反映に関するメンバ関数の宣言
void CItem::applyFunc() {

	// プレイヤーが死んでいたら飛ばす
	if (this->m_activeFlag == false) {
		return;
	}

	// 位置の値の反映
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// 切り取る部分の値を反映
	this->setTextureRect(this->m_chip);
}

/**
 *	@desc	アイテムの効果を適用させる
 *	@param	衝突してきたキャラクターのステータスのアドレス
 */
void CItem::hits(CStatus* pStatus) {

	switch ((ITEM_TYPE)m_anime.m_frame) {

	case ITEM_TYPE::PORTION:

		pStatus->addHp(5);

		break;

	case ITEM_TYPE::BOOST:

		pStatus->addSpeed(2);

		break;
	}
}