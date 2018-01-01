#pragma once

//=========================================================================
//	ステータスクラスのコードはここから書く
//=========================================================================

class CStatus {
public:
	// コンストラクタ
	CStatus() {
	}

	// デストラクタ
	~CStatus() {
	}

private:

	// HP
	int m_hp = 0;

	// 最大HP
	int m_maxHp = 0;

	// 攻撃力
	int m_attackPt = 0;

	// 得点
	int m_score = 0;

	// スピード
	int m_speed = 0;

public:

	/**
	 *  @desc	設定
	 *  @param	HP
	 *  @param	最大HP
	 *  @param	攻撃力
	 *  @param	スコア
	 *  @param	スピード
	 */
	void set(int hp, int maxHp, int attackPt, int score, int speed = 0) {
		this->m_hp = hp;
		this->m_maxHp = maxHp;
		this->m_attackPt = attackPt;
		this->m_score = score;
		this->m_speed = speed;
	}

	// HPの取得
	int getHp() {

		return this->m_hp;
	}

	// 最大HPの取得
	int getMaxHp() {

		return this->m_maxHp;
	}

	// 攻撃力の取得
	int getAttackPt() {

		return this->m_attackPt;
	}

	// スコアの取得
	int getScore() {

		return this->m_score;
	}

	// 速度の取得
	int getSpeed() {

		return this->m_speed;
	}

	/**
	 *  @desc	HPの減少
	 *  @param	減少する値（相手の攻撃力）
	 */
	void decreaseHp(int attackPt) {

		this->m_hp -= attackPt;

		// 制限
		if (m_hp < 0) {
			m_hp = 0;
		}
	}

	/**
	 *  @desc	HP全回復
	 */
	void recoveryHp() {

		this->m_hp = this->m_maxHp;
	}

	/**
	 *  @desc	スコアの加点
	 *  @param	加点するスコア
	 */
	void addScore(int score) {

		this->m_score += score;
	}

	/**
	 *  @desc	HPの回復
	 *  @param	上昇する値
	 */
	void addHp(int hp) {
	
		// HPの回復
		this->m_hp += hp;

		// 制限
		if (this->m_hp > this->m_maxHp) {
			this->m_hp = this->m_maxHp;
		}
	}

	void addSpeed(int speed) {

		// 速度アップ
		this->m_speed += speed;
	}
};
