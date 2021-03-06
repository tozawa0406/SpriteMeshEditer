//-----------------------------------------------------------------------------
//
//	2D―čŧčĮ[Collider2DManager.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _COLLIDER_2D_MANAGER_H_
#define _COLLIDER_2D_MANAGER_H_

#include "../../Define/Define.h"
//#include "DrawCollider.h"
#include "../BaseManager.h"

//-----------------------------------------------------------------------------
//	NXéū
//-----------------------------------------------------------------------------
class Systems;
class Collider2DBase;
class Collision2DManager : public BaseManager<Collider2DBase>
{
	struct SidesVec4
	{
		float l;
		float r;
		float u;
		float d;
	};

	bool HitCircle(float x0, float y0, float r0, float x1, float y1, float r1);	//~Ė―čŧč
	bool HitRectangle(float x0, float y0, float w0, float h0, float x1, float y1, float w1, float h1);
	bool HitCircleToRectangle(float x0, float y0, float r0, float x1, float y1, float w1, float h1);

public:
	Collision2DManager(Systems* systems);
	~Collision2DManager(void) override;
	HRESULT Init(void)   override { return S_OK; }
	void    Update(void) override;

	friend Collider2DBase;
	friend Systems;
//	friend Collider2D::Circle;
//	friend Collider2D::Rectangle;
};

#endif // _COLLEDER_2D_MANAGER_H_

