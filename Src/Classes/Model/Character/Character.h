//
//  Character.h
//  ShootingGame
//
//  Created by �i����v on 2015/05/18.
//
//

#ifndef __ShootingGame__Character__
#define __ShootingGame__Character__

#include "cocos2d.h"
#include "Constants.h"
#include "Data/Status/Status.h"
#include "Data/ActionController/ActionController.h"

//=========================================================================
//	�L�����N�^�[�Ɋւ���^�C�v�Ȃǂ̗񋓂͂������珑���Ă���
//=========================================================================

// �G�̃^�C�v
enum class ENEMY_TYPE : int {
	TYPE_A,
	TYPE_B,
	TYPE_C,
};

// �e�̃^�C�v
enum class BULLET_TYPE {

	/*
	 *  �v���C���[�̒e�̃^�C�v
	 */
	// �ʏ�e
	PLAYER_NORMAL = 1000,

	// 2Way�e
	PLAYER_2WAY,

	// 3Way�e
	PLAYER_3WAY,

	/*
	 *  �G�̒e�̃^�C�v
	 */
	// �ʏ�e
	ENEMY_NORMAL = 2000,

	// �z�[�~���O�e
	ENEMY_AIMING,

	// 7Way�e
	ENEMY_FAN,
};

// �����̃^�C�v
enum class EFFECT_TYPE {

	// �v���C���[�̔���
	PLAYER_EXPLOSION,

	// �G�̔���
	ENEMY_EXPLOSION,
};

/*
 *	�`�b�v�N���X
 *	cocos2d::Rect �� typedef ���Ďg�p����
 *	typedef �Ƃ́u�^�v���u�ʖ��̌^�v�ł��g�p�o����悤�ɂ���w��q
 *
 *	�\��
 *		typedef �^ �ʖ��̌^ ;
 *
 *	���܂Œʂ�̌^�ł��錾�ł��邵�A�ʖ��̌^�ł��錾���鎖���ł���B
 *
 *		typedef cocos2d::Rect CRect ;
 *
 *	�Ƃ���΁A
 *
 *		cocos2d::Rect body ;
 *		CRect body ;
 *
 *	�̂Q�ʂ�Ő錾���鎖���ł���B
 */
typedef cocos2d::Rect CChip ;

//=========================================================================
//	�ړ��p�N���X�̃R�[�h�͂������珑��
//=========================================================================

/*
 *�@�ړ��p�f�[�^�N���X
 */
class CMove {
public:
	float m_posX;	// �ʒuX���W
	float m_posY;	// �ʒuY���W
	float m_velX;	// ���xX
	float m_velY;	// ���xY

	/**
	 *  @desc	�ʒu�̐ݒ�
	 *  @param	X���W
	 *  @param	Y���W
	 */
	void setPos(float posX, float posY) {
		m_posX = posX;
		m_posY = posY;
	}

	/**
	 *  @desc	���x�̐ݒ�
	 *  @param	���xX
	 *  @param	���xY
	 */
	void setVel(float velX, float velY) {
		m_velX = velX;
		m_velY = velY;
	}

	/**
	 *  @desc	���x�ɂ��ړ�
	 */
	void moveByVel() {
		// ���̈ʒu = ���݂̈ʒu + �ړ������
		m_posX = m_posX + m_velX;
		m_posY = m_posY + m_velY;
	}
};

//=========================================================================
//	�A�j���[�V�����p�N���X�̃R�[�h�͂������珑��
//=========================================================================
/*
 *  �A�j���[�V�����p�N���X
 *�@  �A�j���[�V��������摜�����ɕ���ł���ꍇ�݂̂Ɍ���
 */
class CAnimation {
public:
	int m_counter;	// �J�E���^�[
	int m_interval; // �؂�ւ��Ԋu�i�p���p������̑��x�ɂȂ�j
	int m_number;	// �A�j���[�V��������
	int m_frame;	// ���݂̃t���[���m�F�p

	// ���[�v���邩�ǂ����̃t���O
	// true...���[�v����
	bool m_isLoop;

	// �A�j���[�V�������I���������ǂ����̃t���O
	// true...�I������
	bool m_isEnd;

