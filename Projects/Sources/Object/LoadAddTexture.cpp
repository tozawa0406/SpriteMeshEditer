#include "LoadAddTexture.h"
#include <FrameWork/Windows/Windows.h>
#include <FrameWork/Systems/GameSystems.h>
#include <FrameWork/Scene/BaseScene.h>
#include <FrameWork/Systems/Loading.h>

#include <FrameWork/Systems/DebugSetting/GUI.h>

LoadAddTexture::LoadAddTexture(Systems* systems) : Interface(systems)
	, sceneNum_(-1)
	, search_(nullptr)
{
}

HRESULT LoadAddTexture::Init(void)
{
	if (!systems_) { return E_FAIL; }

	search_ = new SearchFile;
	if (!search_) { return E_FAIL; }

	return S_OK;
}

void LoadAddTexture::Uninit(void)
{
	Release(true);
	DeletePtr(search_);
}

int LoadAddTexture::SetUpLoading(Loading* loading, int sceneNum)
{
	sceneNum_ = sceneNum;
	loading_  = loading;

	int size = 0;
	if (search_)
	{
		search_->Search(Define::ResourceDirectoryName + "Edit/", "jpg", list_);
		search_->Search(Define::ResourceDirectoryName + "Edit/", "png", list_);
	}

	return size;
}

HRESULT LoadAddTexture::Load(int sceneNum)
{
	sceneNum_ = sceneNum;

	if (!systems_) { return E_FAIL; }

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& wrapper = graphics->GetWrapper())
		{
			int max = static_cast<int>(list_.size());
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Texture::Base::MAX)) { continue; }
				HRESULT hr = wrapper->LoadTexture(list_[i], i);
				if (FAILED(hr)) { 
					return E_FAIL; }
				loading_->AddLoading();
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	return S_OK;
}

void LoadAddTexture::Release(bool uninit)
{
	if (!systems_) { return; }
	int size = 0, max = 0;
	systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum_), size, max);

	int baseMax = static_cast<int>(Resources::Texture::Base::MAX);
	if (uninit) { baseMax = 0; }

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& wrapper = graphics->GetWrapper())
		{
			for (int i = max - 1; i >= baseMax; --i)
			{
				wrapper->ReleaseTexture(i);
			}
		}
	}
}

void LoadAddTexture::GuiUpdate(void)
{
	if (ImGui::BeginMenu("AddTexture"))
	{
		for (auto& obj : list_)
		{
			ImGui::Text(obj.c_str());
		}
		ImGui::EndMenu();
	}
}
