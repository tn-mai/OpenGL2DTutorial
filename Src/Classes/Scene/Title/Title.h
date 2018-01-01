#pragma once

#include "cocos2d.h"

/*
 *  �^�C�g�����C���[
 *
 *		�^�C�g����\������ׂ̃��C���[
 *
 */
class CTitle : public cocos2d::Layer {
public:
	// �f�X�g���N�^
	~CTitle();

	/**
	 *  @desc		�V�[���̐���
	 *  @return		CTitle���C���[�������V�[���N���X
	 *  @tips		�ÓI�����o�֐�
	 */
	static cocos2d::Scene* createScene();

	/*
	 *	@desc	�V�[���������ɏ������֐�������
	 *			�������֐����s���� NULL ��Ԃ�
	 */
	CREATE_FUNC(CTitle);

	/**
	 *  @desc		������
	 *  @return		true...����	false...���s
	 */
	bool init() override;

	/**
	 *  @desc		�X�V����
	 *  @param		�o�ߎ���
	 */
	void update(float deltaTime) override;

public:

	// �L���t���O
	bool m_activeFlag;

	/**
	 *  @desc		�Q�[�����C���ɑJ��
	 *  @param		�^�C�g�����C���[�̃C���X�^���X
	 *  @tips		�X�^�[�g�{�^���������ꂽ���ɌĂяo�����
	 */
	void callbackChangeGameMain(cocos2d::Ref* pSender);

	/**
	 *  @desc		�Q�[���I��
	 *  @param		�^�C�g�����C���[�̃C���X�^���X
	 *  @param		�Q�[���I���{�^���������ꂽ���ɌĂяo�����
	 */
	void callbackEndGame(cocos2d::Ref* pSender);
};