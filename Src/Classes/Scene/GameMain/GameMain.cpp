//
//  GameMain.cpp
//  ShootingGame
//
//  Created by �i����v on 2015/05/18.
//
//

#include "GameMain.h"
#include "Lib/InputManager/InputManager.h"
#include <stdlib.h>
#include <time.h>

// AudioEngine�C���N���[�h
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

/**
 *	@desc	�V�[���̐���
 *	@return	CMain ���C���[�������V�[���N���X�C���X�^���X
 *	@tips	�ÓI�����o�֐�
 */
cocos2d::Scene* CGameMain::createScene() {
	
	// �V�[���̐���
	cocos2d::Scene* pScene = cocos2d::Scene::create();
	
	// GameMain ���C���[�̐���
	CGameMain* pLayer = CGameMain::create() ;
	
	// CMain ���C���[���V�[���Ɏ��t����
	pScene->addChild( pLayer ) ;
	
	// ���������V�[����Ԃ�
	return pScene ;
}

/**
 *	@desc	�L�[�{�[�h�̃L�[���������ۂ̃C�x���g
 *	@param	�L�[�R�[�h
 *	@param	�C�x���g
 */
void CGameMain::onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event ) {

	// ���͊Ǘ��ɓ��͏������ϑ�
	CInputManager::getInstance()->onKeyPressed( keyCode ) ;
}

/**
 *	@desc	�L�[�{�[�h�̃L�[�𗣂����ۂ̃C�x���g
 *	@param	�L�[�R�[�h
 *	@param	�C�x���g
 */
void CGameMain::onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event ) {

	// ���͊Ǘ��ɓ��͏������ϑ�
	CInputManager::getInstance()->onKeyReleased( keyCode ) ;
}

// �f�X�g���N�^
CGameMain::~CGameMain() {

	// BGM�̒�~
	AudioEngine::stopAll();

	// BGM�̉��
	AudioEngine::uncache(SOUND_FILE_BGM);

	// �G�o���f�[�^�̔z����
	SAFE_DELETE_ARRAY(m_pEnemyLaunchData);

	if (this->m_activeFlag == false) {

		this->m_activeFlag = true;

		return;
	}
	// AudioEngine�̏I������
	AudioEngine::end();

}

/**
 *	@desc	������
 *	@return	true...����	false...���s
 */
bool CGameMain::init() {
	
	// �e���C���[�̏�����
	if ( Layer::init() == false ) {
		return false ;
	}
	
	// ��������
	srand( time( NULL ) ) ;
	
	// �L�[�{�[�h���̓C�x���g�󂯎��ݒ�
	this->setKeyboardEnabled(true) ;
	
	// update �֐� ( �X�V�����֐� ) �Ăяo���ݒ�
	// ���̕�������������R�����g�A�E�g����� update �֐����Ă΂�Ȃ��Ȃ�̂Œ���
	this->scheduleUpdate() ;
	
	//=========================================================================
	//
	//	�������珉�����A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================
    	
	//=========================================================================
	//	�X�e�[�W�̊O���t�@�C���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �X�e�[�W1�̓G�o���f�[�^�̃t�@�C����
	this->m_enemyLaunchDataFileNameList[0] = ENEMY_LAUNCH_DATA_FILE_1;

	// �X�e�[�W2�̓G�o���f�[�^�̃t�@�C����
	this->m_enemyLaunchDataFileNameList[1] = ENEMY_LAUNCH_DATA_FILE_2;

	// �X�e�[�W3�̓G�o���f�[�^�̃t�@�C����
	this->m_enemyLaunchDataFileNameList[2] = ENEMY_LAUNCH_DATA_FILE_3;


	//=========================================================================
	//	�}�b�v�̊O���t�@�C���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �X�e�[�W1�̓G�o���f�[�^�̃t�@�C����
	this->m_mapLaunchDataFileNameList[0] = MAP_FILE_STAGE_1;

	// �X�e�[�W2�̓G�o���f�[�^�̃t�@�C����
	this->m_mapLaunchDataFileNameList[1] = MAP_FILE_STAGE_2;

	// �X�e�[�W3�̓G�o���f�[�^�̃t�@�C����
	this->m_mapLaunchDataFileNameList[2] = MAP_FILE_STAGE_3;

	//=========================================================================
	//	�}�b�v�̏����ݒ�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �}�b�v�̐���
	this->m_pMap = CMap::create(this->m_mapLaunchDataFileNameList[this->m_mapNo - 1]);

	// �����ʒu�̐ݒ�
	this->m_pMap->m_scrollMove.setPos(0, 0);

	// �X�N���[�����x�̐ݒ�
	this->m_pMap->m_scrollMove.m_velX = 1.0f;

	// ���[�v�̐ݒ�i���[�v����̐ݒ�j
	this->m_pMap->m_isLoop = true;

	// ���C���[�Ɏ��t��
	this->addChild(this->m_pMap);

	//=========================================================================
	//	�v���C���[�̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �v���C���[�L�����N�^�[�̐���
	// CPlayerCharacter�N���X�ŃI�[�o�[���C�h���Ă���ÓI�����o�֐�create()��
	// �Ăяo����A������CPlayerCharacter�N���X��init()�����o�֐����Ăяo�����
	m_pPlayerChara = CPlayerCharacter::create();

	// �v���C���[�L�����N�^�[�̎��t��
	this->addChild(this->m_pPlayerChara);
    
	//=========================================================================
	//	�v���C���[�o���b�g ( �v���C���[�̒e ) �̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================
	
	// �v���C���[�̒e�̔z��̏�����
	for(int i = 0; i < CBullet::MAX_BULLET; i++) {

		m_pPlayerBullets[i] = NULL;
	}
	
	//=========================================================================
	//	�G�̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		m_pEnemyCharas[i] = NULL;
	}
	
	//=========================================================================
	//	�G�̒e�̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	//�G�̒e�̔z��̏�����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		m_pEnemyBullets[i] = NULL;
	}

	//=========================================================================
	//	�G�o���f�[�^�̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// ���ݎQ�ƒ��̏o���f�[�^�ԍ��̏�����
	this->m_enemyLaunchIndex = 0;

	// �t���[���J�E���^�[�̏�����
	this->m_frameCounter = 0;

	//=========================================================================
	//	�G�o���f�[�^�̓ǂݍ��݂͂�������
	//=========================================================================

	this->loadEnemyLaunchData(this->m_enemyLaunchDataFileNameList[this->m_stageNo - 1]);

	//=========================================================================
	//	�G�t�F�N�g�̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		m_pEffects[i] = NULL;
	}

	//=========================================================================
	//	�A�C�e���̏������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		m_pItems[i] = NULL;
	}

	//=========================================================================
	//	BGM�̏����ݒ�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// BGM�̍Đ�
	this->m_bgmId = AudioEngine::play2d(SOUND_FILE_BGM);
	
	// ���[�v�̐ݒ�(true...���[�v���� false...���[�v���Ȃ�)
	AudioEngine::setLoop(this->m_bgmId, true);

	//=========================================================================
	//	���[�U�[�C���^�[�t�F�[�X�̏����ݒ�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// HP���x���̐����Ə�����
	m_pHpLabel = CHpLabel::create();
	// �v���C���[�̃X�e�[�^�X��ݒ�
	m_pHpLabel->setStatus(&m_pPlayerChara->m_status);
	// ���C���[�Ɏ��t����
	this->addChild(m_pHpLabel);

	// HP�Q�[�W�̐����Ə�����
	m_pHpGuage = CHpGuage::create();
	// �v���C���[�̃X�e�[�^�X��ݒ�
	m_pHpGuage->setStatus(&m_pPlayerChara->m_status);
	// ���C���[�Ɏ��t����
	this->addChild(m_pHpGuage);

	// �X�R�A���x���̐����Ə�����
	m_pScoreLabel = CScoreLabel::create();
	// �X�R�A�̃X�e�[�^�X��ݒ�
	m_pScoreLabel->setStatus(&m_pPlayerChara->m_status);
	// ���C���[�Ɏ��t����
	this->addChild(m_pScoreLabel);

	// �X�e�[�W���x���̐����Ə�����
	m_pStageNoLabel = CStageNoLabel::create();
	// �X�e�[�W��ݒ�
	m_pStageNoLabel->setStage(&this->m_stageNo);
	// ���C���[�Ɏ��t����
	this->addChild(m_pStageNoLabel);

	//=========================================================================
	//	�X�e�[�W�̏����ݒ�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->m_stageMode = STAGE_MODE::UPDATE;

	//=========================================================================
	//
	//	�����܂łɏ������A�����ݒ�̃R�[�h��ǉ�
	//
	//=========================================================================
	
	return true ;
}

