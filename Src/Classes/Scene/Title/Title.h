#pragma once

#include "cocos2d.h"

/*
 *  タイトルレイヤー
 *
 *		タイトルを表示する為のレイヤー
 *
 */
class CTitle : public cocos2d::Layer {
public:
	// デストラクタ
	~CTitle();

	/**
	 *  @desc		シーンの生成
	 *  @return		CTitleレイヤーを内包したシーンクラス
	 *  @tips		静的メンバ関数
	 */
	static cocos2d::Scene* createScene();

	/*
	 *	@desc	シーン生成時に初期化関数を処理
	 *			初期化関数失敗時は NULL を返す
	 */
	CREATE_FUNC(CTitle);

	/**
	 *  @desc		初期化
	 *  @return		true...成功	false...失敗
	 */
	bool init() override;

	/**
	 *  @desc		更新処理
	 *  @param		経過時間
	 */
	void update(float deltaTime) override;

public:

	// 有効フラグ
	bool m_activeFlag;

	/**
	 *  @desc		ゲームメインに遷移
	 *  @param		タイトルレイヤーのインスタンス
	 *  @tips		スタートボタンが押された時に呼び出される
	 */
	void callbackChangeGameMain(cocos2d::Ref* pSender);

	/**
	 *  @desc		ゲーム終了
	 *  @param		タイトルレイヤーのインスタンス
	 *  @param		ゲーム終了ボタンが押された時に呼び出される
	 */
	void callbackEndGame(cocos2d::Ref* pSender);
};