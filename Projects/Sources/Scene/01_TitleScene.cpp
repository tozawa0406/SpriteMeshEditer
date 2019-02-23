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
		test_->Init(systems_, static_cast<int>(Resources::Texture::Base::WHITE), &trans_);
	}
	trans_.scale = VECTOR3(5);

	client_ = new Client;
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetReciver(&trans_);
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
	// ƒRƒ“ƒgƒ[ƒ‰‚ÌŽæ“¾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// ‘JˆÚˆ—
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
