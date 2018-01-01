#include "Title.h"
#include "Constants.h"
#include "Scene/GameMain/GameMain.h"

// AudioEngine�C���N���[�h
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

// cocos2d::�̏ȗ�
using namespace cocos2d;

// �f�X�g���N�^
CTitle::~CTitle() {

	if (this->m_activeFlag == false) {
		return;
	}
	// AudioEngine�̏I������
	AudioEngine::end();
}

/**
 *  @desc		�V�[���̐���
 *  @return		CTitle���C���[�������V�[���N���X�C���X�^���X
 *  @tips		�ÓI�����o�֐�
 */
cocos2d::Scene* CTitle::createScene() {

	// �V�[���̐���
	cocos2d::Scene* pScene = cocos2d::Scene::create();

	// CTitle���C���[�̐���
	CTitle* pLayer = CTitle::create();

	// CTitle���C���[���V�[���Ɏ��t����
	pScene->addChild(pLayer);

	// ���������V�[����Ԃ�
	return pScene;
}

/**
*  @desc		������
*  @return		true...����	false...���s
*/
bool CTitle::init() {

	// ��ꃌ�C���[�̏�����
	if (Layer::init() == false) {

		return false;
	}

	// update�֐��i�X�V�����֐��j�Ăяo���ݒ�
	this->scheduleUpdate();

	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	//=========================================================================
	//	�w�i�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �^�C�g���w�i�̐����Ǝ��t��
	cocos2d::Sprite* pBg = cocos2d::Sprite::create(IMAGE_FILE_TITLE_BG);

	// �����ʒu�̐ݒ�
	pBg->setPosition(320, 240);

	// ���C���[�Ɏ��t��
	this->addChild(pBg);

	//=========================================================================
	//	�^�C�g�����x���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �^�C�g�����x���̐����Ǝ��t��
	Label* pTitleLabel = Label::createWithTTF("ShootingGame", FONT_FILE_STARWARS, 62);

	// �����ʒu�̐ݒ�
	pTitleLabel->setPosition(320, 360);

	// �����F�̐ݒ�
	pTitleLabel->setColor(Color3B(0, 200, 200));

	// ���C���[�Ɏ��t��
	this->addChild(pTitleLabel);

	//=========================================================================
	//	���j���[�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	/*
	 *  @desc		���j���[�A�C�e���̐����@�X�^�[�g�{�^��
	 *  @param		�ʏ�̉摜��ݒ�
	 *  @param		�����ꂽ���̉摜��ݒ�
	 *  @param		�����ꂽ���ɌĂяo����郁���o�֐��̐ݒ�
	 */
	MenuItemImage* pStartBtnItem = MenuItemImage::create(
		IMAGE_FILE_START_BUTTON,
		IMAGE_FILE_START_BUTTON_SELECTED,
		CC_CALLBACK_1(CTitle::callbackChangeGameMain, this)
	);

	// �ʒu�̐ݒ�
	pStartBtnItem->setPosition(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.4f);

	/*
	 *  @desc		���j���[�A�C�e���̐����@�I���{�^��
	 *  @param		�ʏ�̉摜��ݒ�
	 *  @param		�����ꂽ���̉摜��ݒ�
	 *  @param		�����ꂽ���ɌĂяo����郁���o�֐��̐ݒ�
	 */
	MenuItemImage* pEndBtnItem = MenuItemImage::create(
		IMAGE_FILE_END_BUTTON,
		IMAGE_FILE_END_BUTTON_SLECTED,
		CC_CALLBACK_1(CTitle::callbackEndGame, this)
	);

	// �ʒu�̐ݒ�
	pEndBtnItem->setPosition(WINDOW_RIGHT * 0.5f, WINDOW_TOP * 0.2f);

	// ���j���[�̐����ƃA�C�e���̓o�^
	Menu* pMenu = Menu::create(pStartBtnItem, pEndBtnItem, NULL);

	// �ʒu�̏�����
	pMenu->setPosition(0, 0);

	// ���C���[�Ƀ��j���[��o�^����
	this->addChild(pMenu);

	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================

	return true;
}

/**
*  @desc		�X�V����
*  @param		�o�ߎ���
*/
void CTitle::update(float deltaTime) {

	//=========================================================================
	//
	//	��������X�V�����̃R�[�h��ǉ�
	//
	//=========================================================================


}

/**
 *  @desc		�Q�[�����C���ɑJ��
 *  @param		�^�C�g�����C���[�̃C���X�^���X
 *  @tips		�X�^�[�g�{�^���������ꂽ���ɌĂяo�����
 */
void CTitle::callbackChangeGameMain(cocos2d::Ref * pSender) {

	OutputDebugString(L"�X�^�[�g�{�^���������ꂽ��\n");

	// �V�[���𐶐�����
	Scene* pScene = CGameMain::createScene();

	this->m_activeFlag = true;

	if (this->m_activeFlag == true) {

		this->m_activeFlag = false;

		// �V�[����؂�ւ���
		Director::getInstance()->replaceScene(pScene);
	}
}

/**
 *  @desc		�Q�[���I��
 *  @param		�^�C�g�����C���[�̃C���X�^���X
 *  @param		�Q�[���I���{�^���������ꂽ���ɌĂяo�����
 */
void CTitle::callbackEndGame(cocos2d::Ref * pSender) {

	OutputDebugString(L"�Q�[���I���{�^���������ꂽ��\n");

	Director::getInstance()->end();
}
