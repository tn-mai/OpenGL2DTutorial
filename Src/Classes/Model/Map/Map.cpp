#include "Map.h"

//=========================================================================
//	�}�b�v�N���X�̃����o�֐��̒�`�͂������珑��
//=========================================================================

// �R���X�g���N�^
CMap::CMap() {
}

// �f�X�g���N�^
CMap::~CMap() {
}

/**
 *  @desc		����
 *  @param		tmx�t�@�C����
 *  @return		Map�N���X�̃C���X�^���X
 *  @tips		CREATE_FUNC�}�N���̒��g
 *				�ÓI�����o�֐�
 */
CMap* CMap::create(std::string tmxFile) {

	// �}�b�v�N���X�𐶐�����
	CMap* pMap = new CMap();

	// TMXTiledMap�̏�����������
	// CMap�̏������������s��
	if (pMap->initWithTMXFile(tmxFile) == true && pMap->init() == true) {
		
		// ���������������玩��delete��ݒ�
		pMap->autorelease();

		// �}�b�v�N���X�̃C���X�^���X��Ԃ�
		return pMap;
	}

	// ���������s���Ă�����}�b�v�N���X�̃C���X�^���X�����
	CC_SAFE_DELETE(pMap);

	// NULL��Ԃ�
	return NULL;
}

/**
*  @desc	����������
*/
bool CMap::init() {

	/*
	 *  �X�N���[���I���ʒu�̌v�Z
	 */
	// ��ʃT�C�Y�̎擾
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	// ���C���[�����擾
	cocos2d::TMXLayer* pLayer = this->getLayer("layer");

	// ���C���[�̃T�C�Y���擾�i��60 * ����15�j
	cocos2d::Size layerSize = pLayer->getLayerSize();

	// �}�b�v�`�b�v�̃T�C�Y���擾�i��32 * ����32�j
	cocos2d::Size mapTileSize = pLayer->getMapTileSize();

	// �}�b�v�̏I���ʒu���擾
	this->m_scrollEndPos.x = (layerSize.width * mapTileSize.width) - winSize.width;

	// �����ʒu�̐ݒ�Ɣ��f
	this->m_scrollMove.setPos(0.0f, 0.0f);
	this->setPosition(0.0f, 0.0f);

	return true;
}

// �X�N���[���i�ړ��j����
void CMap::moveFunc() {

	// �}�b�v�X�N���[����X�ړ�����
	if (this->m_scrollMove.m_posX < -this->m_scrollEndPos.x) {

		if (m_isLoop == true) {
			
			// ���[�v����ꍇ�̓}�b�v�̈ʒu��߂�
			this->m_scrollMove.m_posX = 0;
		}
		else {
			
			// ���[�v���Ȃ��ꍇ�͏I���t���O���グ��
			this->m_isEnd = true;
		}
	}
	else {
		
		// �}�b�v���X�N���[��
		this->m_scrollMove.m_posX -= this->m_scrollMove.m_velX;
	}
}

// ���f����
void CMap::applyFunc() {

	this->setPosition(this->m_scrollMove.m_posX, this->m_scrollMove.m_posY);
}