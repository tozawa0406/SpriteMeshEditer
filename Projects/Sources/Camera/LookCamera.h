#ifndef _LOOK_CAMERA_H_
#define _LOOK_CAMERA_H_

#include <FrameWork/Systems/Camera/Camera.h>

class LookCamera : public Camera
{
public:
	/* @brief	コンストラクタ		*/
	LookCamera(void);
	/* @brief	デストラクタ		*/
	virtual ~LookCamera(void);

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

	//! 方向
	VECTOR2 dir_;
	//! 速度
	VECTOR3 velocity_;
};

#endif // _LOOK_CAMERA_H_