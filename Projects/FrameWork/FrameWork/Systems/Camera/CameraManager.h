/*
 * @file		CameraManager.h
 * @brief		�J�����Ǘ��N���X
 * @author		���V�đ�
 * @data		2019/03/06
 */
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include "../../Define/Define.h"
#include "../../Scene/SceneManager.h"
#include "Camera.h"
#include "../GameSystems.h"

#include "MoveCamera.h"

class CameraManager
{
public:
	/* @brief	�R���X�g���N�^		*/
	CameraManager(BaseScene* scene);
	/* @brief	�f�X�g���N�^		*/
	~CameraManager(void);

	/* @brief	����������		*/
	void Init(void);
	/* @breif	�㏈��			*/
	void Uninit(void);
	/* @brief	�X�V����		*/
	void Update(void);

	template<class T>
	T* Create(void)
	{
		T* camera = new T;
		if (camera)
		{
			camera->SetManager(this);
			camera->Init();
			camera_.emplace_back(camera);

			mainCamera_ = static_cast<int>(camera_.size()) - 1;

			return camera;
		}
		return nullptr;
	}


	/* @fn		ChangeCamera
	 * @brief	���C���J�����̕ύX
	 * @param	�ύX�������J�����̔z��ԍ�			*/
	inline void ChangeCamera(int cameraNum) { (cameraNum < static_cast<int>(camera_.size())) ? mainCamera_ = cameraNum : cameraNum; }

	/* @breif	���C���J�����̎擾		*/
	inline Camera*       GetMainCamera(void)	{ return (mainCamera_ >= 0) ? camera_[mainCamera_] : debugCamera_; }
	/* @brief	view�s��̎擾			*/
	inline const MATRIX& GetView(void)			{ return mtxView_;    }
	/* @brief	proj�s��̎擾			*/
	inline const MATRIX& GetProj(void)			{ return mtxProj_;    }
	/* @brief	���ڏ�Ԃ̎擾			*/
	inline bool          GetLook(void)			{ return look_;       }

private:
	/* @brief	�f�o�b�O�J�����ւ̈ڍs
	 * @sa		Update()		*/
	void DebugMove(void);
	/* @brief	�s��ϊ�
	 * @sa		Update()		*/
	void CreateMatrix(void);

	//! �J����
	std::vector<Camera*> camera_;
	Camera* debugCamera_;

	// �s��
	MATRIX  mtxView_;
	MATRIX  mtxProj_;

	// ���C���J����
	int  mainCamera_;

	// �f�o�b�O���Ɏg�p
	bool look_;
	int  frame_;
	int  mainOld_;
	VECTOR3 atOld_;
	VECTOR3 posOld_;
	VECTOR3 diff_;
	VECTOR3 diffPos_;

	BaseScene* scene_;
};

#endif // _CAMERA_MANAGER_H_