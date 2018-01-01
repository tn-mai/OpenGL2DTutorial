#pragma once

#include "cocos2d.h"
#include "Data/Status/Status.h"

//=========================================================================
//	HP�\���p�̃��x���N���X�̃R�[�h�͂������珑��
//=========================================================================

class CHpLabel : public cocos2d::Label {
public:
	CHpLabel();
	~CHpLabel();

	CREATE_FUNC(CHpLabel);

	// ����������
	virtual bool init();

private:
	// ���̃N���X�ŊĎ�����X�e�[�^�X�N���X�̃A�h���X��ۊǂ���|�C���^
	CStatus* m_pStatus;

public:
	/**
	 *  @desc	�X�V����
	 *  @param	�o�ߎ���
	 */
	virtual void update(float deltaTime_) override;

	// �X�e�[�^�X�̐ݒ�
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	HP�Q�[�W�N���X�̃R�[�h�͂������珑��
//=========================================================================

class CHpGuage : public cocos2d::CCProgressTimer {
public:
	CHpGuage();
	~CHpGuage();

	CREATE_FUNC(CHpGuage);

	// ����������
	virtual bool init();

	/**
	 *  @desc	�X�V����
	 *  @param	�o�ߎ���
	 */
	virtual void update(float deltaTime_) override;

private:
	// �Q�Ƃ���X�e�[�^�X
	CStatus* m_pStatus;

public:
	// �Q�Ƃ���X�e�[�^�X�̐ݒ�
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	�X�R�A�\���p�̃��x���N���X�̃R�[�h�͂������珑��
//=========================================================================

class CScoreLabel : public cocos2d::Label {
public:
	CScoreLabel();
	~CScoreLabel();

	CREATE_FUNC(CScoreLabel);

	// ����������
	virtual bool init();

private:
	// ���̃N���X�ŊĎ�����X�e�[�^�X�N���X�̃A�h���X��ۊǂ���|�C���^
	CStatus* m_pStatus;

public:
	/**
	 *  @desc	�X�V����
	 *  @param	�o�ߎ���
	 */
	virtual void update(float deltaTime_) override;

	// �X�e�[�^�X�̐ݒ�
	void setStatus(CStatus* pStatus) {
		this->m_pStatus = pStatus;
	}
};

//=========================================================================
//	�X�e�[�W�\���p�̃��x���N���X�̃R�[�h�͂������珑��
//=========================================================================

class CStageNoLabel : public cocos2d::Label {
public:
	CStageNoLabel();
	~CStageNoLabel();

	CREATE_FUNC(CStageNoLabel);

	// ����������
	virtual bool init();

public:
	// ���̃N���X�ŊĎ�����X�e�[�^�X�N���X�̃A�h���X��ۊǂ���|�C���^
	int* m_pStage;

public:
	/**
	*  @desc	�X�V����
	*  @param	�o�ߎ���
	*/
	virtual void update(float deltaTime_) override;

	// �X�e�[�^�X�̐ݒ�
	void setStage(int* pStage) {
		this->m_pStage = pStage;
	}
};