/**
 *	@desc	�X�V����
 *	@param	�P�t���[���o�ߎ���
 */
void CGameMain::update( float deltaTime_ ) {

	//=========================================================================
	//	�X�e�[�W�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// esc �L�[����������Q�[���I��
	if (inputflag.m_esc == true) {

		cocos2d::Director::getInstance()->end();
	}

	// S�L�[����������
	if (inputflag.m_s == true) {

		// S�L�[��������Ă�����
		if (this->m_sKeyDownFlag == false) {
			// S�L�[�������ꂽ

			this->m_sKeyDownFlag = true;

			// ���[�h���ꎞ��~�ɕύX
			this->m_stageMode = STAGE_MODE::END;

			return;
		}
	}
	else {

		// S�L�[��������Ă�����
		if (this->m_sKeyDownFlag == true) {
			// S�L�[�������ꂽ

			this->m_sKeyDownFlag = false;
		}
	}

	// A�L�[����������
	if (inputflag.m_a == true) {

		// A�L�[��������Ă�����
		if (this->m_aKeyDownFlag == false) {
			// A�L�[�������ꂽ

			this->m_aKeyDownFlag = true;


			// �ǂ̃��[�h�̎���
			if (this->m_stageMode == STAGE_MODE::UPDATE) {

				// ���[�h���ꎞ��~�ɕύX
				this->m_stageMode = STAGE_MODE::PAUSE;
			}
			// �ǂ̃��[�h�̎���
			else if (this->m_stageMode == STAGE_MODE::PAUSE) {

				// ���̃��[�h�ɕύX
				this->m_stageMode = STAGE_MODE::UPDATE;
			}


			return;
		}
	}
	else {

		// A�L�[��������Ă�����
		if (this->m_aKeyDownFlag == true) {
			// A�L�[�������ꂽ

			this->m_aKeyDownFlag = false;
		}
	}

	// ���[�h�ɂ��؂�ւ�
	switch (this->m_stageMode) {

	// �X�e�[�W�J�n����
	case STAGE_MODE::START:

		this->stageStartFunc();

		break;

	// �X�e�[�W�X�V����
	case STAGE_MODE::UPDATE:

		this->stageUpdateFunc(deltaTime_);

		break;

	// �X�e�[�W�I������
	case STAGE_MODE::END:

		this->stageEndFunc();

		break;

	// �X�e�[�W�ꎞ��~����
	case STAGE_MODE::PAUSE:

		this->stagePauseFunc();

		break;

	default:

		break;
	}


}

//=========================================================================
//	�Q�[�����C���N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

/**
 *  @desc	�G�o���f�[�^�̓ǂݍ���
 *  @param	�ǂݍ��ރt�@�C����
 *  @param	true...�ǂݍ��ݐ���
 */
bool CGameMain::loadEnemyLaunchData(std::string fileName) {

	// �t�@�C�����J��
	fileName = std::string("Resources/") + fileName;
	FILE* fp = fopen(fileName.c_str(), "r");

	if (fp == NULL) {
		CCLOG("�t�@�C�������݂��܂���B\n");
	}

	// �G�o���ő吔
	int maxCount = 0;

	// �ǂݍ��ݗp�̓G�o���f�[�^�̃C���X�^���X
	CEnemyLaunchData launchData;

	// �s���i�G�o���ő吔�j�𐔂���ׂɂ���񂵂���
	while (fscanf(fp, "%d, %f, %f, %d",
		&launchData.m_frame,
		&launchData.m_posX, &launchData.m_posY,
		&launchData.m_type) != EOF) {

		maxCount++;
	}

	// �s�����i�G�o���ő吔���j�̔z����쐬����
	this->m_pEnemyLaunchData = new CEnemyLaunchData[maxCount];

	// �t�@�C���|�C���^��擪�ɖ߂�
	rewind(fp);

	// ���������G�o���f�[�^�̔z��ɓǂݍ���
	for (int i = 0; i < maxCount; i++) {

		// 1�s�����e�[�^��ǂݍ���
		fscanf(fp, "%d, %f, %f, %d",
			&this->m_pEnemyLaunchData[i].m_frame,
			&this->m_pEnemyLaunchData[i].m_posX,
			&this->m_pEnemyLaunchData[i].m_posY,
			&this->m_pEnemyLaunchData[i].m_type
		);
	}

	// �t�@�C�������
	fclose(fp);

	// �G�o���ő吔��ݒ�
	CEnemyLaunchData::MAX_LAUNCH = maxCount;

	return true;
}


/**
 *  @desc	�G�̏o��
 *  @param	�G�̃^�C�v
 *  @param	�����ʒuX
 *  @param	�����ʒuY
 */
