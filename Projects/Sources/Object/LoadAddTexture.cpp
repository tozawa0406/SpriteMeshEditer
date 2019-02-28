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
		std::vector<string> temp;
		search_->Search("Edit/", "jpg", temp);
		for (auto name : temp)
		{
			ADD_TEXTURE_DATA add;
			add.name = name;
			add.texNum = 0;
			list_.emplace_back(add);
		}
		temp.clear();
		search_->Search("Edit/", "png", temp);
		for (auto name : temp)
		{
			ADD_TEXTURE_DATA add;
			add.name = name;
			add.texNum = 0;
			list_.emplace_back(add);
		}
		temp.clear();

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
			int max = static_cast<int>(list_.size()) + static_cast<int>(Resources::Texture::Base::MAX);
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Texture::Base::MAX)) { continue; }

				int arrayNum = i - static_cast<int>(Resources::Texture::Base::MAX);
				HRESULT hr = wrapper->LoadTexture(list_[arrayNum].name, i);
				if (FAILED(hr)) { return E_FAIL; }
				list_[arrayNum].texNum = i;
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
}

int LoadAddTexture::SelectTexture(string& textureName)
{
	int ret = -1;
	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::CollapsingHeader("TextureList"))
	{
		if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 200)))
		{
			for (auto& obj : list_)
			{
				bool select = false;

				string name = "";
				for (size_t i = obj.name.size() - 1; obj.name[i] != '/' && i > 0; --i)
				{
					name.insert(name.begin(), obj.name[i]);
				}

				ImGui::MenuItem(name.c_str(), nullptr, &select);
				if (select)
				{
					textureName = obj.name;
					ret = obj.texNum;
				}
			}
			ImGui::EndChild();
		}
	}
	return ret;
}

int LoadAddTexture::SetTexture(const string& texName)
{
	string name = "";
	if (texName.size() <= 0) { return 0; }
	for (size_t i = texName.size() - 1; i > 0 && texName[i] != '/'; --i)
	{
		name.insert(name.begin(), texName[i]);
	}

	for (auto& obj : list_)
	{
		string listName = "";
		for (size_t i = obj.name.size() - 1; i > 0 && obj.name[i] != '/'; --i)
		{
			listName.insert(listName.begin(), obj.name[i]);
		}

		if (name == listName)
		{
			return obj.texNum;
		}
	}
	return 0;
}
