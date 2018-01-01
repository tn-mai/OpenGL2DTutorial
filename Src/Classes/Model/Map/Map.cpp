#include "Map.h"

//=========================================================================
//	マップクラスのメンバ関数の定義はここから書く
//=========================================================================

// コンストラクタ
CMap::CMap() {
}

// デストラクタ
CMap::~CMap() {
}

/**
 *  @desc		生成
 *  @param		tmxファイル名
 *  @return		Mapクラスのインスタンス
 *  @tips		CREATE_FUNCマクロの中身
 *				静的メンバ関数
 */
CMap* CMap::create(std::string tmxFile) {

	// マップクラスを生成する
	CMap* pMap = new CMap();

	// TMXTiledMapの初期化処理と
	// CMapの初期化処理を行う
	if (pMap->initWithTMXFile(tmxFile) == true && pMap->init() == true) {
		
		// 生成が成功したら自動deleteを設定
		pMap->autorelease();

		// マップクラスのインスタンスを返す
		return pMap;
	}

	// 生成が失敗していたらマップクラスのインスタンスを解放
	CC_SAFE_DELETE(pMap);

	// NULLを返す
	return NULL;
}

/**
*  @desc	初期化処理
*/
bool CMap::init() {

	/*
	 *  スクロール終了位置の計算
	 */
	// 画面サイズの取得
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	// レイヤー情報を取得
	cocos2d::TMXLayer* pLayer = this->getLayer("layer");

	// レイヤーのサイズを取得（幅60 * 高さ15）
	cocos2d::Size layerSize = pLayer->getLayerSize();

	// マップチップのサイズを取得（幅32 * 高さ32）
	cocos2d::Size mapTileSize = pLayer->getMapTileSize();

	// マップの終了位置を取得
	this->m_scrollEndPos.x = (layerSize.width * mapTileSize.width) - winSize.width;

	// 初期位置の設定と反映
	this->m_scrollMove.setPos(0.0f, 0.0f);
	this->setPosition(0.0f, 0.0f);

	return true;
}

// スクロール（移動）処理
void CMap::moveFunc() {

	// マップスクロールのX移動処理
	if (this->m_scrollMove.m_posX < -this->m_scrollEndPos.x) {

		if (m_isLoop == true) {
			
			// ループする場合はマップの位置を戻す
			this->m_scrollMove.m_posX = 0;
		}
		else {
			
			// ループしない場合は終了フラグを上げる
			this->m_isEnd = true;
		}
	}
	else {
		
		// マップをスクロール
		this->m_scrollMove.m_posX -= this->m_scrollMove.m_velX;
	}
}

// 反映処理
void CMap::applyFunc() {

	this->setPosition(this->m_scrollMove.m_posX, this->m_scrollMove.m_posY);
}