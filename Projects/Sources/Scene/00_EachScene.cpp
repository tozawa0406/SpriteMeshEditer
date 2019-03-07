#include "00_EachScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "01_EditScene.h"

EachScene::EachScene(void)
{
}

EachScene::~EachScene(void)
{
}

void EachScene::ChangeScene(SceneList sceneChange, SceneList& sceneNum)
{
	if (sceneChange == SceneList::NEXT)
	{
		//シーン遷移
		sceneNum = static_cast<SceneList>(static_cast<int>(sceneNum) + 1);

		//リザルトを越えたらタイトルに戻る
		if (sceneNum >= SceneList::MAX)
		{
			sceneNum = SceneList::EDIT;
		}
	}
	else
	{
		sceneNum = sceneChange;
	}
}

int EachScene::ChangePause(int returnPause)
{
	switch (returnPause)
	{
	case 1:
		return 0;
	case 2:
	case 3:
		return static_cast<int>(SceneList::EDIT);
	}

	return -1;
}

BaseScene* EachScene::CreateScene(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	switch (sceneNum)
	{
	case SceneList::EDIT:
		temp = new EditScene;
		break;
	}

	if (temp) 
	{
		temp->SetManager(manager);
		temp->Init(sceneNum); 
	}

	return temp;
}

BaseScene* EachScene::CreatePause(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	if (temp) 
	{
		temp->SetManager(manager);
		temp->Init(sceneNum); 
	}

	return temp;
}
