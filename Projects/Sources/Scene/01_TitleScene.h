/*
 * @file		TitleScene.h
 * @brief		�^�C�g���V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>
#include "../Object/Command/Receiver.h"
#include <FrameWork/Object/ObjectManager.h>

class Client;
class TitleScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	TitleScene(void);
	/* @brief	�f�X�g���N�^		*/
	~TitleScene(void);


	
	/* @brief	����������
	 * @param	(sceneNum)	�V�[���ԍ�
	 * @return	�Ȃ�				*/
	void Init(SceneList sceneNum) override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�V�[���ԍ�			*/
	SceneList	Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void InspectorView(void) override;

	void HierarchyView(void) override;

	void ConsoleView(void) override;

private:
	ObjectManager*	objectManager_;

	Client* client_;
};

#endif // _TITLE_SCENE_H_
