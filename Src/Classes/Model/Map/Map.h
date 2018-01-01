#pragma once

#include "Model/Character/Character.h"

//=========================================================================
//	�}�b�v�\���p�N���X�̃R�[�h�͂������珑��
//=========================================================================

class CMap : public cocos2d::TMXTiledMap {
public:
	// �R���X�g���N�^
	CMap();
	// �f�X�g���N�^
	~CMap();

	/**
	 *  @desc		����
	 *  @param		tmx�t�@�C����
	 *  @return		CMap�N���X�̃C���X�^���X
	 *  @tips		CREATE_FUNC�}�N���̒�`
	 *				�ÓI�����o�֐�
	 */
	static CMap* create(std::string tmxFile);

	/**
	 *  @desc		�}�b�v�N���X�̏�����
	 */
	virtual bool init();

public:

	//=========================================================================
	//	�����o�錾
	//		���C�����Ŏg�p���郁���o�͂����ȍ~�ɋL�q���Ă���
	//=========================================================================

	// �X�N���[���i�ړ��j�p�f�[�^
	CMove m_scrollMove;

	// �X�N���[���I���ʒu
	cocos2d::Point m_scrollEndPos;

	// ���[�v���邩�ǂ����̃t���O true...���[�v����
	bool m_isLoop = false;

	// �X�N���[�����I���������ǂ��� true...�X�N���[���I���i��ʒ[�ɂ����j
	bool m_isEnd = false;

	// �X�N���[���i�ړ��j����
	void moveFunc();

	// ���f����
	void applyFunc();
};