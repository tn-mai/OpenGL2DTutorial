#pragma once

#include "cocos2d.h"
#include "Data/Status/Status.h"

//=========================================================================
//	HP表示用のラベルクラスのコードはここから書く
//=========================================================================

class CHpLabel : public cocos2d::Label {
public:
	CHpLabel();
	~CHpLabel();

	CREATE_FUNC(CHpLabel);

	// 初期化処理
	virtual bool init();

private:
	// このクラスで監視するステータスクラスのアドレスを保管するポインタ
	CStatus* m_pStatus;

public:
	/**
	 *  @desc	更新処理
	 *  @param	経過時間
	 */
	virtual void update(float deltaTime_) override;

	// ステータスの設定
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	HPゲージクラスのコードはここから書く
//=========================================================================

class CHpGuage : public cocos2d::CCProgressTimer {
public:
	CHpGuage();
	~CHpGuage();

	CREATE_FUNC(CHpGuage);

	// 初期化処理
	virtual bool init();

	/**
	 *  @desc	更新処理
	 *  @param	経過時間
	 */
	virtual void update(float deltaTime_) override;

private:
	// 参照するステータス
	CStatus* m_pStatus;

public:
	// 参照するステータスの設定
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	スコア表示用のラベルクラスのコードはここから書く
//=========================================================================

class CScoreLabel : public cocos2d::Label {
public:
	CScoreLabel();
	~CScoreLabel();

	CREATE_FUNC(CScoreLabel);

	// 初期化処理
	virtual bool init();

private:
	// このクラスで監視するステータスクラスのアドレスを保管するポインタ
	CStatus* m_pStatus;

public:
	/**
	 *  @desc	更新処理
	 *  @param	経過時間
	 */
	virtual void update(float deltaTime_) override;

	// ステータスの設定
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	ステージ表示用のラベルクラスのコードはここから書く
//=========================================================================

class CStageNoLabel : public cocos2d::Label {
public:
	CStageNoLabel();
	~CStageNoLabel();

	CREATE_FUNC(CStageNoLabel);

	// 初期化処理
	virtual bool init();

public:
	// このクラスで監視するステータスクラスのアドレスを保管するポインタ
	int* m_pStage;

public:
	/**
	*  @desc	更新処理
	*  @param	経過時間
	*/
	virtual void update(float deltaTime_) override;

	// ステータスの設定
	void setStage(int* pStage) {
		this->m_pStage = pStage;
	}
};