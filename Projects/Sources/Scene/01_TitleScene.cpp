#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Command/PositionCommand.h"

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, client_(nullptr)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(SceneList sceneNum)
{
	BaseScene::Init(sceneNum);

	test_ = new SpriteRenderer;
	if (test_)
	{
		test_->Init(static_cast<int>(Resources::Texture::Base::LOAD), &trans_);
	}

	client_ = new Client;
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetReceiver(test_);
		client_->Init();
	}
}

void TitleScene::Uninit(void)
{
	UninitDeletePtr(client_);
	DeletePtr(test_);
}

SceneList TitleScene::Update(void)
{
	// コントローラの取得
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// 遷移処理
	return SceneList::NOTCHANGE;
}

void TitleScene::GuiUpdate(void)
{
	if (!client_) { return; }

	if (ImGui::Button("Undo"))
	{
		client_->Undo();
	}
	ImGui::SameLine();
	if (ImGui::Button("Redo"))
	{
		client_->Redo();
	}

	client_->Update();
}

void TitleScene::ConsoleWindow(void)
{
	client_->ConsoleWindow();
}
