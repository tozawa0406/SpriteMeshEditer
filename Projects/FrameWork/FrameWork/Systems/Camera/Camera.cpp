#include "Camera.h"
#include "../../Windows/Windows.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"

Camera::Camera(void) : GUI(Systems::Instance(), nullptr, "Camera")
	, manager_(nullptr)
	, position_(VECTOR3(POSITION_X,	POSITION_Y, POSITION_Z))
	, at_(VECTOR3(0))
	, up_(VECTOR3(0, 1, 0))
	, front_(VECTOR3(0, 0, 1))
	, right_(VECTOR3(1, 0, 0))
	, frontXPlane_(VECTOR3(0, 0, 1))
	, rightXPlane_(VECTOR3(1, 0, 0))
{
}