	// �R���X�g���N�^
	CAnimation() {
		// �A�j���[�V�������I���������ǂ����̃t���O�̏�����
		this->m_isEnd = false;
	}

	/**
	 *  @desc	�ݒ�p
	 *  @param	�؂�ւ��Ԋu
	 *  @param	�A�j���[�V��������
	 *  @param	true...���[�v����	false...���[�v���Ȃ�
	 */
	void set(int interval, int number, bool isLoop = true) {
		this->m_counter = 0;
		this->m_interval = interval;
		this->m_number = number;
		this->m_isLoop = isLoop;
	}

	/**
	 *  @desc	�A�j���[�V�����̍X�V����
	 *  @return	�\������t���[����
	 */
	int update() {

		// �A�j���[�V�������I�����Ă��Ȃ���΃J�E���^�[�̍X�V���s��
		if (this->m_isEnd == false) {

			this->m_counter++;

			// �J�E���^�[�̐����A�j���[�V���������閇���ȏ�ɂȂ�����J�E���^�[�����Z�b�g����
			if (this->m_counter >= this->m_interval * this->m_number) {

				if (this->m_isLoop == true) {
					// ���[�v����A�j���[�V�����̓J�E���^�[�����Z�b�g����
					this->m_counter = 0;
				}
				else {
					// �A�j���[�V�������I���������ǂ����̃t���O�ɑ΂���true������
					this->m_isEnd = true;

					// ���[�v���Ȃ��A�j���[�V�����ɑ΂��Ă͔�яo���J�E���^�[���f�N�������g����
					this->m_counter--;
				}
			}
		}

		// �\������t���[����
		m_frame = this->m_counter / this->m_interval;

		return m_frame;
	}
};

// �O���錾
class CBullet;

//=========================================================================
//	�Փ˔���p�N���X�̃R�[�h�͂������珑��
//=========================================================================

class CBody {
public:
	float m_left;		// ���̈ʒu
	float m_top;		// ��̈ʒu
	float m_right;		// �E�̈ʒu
	float m_bottom;		// ���̈ʒu

	// �R���X�g���N�^
	CBody() {

	}

	/**
	 *  @desc	�ݒ�p
	 *  @param	���̐ݒ�
	 *  @param	��̐ݒ�
	 *  @param	�E�̐ݒ�
	 *  @param	���̐ݒ�
	 */
	void set(float left, float top, float right, float bottom) {
		this->m_left = left;
		this->m_top = top;
		this->m_right = right;
		this->m_bottom = bottom;
	}

	/**
	 *  @desc	 �Փ˔���
	 *  @param	 �Փ˂����Ă���Ώۂ̃f�[�^
	 *  @return	 true...�Փ˂��Ă���	false...�Փ˂��Ă��Ȃ�
	 *  @tips	 �u����(this)�v�Ƃ����̂��u�Փ˂����Ώہv
	 *			 �����́u�Ώ�(pBody)�v�Ƃ����̂��u�Փ˂��Ă���Ώہv
	 */
	bool collision(CBody* pBody) {

		// �����̉E���ƑΏۂ̍����Ǝ����̍����ƑΏۂ̉E��
		if (this->m_right > pBody->m_left && this->m_left < pBody->m_right) {

			// �����̏㑤�ƑΏۂ̉����Ǝ����̉����ƑΏۂ̏㑤
			if (this->m_top > pBody->m_bottom && this->m_bottom < pBody->m_top) {
				// �d�Ȃ����i�Փ˂��Ă���j
				return true;
			}
		}

		// �Փ˂��Ă��Ȃ�
		return false;
	}

	/**
	 *  @desc	 �ʒu�𔽉f�����Փ˔���̃f�[�^���擾
	 *  @param	 �ʒuX
	 *  @param	 �ʒuY
	 *  @return  �ʒu�𔽉f�����Փ˔���p�f�[�^
	 */
	CBody getApplyPositionBody(float posX, float posY) {

		// �V�����Փ˔���p�f�[�^�𐶐�
		CBody body;

		// �u�Փ˔�����s���傫���v�Ɉʒu�𔽉f
		body.set(
			posX + this->m_left,
			posY + this->m_top,
			posX + this->m_right,
			posY + this->m_bottom
		);

		// �u�Փ˔�����s���ۂ̈ʒu�v��Ԃ�
		return body;
	}
};

