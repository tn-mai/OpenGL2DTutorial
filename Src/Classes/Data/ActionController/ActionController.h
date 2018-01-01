#pragma once

//=========================================================================
//	行動管理に関するタイプなどの列挙はここから書いていく
//=========================================================================

// アクションタイプ
enum class ACTION_TYPE {

	// 行動無し
	NONE,

	// ジグザグ
	ZIGZAG,

	// 直進行動
	STRAIGHT,

	// 凸行動
	CONVEX,
};

// CEnemyCharacterクラスを前方宣言
class CEnemyCharacter;

//=========================================================================
//	行動管理のクラスはここから書く
//=========================================================================

class CAction {
public:
	virtual ~CAction() {
	}

	virtual void actionFunc(CEnemyCharacter* pEne) = 0;
};

/*
 *  ジグザグ行動
 */
class CActionControllerZigZag : public CAction{
public:
	// アクションタイプ
	ACTION_TYPE m_actionType;

	// コンストラクタ
	CActionControllerZigZag();

	/**
	 *  @desc	行動管理
	 *  @param	この行動をおこなう敵キャラクターのアドレス
	 */
	void actionFunc(CEnemyCharacter* pEne);
};

/*
 *  直進行動
 *    直進行動中に弾を2発発射
 */
class CActionControllerStraight : public CAction{
public:
	// 行動タイプ
	ACTION_TYPE m_actionType;

	// コンストラクタ
	CActionControllerStraight();

	/**
	 *  @desc	行動処理
	 *  @param	この行動をおこなう敵キャラクターのアドレス
	 */
	void actionFunc(CEnemyCharacter* pEne);
};

/*
 *  凸行動
 */
class CActionControllerConvex : public CAction{
public:
	// 行動タイプ
	ACTION_TYPE m_actionType;

	// コンストラクタ
	CActionControllerConvex();

	/**
	 *  @desc	行動処理
	 *  @param	この行動行う敵キャラクターのアドレス
	 */
	void actionFunc(CEnemyCharacter* pEne);
};