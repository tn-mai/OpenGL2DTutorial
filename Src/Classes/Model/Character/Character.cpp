//
//  Character.cpp
//  ShootingGame
//
//  Created by �i����v on 2015/05/18.
//
//

#include "Character.h"
#include "Lib/InputManager/InputManager.h"
#include "Scene/GameMain/GameMain.h"

//=========================================================================
//	�v���C���[�L�����N�^�[�N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================
// �R���X�g���N�^
CPlayerCharacter::CPlayerCharacter() {
}

// �f�X�g���N�^
CPlayerCharacter::~CPlayerCharacter() {
}

/**
 *	@desc	����������
 */
bool CPlayerCharacter::init() {
	// cocos2d �̃N���X���p��������R���X�g���N�^��
	// �X�[�p�[�N���X�� init �֐����������Ȃ���΂Ȃ�Ȃ��̂Œ���
	if ( Sprite::init() == false ) {
		CCLOG( "CPlayerCharacter() Sprite::init is false !!!" ) ;
		return false ;
	}

	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================
	
	// �摜�̐ݒ�
	this->setTexture(IMAGE_FILE_CHARACTER);

	// �؂����`
	this->m_chip.setRect(0, 0, 48, 32);

	// �؂����`�̐ݒ�
	this->setTextureRect(this->m_chip);

	// �ړ��p�f�[�^�̒l�̏�����
	this->m_move.setPos(320, 240);
	this->m_move.setVel(0, 0);

	// �v���C���[�̃A�j���[�V�����f�[�^�̏�����
	this->m_anime.set(10, 4);

	// �e���˗p�J�E���^�[�̏�����
	this->m_shotCounter = 0;

	// �e���ˊԊu�̏�����
	this->m_shotInterval = 20;

	// �Փ˔���p�f�[�^�̏�����
	this->m_body.set(-24, 16, 24, -16);

	// �X�e�[�^�X�̏�����
	// �v���C���[�̃X�R�A�̓g�[�^���̃X�R�A�Ƃ��Ĉ���
	this->m_status.set(20, 20, 50, 0, 2);

	// �L���t���O�̏�����
	this->m_activeFlag = true;

	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	return true ;
}

/**
 *  @desc	�v���C���[�̒e�̔���
 *  @param	�e�^�C�v
 *  @param	�C���ʒuX
 *  @param	�C���ʒuY
 *  @param	���xX
 *  @param	���xY
 */
CBullet* CPlayerCharacter::shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY) {

	// �v���C���[������ł�����NULL��Ԃ��Ĕ�΂�
	if (this->m_activeFlag == false) {
		return NULL;
	}

	// �v���C���[�̒e�̐���
	CBullet* pBul = CBullet::create();

	// �摜�̐ݒ�
	pBul->setTexture(IMAGE_FILE_BULLET);

	// �؂����`
	pBul->m_chip = CChip(0, 2, 8, 8);

	// �؂����`�̐ݒ�
	pBul->setTextureRect(pBul->m_chip);

	// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
	pBul->m_move.setPos(this->m_move.m_posX + fixX, this->m_move.m_posY + fixY);

	// �ړ��f�[�^�ɏ������x�̐ݒ�
	pBul->m_move.setVel(velX, velY);

	// �Փ˔���p�f�[�^�̏�����
	pBul->m_body.set(-4, 4, 4, -4);

	// �X�e�[�^�X�̏�����
	pBul->m_status.set(1, 1, 20, 0);

	// �L���t���O��true������
	pBul->m_activeFlag = true;

	return pBul;
}

