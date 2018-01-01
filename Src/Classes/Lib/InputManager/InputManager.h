//
//  InputManager.h
//  ShootingGame
//
//  Created by 永瀬鈴久 on 2014/11/20.
//
//

#ifndef __ShootingGame__InputManager__
#define __ShootingGame__InputManager__

#include "cocos2d.h"
#include "Constants.h"

/*
 *	入力タイプ
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
 *	キーボード用入力フラグ
 */
class CInputFlag {
public:
	bool m_up = false ;		// 上キー
	bool m_down = false ;	// 下キー
	bool m_left = false ;	// 左キー
	bool m_right = false ;	// 右キー
	bool m_esc = false ;	// ESC キー
	bool m_space = false ;	// スペースキー
	
	bool m_z = false ;		// z キー
	bool m_x = false ;		// x キー
	bool m_c = false ;		// c キー
	
	bool m_a = false ;		// a キー
	bool m_s = false ;		// s キー
	bool m_d = false ;		// d キー

	/**
	 *	@desc	値のクリア
	 */
	virtual void clear() ;
	
	/**
	 *	@desc	キーのフラグを上げる ( フラグアップ )
	 *	@param	キーコード
	 */
	virtual void up( kInputType keyType_ ) ;
	/**
	 *	@desc	キーのフラグを下げる ( フラグダウン )
	 *	@praram	キーコード
	 */
	virtual void down( kInputType keyType_ ) ;
	
	/**
	 *	@desc	指定したキーが押されているかどうかを取得
	 *	@param	キータイプ
	 *	@return	true...押されている
	 *	@tips	入力のタイプによって変化
	 *			キーボードタイプでは使用する必要はない
	 */
	virtual bool isKeyPressed( kInputType keyType_ ) ;
} ;






/*
 *
 *	入力管理
 *
 *	マルチタッチやコントローラーを２つにする際は CInputFlag を複数管理できるように作り替えなければならない
 *	理解出来るのであればこの部分をカスタマイズしてもかまわない
 */
class CInputManager {
private:
	// コンストラクタ
	CInputManager() ;
	CInputManager( const CInputManager& inputMgr_ ) ;
	
	// 共有インスタンス
	static CInputManager* m_sharedInputManager ;
public:
	// アクセスポイント
	static CInputManager* getInstance() ;
	// 破棄
	static void removeInstance() ;
	
	~CInputManager() ;
	
private:
	// 入力フラグ
	CInputFlag* m_pInputFlag = NULL ;
	
public:
	/**
	 *	@desc	キーコードからキータイプに変換
	 *	@param	キーコード
	 *	@return	キータイプ
	 */
	kInputType changeToInputTypeFromKeyCode( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	キーボードを押した際のイベント
	 *	@param	キーコード
	 *	@tips	入力管理を使用する際は入力受けつけが可能となっているノードの onKeyPressed で呼び出す
	 */
	void onKeyPressed( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	キーボードを離した際のイベント
	 *	@param	キーコード
	 *	@tips	入力管理を使用する際は入力受けつけが可能となっているノードの onKeyReleased で呼び出す
	 */
	void onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode_ ) ;
	
	/**
	 *	@desc	入力フラグの設定
	 *	@param	入力フラグ
	 */
	void setInputFlag( CInputFlag* pInputFlag_ ) ;
	
	/**
	 *	@desc	入力フラグの削除
	 */
	void removeInputFlag() ;
	
	/**
	 *	@desc	入力フラグのクリア
	 */
	void clearInputFlag() ;
	
	/**
	 *	@desc	入力フラグの取得
	 *	@return	入力フラグ
	 */
	CInputFlag* getInputFlag() ;
} ;


// 入力フラグマクロ
#define inputflag (*CInputManager::getInstance()->getInputFlag())



#endif /* defined(__ShootingGame__InputManager__) */
