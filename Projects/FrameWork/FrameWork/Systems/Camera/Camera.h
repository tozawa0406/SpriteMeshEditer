/*
 * @file		Camera.h
 * @brief		�J�������N���X
 * @author		���V�đ�
 * @data		2019/03/06
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../Define/Define.h"
#include "../DebugSetting/GUI.h"

class CameraManager;
class Camera : public GUI
{
public:
	//! ��p
	static constexpr int	FOV = 60;
	//! �j�A�N���b�v
	static constexpr float	C_NEAR = 0.1f;
	//! �t�@�[�N���b�v
	static constexpr float	C_FAR = 1000.0f;

	//! �J�����̏������WX
	static constexpr float	POSITION_X = 0.0f;
	//! �J�����̏������WY
	static constexpr float	POSITION_Y = 0.0f;
	//! �J�����̏������WZ
	static constexpr float	POSITION_Z = -50.0f;

	/* @brief	�R���X�g���N�^		*/
	Camera(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Camera(void)		{}

	/* @brief	����������			*/
	virtual void Init(void)		{}
	/* @brief	�X�V����			*/
	virtual void Update(void)	{}

	/* @brief	�ʒu�擾			*/
	inline const VECTOR3& GetPosition(void) { return position_; }
	/* @brief	�����_�擾			*/
	inline const VECTOR3& GetAt(void) { return at_; }
	/* @brief	��x�N�g���擾		*/
	inline const VECTOR3& GetUp(void) { return up_; }
	/* @brief	�O�x�N�g���擾		*/
	inline const VECTOR3& GetFront(void) { return front_; }
	/* @brief	�E�x�N�g���擾		*/
	inline const VECTOR3& GetRight(void) { return right_; }

	/* @brief	x-z���ʂ̑O�x�N�g���̎擾		*/
	inline const VECTOR3& GetFrontXPlane(void) { return frontXPlane_; }
	/* @brief	x-z���ʂ̉E�x�N�g���̎擾		*/
	inline const VECTOR3& GetRightXPlane(void) { return rightXPlane_; }

	/* @brief	�}�l�[�W���[�̐ݒ�	*/
	inline void SetManager(CameraManager* manager) { manager_ = manager; }
	/* @brief	�ʒu�ݒ�			*/
	inline void SetPosition(VECTOR3 position) { position_ = position; }
	/* @brief	�����_�ݒ�			*/
	inline void SetAt(VECTOR3 at) { at_ = at; }

	/* @brief	�J�����̏���ݒ�	*/
	void SetCamera(Camera* camera)
	{
		position_ = camera->GetPosition();
		at_ = camera->GetAt();
	}

protected:
	//! �}�l�[�W���[
	CameraManager* manager_;

	//! �ʒu���W
	VECTOR3 position_;
	//! �����_
	VECTOR3 at_;
	//! ��x�N�g��
	VECTOR3 up_;
	//! �O�x�N�g��
	VECTOR3 front_;
	//! �E�x�N�g��
	VECTOR3 right_;

	//! x-z���ʂł̑O�x�N�g��
	VECTOR3 frontXPlane_;
	//! x-z���ʂł̉E�x�N�g��
	VECTOR3 rightXPlane_;
};

#endif // _CAMERA_H_