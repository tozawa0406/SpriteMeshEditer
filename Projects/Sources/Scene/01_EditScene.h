/*
 * @file		EditScene.h
 * @brief		�G�f�B�b�g�V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _EDIT_SCENE_H_
#define _EDIT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>
#include "../Object/Client/Receiver.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Systems/Camera/CameraManager.h>

class ModelEditer;
class EditScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	EditScene(void);
	/* @brief	�f�X�g���N�^		*/
	~EditScene(void);


	
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

	virtual void GuiUpdate(void) override;
	
private:
	ModelEditer* client_;
};

#endif // _EDIT_SCENE_H_
