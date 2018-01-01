//
//  InputManager.cpp
//  ShootingGame
//
//  Created by �i����v on 2014/11/20.
//
//

#include "InputManager.h"

/*
 *
 *	�L�[�{�[�h���̓t���O
 *
 */
/**
 *	@desc	�l�̃N���A
 */
void CInputFlag::clear() {
	this->m_up = false ;
	this->m_down = false ;
	this->m_left = false ;
	this->m_right = false ;
	this->m_esc = false ;
	this->m_space = false ;
	
	this->m_z = false ;
	this->m_x = false ;
	this->m_c = false ;
	
	this->m_a = false ;
	this->m_s = false ;
	this->m_d = false ;
}

/**
 *	@desc	�L�[��������Ă��鎞�̔��f
 *	@param	�L�[�R�[�h
 */
void CInputFlag::up( kInputType keyType_ ) {

	switch( keyType_ ) {
		
		case kInputType::UP : this->m_up = true ; break ;
		case kInputType::DOWN : this->m_down = true ; break ;
		case kInputType::LEFT : this->m_left = true ; break ;
		case kInputType::RIGHT : this->m_right = true ; break ;
		case kInputType::ESC : this->m_esc = true ; break ;
		case kInputType::SPACE : this->m_space = true ; break ;
		
		case kInputType::Z : this->m_z = true ; break ;
		case kInputType::X : this->m_x = true ; break ;
		case kInputType::C : this->m_c = true ; break ;
		
		case kInputType::A : this->m_a = true ; break ;
		case kInputType::S : this->m_s = true ; break ;
		case kInputType::D : this->m_d = true ; break ;
		
		default: break ;
	}
}
/**
 *	@desc	�L�[��������Ă��鎞�̔��f
 *	@param	�L�[�R�[�h
 */
void CInputFlag::down( kInputType keyType_ ) {
	switch( keyType_ ) {
		case kInputType::UP : this->m_up = false ; break ;
		case kInputType::DOWN : this->m_down = false ; break ;
		case kInputType::LEFT : this->m_left = false ; break ;
		case kInputType::RIGHT : this->m_right = false ; break ;
		case kInputType::ESC : this->m_esc = false ; break ;
		case kInputType::SPACE : this->m_space = false ; break ;
		
		case kInputType::Z : this->m_z = false ; break ;
		case kInputType::X : this->m_x = false ; break ;
		case kInputType::C : this->m_c = false ; break ;
		
		case kInputType::A : this->m_a = false ; break ;
		case kInputType::S : this->m_s = false ; break ;
		case kInputType::D : this->m_d = false ; break ;
		
		default: break ;
	}
}

/**
 *	@desc	�w�肵���L�[��������Ă��邩�ǂ������擾
 *	@param	�L�[�^�C�v
 *	@return	true...������Ă���
 *	@tips	���͂̃^�C�v�ɂ���ĕω�
 *			�L�[�{�[�h�^�C�v�ł͎g�p����K�v�͂Ȃ�
 */
bool CInputFlag::isKeyPressed( kInputType keyType_ ) {

	switch( keyType_ ) {
		case kInputType::UP : return this->m_up ; break ;
		case kInputType::DOWN : return this->m_down ; break ;
		case kInputType::LEFT : return this->m_left ; break ;
		case kInputType::RIGHT : return this->m_right ; break ;
		case kInputType::ESC : return this->m_esc ; break ;
		case kInputType::SPACE : return this->m_space ; break ;
		
		case kInputType::Z : return this->m_z ; break ;
		case kInputType::X : return this->m_x ; break ;
		case kInputType::C : return this->m_c ; break ;
		
		case kInputType::A : return this->m_a ; break ;
		case kInputType::S : return this->m_s ; break ;
		case kInputType::D : return this->m_d ; break ;

		default: break ;
	}

	return false ;
}




/*
 *
 *	���͊Ǘ�
 *
 */
// ���L�C���X�^���X�{��
CInputManager* CInputManager::m_sharedInputManager = NULL ;

CInputManager::CInputManager() {

	// ���̓t���O�̐����Ɛݒ�
	this->setInputFlag( new CInputFlag() ) ;
}

CInputManager::CInputManager( const CInputManager& inputMgr_ ) {}