CEnemyCharacter* CGameMain::launchEnemy(CEnemyLaunchData* pEneLaunchData) {

	// �G�̐���
	CEnemyCharacter* pEne = CEnemyCharacter::create();

	switch (pEneLaunchData->m_type) {

	// �^�C�vA�̓G
	case ENEMY_TYPE::TYPE_A:

		// �摜�̐ݒ�
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// �؂����`
		pEne->m_chip = CChip(0, 32, 32, 32);

		// �؂����`�̐ݒ�
		pEne->setTextureRect(pEne->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�ix���W��-4���j
		pEne->m_move.setVel(-4, 0);

		// �G�̃A�j���[�V�����f�[�^�̏�����
		pEne->m_anime.set(10, 6);

		// �Փ˔���p�f�[�^�̏�����
		pEne->m_body.set(-16, 16, 16, -16);

		// ���˂���e�̃^�C�v�̐ݒ�
		// �ʏ�e
		pEne->m_bulletType = BULLET_TYPE::ENEMY_NORMAL;

		// �X�e�[�^�X�̏�����
		pEne->m_status.set(1, 1, 1, 100);

		// �s���^�C�v�̐ݒ�
		pEne->m_pAction = new CActionControllerZigZag;

		break;

	// �^�C�vB�̓G
	case ENEMY_TYPE::TYPE_B:

		// �摜�̐ݒ�
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// �؂����`
		pEne->m_chip = CChip(0, 64, 48, 32);

		// �؂����`�̐ݒ�
		pEne->setTextureRect(pEne->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�ix���W��-4���j
		pEne->m_move.setVel(-4, 0);

		// �G�̃A�j���[�V�����f�[�^�̏�����
		pEne->m_anime.set(10, 6);

		// �Փ˔���p�f�[�^�̏�����
		pEne->m_body.set(-24, 16, 24, -16);

		// ���˂���e�̃^�C�v�̐ݒ�
		// �z�[�~���O�e
		pEne->m_bulletType = BULLET_TYPE::ENEMY_AIMING;

		// �X�e�[�^�X�̏�����
		pEne->m_status.set(2, 2, 2, 500);

		// �s���^�C�v�̐ݒ�
		pEne->m_pAction = new CActionControllerConvex();

		break;

	// �^�C�vC�̓G
	case ENEMY_TYPE::TYPE_C:

		// �摜�̐ݒ�
		pEne->setTexture(IMAGE_FILE_CHARACTER);

		// �؂����`
		pEne->m_chip = CChip(0, 96, 48, 32);

		// �؂����`�̐ݒ�
		pEne->setTextureRect(pEne->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pEne->m_move.setPos(pEneLaunchData->m_posX + pEne->m_chip.size.width * 0.5f, pEneLaunchData->m_posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�ix���W��-4���j
		pEne->m_move.setVel(-4, 0);

		// �G�̃A�j���[�V�����f�[�^�̏�����
		pEne->m_anime.set(10, 6);

		// �Փ˔���p�f�[�^�̏�����
		pEne->m_body.set(-24, 16, 24, -16);

		// ���˂���e�̃^�C�v
		// 7Way�e
		pEne->m_bulletType = BULLET_TYPE::ENEMY_FAN;

		// �X�e�[�^�X�̏�����
		pEne->m_status.set(5, 5, 5, 1000);

		// �s���^�C�v�̐ݒ�
		pEne->m_pAction = new CActionControllerStraight();

		break;

	default:

		break;

	}

	// �^�C�v
	pEne->m_enemyType = pEneLaunchData->m_type;

	// �L���t���O��true������
	pEne->m_activeFlag = true;

	return pEne;
}

/**
 *  @desc	�G�t�F�N�g�̏o��
 *  @param	�G�t�F�N�g�̃^�C�v
 *  @param	�o���ʒuX
 *  @param	�o���ʒuY
 *  @return	�G�t�F�N�g�N���X�̃|�C���^
 */
CEffect* CGameMain::launchEffect(EFFECT_TYPE type, float posX, float posY) {

	// �G�t�F�N�g�̐���
	CEffect* pEff = CEffect::create();

	switch (type) {

	// �v���C���[�̔����^�C�v
	case EFFECT_TYPE::PLAYER_EXPLOSION:

		// �摜�̐ݒ�
		pEff->setTexture(IMAGE_FILE_EXPLOSION);

		// �؂����`
		pEff->m_chip = CChip(0, 0, 48, 48);

		// �؂����`�̐ݒ�
		pEff->setTextureRect(pEff->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pEff->m_move.setPos(posX, posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�
		pEff->m_move.setVel(0, 0);

		AudioEngine::play2d(SOUND_FILE_PLAYER_EXPLOSION);

		break;

	// �G�̔����^�C�v
	case EFFECT_TYPE::ENEMY_EXPLOSION:

		// �摜�̐ݒ�
		pEff->setTexture(IMAGE_FILE_EXPLOSION);

		// �؂����`
		pEff->m_chip = CChip(0, 0, 48, 48);

		// �؂����`�̐ݒ�
		pEff->setTextureRect(pEff->m_chip);

		// �����ʒu�̐ݒ�
		pEff->m_move.setPos(posX, posY);

		// �������x�̐ݒ�
		pEff->m_move.setVel(0, 0);

		AudioEngine::play2d(SOUND_FILE_ENEMY_EXPLOSION);

		break;

	default:

		break;
	}

	// �L���t���O��true������
	pEff->m_activeFlag = true;

	return pEff;
}

/**
 *  @desc	�A�C�e���̏o��
 *  @param	�A�C�e���̃^�C�v
 *  @param	�o���ʒuX
 *  @param	�o���ʒuY
 */
CItem * CGameMain::launchItem(ITEM_TYPE type, float posX, float posY) {

	// �A�C�e���̐���
	CItem* pItem = CItem::create();
	
	switch (type) {

	// �|�[�V�����^�C�v
	case ITEM_TYPE::PORTION:

		// �摜�̐ݒ�
		pItem->setTexture(IMAGE_FILE_ITEM);

		// �؂����`
		pItem->m_chip = CChip(0, 0, 24, 24);

		// �؂����`�̐ݒ�
		pItem->setTextureRect(pItem->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pItem->m_move.setPos(posX, posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�
		pItem->m_move.setVel(-1, 0);

		// �Փ˔���p�f�[�^�̏�����
		pItem->m_body.set(-12, 12, 12, -12);

		// �A�C�e���̃A�j���[�V�����f�[�^�̏�����
		pItem->m_anime.set(40, 2);

		//�A�C�e���^�C�v
		pItem->m_itemType = ITEM_TYPE::PORTION;

		break;

	// �u�[�X�g�^�C�v
	case ITEM_TYPE::BOOST:

		// �摜�̐ݒ�
		pItem->setTexture(IMAGE_FILE_ITEM);

		// �؂����`
		pItem->m_chip = CChip(24, 0, 24, 24);

		// �؂����`�̐ݒ�
		pItem->setTextureRect(pItem->m_chip);

		// �ړ��f�[�^�ɏ����ʒu�̐ݒ�
		pItem->m_move.setPos(posX, posY);

		// �ړ��f�[�^�ɏ������x�̐ݒ�
		pItem->m_move.setVel(-1, 0);

		// �Փ˔���p�f�[�^�̏�����
		pItem->m_body.set(-12, 12, 12, -12);

		// �e�̃A�j���[�V�����f�[�^�̏�����
		pItem->m_anime.set(40, 2);

		//�A�C�e���^�C�v
		pItem->m_itemType = ITEM_TYPE::BOOST;

		break;

	default:

		break;
	}

	// �L���t���O��true������
	pItem->m_activeFlag = true;

	return pItem;
}

// �G���o�������邩�ǂ����̃`�F�b�N�Ɋւ��郁���o�֐��̒�`
void CGameMain::checkLaunchEnemy() {

	// �Q�Ƃ���G�o���f�[�^�̔ԍ����o���ő吔�ȏ�ɂȂ�����o��������s��Ȃ�
	if (this->m_enemyLaunchIndex >= CEnemyLaunchData::MAX_LAUNCH) {
		return;
	}

	// ���ݎQ�ƒ��̏o���f�[�^�ԍ��̏o���e�[�^�̃t���[����
	// �t���[���J�E���^�[�Ɠ����Ȃ�o�����s��
	while (this->m_frameCounter == this->m_pEnemyLaunchData[this->m_enemyLaunchIndex].m_frame) {

		// �z���NULL�������Ă���Y���ԍ����擾
		int index = getFreeIndex(m_pEnemyCharas, CEnemyCharacter::MAX_ENEMY);

		// �󂢂Ă���v�f�����݂���Ώo���s��
		if (index != -1) {

			// �G���o��
			CEnemyCharacter* pEne = this->launchEnemy(
				&this->m_pEnemyLaunchData[this->m_enemyLaunchIndex]
			);

			// ���C���[�Ɏ��t����
			this->addChild(pEne);

			// �z��ɃA�h���X�����t����
			this->m_pEnemyCharas[index] = pEne;

			// �o�����s������
			// �Q�Ƃ��Ă���o���f�[�^�̔ԍ������̏o���f�[�^�̔ԍ��ɕύX����
			this->m_enemyLaunchIndex++;
		}
	}
}

/**
 *  @desc	 �G�t�F�N�g�̌Ăяo��
 *  @param	 �G�t�F�N�g�̃^�C�v
 *  @param	 �o���ʒuX
 *  @param	 �o���ʒuY
 */
void CGameMain::checkLaunchEffect(EFFECT_TYPE type, float posX, float posY) {

	// �z���NULL�������Ă���Y���ԍ����擾
	int index = getFreeIndex(m_pEffects, CEffect::MAX_EFFECT);

	// �󂢂Ă���v�f�����݂���Ώo���s��
	if (index != -1) {

		// �G�t�F�N�g
		CEffect* pEff = launchEffect(type, posX, posY);

		// ���C���[�Ɏ��t����
		this->addChild(pEff);

		// �z��ɃA�h���X�����t����
		this->m_pEffects[index] = pEff;
	}
}

/**
 *  @desc	�A�C�e���̌Ăяo��
 *  @param  �o���ʒuX
 *  @param  �o���ʒuY
 */
void CGameMain::checkLaunchItem(ITEM_TYPE type, float posX, float posY) {

	// �z���NULL�������Ă���Y���ԍ����擾
	int index = getFreeIndex(m_pItems, CItem::MAX_ITEM);

	// �󂢂Ă���v�f�����݂���Ώo���s��
	if (index != -1) {

		// �A�C�e��
		CItem* pItem = launchItem(type, posX, posY);

		// ���C���[�Ɏ��t����
		this->addChild(pItem);

		// �z��ɃA�h���X�����t����
		this->m_pItems[index] = pItem;
	}
}

// ���͂Ɋւ��郁���o�֐��̒�`
void CGameMain::inputFunc() {

	// �f�o�b�N�p�A�v���C���[�����񂾏ꍇ�̋~�Ϗ���
	// ����ł���v���C���[�𐶂��Ԃ点��
	if (m_pPlayerChara->m_activeFlag == false) {

		if (inputflag.m_space == true) {

			// HP���ő�܂Ŗ߂�
			m_pPlayerChara->m_status.recoveryHp();

			// �L���t���O��true�ɂ���i�����Ԃ点��j
			m_pPlayerChara->m_activeFlag = true;
			// �\��������
			m_pPlayerChara->setVisible(true);
		}
	}

	// �v���C���[������ł������΂�
	if (m_pPlayerChara->m_activeFlag == false) {

		return;
	}

	// �E�ړ�
	if (inputflag.m_right == true) {
		// �E��󂪉����ꂽ

		// ���x�ɒl��ݒ肷��
		m_pPlayerChara->m_move.m_velX = m_pPlayerChara->m_status.getSpeed();
	}
	// ���ړ�
	if (inputflag.m_left == true) {
		// ����󂪉����ꂽ

		// ���x�ɒl��ݒ�
		m_pPlayerChara->m_move.m_velX = -m_pPlayerChara->m_status.getSpeed();
	}
	// �E�����������ĂȂ�
	if (inputflag.m_right == false && inputflag.m_left == false) {
		// �E���A�����Ƃ��ɗ�����Ă����ԂȂ�

		// ���x�����Z�b�g����
		m_pPlayerChara->m_move.m_velX = 0;
	}
	// ��ړ�
	if (inputflag.m_up == true) {
		// ���󂪉����ꂽ

		// ���x�ɒl��ݒ�
		m_pPlayerChara->m_move.m_velY = m_pPlayerChara->m_status.getSpeed();
	}
	// ���ړ�
	if (inputflag.m_down == true) {
		// ����󂪉����ꂽ

		// ���x�ɒl��ݒ�
		m_pPlayerChara->m_move.m_velY = -m_pPlayerChara->m_status.getSpeed();
	}
	// ������������ĂȂ�
	if (inputflag.m_up == false && inputflag.m_down == false) {
		// ����A�����Ƃ��ɗ����ꂽ��ԂȂ�

		// ���x�����Z�b�g����
		m_pPlayerChara->m_move.m_velY = 0;
	}

	// Z�L�[
	if (inputflag.m_z == true) {
		// z�L�[�������ꂽ

		// �v���C���[�̒e�̔��ˌĂяo��
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_NORMAL);		
	}

	// X�L�[
	if (inputflag.m_x == true) {
		// x�L�[�������ꂽ

		// 2Way�e�̌Ăяo��
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_2WAY);
	}

	// C�L�[
	if (inputflag.m_c == true) {
		// c�L�[�������ꂽ

		// 3Way�e�̌Ăяo��
		this->shotPlayerBullet(BULLET_TYPE::PLAYER_3WAY);
	}
}

// �ړ��Ɋւ��郁���o�֐��̒�`
void CGameMain::moveFunc() {

	// �}�b�v�̈ړ�����
	m_pMap->moveFunc();

	// �v���C���[�̈ړ�����
	m_pPlayerChara->moveFunc();

	// �v���C���[�̒e�̈ړ������i�������̂ق����y�j
	for (CBullet* pBul : this->m_pPlayerBullets) {

		if (pBul == NULL) {
			continue;
		}

		pBul->moveFunc();
	}

	// �G�̈ړ�����
	for (CEnemyCharacter* pEne : this->m_pEnemyCharas) {

		if (pEne == NULL) {
			continue;
		}

		pEne->moveFunc();
	}

	// �G�̒e�̈ړ�����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// NULL���������Ă������΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100�̈ړ��������s��
		m_pEnemyBullets[i]->moveFunc();
	}

	// �G�t�F�N�g�̈ړ�����
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// NULL���������Ă������΂�
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100�̈ړ��������s��
		m_pEffects[i]->moveFunc();
	}

	// �A�C�e���̈ړ�����
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULL���������Ă������΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100�̈ړ��������s��
		m_pItems[i]->moveFunc();
	}
}

/**
 *
 *  @desc	�v���C���[�ƓG�̒e�̏Փ˔���
 *
 */
void CGameMain::collisionPlayerAndEnemyBullets() {

	// �v���C���[������ł������΂�
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// �v���C���[�́u�Փ˔�����s���ۂ̈ʒu�v���擾
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// �G�̒e�͔z��Ȃ̂œG�̒e�A�P�P�ƏՓ˔�������
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// �L���t���O��false�̃C���X�^���X����΂�
		if (m_pEnemyBullets[i]->m_activeFlag == false) {
			continue;
		}

		// �G�̒e1���́u�Փ˔�����s���ۂ̈ʒu�v���擾
		CBody eneBulCollisionBody = m_pEnemyBullets[i]->m_body.getApplyPositionBody(
			m_pEnemyBullets[i]->m_move.m_posX,
			m_pEnemyBullets[i]->m_move.m_posY
		);

		// �Փ˔�����s��
		if (playerCollisionBody.collision(&eneBulCollisionBody) == true) {		
			// �Փ˂��Ă���
			
			// �v���C���[�̏Փ˔����̌v�Z���s��
			m_pPlayerChara->hits(&m_pEnemyBullets[i]->m_status);

			// �G�̒e�̗L���t���O��false�ɂ���
			m_pEnemyBullets[i]->m_activeFlag = false;
			
			// �G�t�F�N�g�̌Ăяo��
			this->checkLaunchEffect(
				EFFECT_TYPE::PLAYER_EXPLOSION,
				m_pPlayerChara->m_move.m_posX,
				m_pPlayerChara->m_move.m_posY
			);
		}
	}
}

/**
 *
 *  @desc	�v���C���[�ƓG�̏Փ˔���
 *
 */
void CGameMain::collisionPlayerAndEnemies() {

	// �v���C���[������ł������΂�
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// �v���C���[�́u�Փ˔�����s���ۂ̈ʒu�v���擾
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// �G�͔z��Ȃ̂ŁA�G�P�P�ƏՓ˔�������
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// �L���t���O��false�̃C���X�^���X����΂�
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		// �G��̕��́u�Փ˔�����s���ۂ̈ʒu�v���擾
		CBody eneCollisionBody = m_pEnemyCharas[i]->m_body.getApplyPositionBody(
			m_pEnemyCharas[i]->m_move.m_posX,
			m_pEnemyCharas[i]->m_move.m_posY
		);

		// �Փ˔�����s��
		if (playerCollisionBody.collision(&eneCollisionBody) == true) {
			// �Փ˂��Ă���

			// �v���C���[�̏Փ˔����̌v�Z���s��
			m_pPlayerChara->hits(&m_pEnemyCharas[i]->m_status);

			// �G�̏Փ˔����̌v�Z���s��
			m_pEnemyCharas[i]->hits(&m_pPlayerChara->m_status);

			// �G��|���Ă�����X�R�A�̌v�Z���s��
			if (m_pEnemyCharas[i]->m_activeFlag == false) {
				m_pPlayerChara->m_status.addScore(m_pEnemyCharas[i]->m_status.getScore());
			}

			// �G�t�F�N�g�̌Ăяo��
			this->checkLaunchEffect(
				EFFECT_TYPE::PLAYER_EXPLOSION,
				m_pPlayerChara->m_move.m_posX,
				m_pPlayerChara->m_move.m_posY
			);

			// �G�t�F�N�g�̌Ăяo��
			this->checkLaunchEffect(
				EFFECT_TYPE::ENEMY_EXPLOSION,
				m_pEnemyCharas[i]->m_move.m_posX,
				m_pEnemyCharas[i]->m_move.m_posY
			);

			// �^�C�vC�̓G�̂�
			if (m_pEnemyCharas[i]->m_enemyType == ENEMY_TYPE::TYPE_C) {

				int num = rand() % 2;

				// �A�C�e���̌Ăяo��
				this->checkLaunchItem(
					(ITEM_TYPE)num,
					m_pEnemyCharas[i]->m_move.m_posX,
					m_pEnemyCharas[i]->m_move.m_posY
				);
			}
		}
	}
}

/**
 *
 *  @desc	�v���C���[�̒e�ƓG�̏Փ˔���
 *
 */
void CGameMain::collisionEnemiesAndPlayerBullets() {

	// �G�͔z��Ȃ̂ŁA�G�P�P�ƏՓ˔�������
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// �L���t���O��false�̃C���X�^���X����΂�
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		// �G��̕��́u�Փ˔�����s���ۂ̈ʒu�v���擾
		CBody eneCollisionBody = m_pEnemyCharas[i]->m_body.getApplyPositionBody(
			m_pEnemyCharas[i]->m_move.m_posX,
			m_pEnemyCharas[i]->m_move.m_posY
		);

		// �e�͔z��Ȃ̂ŁA�e�P�P�ƏՓ˔�������
		for (int j = 0; j < CBullet::MAX_BULLET; j++) {

			// �v�f��NULL�Ȃ��΂�
			if (m_pPlayerBullets[j] == NULL) {
				continue;
			}

			// �L���t���O��false�̃C���X�^���X����΂�
			if (m_pPlayerBullets[j]->m_activeFlag == false) {
				continue;
			}

			// �v���C���[�̒e�́u�Փ˔�����s���ۂ̈ʒu�v���擾
			CBody playerCollisionBullet = m_pPlayerBullets[j]->m_body.getApplyPositionBody(
				m_pPlayerBullets[j]->m_move.m_posX,
				m_pPlayerBullets[j]->m_move.m_posY
			);

			// �Փ˔�����s��
			if (eneCollisionBody.collision(&playerCollisionBullet) == true) {
				// �Փ˂��Ă���

				// �e�̏Փ˔����̌v�Z���s��
				m_pEnemyCharas[i]->hits(&m_pPlayerBullets[j]->m_status);

				// �v���C���[�̒e�̗L���t���O��false�ɂ���
				m_pPlayerBullets[j]->m_activeFlag = false;

				// �G��|���Ă�����X�R�A�̌v�Z���s��
				if (m_pEnemyCharas[i]->m_activeFlag == false) {
					m_pPlayerChara->m_status.addScore(m_pEnemyCharas[i]->m_status.getScore());
				}

				// �G�t�F�N�g�̌Ăяo��
				this->checkLaunchEffect(
					EFFECT_TYPE::ENEMY_EXPLOSION,
					m_pEnemyCharas[i]->m_move.m_posX,
					m_pEnemyCharas[i]->m_move.m_posY
				);

				// �^�C�vC�̓G�̂�
				if (m_pEnemyCharas[i]->m_enemyType == ENEMY_TYPE::TYPE_C ) {

					int num = rand() % 2;

					// �A�C�e���̌Ăяo��
					this->checkLaunchItem(
						(ITEM_TYPE)num,
						m_pEnemyCharas[i]->m_move.m_posX,
						m_pEnemyCharas[i]->m_move.m_posY
					);					
				}
			}
		}
	}
}

