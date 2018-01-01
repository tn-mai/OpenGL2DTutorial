#include "ActionController.h"
#include "Model/Character/Character.h"

//=========================================================================
//	�W�O�U�O�̃N���X�̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CActionControllerZigZag::CActionControllerZigZag() : m_actionType(ACTION_TYPE::ZIGZAG) {

}

/**
 *  @desc	�s������
 *  @param	���̍s�����s���G�L�����N�^�[�̃A�h���X
 */
void CActionControllerZigZag::actionFunc(CEnemyCharacter* pEne) {

	// x���W��320�ȉ��ɂȂ�����
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= 320.0f) {

		// ��Ɉړ�
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = false;

		// �X�e�b�v��1�ɂ���
		pEne->m_actionStep = 1;
	}

	// y���W��240�ȏ�ɂȂ�����
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posY >= 240) {

		// �e�𔭎ˁi�ړ��͂��̂܂܏�Ɉړ��j
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = true;

		// �X�e�b�v��2�ɂ���
		pEne->m_actionStep = 2;
	}

	// y���W��380�ȏ�ɂȂ�����
	if (pEne->m_actionStep == 2 && pEne->m_move.m_posY >= 380.0f) {

		// ���Ɉړ�
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = false;

		// �X�e�b�v��3�ɂ���
		pEne->m_actionStep = 3;
	}
}

//=========================================================================
//	���i�s���̃N���X�̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CActionControllerStraight::CActionControllerStraight() : m_actionType(ACTION_TYPE::STRAIGHT) {

}

/**
 *  @desc	�s������
 *  @param	���̍s���������Ȃ��G�L�����N�^�[�̃A�h���X
 */
void CActionControllerStraight::actionFunc(CEnemyCharacter* pEne) {

	// x���W��3����2���炢�ɗ�����e�𔭎�
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.6666f) {

		// �X�e�b�v��1�ɂ���
		pEne->m_actionStep = 1;

		// ���Ɉړ�
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}

	// x���W��3����1���炢�ɗ�����e�𔭎�
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.3333f) {

		// �X�e�b�v��2�ɂ���
		pEne->m_actionStep = 2;

		// ���Ɉړ�
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}
}

//=========================================================================
//	�ʍs���̃N���X�̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CActionControllerConvex::CActionControllerConvex() : m_actionType(ACTION_TYPE::CONVEX) {

}

/**
 *  @desc	�s������
 *  @param	���̍s���������Ȃ��G�L�����N�^�[�̃A�h���X
 */
void CActionControllerConvex::actionFunc(CEnemyCharacter* pEne) {

	// x���W��3����2���炢�ɗ����牺�Ɉړ�
	if (pEne->m_actionStep == 0 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.6666f) {

		// �X�e�b�v��1�ɂ���
		pEne->m_actionStep = 1;

		// ���Ɉړ�
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = -5.0f;
		pEne->m_shotFlag = false;
	}

	// y���W��100�ȉ��ɗ����獶�Ɉړ����e�𔭎�
	if (pEne->m_actionStep == 1 && pEne->m_move.m_posY <= 100) {

		// �X�e�b�v��2�ɂ���
		pEne->m_actionStep = 2;

		// ���Ɉړ�
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = true;
	}

	// x���W��3����1���炢�ɗ������Ɉړ�
	if (pEne->m_actionStep == 2 && pEne->m_move.m_posX <= WINDOW_RIGHT * 0.3333f) {

		// �X�e�b�v��3�ɂ���
		pEne->m_actionStep = 3;

		// ��Ɉړ�
		pEne->m_move.m_velX = 0.0f;
		pEne->m_move.m_velY = 5.0f;
		pEne->m_shotFlag = false;
	}

	// y���W��380�ȏ�ɗ����獶�Ɉړ�
	if (pEne->m_actionStep == 3 && pEne->m_move.m_posY >= 380) {

		// �X�e�b�v��4�ɂ���
		pEne->m_actionStep = 4;

		// ���Ɉړ�
		pEne->m_move.m_velX = -5.0f;
		pEne->m_move.m_velY = 0.0f;
		pEne->m_shotFlag = false;
	}
}
