#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Command/PositionCommand.h"

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, currentReceiver_(nullptr)
	, objectManager_(nullptr)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(SceneList sceneNum)
{
	BaseScene::Init(sceneNum);

	objectManager_ = new ObjectManager(this);
	if (objectManager_)
	{
		objectManager_->Init();
	}

	pivot_ = objectManager_->Create<Pivot>();
}

void TitleScene::Uninit(void)
{
	for (auto& c : clientList_) { UninitDeletePtr(c); }

	UninitDeletePtr(objectManager_);
}

SceneList TitleScene::Update(void)
{
	// ƒRƒ“ƒgƒ[ƒ‰‚ÌŽæ“¾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }
	if (!currentReceiver_) { return SceneList::NOTCHANGE; }

	if (ctrl->Press(Input::GAMEPAD_L1, DIK_LCONTROL))
	{
		if (ctrl->Press(Input::GAMEPAD_L2, DIK_LSHIFT))
		{
			if (ctrl->Trigger(Input::GAMEPAD_L1, DIK_Z))
			{
				currentReceiver_->Redo();
			}
		}
		else if (ctrl->Trigger(Input::GAMEPAD_R1, DIK_Z))
		{
			currentReceiver_->Undo();
		}

	}

	// ‘JˆÚˆ—
	return SceneList::NOTCHANGE;
}

void TitleScene::HierarchyView(void)
{
	if (ImGui::Button("CreateSprite"))
	{
		CreateReceiver();
	}

	for (auto& list : clientList_)
	{
		if (list)
		{
			if (ImGui::Button(list->GetName().c_str()))
			{
				currentReceiver_ = list;
			}
		}
	}
}

void TitleScene::InspectorView(void)
{
	if (currentReceiver_)
	{
		if (ImGui::Button("Undo"))
		{
			currentReceiver_->Undo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Redo"))
		{
			currentReceiver_->Redo();
		}

		currentReceiver_->Update();

		if (pivot_)
		{
			pivot_->SetTransform(currentReceiver_->GetTransform());
		}
	}
}

void TitleScene::ConsoleView(void)
{
	if (currentReceiver_)
	{
		currentReceiver_->ConsoleWindow();
	}
}

void TitleScene::CreateReceiver(void)
{
	Receiver* client = new Receiver;
	if (client)
	{
		client->SetCtrl(GetCtrl(0));
		client->Init();

		clientList_.emplace_back(client);
		currentReceiver_ = client;
	}
}