/**
*
*  @desc	�A�C�e���ƃv���C���[�̏Փ˔���
*
*/
void CGameMain::collisionPlayerAndItems() {

	// �v���C���[������ł������΂�
	if (m_pPlayerChara->m_activeFlag == false) {
		return;
	}

	// �v���C���[�́u�Փ˔�����s���ۂ̈ʒu�v���擾
	CBody playerCollisionBody = m_pPlayerChara->m_body.getApplyPositionBody(
		m_pPlayerChara->m_move.m_posX,
		m_pPlayerChara->m_move.m_posY
	);

	// �A�C�e���͔z��Ȃ̂ŁA�A�C�e���P�P�ƏՓ˔�������
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		// �L���t���O��false�̃C���X�^���X����΂�
		if (m_pItems[i]->m_activeFlag == false) {
			continue;
		}

		// �G��̕��́u�Փ˔�����s���ۂ̈ʒu�v���擾
		CBody itemCollisionBody = m_pItems[i]->m_body.getApplyPositionBody(
			m_pItems[i]->m_move.m_posX,
			m_pItems[i]->m_move.m_posY
		);

		// �Փ˔�����s��
		if (playerCollisionBody.collision(&itemCollisionBody) == true) {
			// �Փ˂��Ă���

			// �v���C���[�̏Փ˔����̌v�Z���s��
			m_pItems[i]->hits(&m_pPlayerChara->m_status);
			
			// �A�C�e���̗L���t���O��������
			m_pItems[i]->m_activeFlag = false;
		}
	}
}

