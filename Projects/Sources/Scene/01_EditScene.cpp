#include "01_EditScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Client/Command/PositionCommand.h"

#include "../Object/Client/Client.h"
#include "../Object/Pivot.h"

#include "../Camera/LookCamera.h"

EditScene::EditScene(void) : GUI(Systems::Instance(), nullptr, "SceneEdit")
	, client_(nullptr)
	, mode_(Mode::UNKNOWN)
	, create_(false)
{
}

EditScene::~EditScene(void)
{
}

void EditScene::Init(SceneList sceneNum)
{
	BaseScene::Init(sceneNum);

	cameraManager_ = new CameraManager(this);
	if (cameraManager_)
	{
		cameraManager_->Init();
	}

	objectManager_ = new ObjectManager(this);
	if (objectManager_)
	{
		objectManager_->Init();
	}
}

void EditScene::Uninit(void)
{
	UninitDeletePtr(objectManager_);
	UninitDeletePtr(cameraManager_);
}

SceneList EditScene::Update(void)
{
	// ƒRƒ“ƒgƒ[ƒ‰‚ÌŽæ“¾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	if (create_)
	{
		create_ = false;
		if (mode_ == Mode::MODEL_EDIT) { CreateModelEdit(); }
	}

	if (cameraManager_) { cameraManager_->Update(); }
	if (objectManager_) { objectManager_->Update(); }

	// ‘JˆÚˆ—
	return SceneList::NOTCHANGE;
}

void EditScene::GuiUpdate(void)
{
	SelectMode();
}

void EditScene::SelectMode(void)
{
	if (mode_ != Mode::UNKNOWN) { return; }

	VECTOR2 size = VECTOR2(400, 160);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - Half(size.x), Half(Windows::HEIGHT) - Half(size.y) - 100));
	if (ImGui::Begin("SelectMode"))
	{
		if (ImGui::Button("ModelEdit", ImVec2(size.x * 0.9f, 50)))
		{
			mode_ = Mode::MODEL_EDIT;
			create_ = true;
		}
		else if (ImGui::Button("AnimationEdit", ImVec2(size.x * 0.9f, 50)))
		{
			mode_ = Mode::ANIMATION_EDIT;
		}
	}
	ImGui::End();
}

void EditScene::CreateModelEdit(void)
{
	LookCamera* camera = cameraManager_->Create<LookCamera>();
	if (camera)
	{
		camera->SetPosition(VECTOR3(0, 0, -100));
	}

	client_ = objectManager_->Create<Client>();
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetPivot(objectManager_->Create<Pivot>());

		client_->Load();
	}
}
