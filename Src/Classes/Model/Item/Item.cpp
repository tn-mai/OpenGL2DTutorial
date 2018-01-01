#include "Item.h"

//=========================================================================
//	�G�t�F�N�g�̃N���X�̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CItem::CItem() {
}

// �f�X�g���N�^
CItem::~CItem() {
}

/**
*	@desc	����������
*/
bool CItem::init() {

	// cocos2d �̃N���X���p��������R���X�g���N�^��
	// �X�[�p�[�N���X�� init �֐����������Ȃ���΂Ȃ�Ȃ��̂Œ���
	if (Sprite::init() == false) {
		CCLOG("CItem() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================



	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	return true;
}

// �ړ��Ɋւ��郁���o�֐��̐錾
void CItem::moveFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ړ��������s��
	this->m_move.moveByVel();
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̐錾
void CItem::animationFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �A�j���[�V�����̍X�V
	int frame = this->m_anime.update();

	// �t���[��������\������ׂ��`�b�v�f�[�^���v�Z����
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// ��ʊO����Ɋւ��郁���o�֐��̒�`
void CItem::endOfScreen() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �e�摜�̍��[�̍��W����ʉE�[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �e�摜�̉E�[�̍��W����ʍ��[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �e�摜�̉��[�̍��W����ʏ�[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �e�摜�̏�[�̍��W����ʉ��[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}
}

// �l�̔��f�Ɋւ��郁���o�֐��̐錾
void CItem::applyFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ʒu�̒l�̔��f
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// �؂��镔���̒l�𔽉f
	this->setTextureRect(this->m_chip);
}

/**
 *	@desc	�A�C�e���̌��ʂ�K�p������
 *	@param	�Փ˂��Ă����L�����N�^�[�̃X�e�[�^�X�̃A�h���X
 */
void CItem::hits(CStatus* pStatus) {

	switch ((ITEM_TYPE)m_anime.m_frame) {

	case ITEM_TYPE::PORTION:

		pStatus->addHp(5);

		break;

	case ITEM_TYPE::BOOST:

		pStatus->addSpeed(2);

		break;
	}
}