// �A�j���[�V�����Ɋւ��郁���o�֐��̒�`
void CGameMain::animationFunc() {

	// �v���C���[�̃A�j���[�V�����X�V����
	m_pPlayerChara->animationFunc();

	// �v���C���[�̒e�̃A�j���[�V����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100�̃A�j���[�V����
		m_pPlayerBullets[i]->animationFunc();
	}

	// �G�̃A�j���[�V����
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100�̃A�j���[�V����
		m_pEnemyCharas[i]->animationFunc();
	}

	// �G�̒e�̃A�j���[�V����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100�̃A�j���[�V����
		m_pEnemyBullets[i]->animationFunc();
	}

	// �G�t�F�N�g�̃A�j���[�V����
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100�̃A�j���[�V����
		m_pEffects[i]->animationFunc();
	}

	// �A�C�e���̃A�j���[�V����
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULL���������Ă������΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100�̃A�j���[�V����
		m_pItems[i]->animationFunc();
	}
}

// ��ʒ[����Ɋւ��郁���o�֐��̒�`
void CGameMain::endOfScreen() {

	// �v���C���[�̉�ʒ[�̏���
	m_pPlayerChara->endOfScreen();

	// �v���C���[�̒e�̉�ʊO���菈��
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100�̉�ʊO����
		m_pPlayerBullets[i]->endOfScreen();
	}

	// �G�̉�ʊO���菈��
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100�̉�ʊO����
		m_pEnemyCharas[i]->endOfScreen();
	}

	// �G�̒e�̉�ʊO���菈��
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100�̉�ʊO����
		m_pEnemyBullets[i]->endOfScreen();
	}

	// �A�C�e���̉�ʊO���菈��
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULL���������Ă������΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100�̉�ʊO����
		m_pItems[i]->endOfScreen();
	}
}