/*
 *
 *	�v���C���[�L�����N�^�[�N���X
 *
 *		cocos2d::Sprite �N���X���p�����Ďg�p���Ă���
 *		�C���X�^���X�𐶐�����ۂ� �ÓI�����o�֐� create ���g�p�ł��邪
 *		���������� create �����p�ӂ���Ă��Ȃ��̂Œ���
 *
 */
class CPlayerCharacter : public cocos2d::Sprite {
public:

	//=========================================================================
	//	��������͗����o����܂ł͕ύX�֎~
	//=========================================================================
	
	// �R���X�g���N�^�̐錾
	CPlayerCharacter() ;
	// �f�X�g���N�^�̐錾
	~CPlayerCharacter() ;
	
	CREATE_FUNC( CPlayerCharacter ) ;
	
	/**
	 *	@desc	����������
	 */
	virtual bool init() override ;

	//=========================================================================
	//	�����܂ł͗����o����܂ł͕ύX�֎~
	//=========================================================================
	
public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================
	
	// �ړ��p�f�[�^
	CMove m_move;

	// �`�b�v�f�[�^
	CChip m_chip;
	
	// �A�j���[�V�����f�[�^
	CAnimation m_anime;

	// �Փ˔���p�f�[�^
	CBody m_body;

	// �X�e�[�^�X
	CStatus m_status;
	
	// �L���t���O
	bool m_activeFlag;

	// �e���˗p�J�E���^�[
	int m_shotCounter;

	// �e���ˊԊu
	int m_shotInterval;

	/**
	 *  @desc	�v���C���[�̒e�̔���
	 *  @param	�e�^�C�v
	 *  @param	�C���ʒuX
	 *  @param	�C���ʒuY
	 *  @param	���xX
	 *  @param	���xY
	 */
	CBullet* shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY);

	// �ړ��Ɋւ��郁���o�֐��錾
	void moveFunc();
	
	// �A�j���[�V�����Ɋւ��郁���o�֐��錾
	void animationFunc();
	
	// ��ʒ[����Ɋւ��郁���o�֐��錾
	void endOfScreen();
	
	// �l�̔��f�Ɋւ��郁���o�֐��錾
	void applyFunc();

	/**
	 *  @desc	����̍U�������g�ɂ�������
	 *  @param	�Փ˂��Ă�������̃X�e�[�^�X�̃A�h���X
	 */
	void hits(CStatus* pStatus);
};

//=========================================================================
//	�o���b�g ( �e ) �̃N���X�͂������珑��
//=========================================================================

class CBullet : public cocos2d::Sprite {
public:

	//=========================================================================
	//	��������͗����o����܂ł͕ύX�֎~
	//=========================================================================

	// �R���X�g���N�^�̐錾
	CBullet();
	// �f�X�g���N�^�̐錾
	~CBullet();

	CREATE_FUNC(CBullet);

	/**
	 *	@desc	����������
	 */
	virtual bool init() override;

	//=========================================================================
	//	�����܂ł͗����o����܂ł͕ύX�֎~
	//=========================================================================

public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================

	// �ړ��p�f�[�^
	CMove m_move;

	// �`�b�v�f�[�^
	CChip m_chip;

	// �A�j���[�V�����f�[�^
	CAnimation m_anime;

	// �Փ˔���p�f�[�^
	CBody m_body;

	// �X�e�[�^�X
	CStatus m_status;

	// �L���t���O�ifalse...���C���[������O�����j
	bool m_activeFlag;

	// �v���C���[�̒e�̔��˂ł���ő吔
	static const int MAX_BULLET = 100;

	// �ړ��Ɋւ��郁���o�֐��錾
	void moveFunc();

	// �A�j���[�V�����Ɋւ��郁���o�֐��錾
	void animationFunc();

