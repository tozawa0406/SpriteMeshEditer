//-----------------------------------------------------------------------------
//
//	2D当たり判定管理[Collider2DManager.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider2D.h"
#include <math.h>
#include "../../Object/Object.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"
#include "../GameSystems.h"
#include "Collider2D.h"

Collision2DManager::Collision2DManager(Systems* systems) : BaseManager(systems)
{
}

Collision2DManager::~Collision2DManager(void)
{
	for (uint i = 0; i < obj_.size();)
	{
		this->RemovePtr(obj_[i]);
	}
}

//円の当たり判定
bool Collision2DManager::HitCircle(float x0, float y0, float r0, float x1, float y1, float r1)
{
	//オブジェクト同士の距離を取る(２乗)
	float l = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);

	//オブジェクト同士の半径の和(２乗)と上記を比較
	return l < (r0 + r1) * (r0 + r1) ? true : false;

}

// 矩形の当たり判定
bool Collision2DManager::HitRectangle(float x0, float y0, float w0, float h0, float x1, float y1, float w1, float h1)
{
	//上下左右の辺
	float hw0 = w0 * 0.5f;
	float hh0 = h0 * 0.5f;
	float hw1 = w1 * 0.5f;
	float hh1 = h1 * 0.5f;
	SidesVec4 pos0 = { x0 - hw0, x0 + hw0, y0 - hh0, y0 + hh0 };
 	SidesVec4 pos1 = { x1 - hw1, x1 + hw1, y1 - hh1, y1 + hh1 };
	
	//左辺が右辺より右 か 右辺が左辺より左 か 上辺が下辺より下 か 下辺が上辺より上
	if (pos0.l >= pos1.r || pos0.r <= pos1.l || pos0.u >= pos1.d || pos0.d <= pos1.u)
	{
		return false;
	}

	return true;
}

// 円と矩形の当たり判定
bool Collision2DManager::HitCircleToRectangle(float x0, float y0, float r0, float x1, float y1, float w1, float h1)
{
	//四角の上下左右
	float hw1 = w1 * 0.5f;
	float hh1 = h1 * 0.5f;
	SidesVec4 pos1 = { x1 - hw1, x1 + hw1, y1 - hh1, y1 + hh1 };

	//四角の横幅の中にいるとき
	if (pos1.l <= x0 && x0 <= pos1.r)
	{
		//円の最下点が四角の縦幅の中にいるとき か 円の最上点が四角の縦幅の中にいるとき
		if ((pos1.u <= (y0 + r0) && (y0 + r0) <= pos1.d) || (pos1.u <= (y0 - r0) && (y0 - r0) <= pos1.d))
		{
			return true;
		}
	}
	//四角の縦幅の中にいるとき
	else if (pos1.u <= y0 && y0 <= pos1.d)
	{
		//円の最右点が四角の横幅の中にいるとき か 円の最左点が四角の横幅の中にいるとき
		if ((pos1.l <= (x0 + r0) && (x0 + r0) <= pos1.r) || (pos1.l <= (x0 - r0) && (x0 - r0) <= pos1.r))
		{
			return true;
		}
	}

	//左上頂点と円の当たり判定 か 右上頂点と円の当たり判定 か 左下頂点と円の当たり判定 か 右下頂点と円の当たり判定
	if (HitCircle(x0, y0, r0, pos1.l, pos1.u, 0.05f) || HitCircle(x0, y0, r0, pos1.r, pos1.u, 0.05f)
	 || HitCircle(x0, y0, r0, pos1.l, pos1.d, 0.05f) || HitCircle(x0, y0, r0, pos1.r, pos1.d, 0.05f))
	{
		return true;
	}

	return false;
}

void Collision2DManager::Update(void)
{
	for (auto col : obj_)
	{
		col->list_.clear();
		if (!systems_->GetDebug()->GetDebug())
		{
			col->renderer_.enable = false;
		}
	}

	for (auto col1 : obj_)
	{
		if (!col1->enable_) { continue; }
		for (auto col2 : obj_)
		{
			if (!col2->enable_) { continue; }
			if (col1 == col2) { continue; }

			bool isList = false;
			for (auto obj : col2->list_)
			{
				if (obj == col1->object_) { isList = true; }
			}
			if (isList) { continue; }

			if (col1->type_ == Collider2DBase::Type::CIRCLE)
			{
				if (col2->type_ == Collider2DBase::Type::CIRCLE)
				{
					if (this->HitCircle(col1->position_.x, col1->position_.y, Half(col1->size_.x), col2->position_.x, col2->position_.y, Half(col2->size_.x)))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
				else if (col2->type_ == Collider2DBase::Type::RECTANGLE)
				{
					if (this->HitCircleToRectangle(col1->position_.x, col1->position_.y, Half(col1->size_.x), col2->position_.x, col2->position_.y, col2->size_.x, col2->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
			}
			else if (col1->type_ == Collider2DBase::Type::RECTANGLE)
			{
				if (col2->type_ == Collider2DBase::Type::CIRCLE)
				{
					if (this->HitCircleToRectangle(col2->position_.x, col2->position_.y, Half(col2->size_.x), col1->position_.x, col1->position_.y, col1->size_.x, col1->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
				else if (col2->type_ == Collider2DBase::Type::RECTANGLE)
				{
					if (this->HitRectangle(col1->position_.x, col1->position_.y, col1->size_.x, col1->size_.y, col2->position_.x, col2->position_.y, col2->size_.x, col2->size_.y))
					{
						col1->list_.emplace_back(col2->object_);
						col2->list_.emplace_back(col1->object_);
					}
				}
			}

		}
	}
}
