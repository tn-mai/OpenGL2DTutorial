#pragma once

//=========================================================================
//	�s���Ǘ��Ɋւ���^�C�v�Ȃǂ̗񋓂͂������珑���Ă���
//=========================================================================

// �A�N�V�����^�C�v
enum class ACTION_TYPE {

	// �s������
	NONE,

	// �W�O�U�O
	ZIGZAG,

	// ���i�s��
	STRAIGHT,

	// �ʍs��
	CONVEX,
};

// CEnemyCharacter�N���X��O���錾
class CEnemyCharacter;

//=========================================================================
//	�s���Ǘ��̃N���X�͂������珑��
//=========================================================================

class CAction {
public:
	virtual ~CAction() {
	}

	virtual void actionFunc(CEnemyCharacter* pEne) = 0;
};

/*
 *  �W�O�U�O�s��
 */
class CActionControllerZigZag : public CAction{
public:
	// �A�N�V�����^�C�v
	ACTION_TYPE m_actionType;

	// �R���X�g���N�^
	CActionControllerZigZag();

	/**
	 *  @desc	�s���Ǘ�
	 *  @param	���̍s���������Ȃ��G�L�����N�^�[�̃A�h���X
	 */
	void actionFunc(CEnemyCharacter* pEne);
};

/*
 *  ���i�s��
 *    ���i�s�����ɒe��2������
 */
class CActionControllerStraight : public CAction{
public:
	// �s���^�C�v
	ACTION_TYPE m_actionType;

	// �R���X�g���N�^
	CActionControllerStraight();

	/**
	 *  @desc	�s������
	 *  @param	���̍s���������Ȃ��G�L�����N�^�[�̃A�h���X
	 */
	void actionFunc(CEnemyCharacter* pEne);
};

/*
 *  �ʍs��
 */
class CActionControllerConvex : public CAction{
public:
	// �s���^�C�v
	ACTION_TYPE m_actionType;

	// �R���X�g���N�^
	CActionControllerConvex();

	/**
	 *  @desc	�s������
	 *  @param	���̍s���s���G�L�����N�^�[�̃A�h���X
	 */
	void actionFunc(CEnemyCharacter* pEne);
};