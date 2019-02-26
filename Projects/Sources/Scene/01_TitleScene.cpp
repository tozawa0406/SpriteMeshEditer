#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Pivot.h"
#include "../Object/Command/PositionCommand.h"

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, client_(nullptr)
	, objectManager_(nullptr)
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
		test_->Init(static_cast<int>(Resources::Texture::Base::WHITE), &trans_);
	}

	objectManager_ = new ObjectManager(this);
	if (objectManager_)
	{
		objectManager_->Init();
	}

	objectManager_->Create<Pivot>();

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
	UninitDeletePtr(objectManager_);
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
