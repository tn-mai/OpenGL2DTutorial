//
//  InputManager.cpp
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2014/11/20.
//
//

#include "InputManager.h"

/*
 *
 *	キーボード入力フラグ
 *
 */
/**
 *	@desc	値のクリア
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
 *	@desc	キーが押されている時の反映
 *	@param	キーコード
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
 *	@desc	キーが離されている時の反映
 *	@param	キーコード
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
 *	@desc	指定したキーが押されているかどうかを取得
 *	@param	キータイプ
 *	@return	true...押されている
 *	@tips	入力のタイプによって変化
 *			キーボードタイプでは使用する必要はない
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
 *	入力管理
 *
 */
// 共有インスタンス本体
CInputManager* CInputManager::m_sharedInputManager = NULL ;

CInputManager::CInputManager() {

	// 入力フラグの生成と設定
	this->setInputFlag( new CInputFlag() ) ;
}

CInputManager::CInputManager( const CInputManager& inputMgr_ ) {}

// アクセスポイント
std::mutex inputmanager_singleton_mtx_;
CInputManager* CInputManager::getInstance() {
	std::lock_guard<std::mutex> lock( inputmanager_singleton_mtx_ ) ;
	if ( CInputManager::m_sharedInputManager == NULL ) {
		CInputManager::m_sharedInputManager = new CInputManager() ;
	}
	return CInputManager::m_sharedInputManager ;
}


// デストラクタ
CInputManager::~CInputManager() {

	// 入力フラグの破棄
	SAFE_DELETE( this->m_pInputFlag ) ;
}

void CInputManager::removeInstance() {

	SAFE_DELETE( CInputManager::m_sharedInputManager ) ;
}


/**
 *	@desc	キーコードからキータイプに変換
 *	@param	キーコード
 *	@return	キータイプ
 */
kInputType CInputManager::changeToInputTypeFromKeyCode( cocos2d::EventKeyboard::KeyCode keyCode_ ) {

	// エスケープキーかどうか判定
	if ( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE == keyCode_ )
	{
		// エスケープキーを返す
		return kInputType::ESC ;
	}
	// スペースキーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_SPACE == keyCode_ )
	{
		// スペースキーを返す
		return kInputType::SPACE ;
	}
	// 上矢印キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode_ )
	{
		// 上矢印キーを返す
		return kInputType::UP ;
	}
	// 下矢印キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode_ )
	{
		// 下矢印キーを返す
		return kInputType::DOWN ;
	}
	// 左矢印キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode_ )
	{
		// 左矢印キーを返す
		return kInputType::LEFT ;
	}
	// 右矢印キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode_ )
	{
		// 右矢印キーを返す
		return kInputType::RIGHT ;
	}
	// Z キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_Z == keyCode_ )
	{
		// Z キーを返す
		return kInputType::Z ;
	}
	// X キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_X == keyCode_ )
	{
		// X キーを返す
		return kInputType::X ;
	}
	// C キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_C == keyCode_ )
	{
		// C キーを返す
		return kInputType::C ;
	}
	// A キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_A == keyCode_ )
	{
		// A キーを返す
		return kInputType::A ;
	}
	// S キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_S == keyCode_ )
	{
		// S キーを返す
		return kInputType::S ;
	}
	// D キーかどうか判定
	else if ( cocos2d::EventKeyboard::KeyCode::KEY_D == keyCode_ )
	{
		// D キーを返す
		return kInputType::D ;
	}
	
	// エラー
	return kInputType::NONE ;
}

/**
 *	@desc	キーボードを押した際のイベント
 *	@param	キーコード
 */
void CInputManager::onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode_ ) {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->up( this->changeToInputTypeFromKeyCode( keyCode_ ) ) ;
	}
}

/**
 *	@desc	キーボードを離した際のイベント
 *	@param	キーコード
 */
void CInputManager::onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode_ ) {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->down( this->changeToInputTypeFromKeyCode( keyCode_ ) ) ;
	}
}

/**
 *	@desc	入力フラグの設定
 *	@param	入力フラグ
 */
void CInputManager::setInputFlag( CInputFlag* pInputFlag_ ) {
	if ( this->m_pInputFlag ) {
		printf( "既に取り付け済み\n" ) ;
		return ;
	}
	this->m_pInputFlag = pInputFlag_ ;
}

/**
 *	@desc	入力フラグの削除
 */
void CInputManager::removeInputFlag() {
	if ( this->m_pInputFlag ) {
		delete this->m_pInputFlag ;
		this->m_pInputFlag = NULL ;
	}
}

/**
 *	@desc	入力フラグのクリア
 */
void CInputManager::clearInputFlag() {
	if ( this->m_pInputFlag ) {
		this->m_pInputFlag->clear() ;
	}
}

/**
 *	@desc	入力フラグの取得
 *	@return	入力フラグ
 */
CInputFlag* CInputManager::getInputFlag() {
	return this->m_pInputFlag ;
}