// �l�̔��f�Ɋւ��郁���o�֐��̒�`
void CGameMain::applyFunc() {

	// �}�b�v�̒l�̔��f����
	m_pMap->applyFunc();

	// �v���C���[�̒l�̔��f����
	m_pPlayerChara->applyFunc();

	// �v���C���[�̒e�̒l�̔��f����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {
		//NULL���������Ă������΂�
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		// 100�̒l�̔��f���s��
		m_pPlayerBullets[i]->applyFunc();
	}

	// �G�̒l�̔��f����
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {
		// NULL���������Ă������΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// 100�̒l�̔��f���s��
		m_pEnemyCharas[i]->applyFunc();
	}

	// �G�̒e�̒l�̔��f����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {
		// NULL���������Ă������΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		// 100�̒l�̔��f���s��
		m_pEnemyBullets[i]->applyFunc();
	}

	// �G�t�F�N�g�̒l�̔��f����
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// NULL���������Ă������΂�
		if (m_pEffects[i] == NULL) {
			continue;
		}

		// 100�̒l�̔��f���s��
		m_pEffects[i]->applyFunc();
	}

	// �A�C�e���̒l�̔��f����
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// NULL���������Ă������΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		// 100�̒l�̔��f���s��
		m_pItems[i]->applyFunc();
	}
}

// ���O�������Ɋւ��郁���o�֐��̒�`
void CGameMain::checkActiveFlagAndRemove() {

	// �v���C���[�̒e�̃��C���[����̎��O������
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		if (m_pPlayerBullets[i]->m_activeFlag == false) {

			// ���C���[������O��
			m_pPlayerBullets[i]->removeFromParent();

			// �z�����NULL����
			m_pPlayerBullets[i] = NULL;
		}
	}

	// �G�̃��C���[����̎��O������
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		if (m_pEnemyCharas[i]->m_activeFlag == false) {

			// ���C���[������O��
			m_pEnemyCharas[i]->removeFromParent();

			// �z�����NULL����
			m_pEnemyCharas[i] = NULL;
		}
	}

	// �G�̒e�̃��C���[����̎��O������
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		if (m_pEnemyBullets[i]->m_activeFlag == false) {
			
			// ���C���[������O��
			m_pEnemyBullets[i]->removeFromParent();

			// �z�����NULL����
			m_pEnemyBullets[i] = NULL;
		}
	}

	// �G�t�F�N�g�̃��C���[����̎��O������
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEffects[i] == NULL) {
			continue;
		}
		
		if (m_pEffects[i]->m_activeFlag == false) {
			
			// ���C���[������O��
			m_pEffects[i]->removeFromParent();

			// �z�����NULL����
			m_pEffects[i] = NULL;
		}
	}

	// �A�C�e���̃��C���[����̎��O������
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pItems[i] == NULL) {
			continue;
		}

		if (m_pItems[i]->m_activeFlag == false) {

			// ���C���[������O��
			m_pItems[i]->removeFromParent();

			// �z�����NULL����
			m_pItems[i] = NULL;
		}
	}
}

/**
 *  @desc	�v���C���[�̒e�̔���
 *  @param	�e�^�C�v
 */
void CGameMain::shotPlayerBullet(BULLET_TYPE type) {
	
	if (m_pPlayerChara->m_shotCounter > 0) {

		// �e���˃J�E���^�[��0����Ȃ�
		// �e���˗p�J�E���^�[���f�N�������g
		m_pPlayerChara->m_shotCounter--;
	}

	// �e���˗p�J�E���^�[��0�ȉ��Ȃ�e�̔��˂��s��
	if (m_pPlayerChara->m_shotCounter <= 0) {

		switch (type) {

			// �ʏ�e
		case BULLET_TYPE::PLAYER_NORMAL:
		{

			// �z���NULL�������Ă���Y���ԍ����擾
			int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

			// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
			if (index == -1) {
				return;
			}

			// �v���C���[�̒e�̔���
			CBullet* pBul = m_pPlayerChara->shotBullet(
				BULLET_TYPE::PLAYER_NORMAL,
				24.0f, 0.0f, 6.0f, 0.0f
			);

			// �v�f��NULL�Ȃ��΂�
			if (pBul == NULL) {
				return;
			}

			// ���C���[�Ɏ��t����
			this->addChild(pBul);

			// �z��ɃA�h���X�����t����
			this->m_pPlayerBullets[index] = pBul;
		}

		break;

		// 2Way�e
		case BULLET_TYPE::PLAYER_2WAY:
		{

			for (int i = 0; i < 2; i++) {

				// �z���NULL�������Ă���Y���ԍ����擾
				int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

				// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
				if (index == -1) {
					return;
				}

				// ���ˏC���ʒu�̌v�Z
				float fixX = (i == 0) ? 24 : -24;

				// ���x�̌v�Z
				float velX = (i == 0) ? 6 : -6;

				// �v���C���[�̒e�̔���
				CBullet* pBul = m_pPlayerChara->shotBullet(
					BULLET_TYPE::PLAYER_NORMAL,
					fixX, 0.0f, velX, 0.0f
				);

				// �v�f��NULL�Ȃ��΂�
				if (pBul == NULL) {
					return;
				}

				// ���C���[�Ɏ��t����
				this->addChild(pBul);

				// �z��ɃA�h���X�����t����
				this->m_pPlayerBullets[index] = pBul;
			}
		}

		break;

		// 3Way�e
		case BULLET_TYPE::PLAYER_3WAY:
		{

			for (int i = 0; i < 3; i++) {

				// �z���NULL�������Ă���Y���ԍ����擾
				int index = getFreeIndex(m_pPlayerBullets, CBullet::MAX_BULLET);

				// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
				if (index == -1) {
					return;
				}

				// �p�x
				float angle = i * -25.0f + 25.0f;

				// �p�x����2�����x�N�g�����擾
				float velX = cos(angle * M_PI / 180.0f) * 6.0f;
				float velY = sin(angle * M_PI / 180.0f) * 6.0f;

				// �v���C���[�̒e�̔���
				CBullet* pBul = m_pPlayerChara->shotBullet(
					BULLET_TYPE::PLAYER_3WAY,
					24.0f, 0.0f, velX, velY
				);

				// �v�f��NULL�Ȃ��΂�
				if (pBul == NULL) {
					return;
				}

				// ���C���[�Ɏ��t����
				this->addChild(pBul);

				// �z��ɃA�h���X�����t����
				this->m_pPlayerBullets[index] = pBul;
			}
		}

		break;

		default:

			break;
		}

		// �e���ˌ��ʉ��̍Đ�
		AudioEngine::play2d(SOUND_FILE_LAUNCH_BULLET);

		// �e�����˂��ꂽ��J�E���^�[���C���^�[�o���Ń��Z�b�g����
		m_pPlayerChara->m_shotCounter = m_pPlayerChara->m_shotInterval;
	}
}

