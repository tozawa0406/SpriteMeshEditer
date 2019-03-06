#include "CameraManager.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"
#include "../../Scene/SceneManager.h"
#include "../DebugSetting/GuiManager.h"
#include "MoveCamera.h"

CameraManager::CameraManager(BaseScene* scene) :
	scene_(scene)
	, debugCamera_(nullptr)
	, mainCamera_(0)
	, look_(false)
	, frame_(0)
{
}

CameraManager::~CameraManager(void)
{
}

void CameraManager::Init(void)
{
	Create<Camera>();
	debugCamera_ = new MoveCamera;
	if (debugCamera_)
	{
		debugCamera_->SetManager(this);
	}
}

void CameraManager::Uninit(void)
{
	// カメラの削除
	for (auto obj : camera_) { DeletePtr(obj); }
	DeletePtr(debugCamera_);
}

void CameraManager::Update(void)
{
	// デバッグカメラへの移行
	DebugMove();

	// カメラの更新処理
	for (auto obj : camera_)
	{
		if (obj)
		{
			obj->Update();
		}
	}
	debugCamera_->Update();

	// View行列をProj行列の生成
	CreateMatrix();
}

void CameraManager::DebugMove(void)
{
	VECTOR3 temp = { 0, 0, 0 };
	// ImGuiで選択されているオブジェクトのpos
	const VECTOR3* selectObj = nullptr;

	GuiManager* guiManager = nullptr;
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& debug = systems->GetDebug())
		{
			guiManager = debug->GetGuiManager();
		}
	}
	if (!guiManager) { return; }

#ifdef _SELF_DEBUG
	selectObj = guiManager->GetSelect();
#endif
	if (!guiManager->GetLookObject())
	{
		selectObj = nullptr;
	}

	// 選ばれていたら
	if (*selectObj != nullptr)
	{
		// 注視点の変更
		if (!look_)
		{
			// フラグ
			look_ = true;
			// 現在のカメラのposとatをデバッグ用のカメラに設定
			debugCamera_->SetCamera(camera_[mainCamera_]);
			// 現在カメラのposとatを保存
			posOld_ = camera_[mainCamera_]->GetPosition();
			atOld_  = camera_[mainCamera_]->GetAt();
			// メインカメラの設定を配列外へ
			mainOld_    = mainCamera_;
			mainCamera_ = -1;
			// 選択されたオブジェクトと現在の注視点の距離を移動に掛けるフレーム数で割る
			diff_ = (*selectObj - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
		}

		// 規定のフレーム間
		if (frame_++ < GuiManager::SELECT_GUI)
		{
			temp = debugCamera_->GetAt() + diff_;
			// 注視点を 1フレームの移動距離だけ移動
			debugCamera_->SetAt(temp);
		}
	}
	// 選ばれていない時にフラグが上がっていたら(選択解除時)
	else if (look_)
	{
		if (frame_ > 0)
		{
			// フレーム数ををマイナスにする
			frame_ = -GuiManager::SELECT_GUI;
			diff_ = (atOld_ - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
			diffPos_ = (posOld_ - debugCamera_->GetPosition()) / (float)GuiManager::SELECT_GUI;
		}
		// 規定のフレーム間
		else if (frame_++ < 0)
		{
			// 注視点の移動
			temp = debugCamera_->GetAt() + diff_;
			debugCamera_->SetAt(temp);
			// 位置の移動
			temp = debugCamera_->GetPosition() + diffPos_;
			debugCamera_->SetPosition(temp);
		}
		// 規定のフレーム数を終えたら
		else
		{
			// メインカメラを戻す
			mainCamera_ = mainOld_;
			// フラグを戻す
			look_ = false;
		}
	}
}

void CameraManager::CreateMatrix(void)
{
	// メインカメラの取得
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
	else if(mainCamera_ == -1) { main = debugCamera_;        }

	if (main == nullptr) { return; }

	// ビュー変換
	mtxView_ = CreateViewMatrix(main->GetPosition(), main->GetAt(), main->GetUp());
	//プロジェクション行列の作成
	mtxProj_ = CreateProjectionMatrix(Camera::FOV, Windows::WIDTH / (float)Windows::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}
