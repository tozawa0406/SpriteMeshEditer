#include "01_EditScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Client/Command/PositionCommand.h"

#include "../Object/Client/ModelEditer.h"
#include "../Object/Pivot.h"

#include "../Camera/LookCamera.h"

EditScene::EditScene(void) : GUI(Systems::Instance(), nullptr, "SceneEdit")
	, client_(nullptr)
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

	LookCamera* camera = cameraManager_->Create<LookCamera>();
	if (camera)
	{
		camera->SetPosition(VECTOR3(0, 0, -100));
	}

	client_ = objectManager_->Create<ModelEditer>();
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetPivot(objectManager_->Create<Pivot>());

		client_->Load();
	}
}

void EditScene::Uninit(void)
{
	UninitDeletePtr(objectManager_);
	UninitDeletePtr(cameraManager_);
}

SceneList EditScene::Update(void)
{
	// ƒRƒ“ƒgƒ[ƒ‰‚Ìæ“¾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	if (cameraManager_) { cameraManager_->Update(); }
	if (objectManager_) { objectManager_->Update(); }

	// ‘JˆÚˆ—
	return SceneList::NOTCHANGE;
}

void EditScene::GuiUpdate(void)
{
}