// �ړ��Ɋւ��郁���o�֐��̒�`
void CPlayerCharacter::moveFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// ���x����ʒu���v�Z
	this->m_move.moveByVel();
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̒�`
void CPlayerCharacter::animationFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �A�j���[�V�����̍X�V���s��
	int frame = this->m_anime.update();

	// �t���[��������\������ׂ��`�b�v�f�[�^���v�Z����
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// ��ʒ[����Ɋւ��郁���o�֐��̒�`
void CPlayerCharacter::endOfScreen() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �E�[
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// �v���C���[�̉E�[���E�B���h�E�����яo���i��ʂ̉E�[�ɓ��B�����j

		// ��яo�������v�Z�i�v���C���[�̉E�[ - �E�B���h�E���j
		float boundary = (this->m_move.m_posX + this->m_chip.size.width * 0.5f) - WINDOW_RIGHT;

		// ���݂̈ʒu�����яo�������������ʒu���v�Z
		this->m_move.m_posX = this->m_move.m_posX - boundary;
	}
	// ���[
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// �v���C���[�̍��[���E�B���h�E�����яo���i��ʂ̍��[�ɓ��B�����j

		// ��яo�������v�Z�i�v���C���[�̍��[ - �E�B���h�E���j
		float boundary = (this->m_move.m_posX - this->m_chip.size.width * 0.5f) - WINDOW_LEFT;

		// ���݂̈ʒu�����яo�������������ʒu���v�Z
		this->m_move.m_posX = this->m_move.m_posX - boundary;
	}
	// ��[
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// �v���C���[�̏�[���E�B���h�E�����яo���i��ʂ̏�[�ɓ��B�����j

		// ��яo�������v�Z�i�v���C���[�̏�[ - �E�B���h�E���j
		float boundary = (this->m_move.m_posY + this->m_chip.size.height * 0.5f) - WINDOW_TOP;

		// ���݂̈ʒu�����яo�������������ʒu���v�Z
		this->m_move.m_posY = this->m_move.m_posY - boundary;
	}
	// ���[
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// �v���C���[�̉��[���E�B���h�E�����яo���i��ʂ̉��[�ɓ��B�����j

		// ��яo�������v�Z�i�v���C���[�̉��[ - �E�B���h�E���j
		float boundary = (this->m_move.m_posY - this->m_chip.size.height * 0.5f) - WINDOW_BOTTOM;

		// ���݂̈ʒu�����яo�������������ʒu���v�Z
		this->m_move.m_posY = this->m_move.m_posY - boundary;
	}
}

