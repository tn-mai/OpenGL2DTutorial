#pragma once

#include "Model/Character/Character.h"
#include "Data/ActionController/ActionController.h"

/*
 *
 *	敵の出撃データ
 *		敵1体分の出撃のデータ
 */
class CEnemyLaunchData {
public:

	// 出撃する敵のタイプ
	ENEMY_TYPE m_type;

	// 出撃する時間（フレーム）
	int m_frame;

	// 出撃位置X
	float m_posX;

	// 出撃位置Y
	float m_posY;

	// 敵の出撃最大数
	static int MAX_LAUNCH;

	/*
	 *  @desc	値の設定
	 *  @param	出撃するフレーム
	 *  @param	出撃位置X
	 *  @param	出撃位置Y
	 *  @param	出撃する敵のタイプ
	 *  @param	行動タイプ
	 */
	void set(int frame, float posX, float posY, ENEMY_TYPE type) {
		this->m_frame = frame;
		this->m_posX = posX;
		this->m_posY = posY;
		this->m_type = type;
	}
};



