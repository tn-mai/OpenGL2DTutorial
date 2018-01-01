#include "UserInterface.h"
#include "Constants.h"
#include "Scene/GameMain/GameMain.h"

//=========================================================================
//	HP表示用のラベルクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CHpLabel::CHpLabel() {

	m_pStatus = NULL;
}

// デストラクタ
CHpLabel::~CHpLabel() {
}

// 初期化処理
bool CHpLabel::init() {

	// 文字列のフォントのタイプ
	this->setSystemFontName("Arial");

	// 文字列のフォントサイズ
	this->setSystemFontSize(24);

	// 左寄せ
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::LEFT);

	// 箱
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// 位置の設定
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.97f));

	// 自動で更新処理を呼び出す
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	更新処理
 *  @param	経過時間
 */
void CHpLabel::update(float deltaTime_) {

	// 要素がNULLなら飛ばす
	if (m_pStatus == NULL) {
		return;
	}

	// 表示するHPを文字列に変換
	std::string hpStr = cocos2d::StringUtils::format("HP:%d", this->m_pStatus->getHp());

	// 表示する文字列
	this->setString(hpStr);
}

//=========================================================================
//	HPゲージクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CHpGuage::CHpGuage() {

	m_pStatus = NULL;
}

// デストラクタ
CHpGuage::~CHpGuage() {
}

// 初期化処理
bool CHpGuage::init() {

	// HPゲージで使用する画像を生成
	cocos2d::Sprite* pSprite = cocos2d::Sprite::create(IMAGE_FILE_HP_GUAGE);

	// 生成したスプライトを使って基底クラスの初期化を行う
	if (cocos2d::ProgressTimer::initWithSprite(pSprite) == false) {

		CCLOG("CHpGuage() ProgressTimer::initWithSprite is false !!!");
		
		return false;
	}

	// プログレスタイマーの初期の長さを設定　バーは％で表示
	this->setPercentage(0.0f);

	// プログレスタイマーのタイプを設定（バーに設定）
	this->setType(cocos2d::ProgressTimer::Type::BAR);

	// プログレスタイマーバーの伸びる方向（x方向に設定）
	this->setBarChangeRate(cocos2d::Point(1, 0));

	// プログレスタイマーバーの基準点（左側に設定）
	this->setMidpoint(cocos2d::Point(0, 0));

	// プログレスタイマーバーの位置を設定
	this->setPosition(WINDOW_RIGHT * 0.6f, WINDOW_TOP * 0.005f);

	// 更新処理の設定
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	更新処理
 *  @param	経過時間
 */
void CHpGuage::update(float deltaTime_) {

	// 要素がNULLなら飛ばす
	if (m_pStatus == NULL) {
		return;
	}

	// プレイヤーのHPバーラベルの設定
	if (0 <= this->m_pStatus->getHp()) {

		// 現在のライフを最大ライフと比較してパーセンテージに変換
		// int型で計算すると0になってしまうのでfloat型にキャストして計算する
		float hp = ((float)this->m_pStatus->getHp() / (float)this->m_pStatus->getMaxHp()) * 100.0f;

		// 表示する画像の割合を設定
		this->setPercentage(hp);
	}
}

//=========================================================================
//	HP表示用のラベルクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CScoreLabel::CScoreLabel() {

	m_pStatus = NULL;
}

// デストラクタ
CScoreLabel::~CScoreLabel() {
}

// 初期化処理
bool CScoreLabel::init() {

	// 文字列のフォントのタイプ
	this->setSystemFontName("Arial");

	// 文字列のフォントサイズ
	this->setSystemFontSize(24);

	// 左寄せ
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::LEFT);

	// 箱
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// 位置の設定
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.9f));

	// 自動で更新処理を呼び出す
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	更新処理
 *  @param	経過時間
 */
void CScoreLabel::update(float deltaTime_) {

	// 要素がNULLなら飛ばす
	if (m_pStatus == NULL) {
		return;
	}

	// 表示するスコアを文字列に変換
	std::string scoreStr = cocos2d::StringUtils::format("TOTALSCORE:%d", this->m_pStatus->getScore());

	// 表示する文字列
	this->setString(scoreStr);
}

//=========================================================================
//	ステージ表示用のラベルクラスのコードはここから書く
//=========================================================================

// コンストラクタ
CStageNoLabel::CStageNoLabel() {

	m_pStage = NULL;
}

// デストラクタ
CStageNoLabel::~CStageNoLabel() {
}

// 初期化処理
bool CStageNoLabel::init() {

	// 文字列のフォントのタイプ
	this->setSystemFontName("Arial");

	// 文字列のフォントサイズ
	this->setSystemFontSize(24);

	// 右寄せ
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::RIGHT);

	// 箱
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// 位置の設定
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.97f));

	// 自動で更新処理を呼び出す
	this->scheduleUpdate();

	return true;
}

/**
*  @desc	更新処理
*  @param	経過時間
*/
void CStageNoLabel::update(float deltaTime_) {

	// 要素がNULLなら飛ばす
	if (m_pStage == NULL) {
		return;
	}

	// 表示するステージを文字列に変換
	std::string stageStr = cocos2d::StringUtils::format("STAGE:%d", *this->m_pStage);

	// 表示する文字列
	this->setString(stageStr);
}