// �l�̔��f�Ɋւ��郁���o�֐��̒�`
void CPlayerCharacter::applyFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �v���C���[�̈ړ��p�f�[�^���v���C���[�̎��ۂ̈ʒu�ɔ��f
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// �؂��镔���̒l�𔽉f
	this->setTextureRect(this->m_chip);
}

/**
 *  @desc	����̍U�������g�ɂ�������
 *  @param	�Փ˂��Ă�������̃X�e�[�^�X�̃A�h���X
 */
void CPlayerCharacter::hits(CStatus* pStatus) {

	// HP������������
	this->m_status.decreaseHp(pStatus->getAttackPt());

	// HP��0�ȉ��������ꍇ�L���t���O��������
	if (this->m_status.getHp() <= 0) {
		this->m_activeFlag = false;
	}
}

//=========================================================================
//	�o���b�g ( �e ) �̃N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CBullet::CBullet() {
}

// �f�X�g���N�^
CBullet::~CBullet() {
}

/**
 *	@desc	����������
 */
bool CBullet::init() {

	// cocos2d �̃N���X���p��������R���X�g���N�^��
	// �X�[�p�[�N���X�� init �֐����������Ȃ���΂Ȃ�Ȃ��̂Œ���
	if (Sprite::init() == false) {
		CCLOG("CBullet() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	// �e��傫������i�A�j���[�V�������邽�߁j
	//this->setScale(5.0f);

	// �e�̃A�j���[�V�����f�[�^�̏�����
	this->m_anime.set(5, 4);

	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	return true;
}

// �ړ��Ɋւ��郁���o�֐��̒�`
void CBullet::moveFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ړ��������s��
	this->m_move.moveByVel();
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̒�`
void CBullet::animationFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �A�j���[�V�����̍X�V���s��
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
void CBullet::endOfScreen() {

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

// �l�̔��f�Ɋւ��郁���o�֐��̒�`
void CBullet::applyFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// �؂��镔���̒l�𔽉f
	this->setTextureRect(this->m_chip);
}

//=========================================================================
//	�G�̃N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CEnemyCharacter::CEnemyCharacter() {
}

// �f�X�g���N�^
CEnemyCharacter::~CEnemyCharacter() {
}

/**
 *	@desc	����������
 */
bool CEnemyCharacter::init() {

	// cocos2d �̃N���X���p��������R���X�g���N�^��
	// �X�[�p�[�N���X�� init �֐����������Ȃ���΂Ȃ�Ȃ��̂Œ���
	if (Sprite::init() == false) {
		CCLOG("CEnemyCharacter() Sprite::init is false !!!");
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

/**
 *  @desc	�G�̒e�̔���
 *  @param	�e�^�C�v
 *  @param	�C���ʒuX
 *  @param	�C���ʒuY
 *  @param	���xX
 *  @param	���xY
 */
CBullet* CEnemyCharacter::shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY) {

	// �v���C���[������ł�����NULL��Ԃ��Ĕ�΂�
	if (this->m_activeFlag == false) {
		return NULL;
	}

	// �G�̒e�̐���
	CBullet* pBul = CBullet::create();

	// �摜�̐ݒ�
	pBul->setTexture(IMAGE_FILE_BULLET);

	// �؂����`�i�F��\���j
	pBul->m_chip = CChip(0, 10, 8, 8);

	// �؂����`�̐ݒ�
	pBul->setTextureRect(pBul->m_chip);

	// �ړ��f�[�^�ɏ����ʒu�̐ݒ�i�G�̍��[���甭�˂����悤�ɂ���j
	pBul->m_move.setPos(this->m_move.m_posX + fixX, this->m_move.m_posY + fixY);

	// �ړ��f�[�^�ɏ������x�̐ݒ�
	pBul->m_move.setVel(velX, velY);

	// �Փ˔���p�f�[�^�̏�����
	pBul->m_body.set(-4, 4, 4, -4);

	// �X�e�[�^�X�̏�����
	pBul->m_status.set(1, 1, 1, 0);

	// �L���t���O��true������
	pBul->m_activeFlag = true;

	return pBul;
}

// �ړ��Ɋւ��郁���o�֐��̒�`
void CEnemyCharacter::moveFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ړ�����
	this->m_move.moveByVel();
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̒�`
void CEnemyCharacter::animationFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �A�j���[�V�����̍X�V���s��
	int frame = this->m_anime.update();

	// �t���[��������\������ׂ��`�b�v�f�[�^���v�Z����
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);
}

// ��ʒ[����Ɋւ��郁���o�֐��̒�`
void CEnemyCharacter::endOfScreen() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �G�摜�̍��[�̍��W����ʉE�[�[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posX - this->m_chip.size.width * 0.5f > WINDOW_RIGHT) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �G�摜�̉E�[�̍��W����ʍ��[�̍��W��菬�����Ȃ�����
	if (this->m_move.m_posX + this->m_chip.size.width * 0.5f < WINDOW_LEFT) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �G�摜�̉��[�̍��W����ʏ�[�̍��W���傫���Ȃ�����
	if (this->m_move.m_posY - this->m_chip.size.height * 0.5f > WINDOW_TOP) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

	// �G�摜�̏�[�̍��W����ʉ��[�̍��W��菬�����Ȃ�����
	if (this->m_move.m_posY + this->m_chip.size.height * 0.5f < WINDOW_BOTTOM) {
		// �L���t���O��������
		this->m_activeFlag = false;
	}

}

// �l�̔��f�Ɋւ��郁���o�֐��̒�`
void CEnemyCharacter::applyFunc() {

	//�v ���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ʒu�̒l�̔��f
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// �؂��镔���̒l�𔽉f
	this->setTextureRect(this->m_chip);
}

void CEnemyCharacter::actionFunc() {

	m_pAction->actionFunc(this);
}

/**
 *  @desc	�U��������ɂ�������
 *  @param	�Փ˂�������̃X�e�[�^�X�̃A�h���X
 */
void CEnemyCharacter::hits(CStatus * pStatus) {

	// HP������������
	this->m_status.decreaseHp(pStatus->getAttackPt());

	// HP��0�ȉ��������ꍇ�L���t���O��������
	if (this->m_status.getHp() <= 0) {
		this->m_activeFlag = false;
	}
}

//=========================================================================
//	�G�t�F�N�g�̃N���X�̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CEffect::CEffect() {
}

// �f�X�g���N�^
CEffect::~CEffect() {
}

/**
 *	@desc	����������
 */
bool CEffect::init() {

	// cocos2d �̃N���X���p��������R���X�g���N�^��
	// �X�[�p�[�N���X�� init �֐����������Ȃ���΂Ȃ�Ȃ��̂Œ���
	if (Sprite::init() == false) {
		CCLOG("CEffect() Sprite::init is false !!!");
		return false;
	}

	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	// �G�t�F�N�g�̃A�j���[�V�����f�[�^�̏�����
	m_anime.set(5, 8, false);

	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	return true;
}

// �ړ��Ɋւ��郁���o�֐��̐錾
void CEffect::moveFunc() {

	//�v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ړ��������s��
	this->m_move.moveByVel();
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̐錾
void CEffect::animationFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �A�j���[�V�����̍X�V���s��
	int frame = this->m_anime.update();

	// �t���[��������\������ׂ��`�b�v�f�[�^���v�Z����
	this->m_chip.setRect(
		this->m_chip.size.width * frame,
		this->m_chip.origin.y,
		this->m_chip.size.width,
		this->m_chip.size.height
	);

	// true�Ȃ烋�[�v�������t���O��������
	if (m_anime.m_isEnd == true) {
		this->m_activeFlag = false;
	}
}

// �l�̔��f�Ɋւ��郁���o�֐��̐錾
void CEffect::applyFunc() {

	// �v���C���[������ł������΂�
	if (this->m_activeFlag == false) {
		return;
	}

	// �ʒu�̒l�̔��f
	this->setPosition(this->m_move.m_posX, this->m_move.m_posY);

	// �؂��镔���̒l�𔽉f
	this->setTextureRect(this->m_chip);
}