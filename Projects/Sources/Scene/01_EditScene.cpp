#include "01_EditScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Client/Command/PositionCommand.h"

#include "../Object/Client/Editer.h"
#include "../Object/Pivot.h"

#include "../Camera/LookCamera.h"

EditScene::EditScene(void) : GUI(Systems::Instance(), nullptr, "SceneEdit")
	, editer_(nullptr)
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

	editer_ = objectManager_->Create<Editer>();
	if (editer_)
	{
		editer_->SetCtrl(GetCtrl(0));
		editer_->Setup();
	}
}

void EditScene::Uninit(void)
{
	UninitDeletePtr(objectManager_);
	UninitDeletePtr(cameraManager_);
}

SceneList EditScene::Update(void)
{
	// �R���g���[���̎擾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	if (cameraManager_) { cameraManager_->Update(); }
	if (objectManager_) { objectManager_->Update(); }

	// �J�ڏ���
	return SceneList::NOTCHANGE;
}

void EditScene::GuiUpdate(void)
{
}
