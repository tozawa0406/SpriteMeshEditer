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
	// �J�����̍폜
	for (auto obj : camera_) { DeletePtr(obj); }
	DeletePtr(debugCamera_);
}

void CameraManager::Update(void)
{
	// �f�o�b�O�J�����ւ̈ڍs
	DebugMove();

	// �J�����̍X�V����
	for (auto obj : camera_)
	{
		if (obj)
		{
			obj->Update();
		}
	}
	debugCamera_->Update();

	// View�s���Proj�s��̐���
	CreateMatrix();
}

void CameraManager::DebugMove(void)
{
	VECTOR3 temp = { 0, 0, 0 };
	// ImGui�őI������Ă���I�u�W�F�N�g��pos
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

	// �I�΂�Ă�����
	if (*selectObj != nullptr)
	{
		// �����_�̕ύX
		if (!look_)
		{
			// �t���O
			look_ = true;
			// ���݂̃J������pos��at���f�o�b�O�p�̃J�����ɐݒ�
			debugCamera_->SetCamera(camera_[mainCamera_]);
			// ���݃J������pos��at��ۑ�
			posOld_ = camera_[mainCamera_]->GetPosition();
			atOld_  = camera_[mainCamera_]->GetAt();
			// ���C���J�����̐ݒ��z��O��
			mainOld_    = mainCamera_;
			mainCamera_ = -1;
			// �I�����ꂽ�I�u�W�F�N�g�ƌ��݂̒����_�̋������ړ��Ɋ|����t���[�����Ŋ���
			diff_ = (*selectObj - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
		}

		// �K��̃t���[����
		if (frame_++ < GuiManager::SELECT_GUI)
		{
			temp = debugCamera_->GetAt() + diff_;
			// �����_�� 1�t���[���̈ړ����������ړ�
			debugCamera_->SetAt(temp);
		}
	}
	// �I�΂�Ă��Ȃ����Ƀt���O���オ���Ă�����(�I��������)
	else if (look_)
	{
		if (frame_ > 0)
		{
			// �t���[���������}�C�i�X�ɂ���
			frame_ = -GuiManager::SELECT_GUI;
			diff_ = (atOld_ - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
			diffPos_ = (posOld_ - debugCamera_->GetPosition()) / (float)GuiManager::SELECT_GUI;
		}
		// �K��̃t���[����
		else if (frame_++ < 0)
		{
			// �����_�̈ړ�
			temp = debugCamera_->GetAt() + diff_;
			debugCamera_->SetAt(temp);
			// �ʒu�̈ړ�
			temp = debugCamera_->GetPosition() + diffPos_;
			debugCamera_->SetPosition(temp);
		}
		// �K��̃t���[�������I������
		else
		{
			// ���C���J������߂�
			mainCamera_ = mainOld_;
			// �t���O��߂�
			look_ = false;
		}
	}
}

void CameraManager::CreateMatrix(void)
{
	// ���C���J�����̎擾
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
	else if(mainCamera_ == -1) { main = debugCamera_;        }

	if (main == nullptr) { return; }

	// �r���[�ϊ�
	mtxView_ = CreateViewMatrix(main->GetPosition(), main->GetAt(), main->GetUp());
	//�v���W�F�N�V�����s��̍쐬
	mtxProj_ = CreateProjectionMatrix(Camera::FOV, Windows::WIDTH / (float)Windows::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}
