#pragma once

//=========================================================================
//	�X�e�[�^�X�N���X�̃R�[�h�͂������珑��
//=========================================================================

class CStatus {
public:
	// �R���X�g���N�^
	CStatus() {
	}

	// �f�X�g���N�^
	~CStatus() {
	}

private:

	// HP
	int m_hp = 0;

	// �ő�HP
	int m_maxHp = 0;

	// �U����
	int m_attackPt = 0;

	// ���_
	int m_score = 0;

	// �X�s�[�h
	int m_speed = 0;

public:

	/**
	 *  @desc	�ݒ�
	 *  @param	HP
	 *  @param	�ő�HP
	 *  @param	�U����
	 *  @param	�X�R�A
	 *  @param	�X�s�[�h
	 */
	void set(int hp, int maxHp, int attackPt, int score, int speed = 0) {
		this->m_hp = hp;
		this->m_maxHp = maxHp;
		this->m_attackPt = attackPt;
		this->m_score = score;
		this->m_speed = speed;
	}

	// HP�̎擾
	int getHp() {

		return this->m_hp;
	}

	// �ő�HP�̎擾
	int getMaxHp() {

		return this->m_maxHp;
	}

	// �U���͂̎擾
	int getAttackPt() {

		return this->m_attackPt;
	}

	// �X�R�A�̎擾
	int getScore() {

		return this->m_score;
	}

	// ���x�̎擾
	int getSpeed() {

		return this->m_speed;
	}

	/**
	 *  @desc	HP�̌���
	 *  @param	��������l�i����̍U���́j
	 */
	void decreaseHp(int attackPt) {

		this->m_hp -= attackPt;

		// ����
		if (m_hp < 0) {
			m_hp = 0;
		}
	}

	/**
	 *  @desc	HP�S��
	 */
	void recoveryHp() {

		this->m_hp = this->m_maxHp;
	}

	/**
	 *  @desc	�X�R�A�̉��_
	 *  @param	���_����X�R�A
	 */
	void addScore(int score) {

		this->m_score += score;
	}

	/**
	 *  @desc	HP�̉�
	 *  @param	�㏸����l
	 */
	void addHp(int hp) {
	
		// HP�̉�
		this->m_hp += hp;

		// ����
		if (this->m_hp > this->m_maxHp) {
			this->m_hp = this->m_maxHp;
		}
	}

	void addSpeed(int speed) {

		// ���x�A�b�v
		this->m_speed += speed;
	}
};
