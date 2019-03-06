/*
 * @file		Camera.h
 * @brief		カメラ基底クラス
 * @author		戸澤翔太
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
	//! 画角
	static constexpr int	FOV = 60;
	//! ニアクリップ
	static constexpr float	C_NEAR = 0.1f;
	//! ファークリップ
	static constexpr float	C_FAR = 1000.0f;

	//! カメラの初期座標X
	static constexpr float	POSITION_X = 0.0f;
	//! カメラの初期座標Y
	static constexpr float	POSITION_Y = 0.0f;
	//! カメラの初期座標Z
	static constexpr float	POSITION_Z = -50.0f;

	/* @brief	コンストラクタ		*/
	Camera(void);
	/* @brief	デストラクタ		*/
	virtual ~Camera(void)		{}

	/* @brief	初期化処理			*/
	virtual void Init(void)		{}
	/* @brief	更新処理			*/
	virtual void Update(void)	{}

	/* @brief	位置取得			*/
	inline const VECTOR3& GetPosition(void) { return position_; }
	/* @brief	注視点取得			*/
	inline const VECTOR3& GetAt(void) { return at_; }
	/* @brief	上ベクトル取得		*/
	inline const VECTOR3& GetUp(void) { return up_; }
	/* @brief	前ベクトル取得		*/
	inline const VECTOR3& GetFront(void) { return front_; }
	/* @brief	右ベクトル取得		*/
	inline const VECTOR3& GetRight(void) { return right_; }

	/* @brief	x-z平面の前ベクトルの取得		*/
	inline const VECTOR3& GetFrontXPlane(void) { return frontXPlane_; }
	/* @brief	x-z平面の右ベクトルの取得		*/
	inline const VECTOR3& GetRightXPlane(void) { return rightXPlane_; }

	/* @brief	マネージャーの設定	*/
	inline void SetManager(CameraManager* manager) { manager_ = manager; }
	/* @brief	位置設定			*/
	inline void SetPosition(VECTOR3 position) { position_ = position; }
	/* @brief	注視点設定			*/
	inline void SetAt(VECTOR3 at) { at_ = at; }

	/* @brief	カメラの情報を設定	*/
	void SetCamera(Camera* camera)
	{
		position_ = camera->GetPosition();
		at_ = camera->GetAt();
	}

protected:
	//! マネージャー
	CameraManager* manager_;

	//! 位置座標
	VECTOR3 position_;
	//! 注視点
	VECTOR3 at_;
	//! 上ベクトル
	VECTOR3 up_;
	//! 前ベクトル
	VECTOR3 front_;
	//! 右ベクトル
	VECTOR3 right_;

	//! x-z平面での前ベクトル
	VECTOR3 frontXPlane_;
	//! x-z平面での右ベクトル
	VECTOR3 rightXPlane_;
};

#endif // _CAMERA_H_