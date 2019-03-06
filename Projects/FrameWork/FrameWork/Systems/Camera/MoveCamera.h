//-----------------------------------------------------------------------------
//
//	�J����[Camera.h]
//	Auther : ���V�đ�
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _MOVE_CAMERA_H_
#define _MOVE_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

class MoveCamera : public Camera
{
public:
	/* @brief	�R���X�g���N�^		*/
	MoveCamera(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~MoveCamera(void);

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
	/* @brief	�����𒆐S�ɉ�]
	 * @sa		Update()			*/
	void MoveRotation(void);
	/* @brief	�����_�𒆐S�ɉ�]
	 * @sa		Update()			*/
	void MoveAtRotation(void);

	//! ����
	VECTOR2 dir_;
	//! ���x
	VECTOR2 velocity_;
	//! ��]���x
	VECTOR2 rotVelocity_;
	//! �����_�̉�]���x
	VECTOR2 rotAtVelocity_;
};

#endif // _MOVE_CAMERA_H_