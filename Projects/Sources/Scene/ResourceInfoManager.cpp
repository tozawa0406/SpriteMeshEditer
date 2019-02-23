#include "ResourceInfoManager.h"
#include <FrameWork/Scene/SceneManager.h>

ResourceInfoManager::ResourceInfoManager(void)
{
}

ResourceInfoManager::~ResourceInfoManager(void)
{
}

const string* ResourceInfoManager::LoadTexture(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Texture::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
		return &texture_.baseFileName[0];
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Texture::Title::MAX);
		size = max - baseMax;
//		return &texture_.titleFileName[0];
		break;
	}

	return nullptr;
}

const string* ResourceInfoManager::LoadModel(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Model::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
//		return &model_.baseFileName[0];
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Model::Title::MAX);
		size = max - baseMax;
//		return &model_.titleFileName[0];
		break;
	}

	return nullptr;
}

const ANIMATION_INFO* ResourceInfoManager::LoadModelAnimation(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Model::Animation::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
//		return &model_..;
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Model::Animation::Title::MAX);
		size = max - baseMax;
	//	return &model_.titleFileName[0];
		break;
	}

	return nullptr;
}

const SOUNDPARAM* ResourceInfoManager::LoadSound(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Sound::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max  = baseMax;
		size = max;
		return &sound_.baseFileName[0];
		break;
	case SceneList::TITLE:
		max  = static_cast<int>(Resources::Sound::Title::MAX);
		size = max - baseMax;
//		return &sound_.titleFileName[0];
		break;
	}

	return nullptr;
}

const string& ResourceInfoManager::GetWhiteTextureName(void) const
{
	return texture_.baseFileName[0];
}
