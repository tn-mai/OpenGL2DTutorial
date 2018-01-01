#pragma once

#include "Model/Character/Character.h"
#include "Data/ActionController/ActionController.h"

/*
 *
 *	�G�̏o���f�[�^
 *		�G1�̕��̏o���̃f�[�^
 */
class CEnemyLaunchData {
public:

	// �o������G�̃^�C�v
	ENEMY_TYPE m_type;

	// �o�����鎞�ԁi�t���[���j
	int m_frame;

	// �o���ʒuX
	float m_posX;

	// �o���ʒuY
	float m_posY;

	// �G�̏o���ő吔
	static int MAX_LAUNCH;

	/*
	 *  @desc	�l�̐ݒ�
	 *  @param	�o������t���[��
	 *  @param	�o���ʒuX
	 *  @param	�o���ʒuY
	 *  @param	�o������G�̃^�C�v
	 *  @param	�s���^�C�v
	 */
	void set(int frame, float posX, float posY, ENEMY_TYPE type) {
		this->m_frame = frame;
		this->m_posX = posX;
		this->m_posY = posY;
		this->m_type = type;
	}
};



