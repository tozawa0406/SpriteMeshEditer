#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, frameCnt_(0)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(SceneList sceneNum)
{
	BaseScene::Init(sceneNum);

	// 背景
	back_.Init(0, static_cast<int>(Resources::Texture::Base::WHITE));
	back_.SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	back_.SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	back_.SetColor(COLOR::RGBA(150, 150, 150));

}

void TitleScene::Uninit(void)
{
	title_.Uninit();
	back_.Uninit();
	button_.Uninit();
	press_.Uninit();
}

SceneList TitleScene::Update(void)
{
	// コントローラの取得
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// 入力判定
	JudgeCtrlType(*ctrl);
	// 点滅
	Flashing();

	// 遷移処理
	return EndScene(*ctrl);
}

void TitleScene::Flashing(void)
{
	// フレームカウンタ
	frameCnt_++;
}

void TitleScene::JudgeCtrlType(Controller& ctrl)
{
	switch (static_cast<Controller::CtrlNum>(ctrl.GetCtrlNum()))
	{
	case Controller::CtrlNum::Key:
		button_.SetString("Enter");
		break;
	case Controller::CtrlNum::PS4:
		button_.SetString("〇");
		break;
	case Controller::CtrlNum::X:
		button_.SetString("B");
		break;
	}
}

SceneList TitleScene::EndScene(Controller& ctrl)
{
	// 入力
	if (ctrl.Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{		
		if (const auto& sound = GetSound())
		{
			sound->Play(static_cast<int>(Resources::Sound::Base::SE_ENTER));
		}
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}

void TitleScene::GuiUpdate(void)
{
}
