/*
 * @file		CameraManager.h
 * @brief		カメラ管理クラス
 * @author		戸澤翔太
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
	/* @brief	コンストラクタ		*/
	CameraManager(BaseScene* scene);
	/* @brief	デストラクタ		*/
	~CameraManager(void);

	/* @brief	初期化処理		*/
	void Init(void);
	/* @breif	後処理			*/
	void Uninit(void);
	/* @brief	更新処理		*/
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
	 * @brief	メインカメラの変更
	 * @param	変更したいカメラの配列番号			*/
	inline void ChangeCamera(int cameraNum) { (cameraNum < static_cast<int>(camera_.size())) ? mainCamera_ = cameraNum : cameraNum; }

	/* @breif	メインカメラの取得		*/
	inline Camera*       GetMainCamera(void)	{ return (mainCamera_ >= 0) ? camera_[mainCamera_] : debugCamera_; }
	/* @brief	view行列の取得			*/
	inline const MATRIX& GetView(void)			{ return mtxView_;    }
	/* @brief	proj行列の取得			*/
	inline const MATRIX& GetProj(void)			{ return mtxProj_;    }
	/* @brief	注目状態の取得			*/
	inline bool          GetLook(void)			{ return look_;       }

private:
	/* @brief	デバッグカメラへの移行
	 * @sa		Update()		*/
	void DebugMove(void);
	/* @brief	行列変換
	 * @sa		Update()		*/
	void CreateMatrix(void);

	//! カメラ
	std::vector<Camera*> camera_;
	Camera* debugCamera_;

	// 行列
	MATRIX  mtxView_;
	MATRIX  mtxProj_;

	// メインカメラ
	int  mainCamera_;

	// デバッグ時に使用
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