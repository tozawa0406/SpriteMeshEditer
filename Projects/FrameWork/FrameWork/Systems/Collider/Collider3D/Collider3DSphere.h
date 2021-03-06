/*
 * @file		Collider3DSphere.h
 * @brief		��
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_SPHERE_H_
#define _COLLIDER_3D_SPHERE_H_

#include "Collider3DBase.h"

namespace Collider3D
{
	class Sphere : public Collider3DBase
	{
	public:
		Sphere(Object* obj);
		void Update(void) override;
		void SetSize(float d) { size_ = VECTOR3(d, d, d); }
		void SetColliderTag(ColliderTag tag) override;

	private:
		Transform			transform2_;
		ColliderRenderer	renderer2_;
	};
}

#endif // _COLLISION_3D_SPHERE_H_
