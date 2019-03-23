//-----------------------------------------------------------------------------
//
//	テクスチャ管理[Texture.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Texture.h"
#include "../../../Windows/Windows.h"
#include "../../GameSystems.h"
#include "../../../Scene/BaseScene.h"
#include "../../Loading.h"

Texture::Texture(Systems* systems) : Interface(systems)
	, sceneNum_(-1)
	, loadAdd_(nullptr)
{
}

HRESULT Texture::Init(void)
{
	if (!systems_) { return E_FAIL; }
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	int size = 0, max = 0;
	const string* fileName = systems_->GetResource().LoadTexture(SceneList::MAX, size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			for (int i = 0; i < max; ++i)
			{
				ITextureResource* temp = nullptr;
				string na = fileName[i];
				string no = "が開けませんでした";
				na += no;

				if (window->ErrorMessage(na.c_str(), "エラー", dev->Load(&temp, fileName[i]))) { return E_FAIL; }

				list_.emplace_back(temp);
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	loadAdd_ = new LoadAddTexture(systems_);
	if (loadAdd_)
	{
		loadAdd_->Init();
	}

	return S_OK;
}

void Texture::Uninit(void)
{
	if (loadAdd_)
	{
		loadAdd_->Uninit();
	}
	DeletePtr(loadAdd_);
	Release(true);
}

int Texture::SetUpLoading(Loading* loading, int sceneNum)
{
	sceneNum_ = sceneNum;
	loading_  = loading;

	if (!systems_) { return 0; }
	int size = 0, max = 0;
	systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum), size, max);

	if (loadAdd_)
	{
		size += loadAdd_->SetUpLoading(loading, sceneNum);
	}

	return size;
}

HRESULT Texture::Load(int sceneNum)
{
	sceneNum_ = sceneNum;

	if (!systems_) { return E_FAIL; }
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	int size = 0, max = 0;
	const auto& fileName = systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum), size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Texture::Base::MAX)) { continue; }
				ITextureResource* temp = nullptr;
				string na = fileName[i];
				string no = "が開けませんでした";
				na += no;

				if (window->ErrorMessage(na.c_str(), "エラー", dev->Load(&temp, fileName[i - static_cast<int>(Resources::Texture::Base::MAX)]))) { return E_FAIL; }

				list_.emplace_back(temp);
				loading_->AddLoading();
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	if (loadAdd_)
	{
		loadAdd_->Load(sceneNum_);
	}

	return S_OK;
}

void Texture::Release(bool uninit)
{
	if (!systems_) { return; }

	if (loadAdd_)
	{
		loadAdd_->Release(uninit);
	}

	int size = 0, max = 0;
	systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum_), size, max);

	int baseMax = static_cast<int>(Resources::Texture::Base::MAX);
	if (uninit) { baseMax = 0; }

	for (int i = max - 1; i >= baseMax; --i)
	{
		auto list = list_[i];
		RemoveVector(list_, list);
		ReleasePtr(list);
	}
}

VECTOR2 Texture::GetTextureSize(int texNum) const
{
	if (list_.size() > static_cast<size_t>(texNum))
	{
		if (list_[texNum])
		{
			return list_[texNum]->GetSize();
		}
	}
	return VECTOR2(0);
}

void Texture::GuiUpdate(void)
{
	if (loadAdd_)
	{
//		loadAdd_->GuiUpdate();
	}
}
