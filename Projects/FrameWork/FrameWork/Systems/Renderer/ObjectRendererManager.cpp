#include "ObjectRendererManager.h"

#include "ObjectRenderer.h"
#include "Sprite/SpriteRenderer.h"
#include "Model/MeshRenderer.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"

#include "../../Graphics/DirectX11/DirectX11.h"
#include "Shader/Default.h"
#include "Shader/CascadeShadow.h"

void ObjectRendererManager::Add(ObjectRenderer* obj)
{
	BaseManager::Add(obj);
	this->Sort();
}

void ObjectRendererManager::Sort(void)
{
	int size = (int)obj_.size() - 1;

	std::vector<float> wVec;
	VECTOR3 wPos;
	VECTOR3 cameraPos;
	if (!systems_->GetSceneManager()) { return; }
	if (const auto& scene = systems_->GetSceneManager()->GetScene())
	{
		cameraPos = scene->GetCameraManager()->GetMainCamera()->GetPosition();
	}

	for (auto obj : obj_)
	{
		float vecTemp = 0;

		wPos = obj->GetTransform()->position - cameraPos;
		vecTemp = VecLengthSq(wPos);
		if (obj->GetType() == ObjectRenderer::RendererType::SPRITE)
		{
			vecTemp = obj->GetTransform()->position.z - cameraPos.z;
			vecTemp -= LAYER_DIST * static_cast<SpriteRenderer*>(obj)->GetLayer();
		}
		wVec.emplace_back(vecTemp);
	}

	bool sort = false;
	for (;;)
	{
		sort = false;
		for (int i = 0; i < size; ++i)
		{
			if (obj_[i] && !obj_[i]->IsSort()) { continue; }
			if (wVec[i] < wVec[i + 1])
			{
				Swap(obj_[i], obj_[i + 1]);
				Swap(wVec[i], wVec[i + 1]);
				sort = true;
			}
		}
		if (!sort) { break; }
	}
}

void ObjectRendererManager::FastDraw(void)
{
	const auto& directX11	= static_cast<DirectX11*>(systems_->GetGraphics());
	const auto& target		= directX11->GetRenderTarget();
	const auto& dev			= systems_->GetGraphics()->GetWrapper();

	target->BeginMultiRendererTarget();
	dev->BeginDrawObjectRenderer();

	for (auto& obj : obj_)
	{
		if (!obj) { continue; }
		if (obj->IsEnable() && obj->IsFastDraw())
		{
			if (obj->GetType() == ObjectRenderer::RendererType::SPRITE)
			{
				Shader* shader = nullptr;
				const auto& sprite = (SpriteRenderer*)obj;
				if (obj->GetShader()!= Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->GetShader());
					shader->SetParam(MATRIX().Identity().Create(obj->GetTransform()), obj->GetMaterial().diffuse, sprite->GetTexcoord());
				}
				dev->Draw(sprite, shader);
			}
			else if (obj->GetType() == ObjectRenderer::RendererType::MODEL)
			{
				Shader* shader = nullptr;
				const auto& model = (MeshRenderer*)obj;
				if (obj->GetShader() != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->GetShader());
					shader->SetParam(MATRIX().Identity().Create(obj->GetTransform()), obj->GetMaterial().diffuse, VECTOR4(0, 0, 1, 1));
				}
				dev->Draw(model, shader);
			}
		}
	}

	dev->EndDrawRenderer();
}

void ObjectRendererManager::Draw(void)
{	
	const auto& directX11	= static_cast<DirectX11*>(systems_->GetGraphics());
	const auto& target		= directX11->GetRenderTarget();
	const auto& dev			= systems_->GetGraphics()->GetWrapper();

	this->Sort();
	dev->BeginDrawObjectRenderer();

	for (auto& obj : obj_)
	{
		if (!obj) { continue; }
		if (obj->IsEnable() && !obj->IsFastDraw())
		{
			if (obj->GetType() == ObjectRenderer::RendererType::SPRITE)
			{
				Shader* shader = nullptr;
				const auto& sprite = (SpriteRenderer*)obj;
				if (obj->GetShader() != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->GetShader());
					shader->SetParam(MATRIX().Identity().Create(obj->GetTransform()), obj->GetMaterial().diffuse, sprite->GetTexcoord());
				}
				dev->Draw(sprite, shader);
			}
			else if (obj->GetType() == ObjectRenderer::RendererType::MODEL)
			{
				Shader* shader = nullptr;
				const auto& model = (MeshRenderer*)obj;
				if (obj->GetShader() != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->GetShader());
					shader->SetParam(MATRIX().Identity().Create(obj->GetTransform()), obj->GetMaterial().diffuse, VECTOR4(0, 0, 1, 1));
				}
				dev->Draw(model, shader);
			}
		}
	}

	dev->EndDrawRenderer();
	target->EndMultiRendererTarget();
}

void ObjectRendererManager::DrawShadow(void)
{
	return;
	const auto& graphics = systems_->GetGraphics();
	const auto& dev = graphics->GetWrapper();

	CascadeShadow* shader = static_cast<CascadeShadow*>(systems_->GetShader()->GetShader(Shader::ENUM::ZTEXTURE));
	dev->BeginDrawObjectRenderer();

	for (int i = 0; i < CascadeManager::MAX_CASCADE; ++i)
	{
		shader->BeginDraw(i);

		for (auto& obj : obj_)
		{
			if (!obj) { continue; }
			if (obj->IsShadow())
			{
				if (obj->GetType() == ObjectRenderer::RendererType::SPRITE)
				{
					const auto& sprite = (SpriteRenderer*)obj;
					dev->Draw(sprite, shader);
				}
				else if (obj->GetType() == ObjectRenderer::RendererType::MODEL)
				{
					const auto& model = (MeshRenderer*)obj;
					dev->Draw(model, shader);
				}
			}
		}
		shader->EndDraw();
	}

	dev->EndDrawRenderer();
}
