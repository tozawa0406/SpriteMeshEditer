#ifndef _LOOK_CAMERA_H_
#define _LOOK_CAMERA_H_

#include <FrameWork/Systems/Camera/Camera.h>

class LookCamera : public Camera
{
public:
	/* @brief	�R���X�g���N�^		*/
	LookCamera(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~LookCamera(void);

	/* @brief	�X�V����			*/
	virtual void Update(void)		override;
	/* @brief	Gui�̍X�V����		*/
	virtual void GuiUpdate(void)	override;

private:
	/* @brief	�L�[���͏���
	 * @sa		Update()			*/
	void Input(void);
	/* @brief	���s�ړ�
	 * @sa		Update()			*/
	void MoveTrans(void);

	//! ����
	VECTOR2 dir_;
	//! ���x
	VECTOR3 velocity_;
};

#endif // _LOOK_CAMERA_H_