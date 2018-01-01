//
//  Constants_h
//  ShootingGame
//
//  Created by vantan-303-t on 2014/11/20.
//
//	�ėp�E�萔��`�t�@�C��
//

#ifndef ShootingGame_Constants_h
#define ShootingGame_Constants_h


//=========================================================================
//	�ėp�I�Ɏg����C���N���[�h�͂������珑��
//=========================================================================

#include "cocos2d.h"


//=========================================================================
//	�萔��}�N���͂������珑��
//=========================================================================

// ���������
#define SAFE_DELETE( p_ ) if ( p_ ) { delete p_ ; p_ = NULL ; }
// �z�񃁃������
#define SAFE_DELETE_ARRAY( p_ ) if ( p_ ) { delete [] p_ ; p_ = NULL ; }





// CGameMain �̃^�O
#define TAG_GAME_MAIN (1000)


// ���ݎ��t����Ă��郌�C���[�̎擾
#define getCurrentLayer( tag_ ) cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( tag_ )




// �E�B���h�E�T�C�Y
#define WINDOW_LEFT			(0)
#define WINDOW_TOP			(480)
#define WINDOW_RIGHT		(640)
#define WINDOW_BOTTOM		(0)



//=========================================================================
//	�摜�t�@�C���̃}�N���͂������珑��
//=========================================================================

// �w�i
#define IMAGE_FILE_NAME_BG					"Images/bg.png"

// �L�����N�^�[
#define IMAGE_FILE_CHARACTER				"Images/hStgChrTest.png"

// �e
#define IMAGE_FILE_BULLET					"Images/bullet.png"

// �����G�t�F�N�g
#define	IMAGE_FILE_EXPLOSION				"Images/explosion.png"

// HP�Q�[�W
#define IMAGE_FILE_HP_GUAGE					"Images/hpGuage.png"

// �A�C�e��
#define IMAGE_FILE_ITEM						"Images/item.png"

// �^�C�g���w�i
#define IMAGE_FILE_TITLE_BG					"title.png"

// �X�^�[�g�{�^��
#define IMAGE_FILE_START_BUTTON				"startButton.png"
// �X�^�[�g�{�^���I����
#define IMAGE_FILE_START_BUTTON_SELECTED	"startButtonSelected.png"

// �I���{�^��
#define IMAGE_FILE_END_BUTTON				"endButton.png"
// �I���{�^���I����
#define IMAGE_FILE_END_BUTTON_SLECTED		"endButtonSelected.png"

//=========================================================================
//	�T�E���h�t�@�C���̃}�N���͂������珑��
//=========================================================================

// BGM
#define SOUND_FILE_BGM				"Sounds/lucky_star.mp3"

// �e���ˌ��ʉ�
#define SOUND_FILE_LAUNCH_BULLET	"Sounds/Se01.mp3"

// �v���C���[������
#define SOUND_FILE_PLAYER_EXPLOSION	"Sounds/player_Se02.mp3"

// �G������
#define SOUND_FILE_ENEMY_EXPLOSION	"Sounds/enemy_Se01.mp3"

//=========================================================================
//	�}�b�v�t�@�C���̃}�N���͂������珑��
//=========================================================================

// �}�b�v1
#define MAP_FILE_STAGE_1			"Stage1.tmx"

// �}�b�v2
#define MAP_FILE_STAGE_2			"Stage2.tmx"

// �}�b�v2
#define MAP_FILE_STAGE_3			"Stage3.tmx"

//=========================================================================
//	�G�o���t�@�C���̃}�N���͂������珑��
//=========================================================================

// �G�o���f�[�^1
#define ENEMY_LAUNCH_DATA_FILE_1	"EnemyLaunchData1.txt"

// �G�o���f�[�^2
#define ENEMY_LAUNCH_DATA_FILE_2	"EnemyLaunchData2.txt"

// �G�o���f�[�^3
#define ENEMY_LAUNCH_DATA_FILE_3	"EnemyLaunchData3.txt"

//=========================================================================
//	�t�H���g�̃}�N���͂������珑��
//=========================================================================

// �t�H���g
#define FONT_FILE_STARWARS			"STARWARS.ttf"

//=========================================================================
//	�ėp�I�Ɏg�p�����e���v���[�g�͂������珑��
//=========================================================================
/**
 *	@desc	�z��� NULL �������Ă���Y���ԍ����擾
 *	@param	��������z��
 *	@return	NULL �������Ă���Y���ԍ�
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
