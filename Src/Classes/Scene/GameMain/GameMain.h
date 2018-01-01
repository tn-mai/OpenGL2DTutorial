//
//  GameMain.h
//  ShootingGame
//
//  Created by �i����v on 2015/05/18.
//
//

#ifndef __ShootingGame__Main__
#define __ShootingGame__Main__

#include "cocos2d.h"

//=========================================================================
//	�ǉ��̃C���N���[�h�͂�������
//=========================================================================

#include "Constants.h"
#include "Model/Character/Character.h"
#include "Data/EnemyLaunchData/EnemyLaunchData.h"
#include "Model/UserInterface/UserInterface.h"
#include "Model/Item/Item.h"
#include "Model/Map/Map.h"

//=========================================================================
//	�X�e�[�W�Ɋւ���^�C�v�Ȃǂ̗񋓂͂������珑���Ă���
//=========================================================================

// �X�e�[�W���[�h
enum class STAGE_MODE {

	// �X�e�[�W���[�h����
	NONE = -1,

	// �X�e�[�W�J�n����
	START,

	// �X�e�[�W�X�V������
	UPDATE,

	// �X�e�[�W�I������
	END,

	// �X�e�[�W�ꎞ��~��
	PAUSE,
};

/*
 *	�Q�[�����C�����C���[
 *
 *		�Q�[���̃��C���ƂȂ郌�C���[
 *		���߂͂��̕����ɑS�Ă������Ă���
 *
 */
class CGameMain : public cocos2d::Layer {
public:
	//=========================================================================
	//	��������͗����o����܂ł͕ύX�֎~
	//=========================================================================

	// �f�X�g���N�^
	~CGameMain() ;
	
	/**
	 *	@desc	�V�[���̐���
	 *	@return	CMain ���C���[�������V�[���N���X�C���X�^���X
	 *	@tips	�ÓI�����o�֐�
	 */
    static cocos2d::Scene* createScene() ;
	
	/*
	 *	@desc	�V�[���������ɏ������֐�������
	 *			�������֐����s���� NULL ��Ԃ�
	 */
    CREATE_FUNC( CGameMain );
	
	/**
	 *	@desc	�L�[�{�[�h�̃L�[���������ۂ̃C�x���g
	 *	@param	�L�[�R�[�h
	 *	@param	�C�x���g
	 */
	virtual void onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event ) override ;
	
	/**
	 *	@desc	�L�[�{�[�h�̃L�[�𗣂����ۂ̃C�x���g
	 *	@param	�L�[�R�[�h
	 *	@param	�C�x���g
	 */
	virtual void onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event ) override ;
	
	/**
	 *	@desc	������
	 *	@return	true...����	false...���s
	 */
    virtual bool init() override ;

	/**
	 *	@desc	�X�V����
	 *	@param	�o�ߎ���
	 */
	virtual void update( float deltaTime_ ) override ;
	
	//=========================================================================
	//	�����܂ł͗����o����܂ł͕ύX�֎~
	//=========================================================================
	
