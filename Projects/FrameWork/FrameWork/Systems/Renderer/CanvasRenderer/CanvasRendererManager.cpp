//-----------------------------------------------------------------------------
//
//	2D`æĮ[CanvasRendererManager.cpp]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CanvasRendererManager.h"
#include <math.h>
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Utility/Font.h"
#include "../../GameSystems.h"

#include "CanvasRendererBase.h"
#include "CanvasRendererImage.h"
#include "CanvasRendererText.h"

void CanvasRendererManager::Uninit(void)
{
	for (auto& obj : obj_)
	{
		if (obj) { obj->Uninit(); }
	}
}

void CanvasRendererManager::Add(CanvasRendererBase* obj)
{
	BaseManager::Add(obj);
	this->Sort();
}

void CanvasRendererManager::Sort(void)
{
	int size = (int)obj_.size() - 1;
	for (int i = 0; i < size; ++i)
	{
		// DæĘÉķÄüęÖĶ
		if (obj_[i]->GetPriority() > obj_[i + 1]->GetPriority())
		{
			Swap(obj_[i], obj_[i + 1]);
			i = -1;		// CNgģęÄ0Éßé
		}
	}
}

//|S`æ
void CanvasRendererManager::Draw(void)
{
	const auto& dev = systems_->GetGraphics()->GetWrapper();

	dev->BeginDrawCanvasRenderer();

	for (auto& obj : obj_)
	{
		if (obj->GetEnable())
		{
			const auto& type = obj->GetCanvasType();
			if (type == CanvasRendererBase::CanvasType::Image)
			{
				const auto& img = static_cast<CanvasRenderer::Image*>(obj);
				Shader* shader = nullptr;
				Shader::ENUM draw = img->GetShader();
				if (draw != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(draw);
					MATRIX m;
					m._11 = img->GetAngle();
					shader->SetParam(m, img->GetColor(), VECTOR4(0, 0, 1, 1));
				}
				dev->Draw(img, shader);
				if (draw != Shader::ENUM::UNKOUWN)
				{
					dev->BeginDrawCanvasRenderer();
				}
			}
			else if (type == CanvasRendererBase::CanvasType::Text)
			{
				if (const auto& font = dev->GetFont())
				{
					const auto& text = static_cast<CanvasRenderer::Text*>(obj);
					font->Draw(text->GetString(), text->GetPosition(), text->GetSize(), text->GetColor(), text->GetFaldBack(), text->GetDrawSize());
				}
			}
		}
	}

	dev->EndDrawRenderer();
}
