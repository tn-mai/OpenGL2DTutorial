//
//  InputManager.h
//  ShootingGame
//
//  Created by �i����v on 2014/11/20.
//
//

#ifndef __ShootingGame__InputManager__
#define __ShootingGame__InputManager__

#include "cocos2d.h"
#include "Constants.h"

/*
 *	���̓^�C�v
 */
enum class kInputType : int {
	NONE	= -1,
	UP		= 0,
	DOWN	= 1,
	LEFT	= 2,
	RIGHT	= 3,
	ESC		= 4,
	SPACE	= 5,
	Z		= 6,
	X		= 7,
	C		= 8,
	A		= 9,
	S		= 10,
	D		= 11,
} ;

/*
 *	�L�[�{�[�h�p���̓t���O
 */
class CInputFlag {
public:
	bool m_up = false ;		// ��L�[
	bool m_down = false ;	// ���L�[
	bool m_left = false ;	// ���L�[
	bool m_right = false ;	// �E�L�[
	bool m_esc = false ;	// ESC �L�[
	bool m_space = false ;	// �X�y�[�X�L�[
	
	bool m_z = false ;		// z �L�[
	bool m_x = false ;		// x �L�[
	bool m_c = false ;		// c �L�[
	
	bool m_a = false ;		// a �L�[
	bool m_s = false ;		// s �L�[
	bool m_d = false ;		// d �L�[

	/**
	 *	@desc	�l�̃N���A
	 */
	virtual void clear() ;
	
	/**
	 *	@desc	�L�[�̃t���O���グ�� ( �t���O�A�b�v )
	 *	@param	�L�[�R�[�h
	 */
	virtual void up( kInputType keyType_ ) ;
	/**
	 *	@desc	�L�[�̃t���O�������� ( �t���O�_�E�� )
	 *	@praram	�L�[�R�[�h
	 */
	virtual void down( kInputType keyType_ ) ;
	
	/**
	 *	@desc	�w�肵���L�[��������Ă��邩�ǂ������擾
	 *	@param	�L�[�^�C�v
	 *	@return	true...������Ă���
	 *	@tips	���͂̃^�C�v�ɂ���ĕω�
	 *			�L�[�{�[�h�^�C�v�ł͎g�p����K�v�͂Ȃ�
	 */
	virtual bool isKeyPressed( kInputType keyType_ ) ;
} ;






/*
 *
 *	���͊Ǘ�
 *
 *	�}���`�^�b�`��R���g���[���[���Q�ɂ���ۂ� CInputFlag �𕡐��Ǘ��ł���悤�ɍ��ւ��Ȃ���΂Ȃ�Ȃ�
 *	�����o����̂ł���΂��̕������J�X�^�}�C�Y���Ă����܂�Ȃ�
 */
class CInputManager {
private:
	// �R���X�g���N�^
	CInputManager() ;
	CInputManager( const CInputManager& inputMgr_ ) ;
	
	// ���L�C���X�^���X
	static CInputManager* m_sharedInputManager ;
public:
	// �A�N�Z�X�|�C���g
	static CInputManager* getInstance() ;
	// �j��
	static void removeInstance() ;
	
	~CInputManager() ;
	
private:
	// ���̓t���O
	CInputFlag* m_pInputFlag = NULL ;
	
public:
	/**
	 *	@desc	�L�[�R�[�h����L�[�^�C�v�ɕϊ�
	 *	@param	�L�[�R�[�h
	 *	@return	�L�[�^�C�v
	 */
	kInputType changeToInputTypeFromKeyCode( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	�L�[�{�[�h���������ۂ̃C�x���g
	 *	@param	�L�[�R�[�h
	 *	@tips	���͊Ǘ����g�p����ۂ͓��͎󂯂����\�ƂȂ��Ă���m�[�h�� onKeyPressed �ŌĂяo��
	 */
	void onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	�L�[�{�[�h�𗣂����ۂ̃C�x���g
	 *	@param	�L�[�R�[�h
	 *	@tips	���͊Ǘ����g�p����ۂ͓��͎󂯂����\�ƂȂ��Ă���m�[�h�� onKeyReleased �ŌĂяo��
	 */
	void onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	���̓t���O�̐ݒ�
	 *	@param	���̓t���O
	 */
	void setInputFlag( CInputFlag* pInputFlag_ ) ;
	
	/**
	 *	@desc	���̓t���O�̍폜
	 */
	void removeInputFlag() ;
	
	/**
	 *	@desc	���̓t���O�̃N���A
	 */
	void clearInputFlag() ;
	
	/**
	 *	@desc	���̓t���O�̎擾
	 *	@return	���̓t���O
	 */
	CInputFlag* getInputFlag() ;
} ;


// ���̓t���O�}�N��
#define inputflag (*CInputManager::getInstance()->getInputFlag())



#endif /* defined(__ShootingGame__InputManager__) */
