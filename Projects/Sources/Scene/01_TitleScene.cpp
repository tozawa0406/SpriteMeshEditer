#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Client/Command/PositionCommand.h"

#include "../Object/Client/Client.h"
#include "../Object/Pivot.h"

#include "../Camera/LookCamera.h"

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

	client_ = objectManager_->Create<Client>();
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetPivot(objectManager_->Create<Pivot>());

		client_->Load();
	}

	LookCamera* camera = cameraManager_->Create<LookCamera>();
	if (camera)
	{
		camera->SetPosition(VECTOR3(0, 0, -100));
	}

}

void TitleScene::Uninit(void)
{
	UninitDeletePtr(objectManager_);
	UninitDeletePtr(cameraManager_);
}

SceneList TitleScene::Update(void)
{
	// ƒRƒ“ƒgƒ[ƒ‰‚ÌŽæ“¾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	if (cameraManager_) { cameraManager_->Update(); }
	if (objectManager_) { objectManager_->Update(); }

	// ‘JˆÚˆ—
	return SceneList::NOTCHANGE;
}

void TitleScene::HierarchyView(void)
{
	if (client_) { client_->HierarchyView(); }
}

void TitleScene::InspectorView(void)
{
	if (client_) { client_->InspectorView(); }
}

void TitleScene::ConsoleView(void)
{
	if (client_) { client_->ConsoleView(); }
}
