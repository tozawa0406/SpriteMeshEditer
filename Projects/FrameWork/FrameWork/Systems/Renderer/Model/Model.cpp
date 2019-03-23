//-----------------------------------------------------------------------------
//
//	モデル管理[Model.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Model.h"
#include "../../../Systems/GameSystems.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"
#include "../../../Scene/BaseScene.h"
#include "../../Loading.h"
#include "../../../../../Sources/Scene/00_EachScene.h"

Model::Model(Systems* systems) : Interface(systems)
{
}

HRESULT Model::Init(void)
{
	if (!systems_) { return E_FAIL; }
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	int size = 0, max = 0;
	const string* fileName = systems_->GetResource().LoadModel(SceneList::MAX, size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			for (int i = 0; i < max; ++i)
			{
				MESH_RESOURCE mesh;
				string na = fileName[i];
				string no = "が開けませんでした";
				na += no;
				if (window->ErrorMessage(na.c_str(), "エラー", dev->Load(mesh, fileName[i]))) { return E_FAIL; }

				mesh_.emplace_back(mesh);
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	return S_OK;
}

void Model::Uninit(void)
{
	Release(true);
}

int	Model::SetUpLoading(Loading* loading, int sceneNum)
{
	sceneNum_ = sceneNum;
	loading_  = loading;

	if (!systems_) { return 0; }
	int size = 0, max = 0;
	systems_->GetResource().LoadModel(static_cast<SceneList>(sceneNum), size, max);

	int animSize = 0;
	systems_->GetResource().LoadModelAnimation(static_cast<SceneList>(sceneNum), animSize, max);

	return size + animSize;
}

HRESULT Model::Load(int sceneNum)
{
	sceneNum_ = sceneNum;

	if (!systems_) { return E_FAIL; }
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	int size = 0, max = 0;
	const auto& fileName = systems_->GetResource().LoadModel(static_cast<SceneList>(sceneNum), size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Model::Base::MAX)) { continue; }

				MESH_RESOURCE mesh;
				string na = fileName[i];
				string no = "が開けませんでした";
				na += no;
				if (window->ErrorMessage(na.c_str(), "エラー", dev->Load(mesh, fileName[i]))) { return E_FAIL; }

				mesh_.emplace_back(mesh);
				loading_->AddLoading();
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	// アニメーション
	const auto& animFileName = systems_->GetResource().LoadModelAnimation(static_cast<SceneList>(sceneNum), size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Model::Animation::Base::MAX)) { continue; }
				auto anim = animFileName[i - static_cast<int>(Resources::Model::Animation::Base::MAX)];

				MESH_RESOURCE mesh;
				string na = anim.fileName;
				string no = "が開けませんでした";
				na += no;
				if (window->ErrorMessage(na.c_str(), "エラー", dev->Load(mesh, fileName[i], true))) { return E_FAIL; }

				loading_->AddLoading();
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	return S_OK;
}

void Model::Release(bool uninit)
{
	if (!systems_) { return; }
	int size = 0, max = 0;
	systems_->GetResource().LoadModel(static_cast<SceneList>(sceneNum_), size, max);

	int baseMax = static_cast<int>(Resources::Model::Base::MAX);
	if (uninit) { baseMax = 0; }

	for (int i = max - 1; i >= baseMax; --i)
	{
		for (auto& mesh : mesh_[i].mesh)
		{
			ReleasePtr(mesh.material.texture[0]);
			ReleasePtr(mesh.material.texture[1]);
			ReleasePtr(mesh.vertexBuffer);
			ReleasePtr(mesh.indexBuffer);
		}

		for (auto itr = mesh_.begin(); itr != mesh_.end();)
		{
			if (&(*itr) == &mesh_[i])
			{
				itr = mesh_.erase(itr);		//配列削除
				break;
			}
			else { itr++; }
		}
		mesh_.shrink_to_fit();
	}
}