// �G�̒e�̔��˂Ɋւ����`
void CGameMain::shotEnemyBullet() {

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// NULL��������ꂽ�炢�����΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// �e���˃t���O��false�Ȃ��΂�
		if (m_pEnemyCharas[i]->m_shotFlag == false) {
			continue;
		}

		// �G�������Ă���G�̒e�̃^�C�v�𔻒肵�Ēe�𔭎˂�����
		switch (m_pEnemyCharas[i]->m_bulletType) {

			// �ʏ�e
			case BULLET_TYPE::ENEMY_NORMAL:
			{
				
				// �z���NULL�������Ă���Y���ԍ����擾
				int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

				// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
				if (index == -1) {
					return;
				}

				// �G�̒e�̔���
				CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
					BULLET_TYPE::ENEMY_NORMAL,
					-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f),0, -8, 0
				);

				// �v�f��NULL�Ȃ��΂�
				if (pBul == NULL) {
					continue;
				}

				// ���C���[�Ɏ��t����
				this->addChild(pBul);

				// �z��ɃA�h���X�����t����
				this->m_pEnemyBullets[index] = pBul;
			}

				break;

			// �z�[�~���O�e
			case BULLET_TYPE::ENEMY_AIMING:
			{

				// �z���NULL�������Ă���Y���ԍ����擾
				int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

				// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
				if (index == -1) {
					return;
				}

				// �v���C���[�̈ʒu���擾
				float playerPosX = m_pPlayerChara->m_move.m_posX;
				float playerPosY = m_pPlayerChara->m_move.m_posY;

				// �G�̈ʒu���擾
				float enePosX = m_pEnemyCharas[i]->m_move.m_posX;
				float enePosY = m_pEnemyCharas[i]->m_move.m_posY;

				// �G����v���C���[�܂ł̃x�N�g�����v�Z
				float vecX = playerPosX - enePosX;
				float vecY = playerPosY - enePosY;

				// �x�N�g���̒��������߂�
				float len = sqrt(vecX * vecX + vecY * vecY);

				// 1�t���[���P�ʂ̃x�N�g�����v�Z����A�܂葬�x��1�̎��̑��x�����߂�B
				float velX = vecX / len;
				float velY = vecY / len;

				// �������v�Z����
				velX = velX * 5;
				velY = velY * 5;

				// �G�̒e�̔���
				CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
					BULLET_TYPE::ENEMY_NORMAL,
					-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f),0, velX, velY
				);

				// �v�f��NULL�Ȃ��΂�
				if (pBul == NULL) {
					continue;
				}

				// ���C���[�Ɏ��t����
				this->addChild(pBul);

				// �z��ɃA�h���X�����t����
				this->m_pEnemyBullets[index] = pBul;
			}

				break;

			// 7Way�e
			case BULLET_TYPE::ENEMY_FAN:
			{

				for (int j = 0; j < 7; j++) {

					// �z���NULL�������Ă���Y���ԍ����擾
					int index = getFreeIndex(m_pEnemyBullets, CBullet::MAX_BULLET);

					// �󂢂Ă���v�f�����݂��Ȃ���Βe�̔��ˏ����͍s��Ȃ�
					if (index == -1) {
						return;
					}

					// �p�x
					float angle = j * 30.0f + 90.0f;

					// �p�x����2�����x�N�g�����擾
					float velX = cos(angle * M_PI / 180.0f) * 6.0f;
					float velY = sin(angle * M_PI / 180.0f) * 6.0f;

					// �G�̒e�̔���
					CBullet* pBul = m_pEnemyCharas[i]->shotBullet(
						BULLET_TYPE::ENEMY_FAN,
						-(m_pEnemyCharas[i]->m_chip.size.width * 0.5f), 0, velX, velY
					);

					// �v�f��NULL�Ȃ��΂�
					if (pBul == NULL) {
						return;
					}

					// ���C���[�Ɏ��t����
					this->addChild(pBul);

					// �z��ɃA�h���X�����t����
					this->m_pEnemyBullets[index] = pBul;
				}
			}

				break;

			default:

				break;
		}

		// �e���˃t���O��������
		m_pEnemyCharas[i]->m_shotFlag = false;
	}
}

// �s���Ǘ��Ɋւ����`
void CGameMain::actionFunc() {

	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		// �v�f��NULL�Ȃ��΂�
		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		// �L���t���O��false�Ȃ��΂�
		if (m_pEnemyCharas[i]->m_activeFlag == false) {
			continue;
		}

		m_pEnemyCharas[i]->actionFunc();
	}
}