// �A�N�Z�X�|�C���g
std::mutex inputmanager_singleton_mtx_;
CInputManager* CInputManager::getInstance() {
	std::lock_guard<std::mutex> lock( inputmanager_singleton_mtx_ ) ;
	if ( CInputManager::m_sharedInputManager == NULL ) {
		CInputManager::m_sharedInputManager = new CInputManager() ;
	}
	return CInputManager::m_sharedInputManager ;
}


// �f�X�g���N�^
CInputManager::~CInputManager() {

	// ���̓t���O�̔j��
	SAFE_DELETE( this->m_pInputFlag ) ;
}

void CInputManager::removeInstance() {

	SAFE_DELETE( CInputManager::m_sharedInputManager ) ;
}


/**
 *	@desc	�L�[�R�[�h����L�[�^�C�v�ɕϊ�
 *	@param	�L�[�R�[�h
 *	@return	�L�[�^�C�v
 */
kInputType CInputManager::changeToInputTypeFromKeyCode( cocos2d::EventKeyboard::KeyCode keyCode_ ) {

	// �G�X�P�[�v�L�[���ǂ�������
	if ( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE == keyCode_ )
	{
		// �G�X�P�[�v�L�[��Ԃ�
		return kInputType::ESC ;
	}
	// �X�y�[�X�L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_SPACE == keyCode_ )
	{
		// �X�y�[�X�L�[��Ԃ�
		return kInputType::SPACE ;
	}
	// ����L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode_ )
	{
		// ����L�[��Ԃ�
		return kInputType::UP ;
	}
	// �����L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode_ )
	{
		// �����L�[��Ԃ�
		return kInputType::DOWN ;
	}
	// �����L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode_ )
	{
		// �����L�[��Ԃ�
		return kInputType::LEFT ;
	}
	// �E���L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode_ )
	{
		// �E���L�[��Ԃ�
		return kInputType::RIGHT ;
	}
	// Z �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_Z == keyCode_ )
	{
		// Z �L�[��Ԃ�
		return kInputType::Z ;
	}
	// X �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_X == keyCode_ )
	{
		// X �L�[��Ԃ�
		return kInputType::X ;
	}
	// C �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_C == keyCode_ )
	{
		// C �L�[��Ԃ�
		return kInputType::C ;
	}
	// A �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_A == keyCode_ )
	{
		// A �L�[��Ԃ�
		return kInputType::A ;
	}
	// S �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_S == keyCode_ )
	{
		// S �L�[��Ԃ�
		return kInputType::S ;
	}
	// D �L�[���ǂ�������
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_D == keyCode_ )
	{
		// D �L�[��Ԃ�
		return kInputType::D ;
	}
	
	// �G���[
	return kInputType::NONE ;
}

/**
 *	@desc	�L�[�{�[�h���������ۂ̃C�x���g
 *	@param	�L�[�R�[�h
 */
void CInputManager::onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode_ ) {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->up( this->changeToInputTypeFromKeyCode( keyCode_ ) ) ;
	}
}

/**
 *	@desc	�L�[�{�[�h�𗣂����ۂ̃C�x���g
 *	@param	�L�[�R�[�h
 */
void CInputManager::onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode_ ) {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->down( this->changeToInputTypeFromKeyCode( keyCode_ ) ) ;
	}
}

/**
 *	@desc	���̓t���O�̐ݒ�
 *	@param	���̓t���O
 */
void CInputManager::setInputFlag( CInputFlag* pInputFlag_ ) {
	if ( this->m_pInputFlag ) {
		printf( "���Ɏ��t���ς�\n" ) ;
		return ;
	}
	this->m_pInputFlag = pInputFlag_ ;
}

/**
 *	@desc	���̓t���O�̍폜
 */
void CInputManager::removeInputFlag() {
	if ( this->m_pInputFlag ) {
		delete this->m_pInputFlag ;
		this->m_pInputFlag = NULL ;
	}
}

/**
 *	@desc	���̓t���O�̃N���A
 */
void CInputManager::clearInputFlag() {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->clear() ;
	}
}

/**
 *	@desc	���̓t���O�̎擾
 *	@return	���̓t���O
 */
CInputFlag* CInputManager::getInputFlag() {
	return this->m_pInputFlag ;
}