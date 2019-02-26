#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "../Object/Command/PositionCommand.h"

#include "../Object/Command/Client.h"
#include "../Object/Pivot.h"

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, objectManager_(nullptr)
	, client_(nullptr)
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

	client_ = objectManager_->Create<Client>();
	if (client_)
	{
		client_->SetCtrl(GetCtrl(0));
		client_->SetPivot(objectManager_->Create<Pivot>());
	}
}

void TitleScene::Uninit(void)
{
	UninitDeletePtr(objectManager_);
}

SceneList TitleScene::Update(void)
{
	// コントローラの取得
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	if (objectManager_) { objectManager_->Update(); }

	// 遷移処理
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