public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================
	
	// �v���C���[�L�����N�^�[
	CPlayerCharacter* m_pPlayerChara;
	
	// �G�L�����N�^�[
	CEnemyCharacter* m_pEnemyCharas[CEnemyCharacter::MAX_ENEMY];

	// �v���C���[�o���b�g ( �v���C���[�̒e )
	CBullet* m_pPlayerBullets[CBullet::MAX_BULLET];

	// �G�l�~�[�o���b�g ( �G�̒e )
	CBullet* m_pEnemyBullets[CBullet::MAX_BULLET];

	// �G�o���f�[�^
	CEnemyLaunchData* m_pEnemyLaunchData;

	// �G�t�F�N�g
	CEffect* m_pEffects[CEffect::MAX_EFFECT];

	// �A�C�e��
	CItem* m_pItems[CItem::MAX_ITEM];

	// HP���x��
	CHpLabel* m_pHpLabel;

	// HP�Q�[�W
	CHpGuage* m_pHpGuage;

	// �X�R�A���x��
	CScoreLabel* m_pScoreLabel;

	// �X�e�[�W���x��
	CStageNoLabel* m_pStageNoLabel;

	// �}�b�v
	CMap* m_pMap = NULL;

	// �X�e�[�^�X
	CStatus m_status;
	
	// �X�e�[�W���[�h
	STAGE_MODE m_stageMode = STAGE_MODE::NONE;

	// �X�e�[�W�ő吔
	static const int MAX_STAGE_COUNT = 3;

	// �}�b�v�ő吔
	static const int MAX_MAP_COUNT = 3;

	// ���݃X�e�[�W
	int m_stageNo = 1;

	// ���݂̃}�b�v
	int m_mapNo = 1;

	// ���݉��Ԗڂ̏o���f�[�^�𒲂ׂĂ���̂��̔ԍ�
	int m_enemyLaunchIndex;

	// �t���[���J�E���^�[
	int m_frameCounter;

	// BGM�̎���ID
	int m_bgmId;

	// �X�e�[�W���̓G�o���f�[�^�̃t�@�C�����̔z��i�X�e�[�W�ő吔���j
	std::string m_enemyLaunchDataFileNameList[CGameMain::MAX_STAGE_COUNT];

	// �X�e�[�W���̃}�b�v�f�[�^�̃t�@�C�����̔z��i�}�b�v�ő吔���j
	std::string m_mapLaunchDataFileNameList[CGameMain::MAX_MAP_COUNT];

	/**
	 *  @desc	�G�o���f�[�^�̓ǂݍ���
	 *  @param	�ǂݍ��ރt�@�C����
	 *  @param	true...�ǂݍ��ݐ���
	 */
	bool loadEnemyLaunchData(std::string fileName);

	// A�L�[���d����h�~�t���O�i������Ă�����true�j
	bool m_aKeyDownFlag = false;

	// S�L�[���d����h�~�t���O
	bool m_sKeyDownFlag = false;

	// �L���t���O
	bool m_activeFlag;

	/**
	 *  @desc	�G�̏o��
	 *  @param	�Q�Ƃ���G�Ǘ��f�[�^�̃A�h���X
	 */
	CEnemyCharacter* launchEnemy(CEnemyLaunchData* pEneLaunchData);

	/**
	 *  @desc	 �G�t�F�N�g�̏o��
	 *  @param	 �G�t�F�N�g�̃^�C�v
	 *  @param	 �o���ʒuX
	 *  @param	 �o���ʒuY
	 *  @return	 �G�t�F�N�g�N���X�̃|�C���^
	 */
	CEffect* launchEffect(EFFECT_TYPE type, float posX, float posY);

	/**
	 *  @desc	�A�C�e���̏o��
	 *  @param	�A�C�e���̃^�C�v
	 *  @param	�o���ʒuX
	 *  @param	�o���ʒuY
	 */
	CItem* launchItem(ITEM_TYPE type, float posX, float posY);

	// �G���o�������邩�ǂ����̃`�F�b�N�Ɋւ��郁���o�֐�
	void checkLaunchEnemy();

	/**
	 *  @desc	 �G�t�F�N�g�̌Ăяo��
	 *  @param	 �G�t�F�N�g�̃^�C�v
	 *  @param	 �o���ʒuX
	 *  @param	 �o���ʒuY
	 */
	void checkLaunchEffect(EFFECT_TYPE type, float posX, float posY);

	/**
	 *  @desc	�A�C�e���̌Ăяo��
	 *  @param	�A�C�e���̃^�C�v
	 *  @param  �o���ʒuX
	 *  @param  �o���ʒuY
	 */
	void checkLaunchItem(ITEM_TYPE type, float posX, float posY);

	// ���͂Ɋւ��郁���o�֐�
	void inputFunc();
		
	// �ړ��Ɋւ��郁���o�֐�
	void moveFunc();
	
	// �v���C���[�ƓG�̒e�̏Փ�
	void collisionPlayerAndEnemyBullets();

	// �v���C���[�ƓG�̏Փ�
	void collisionPlayerAndEnemies();

	// �G�ƃv���C���[�̒e�̏Փ�
	void collisionEnemiesAndPlayerBullets();

	//�v���C���[�ƃA�C�e���̏Փ�
	void collisionPlayerAndItems();
	
	// �A�j���[�V�����Ɋւ��郁���o�֐�
	void animationFunc();
	
	// ��ʒ[����Ɋւ��郁���o�֐�
	void endOfScreen();

	// �l�̔��f�Ɋւ��郁���o�֐�
	void applyFunc();

	// ���O�������Ɋւ��郁���o�֐�
	void checkActiveFlagAndRemove();

	/**
	 *  @desc	�v���C���[�̒e�̔���
	 *  @param	�e�^�C�v
	 */
	void shotPlayerBullet(BULLET_TYPE type);

	// �G�̒e�̔��˂Ɋւ���錾
	void shotEnemyBullet();

	// �s���Ǘ��Ɋւ���錾
	void actionFunc();

	// �X�e�[�W�J�n����
	void stageStartFunc();

	/**
	 *  @desc	�X�e�[�W�X�V����
	 *  @param	�o�ߎ���
	 */
	void stageUpdateFunc(float deltaTime);

	// �X�e�[�W�I������
	void stageEndFunc();

	// �X�e�[�W�ꎞ��~����
	void stagePauseFunc();
} ;

#endif /* defined(__ShootingGame__Main__) */