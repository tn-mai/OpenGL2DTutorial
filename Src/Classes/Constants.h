//
//  Constants_h
//  ShootingGame
//
//  Created by vantan-303-t on 2014/11/20.
//
//	汎用・定数定義ファイル
//

#ifndef ShootingGame_Constants_h
#define ShootingGame_Constants_h


//=========================================================================
//	汎用的に使われるインクルードはここから書く
//=========================================================================

#include "cocos2d.h"


//=========================================================================
//	定数やマクロはここから書く
//=========================================================================

// メモリ解放
#define SAFE_DELETE( p_ ) if ( p_ ) { delete p_ ; p_ = NULL ; }
// 配列メモリ解放
#define SAFE_DELETE_ARRAY( p_ ) if ( p_ ) { delete [] p_ ; p_ = NULL ; }





// CGameMain のタグ
#define TAG_GAME_MAIN (1000)


// 現在取り付けらているレイヤーの取得
#define getCurrentLayer( tag_ ) cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( tag_ )




// ウィンドウサイズ
#define WINDOW_LEFT			(0)
#define WINDOW_TOP			(480)
#define WINDOW_RIGHT		(640)
#define WINDOW_BOTTOM		(0)



//=========================================================================
//	画像ファイルのマクロはここから書く
//=========================================================================

// 背景
#define IMAGE_FILE_NAME_BG					"Images/bg.png"

// キャラクター
#define IMAGE_FILE_CHARACTER				"Images/hStgChrTest.png"

// 弾
#define IMAGE_FILE_BULLET					"Images/bullet.png"

// 爆発エフェクト
#define	IMAGE_FILE_EXPLOSION				"Images/explosion.png"

// HPゲージ
#define IMAGE_FILE_HP_GUAGE					"Images/hpGuage.png"

// アイテム
#define IMAGE_FILE_ITEM						"Images/item.png"

// タイトル背景
#define IMAGE_FILE_TITLE_BG					"title.png"

// スタートボタン
#define IMAGE_FILE_START_BUTTON				"startButton.png"
// スタートボタン選択後
#define IMAGE_FILE_START_BUTTON_SELECTED	"startButtonSelected.png"

// 終了ボタン
#define IMAGE_FILE_END_BUTTON				"endButton.png"
// 終了ボタン選択後
#define IMAGE_FILE_END_BUTTON_SLECTED		"endButtonSelected.png"

//=========================================================================
//	サウンドファイルのマクロはここから書く
//=========================================================================

// BGM
#define SOUND_FILE_BGM				"Sounds/lucky_star.mp3"

// 弾発射効果音
#define SOUND_FILE_LAUNCH_BULLET	"Sounds/Se01.mp3"

// プレイヤー爆発音
#define SOUND_FILE_PLAYER_EXPLOSION	"Sounds/player_Se02.mp3"

// 敵爆発音
#define SOUND_FILE_ENEMY_EXPLOSION	"Sounds/enemy_Se01.mp3"

//=========================================================================
//	マップファイルのマクロはここから書く
//=========================================================================

// マップ1
#define MAP_FILE_STAGE_1			"Stage1.tmx"

// マップ2
#define MAP_FILE_STAGE_2			"Stage2.tmx"

// マップ2
#define MAP_FILE_STAGE_3			"Stage3.tmx"

//=========================================================================
//	敵出撃ファイルのマクロはここから書く
//=========================================================================

// 敵出撃データ1
#define ENEMY_LAUNCH_DATA_FILE_1	"EnemyLaunchData1.txt"

// 敵出撃データ2
#define ENEMY_LAUNCH_DATA_FILE_2	"EnemyLaunchData2.txt"

// 敵出撃データ3
#define ENEMY_LAUNCH_DATA_FILE_3	"EnemyLaunchData3.txt"

//=========================================================================
//	フォントのマクロはここから書く
//=========================================================================

// フォント
#define FONT_FILE_STARWARS			"STARWARS.ttf"

//=========================================================================
//	汎用的に使用されるテンプレートはここから書く
//=========================================================================
/**
 *	@desc	配列の NULL が入っている添字番号を取得
 *	@param	検索する配列
 *	@return	NULL が入っている添字番号
 */
template<typename Ty>
int getFreeIndex( Ty** array, int size ) {
	
	for ( int i = 0 ; i < size ; i++ ) {
		if ( array[ i ] == NULL ) {
			return i ;
		}
	}
	return -1 ;
}


#endif // ShootingGame_Constants_h
