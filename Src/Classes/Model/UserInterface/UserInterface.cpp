#include "UserInterface.h"
#include "Constants.h"
#include "Scene/GameMain/GameMain.h"

//=========================================================================
//	HP�\���p�̃��x���N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CHpLabel::CHpLabel() {

	m_pStatus = NULL;
}

// �f�X�g���N�^
CHpLabel::~CHpLabel() {
}

// ����������
bool CHpLabel::init() {

	// ������̃t�H���g�̃^�C�v
	this->setSystemFontName("Arial");

	// ������̃t�H���g�T�C�Y
	this->setSystemFontSize(24);

	// ����
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::LEFT);

	// ��
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// �ʒu�̐ݒ�
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.97f));

	// �����ōX�V�������Ăяo��
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	�X�V����
 *  @param	�o�ߎ���
 */
void CHpLabel::update(float deltaTime_) {

	// �v�f��NULL�Ȃ��΂�
	if (m_pStatus == NULL) {
		return;
	}

	// �\������HP�𕶎���ɕϊ�
	std::string hpStr = cocos2d::StringUtils::format("HP:%d", this->m_pStatus->getHp());

	// �\�����镶����
	this->setString(hpStr);
}

//=========================================================================
//	HP�Q�[�W�N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CHpGuage::CHpGuage() {

	m_pStatus = NULL;
}

// �f�X�g���N�^
CHpGuage::~CHpGuage() {
}

// ����������
bool CHpGuage::init() {

	// HP�Q�[�W�Ŏg�p����摜�𐶐�
	cocos2d::Sprite* pSprite = cocos2d::Sprite::create(IMAGE_FILE_HP_GUAGE);

	// ���������X�v���C�g���g���Ċ��N���X�̏��������s��
	if (cocos2d::ProgressTimer::initWithSprite(pSprite) == false) {

		CCLOG("CHpGuage() ProgressTimer::initWithSprite is false !!!");
		
		return false;
	}

	// �v���O���X�^�C�}�[�̏����̒�����ݒ�@�o�[�́��ŕ\��
	this->setPercentage(0.0f);

	// �v���O���X�^�C�}�[�̃^�C�v��ݒ�i�o�[�ɐݒ�j
	this->setType(cocos2d::ProgressTimer::Type::BAR);

	// �v���O���X�^�C�}�[�o�[�̐L�т�����ix�����ɐݒ�j
	this->setBarChangeRate(cocos2d::Point(1, 0));

	// �v���O���X�^�C�}�[�o�[�̊�_�i�����ɐݒ�j
	this->setMidpoint(cocos2d::Point(0, 0));

	// �v���O���X�^�C�}�[�o�[�̈ʒu��ݒ�
	this->setPosition(WINDOW_RIGHT * 0.6f, WINDOW_TOP * 0.005f);

	// �X�V�����̐ݒ�
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	�X�V����
 *  @param	�o�ߎ���
 */
void CHpGuage::update(float deltaTime_) {

	// �v�f��NULL�Ȃ��΂�
	if (m_pStatus == NULL) {
		return;
	}

	// �v���C���[��HP�o�[���x���̐ݒ�
	if (0 <= this->m_pStatus->getHp()) {

		// ���݂̃��C�t���ő僉�C�t�Ɣ�r���ăp�[�Z���e�[�W�ɕϊ�
		// int�^�Ōv�Z�����0�ɂȂ��Ă��܂��̂�float�^�ɃL���X�g���Čv�Z����
		float hp = ((float)this->m_pStatus->getHp() / (float)this->m_pStatus->getMaxHp()) * 100.0f;

		// �\������摜�̊�����ݒ�
		this->setPercentage(hp);
	}
}

//=========================================================================
//	HP�\���p�̃��x���N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CScoreLabel::CScoreLabel() {

	m_pStatus = NULL;
}

// �f�X�g���N�^
CScoreLabel::~CScoreLabel() {
}

// ����������
bool CScoreLabel::init() {

	// ������̃t�H���g�̃^�C�v
	this->setSystemFontName("Arial");

	// ������̃t�H���g�T�C�Y
	this->setSystemFontSize(24);

	// ����
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::LEFT);

	// ��
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// �ʒu�̐ݒ�
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.9f));

	// �����ōX�V�������Ăяo��
	this->scheduleUpdate();

	return true;
}

/**
 *  @desc	�X�V����
 *  @param	�o�ߎ���
 */
void CScoreLabel::update(float deltaTime_) {

	// �v�f��NULL�Ȃ��΂�
	if (m_pStatus == NULL) {
		return;
	}

	// �\������X�R�A�𕶎���ɕϊ�
	std::string scoreStr = cocos2d::StringUtils::format("TOTALSCORE:%d", this->m_pStatus->getScore());

	// �\�����镶����
	this->setString(scoreStr);
}

//=========================================================================
//	�X�e�[�W�\���p�̃��x���N���X�̃R�[�h�͂������珑��
//=========================================================================

// �R���X�g���N�^
CStageNoLabel::CStageNoLabel() {

	m_pStage = NULL;
}

// �f�X�g���N�^
CStageNoLabel::~CStageNoLabel() {
}

// ����������
bool CStageNoLabel::init() {

	// ������̃t�H���g�̃^�C�v
	this->setSystemFontName("Arial");

	// ������̃t�H���g�T�C�Y
	this->setSystemFontSize(24);

	// �E��
	this->setHorizontalAlignment(cocos2d::CCTextAlignment::RIGHT);

	// ��
	this->setDimensions(WINDOW_RIGHT, this->getContentSize().height);

	// �ʒu�̐ݒ�
	this->setPosition(cocos2d::Point(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.97f));

	// �����ōX�V�������Ăяo��
	this->scheduleUpdate();

	return true;
}

/**
*  @desc	�X�V����
*  @param	�o�ߎ���
*/
void CStageNoLabel::update(float deltaTime_) {

	// �v�f��NULL�Ȃ��΂�
	if (m_pStage == NULL) {
		return;
	}

	// �\������X�e�[�W�𕶎���ɕϊ�
	std::string stageStr = cocos2d::StringUtils::format("STAGE:%d", *this->m_pStage);

	// �\�����镶����
	this->setString(stageStr);
}