// �X�e�[�W�J�n�����Ɋւ����`
void CGameMain::stageStartFunc() {

	//=========================================================================
	//	BGM�̃X�e�[�W�؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// BGM�̓ǂݍ���
	AudioEngine::play2d(SOUND_FILE_BGM);

	//=========================================================================
	//	�G�o���̃X�e�[�W�؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// ���̃X�e�[�W�̓G�o���f�[�^�ǂݍ���
	this->loadEnemyLaunchData(this->m_enemyLaunchDataFileNameList[this->m_stageNo - 1]);

	//=========================================================================
	//	�}�b�v�̃X�e�[�W�؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// ���̃X�e�[�W�̃}�b�v�̐���
	// �}�b�v�̐���
	this->m_pMap = CMap::create(this->m_mapLaunchDataFileNameList[this->m_mapNo - 1]);

	// �����ʒu�̐ݒ�
	this->m_pMap->m_scrollMove.setPos(0, 0);

	// �X�N���[�����x�̐ݒ�
	this->m_pMap->m_scrollMove.m_velX = 1.0f;

	// ���[�v�ݒ�i���[�v����ݒ�j
	this->m_pMap->m_isLoop = true;

	// ���C���[�Ɏ��t����
	this->addChild(this->m_pMap, -1);

	//=========================================================================
	//	�v���C���[�̃X�e�[�W�؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �v���C���[�̗L���t���O���グ��
	m_pPlayerChara->m_activeFlag = true;

	// �v���C���[��\������
	m_pPlayerChara->setVisible(true);

	// �v���C���[�̏����ʒu�̐ݒ�
	m_pPlayerChara->m_move.setPos(100.0f, 240.0f);

	// �����ʒu�̔��f
	m_pPlayerChara->setPosition(m_pPlayerChara->m_move.m_posX, m_pPlayerChara->m_move.m_posY);

	//=========================================================================
	//	UI�̃X�e�[�W�؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// HP���x���̕\��
	m_pHpLabel->setVisible(true);

	// HP�Q�[�W�̕\��
	m_pHpGuage->setVisible(true);

	// �X�R�A���x���̕\��
	m_pScoreLabel->setVisible(true);

	// �X�e�[�W���x���̕\��
	m_pStageNoLabel->setVisible(true);

	// ���[�h���X�e�[�W�X�V�����ɐ؂�ւ���
	this->m_stageMode = STAGE_MODE::UPDATE;
}

/**
 *  @desc	�X�e�[�W�X�V����
 *  @param	�o�ߎ���
 */
void CGameMain::stageUpdateFunc(float deltaTime) {

	//=========================================================================
	//
	//	�����ɍX�V�����̃R�[�h��ǉ����Ă���
	//
	//=========================================================================

	//=========================================================================
	//	���͏����Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->inputFunc();

	//=========================================================================
	//	�G�̏o���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->checkLaunchEnemy();

	//=========================================================================
	//	�s�������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->actionFunc();

	//=========================================================================
	//	�ړ������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->moveFunc();

	//=========================================================================
	//	��ʒ[���菈���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->endOfScreen();

	//=========================================================================
	//	�Փ˔��菈���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->collisionPlayerAndEnemyBullets();
	this->collisionPlayerAndEnemies();
	this->collisionEnemiesAndPlayerBullets();
	this->collisionPlayerAndItems();

	//=========================================================================
	//	�G�̒e�̔��˂Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->shotEnemyBullet();

	//=========================================================================
	//	�A�j���[�V���������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->animationFunc();

	//=========================================================================
	//	��\�������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	if (m_pPlayerChara->m_activeFlag == false) {

		//���ɔ�\���̏ꍇ�͔�\�������͍s��Ȃ�
		if (m_pPlayerChara->isVisible() == true) {

			//��\���ɂ���
			m_pPlayerChara->setVisible(false);
		}
	}

	//=========================================================================
	//	���O�������Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->checkActiveFlagAndRemove();

	//=========================================================================
	//	�v�Z�p�f�[�^�𔽉f�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	this->applyFunc();

	//=========================================================================
	//	���̑��̃R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �t���[���J�E���^�[�̍X�V
	this->m_frameCounter++;

}

// �X�e�[�W�I�������Ɋւ����`
void CGameMain::stageEndFunc() {

	//=========================================================================
	//	BGM�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// BGM�̒�~
	AudioEngine::stopAll();

	//=========================================================================
	//	�v���C���[�L�����N�^�[�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �v���C���[�̗L���t���O�������ĉ�ʂ����\��
	if (m_pPlayerChara != NULL) {

		// �L���t���O���Q�[���̍��ɂ���Ă͉�����K�v���Ȃ�
		m_pPlayerChara->m_activeFlag = false;

		// �v���C���[�͎g���܂킷�̂ł��������폜������\���ɂ��邾��
		m_pPlayerChara->setVisible(false);
	}

	//=========================================================================
	//	�v���C���[�o���b�g�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �v���C���[�o���b�g�����C���[������O����NULL����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		if (m_pPlayerBullets[i] == NULL) {
			continue;
		}

		m_pPlayerBullets[i]->removeFromParent();
		m_pPlayerBullets[i] = NULL;
	}

	//=========================================================================
	//	�G�L�����N�^�[�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �G�����C���[������O����NULL����
	for (int i = 0; i < CEnemyCharacter::MAX_ENEMY; i++) {

		if (m_pEnemyCharas[i] == NULL) {
			continue;
		}

		m_pEnemyCharas[i]->removeFromParent();
		m_pEnemyCharas[i] = NULL;
	}

	//=========================================================================
	//	�G�l�~�[�o���b�g�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �G�l�~�[�o���b�g�����C���[������O����NULL����
	for (int i = 0; i < CBullet::MAX_BULLET; i++) {

		if (m_pEnemyBullets[i] == NULL) {
			continue;
		}

		m_pEnemyBullets[i]->removeFromParent();
		m_pEnemyBullets[i] = NULL;
	}

	//=========================================================================
	//	�G�t�F�N�g�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �G�t�F�N�g�����C���[������O����NULL����
	for (int i = 0; i < CEffect::MAX_EFFECT; i++) {

		if (m_pEffects[i] == NULL) {
			continue;
		}

		m_pEffects[i]->removeFromParent();
		m_pEffects[i] = NULL;
	}

	//=========================================================================
	//	�A�C�e���Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �A�C�e�������C���[������O����NULL����
	for (int i = 0; i < CItem::MAX_ITEM; i++) {

		if (m_pItems[i] == NULL) {
			continue;
		}

		m_pItems[i]->removeFromParent();
		m_pItems[i] = NULL;
	}

	//=========================================================================
	//	�G�o���f�[�^�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �G�o���f�[�^�̉��
	SAFE_DELETE_ARRAY(m_pEnemyLaunchData);

	// �G�o���C���f�b�N�X�̃��Z�b�g
	m_enemyLaunchIndex = 0;

	// �t���[���J�E���^�[�̃��Z�b�g
	m_frameCounter = 0;

	//=========================================================================
	//	�}�b�v�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �}�b�v���C���[������O����NULL����
	if (m_pMap != NULL) {
		
		m_pMap->removeFromParent();
		m_pMap = NULL;
	}

	//=========================================================================
	//	UI�Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	// �S�Ă�UI����U��\���ɂ���
	m_pHpLabel->setVisible(false);
	m_pHpGuage->setVisible(false);
	m_pScoreLabel->setVisible(false);
	m_pStageNoLabel->setVisible(false);

	//=========================================================================
	//	�X�e�[�W�̐؂�ւ��Ɋւ���R�[�h�̒ǉ��͂�������
	//=========================================================================

	if (this->m_stageNo >= CGameMain::MAX_STAGE_COUNT) {

		// TODO:�����Łu�G���f�B���O�V�[���v�ɐi��
	}
	else {

		// �X�e�[�W���X�V�i�C���N�������g���邾���j
		this->m_stageNo++;

		this->m_mapNo++;

		this->m_activeFlag = true;

		if (this->m_activeFlag == true) {

			this->m_activeFlag = false;

			// ���[�h���X�e�[�W�J�n�ɐ؂�ւ���
			this->m_stageMode = STAGE_MODE::START;
		}
	}
}

// �X�e�[�W�ꎞ��~�����Ɋւ����`
void CGameMain::stagePauseFunc() {

}