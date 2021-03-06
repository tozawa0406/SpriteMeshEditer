//-----------------------------------------------------------------------------
//
//	ロード画面[Loading.cpp]
//	Auther : 戸澤翔太
//																	2018/09/10
//-----------------------------------------------------------------------------
#include "Loading.h"
#include "GameSystems.h"
#include "../Windows/Windows.h"

Loading::Loading(Systems* systems) : Interface(systems)
	, isLoading_(false)
	, isFail_(false)
	, thread_(nullptr)
	, nowLoading_(0)
	, allTask_(0)
	, loadingCnt_(0)
	, patternCnt_(0)
{
	texture_ = systems_->GetTexture();
	model_   = systems_->GetModel();
	sound_   = systems_->GetSound();
}

Loading::~Loading(void)
{
}

HRESULT Loading::Init(void)
{
	display_[0].Init(251, (int)Resources::Texture::Base::WHITE);
	display_[0].SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	display_[0].SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	display_[0].SetColor(COLOR(0, 0, 0, 0));
	display_[0].SetEnable(false);

	float c = 0.3f;
	display_[1].Init(252, (int)Resources::Texture::Base::WHITE);
	VECTOR2 adjust = { BAR_PADDING_X + Half(BAR_SIZE_X), BAR_PADDING_Y + Half(BAR_SIZE_Y) };
	display_[1].SetPosition(VECTOR2(Windows::WIDTH - adjust.x, Windows::HEIGHT - adjust.y));
	display_[1].SetSize(VECTOR2(BAR_SIZE_X, BAR_SIZE_Y));
	display_[1].SetColor(COLOR(c, c, c, 1));
	display_[1].SetEnable(false);

	c = 0.7f;
	display_[2].Init(253, (int)Resources::Texture::Base::WHITE);
	display_[2].SetPosition(VECTOR2(Windows::WIDTH - adjust.x, Windows::HEIGHT - adjust.y));
	display_[2].SetSize(VECTOR2(0, (float)BAR_SIZE_Y));
	display_[2].SetColor(COLOR(c, c, c, 1));
	display_[2].SetEnable(false);

	display_[3].Init(253, (int)Resources::Texture::Base::WHITE);
	auto p = display_[0].GetPosition();
	p.y -= 100;
	display_[3].SetPosition(p);
	display_[3].SetSize(VECTOR2(150, 150));
	display_[3].SetRotationOffset(VECTOR2(75, 75));
	display_[3].SetColor(COLOR(1, 1, 1, 1));
	display_[3].SetEnable(false);
	display_[3].SetPattern(0);

	loadingPercent_ = new Score(systems_->GetSceneManager(), false, 3);
	loadingPercent_->SetPriority(252);
	loadingPercent_->SetScale(0.5f);
	loadingPercent_->SetColor(COLOR(c, c, c, 1));

	return S_OK;
}

void Loading::Uninit(void)
{
	for (auto& d : display_) { d.Uninit(); }
	DeletePtr(loadingPercent_);
}

void Loading::FadeAlpha(float a)
{
	// ％の描画
	if (allTask_ > 0)
	{
		float percent = nowLoading_ / (float)allTask_;
		auto size = display_[2].GetSize();
		size.x = percent * BAR_SIZE_X;
		display_[2].SetSize(size);
		auto pos = display_[2].GetPosition();
		pos.x = Windows::WIDTH - (BAR_PADDING_X + BAR_SIZE_X) + Half(size.x);
		display_[2].SetPosition(pos);
		
		pos = { Windows::WIDTH - (BAR_PADDING_X + loadingPercent_->GetSize().x) , Windows::HEIGHT - BAR_PADDING_Y * 1.3f + Half(BAR_SIZE_Y) };
		loadingPercent_->Update(min((int)(percent * 100), 100), pos);
	}

	// 中央画像
	if (isLoading_)
	{
		patternCnt_ += 0.025f;
		display_[3].SetPattern((float)((int)patternCnt_ % 2));
		if ((int)patternCnt_ % 4 == 3) { display_[3].SetPattern(2); }
		display_[3].SetEnable(true);
	}

	for (int i = 0; i < 3; ++i)
	{
		auto c = display_[i].GetColor();
		c.a = a;
		display_[i].SetColor(c);
		display_[i].SetEnable(true);
	}
}

void Loading::Start(int sceneNum)
{
	LoadingSafe(true);

	// 既に持っているリソースの破棄
	if (texture_)	{ texture_->Release();	}
	if (model_)		{ model_->Release();	}
	if (sound_)		{ sound_->Release();	}

	// 全リソース数を取得
	if (texture_)	{ allTask_ += texture_->SetUpLoading(this, sceneNum);	}
	if (model_)		{ allTask_ += model_->SetUpLoading(this, sceneNum);		}
	if (sound_)		{ allTask_ += sound_->SetUpLoading(this, sceneNum);		}

	thread_ = new std::thread([=]() { return Load(sceneNum); });
}

bool Loading::Load(int sceneNum)
{
	if (texture_)
	{
		if (FAILED(texture_->Load(sceneNum)))
		{
			SetFail(true);
			LoadingSafe(false);
			return true;
		}
	}
	if (model_)
	{
		if (FAILED(model_->Load(sceneNum)))
		{
			SetFail(true);
			LoadingSafe(false);
			return true;
		}
	}
	if (sound_)
	{
		if (FAILED(sound_->Load(sceneNum)))
		{
			SetFail(true);
			LoadingSafe(false);
			return true;
		}
	}

	// 100%を描画するため終了まで100ミリ秒(0.1秒)待つ
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	LoadingSafe(false);
	return true;
}

void Loading::End(void)
{
	allTask_			= 0;
	nowLoading_			= 0;
	loadingCnt_			= 0;
	patternCnt_			= 0;
	auto size = display_[2].GetSize();
	size.x = 0;
	display_[2].SetSize(size);
	display_[2].SetEnable(true);
	display_[3].SetAngle(0);
	display_[3].SetEnable(false);

	thread_->detach();
	DeletePtr(thread_);
}

bool Loading::LoadingSafe(void)
{
	std::lock_guard<std::mutex> lock(mutex_);

	return isLoading_;
}

void Loading::LoadingSafe(bool loading)
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	isLoading_ = loading;
}

bool Loading::IsFail(void)
{
	std::lock_guard<std::mutex> lock(mutex_);

	return isFail_;
}

void Loading::SetFail(bool fail)
{
	std::lock_guard<std::mutex> lock(mutex_);

	isFail_ = fail;
}
