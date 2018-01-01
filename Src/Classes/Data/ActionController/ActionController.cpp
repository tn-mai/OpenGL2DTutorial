#include "ActionController.h"
#include "Model/Character/Character.h"

//=========================================================================
//	ジグザグのクラスの定義はここから書く
//=========================================================================

// コンストラクタ
CActionControllerZigZag::CActionControllerZigZag() : m_actionType(ACTION_TYPE::ZIGZAG) {

}

/**
 *  @desc	行動処理
 *  @param	この行動を行う敵キャラクターのアドレス
 */
void CActionControllerZigZag::actionFunc(CEnemyCharacter* pEne) {

	// x座標が320以下になったら
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= 320.0f) {

		// 上に移動
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = false;

		// ステップを1にする
		pEne->m_actionStep = 1;
	}

	// y座標が240以上になったら
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posY >= 240) {

		// 弾を発射（移動はそのまま上に移動）
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = true;

		// ステップを2にする
		pEne->m_actionStep = 2;
	}

	// y座標が380以上になったら
	if (pEne->m_actionStep == 2 && pEne->m_move.m_posY >= 380.0f) {

		// 左に移動
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = false;

		// ステップを3にする
		pEne->m_actionStep = 3;
	}
}

//=========================================================================
//	直進行動のクラスの定義はここから書く
//=========================================================================

// コンストラクタ
CActionControllerStraight::CActionControllerStraight() : m_actionType(ACTION_TYPE::STRAIGHT) {

}

/**
 *  @desc	行動処理
 *  @param	この行動をおこなう敵キャラクターのアドレス
 */
void CActionControllerStraight::actionFunc(CEnemyCharacter* pEne) {

	// x座標が3分の2ぐらいに来たら弾を発射
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.6666f) {

		// ステップを1にする
		pEne->m_actionStep = 1;

		// 左に移動
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}

	// x座標が3分の1ぐらいに来たら弾を発射
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.3333f) {

		// ステップを2にする
		pEne->m_actionStep = 2;

		// 左に移動
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}
}

//=========================================================================
//	凸行動のクラスの定義はここから書く
//=========================================================================

// コンストラクタ
CActionControllerConvex::CActionControllerConvex() : m_actionType(ACTION_TYPE::CONVEX) {

}

/**
 *  @desc	行動処理
 *  @param	この行動をおこなう敵キャラクターのアドレス
 */
void CActionControllerConvex::actionFunc(CEnemyCharacter* pEne) {

	// x座標が3分の2ぐらいに来たら下に移動
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.6666f) {

		// ステップを1にする
		pEne->m_actionStep = 1;

		// 下に移動
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = -5.0f;
		pEne->m_shotFlag = false;
	}

	// y座標が100以下に来たら左に移動し弾を発射
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posY <= 100) {

		// ステップを2にする
		pEne->m_actionStep = 2;

		// 左に移動
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}

	// x座標が3分の1ぐらいに来たら上に移動
	if (pEne->m_actionStep == 2 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.3333f) {

		// ステップを3にする
		pEne->m_actionStep = 3;

		// 上に移動
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = false;
	}

	// y座標が380以上に来たら左に移動
	if (pEne->m_actionStep == 3 && pEne->m_move.m_posY >= 380) {

		// ステップを4にする
		pEne->m_actionStep = 4;

		// 左に移動
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = false;
	}
}
