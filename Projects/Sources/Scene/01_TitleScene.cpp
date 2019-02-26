#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Command/PositionCommand.h"

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, currentClient_(nullptr)
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
	if (!currentClient_) { return SceneList::NOTCHANGE; }

	if (ctrl->Press(Input::GAMEPAD_L1, DIK_LCONTROL))
	{
		if (ctrl->Press(Input::GAMEPAD_L2, DIK_LSHIFT))
		{
			if (ctrl->Trigger(Input::GAMEPAD_L1, DIK_Z))
			{
				currentClient_->Redo();
			}
		}
		else if (ctrl->Trigger(Input::GAMEPAD_R1, DIK_Z))
		{
			currentClient_->Undo();
		}

	}

	// ‘JˆÚˆ—
	return SceneList::NOTCHANGE;
}

void TitleScene::HierarchyView(void)
{
	if (ImGui::Button("CreateSprite"))
	{
		CreateClient();
	}

	for (auto& list : clientList_)
	{
		if (list)
		{
			ImGui::Text(list->GetName().c_str());
		}
	}
}

void TitleScene::InspectorView(void)
{
	if (currentClient_)
	{
		if (ImGui::Button("Undo"))
		{
			currentClient_->Undo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Redo"))
		{
			currentClient_->Redo();
		}

		currentClient_->Update();

		if (pivot_)
		{
			pivot_->SetTransform(currentClient_->GetTransform());
		}
	}
}

void TitleScene::ConsoleView(void)
{
	if (currentClient_)
	{
		currentClient_->ConsoleWindow();
	}
}

void TitleScene::CreateClient(void)
{
	Client* client = new Client;
	if (client)
	{
		client->SetCtrl(GetCtrl(0));
		client->Init();

		clientList_.emplace_back(client);
		currentClient_ = client;
	}
}