	// ��ʒ[����Ɋւ��郁���o�֐��錾
	void endOfScreen();

	// �l�̔��f�Ɋւ��郁���o�֐��錾
	void applyFunc();
};

//=========================================================================
//	�G�̃N���X�͂������珑��
//=========================================================================

class CEnemyCharacter : public cocos2d::Sprite {
public:

	//=========================================================================
	//	��������͗����o����܂ł͕ύX�֎~
	//=========================================================================

	// �R���X�g���N�^�̐錾
	CEnemyCharacter();
	// �f�X�g���N�^�̐錾
	~CEnemyCharacter();

	CREATE_FUNC(CEnemyCharacter);

	/**
	 *	@desc	����������
	 */
	virtual bool init() override;

	//=========================================================================
	//	�����܂ł͗����o����܂ł͕ύX�֎~
	//=========================================================================

public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================

	// �ړ��p�f�[�^
	CMove m_move;

	// �`�b�v�f�[�^
	CChip m_chip;

	// �A�j���[�V�����f�[�^
	CAnimation m_anime;

	// �Փ˔���p�f�[�^
	CBody m_body;

	// �X�e�[�^�X
	CStatus m_status;

	// �G�̃^�C�v
	ENEMY_TYPE m_enemyType;

	// ���˂���e�̃^�C�v
	BULLET_TYPE m_bulletType;

	CAction* m_pAction = NULL;

	// �L���t���O�ifalse...���C���[������O�����j
	bool m_activeFlag;

	// �e���˃t���O
	bool m_shotFlag = false;

	// �G�̍ő吔
	static const int MAX_ENEMY = 100;

	// ���݂̍s���X�e�b�v
	int m_actionStep = 0;

	/**
	 *  @desc	�G�̒e�̔���
	 *  @param	�e�^�C�v
	 *  @param	�C���ʒuX
	 *  @param	�C���ʒuY
	 *  @param	���xX
	 *  @param	���xY
	 */
	CBullet* shotBullet(BULLET_TYPE type, float fixX, float fixY, float velX, float velY);

	// �ړ��Ɋւ��郁���o�֐��錾
	void moveFunc();

	// �A�j���[�V�����Ɋւ��郁���o�֐��錾
	void animationFunc();

	// ��ʒ[����Ɋւ��郁���o�֐��錾
	void endOfScreen();

	// �l�̔��f�Ɋւ��郁���o�֐��錾
	void applyFunc();

	// �s���Ɋւ��郁���o�֐��錾
	void actionFunc();

	/**
	 *  @desc	�U��������ɂ�������
	 *  @param	�Փ˂�������̃X�e�[�^�X�̃A�h���X
	 */
	void hits(CStatus* pStatus);
};

//=========================================================================
//	�G�t�F�N�g�̃N���X�͂������珑��
//=========================================================================

class CEffect : public cocos2d::Sprite {
public:

	//=========================================================================
	//	��������͗����o����܂ł͕ύX�֎~
	//=========================================================================

	// �R���X�g���N�^�̐錾
	CEffect();
	// �f�X�g���N�^�̐錾
	~CEffect();

	CREATE_FUNC(CEffect);

	/**
	 *	@desc	����������
	 */
	virtual bool init() override;

	//=========================================================================
	//	�����܂ł͗����o����܂ł͕ύX�֎~
	//=========================================================================

public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================

	// �ړ��p�f�[�^
	CMove m_move;

	// �`�b�v�f�[�^
	CChip m_chip;

	// �A�j���[�V�����f�[�^
	CAnimation m_anime;

	// �G�t�F�N�g�^�C�v
	EFFECT_TYPE m_effectType;

	// �L���t���O
	bool m_activeFlag;

	// �G�t�F�N�g�̍ő吔
	static const int MAX_EFFECT = 100;

	// �ړ��Ɋւ��郁���o�֐��錾
	void moveFunc();

	// �A�j���[�V�����Ɋւ��郁���o�֐��錾
	void animationFunc();

	// �l�̔��f�Ɋւ��郁���o�֐��錾
	void applyFunc();
};

#endif /* defined(__ShootingGame__Character__) */