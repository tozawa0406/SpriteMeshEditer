//-----------------------------------------------------------------------------
//
//	カメラ[Camera.h]
//	Auther : 戸澤翔太
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _MOVE_CAMERA_H_
#define _MOVE_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

class MoveCamera : public Camera
{
public:
	/* @brief	コンストラクタ		*/
	MoveCamera(void);
	/* @brief	デストラクタ		*/
	virtual ~MoveCamera(void);

	/* @brief	更新処理			*/
	virtual void Update(void)		override;
	/* @brief	Guiの更新処理		*/
	virtual void GuiUpdate(void)	override;

private:
	/* @brief	キー入力処理
	 * @sa		Update()			*/
	void Input(void);
	/* @brief	平行移動
	 * @sa		Update()			*/
	void MoveTrans(void);
	/* @brief	自分を中心に回転
	 * @sa		Update()			*/
	void MoveRotation(void);
	/* @brief	注視点を中心に回転
	 * @sa		Update()			*/
	void MoveAtRotation(void);

	//! 方向
	VECTOR2 dir_;
	//! 速度
	VECTOR2 velocity_;
	//! 回転速度
	VECTOR2 rotVelocity_;
	//! 注視点の回転速度
	VECTOR2 rotAtVelocity_;
};

#endif // _MOVE_CAMERA_H_