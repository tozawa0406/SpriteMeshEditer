#include "Client.h"
#include "Receiver.h"
#include "../Pivot.h"
#include "../Search.h"
#include "Command/CreateSpriteCommand.h"

#include "LoadSpriteMesh.h"

Client::Client(void) : Object(ObjectTag::STATIC)
	, currentReceiver_(nullptr)
	, ctrl_(nullptr)
	, pivot_(nullptr)
	, name_("")
{
}

Client::~Client(void)
{
}

void Client::Init(void)
{
	name_.resize(256);
}

void Client::Load(void)
{
	LoadData();
}

void Client::Uninit(void)
{
	for (auto& c : receiverList_) { UninitDeletePtr(c); }

	for (auto& c : prevCommand_) { UninitDeletePtr(c); }
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }
}

void Client::Update(void)
{
	// �L�[�{�[�h�Ή�
	if (ctrl_->Press(Input::GAMEPAD_L1, DIK_LCONTROL))
	{
		if (ctrl_->Press(Input::GAMEPAD_L2, DIK_LSHIFT))
		{
			if (ctrl_->Trigger(Input::GAMEPAD_L1, DIK_Z))
			{
				Redo();
			}
		}
		else if (ctrl_->Trigger(Input::GAMEPAD_R1, DIK_Z))
		{
			Undo();
		}
		else if (ctrl_->Trigger(Input::GAMEPAD_START, DIK_S))
		{
			SaveData();
		}
	}
}

void Client::InspectorView(void)
{
	// ���݂̃X�v���C�g�̏��
	if (currentReceiver_)
	{
		currentReceiver_->Update();
	}

	// �s�{�b�g�̈ʒu����
	if (pivot_ && currentReceiver_)
	{
		Transform transform = currentReceiver_->GetTransform();
		transform.position.z -= 1;
		pivot_->SetTransform(transform);
	}
}

void Client::ConsoleView(void)
{
	// �R���\�[���Ń��b�Z�[�W�`��
	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}

void Client::HierarchyView(void)
{
	// �q�G�����L�[�`��������Z�b�g
	for (auto& r : receiverList_) { if (r) { r->SetHierarchy(false); } }

	// �t�@�C����
	ImGui::InputText("fileName", &name_[0], 256);

	// Undo/Redo�{�^��
	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::Button("Undo")) { Undo(); }
	ImGui::SameLine();
	if (ImGui::Button("Redo")) { Redo(); }

	// �X�v���C�g�ǉ�
	if (ImGui::Button("CreateSprite")) { CreateReceiver(); }

	// �q�G�����L�[�ɃX�v���C�g�̈ꗗ�`��
	ImGui::Dummy(ImVec2(0, 5));
	if(ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 400)))
	{
		for (auto& list : receiverList_)
		{
			// �e�����Ȃ����̂�`��(�q�͐e����ċN�ŕ`�悳���)
			if (list && !list->GetTransform().parent)
			{
				string blank = " ";
				DrawHierarchy(list, blank);
			}
		}
		ImGui::EndChild();
	}

	// �Z�[�u����
	if (ImGui::Button("Save", ImVec2(400, 40))) { SaveData(); }
}

void Client::DrawHierarchy(Receiver* draw, string& blank)
{
	// �q�G�����L�[�r���[�ɕ`��
	if (draw && !draw->IsHierarchy())
	{
		// �󔒕`��
		ImGui::TextAlign(blank);
		// �I���J�[�\���̕`��
		if (draw == currentReceiver_) { ImGui::TextAlign(">"); }
		else { ImGui::TextAlign(" "); }

		if (draw->GetChild().size() > 0)
		{
			ImGui::TextAlign("*");
		}
		else
		{
			ImGui::TextAlign(" ");
		}

		bool select = false;
		ImGui::MenuItem(draw->GetName().c_str(), nullptr, &select);
		// �I�����ꂽ�猻�݂̃��[�N�X�y�[�X��
		if (select) 
		{
			currentReceiver_ = draw; 
			draw->SetHierarchyChild(!draw->IsHierarchChild());
		}

		// �`��ςݏ��
		draw->SetHierarchy(true);

		// �q�v�f�̕`��
		if (draw->IsHierarchChild())
		{
			const auto& child = draw->GetChild();
			for (auto& c : child)
			{
				// �󔒂̒ǉ�
				string addBlank = blank + "  ";
				DrawHierarchy(c, addBlank);
			}
		}
	}
}

void Client::AddCommand(ICommand* command)
{
	// �߂鏈���̒ǉ�
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }

	// �i�ޏ����̃��Z�b�g
	nextCommand_.clear();
}

void Client::AddMessage(const string& message)
{
	// ���b�Z�[�W�̒ǉ�
	message_.insert(message_.begin(), message);
	int size = static_cast<int>(message_.size());
	// ���̃��b�Z�[�W�����͏���
	if (size > 30)
	{
		message_.erase(message_.end() - 1);
	}
}

void Client::Undo(void)
{
	// �߂�R�}���h�����邩
	if (prevCommand_.size() <= 0) { return; }

	// �擪�����s
	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		// �i�ޏ����ɒǉ�
		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		// �߂鏈������͍폜
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Client::Redo(void)
{
	// �i�ރR�}���h�͂��邩
	if (nextCommand_.size() <= 0) { return; }

	// �擪�����s
	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		// �߂鏈���ɒǉ�
		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		// �i�ޏ�������͍폜
		nextCommand_.erase(nextCommand_.begin());
	}

	AddMessage("performed \"Redo\" process");
}

void Client::CreateReceiver(SPRITE_MESH_RESOURCE* resouce)
{
	// �R�}���h���s
	CreateSpriteCommand* command = new CreateSpriteCommand;
	if (command)
	{
		// �X�v���C�g����
		Receiver* receiver = new Receiver;
		if (receiver)
		{
			// �X�v���C�g������
			receiver->SetCtrl(ctrl_);
			receiver->Init(this);

			// ���X�g�ɒǉ�
			receiverList_.emplace_back(receiver);

			// ���[�h��
			if (resouce)
			{
				// �R�}���h�͐ς܂Ȃ�
				DeletePtr(command);
				// �f�[�^�̃��[�h
				if (!receiver->LoadData(*resouce))
				{
					// ���s�����ꍇ�A�X�v���C�g�̃��������
					RemoveVector(receiverList_, receiver);
					UninitDeletePtr(receiver);
					return;
				}
			}
			else
			{
				// �R�}���h�̎��s
				command->SetReceiver(receiver);
				command->SetClient(this);
				command->Invoke();

				AddCommand(command);
				AddMessage("\"Create Sprite\"");
			}

			// ���[�N�X�y�[�X�Ƃ��Ĉ���
			currentReceiver_ = receiver;
		}
	}
}

void Client::SaveData(void)
{
	// �f�B���N�g�����̐���
	string directory = "Export/" + name_;

	for (size_t i = directory.size() - 1; i > 0; --i)
	{
		if (directory[i] == 0) { directory.erase(directory.begin() + i); }
	}

	directory += ".bin";

	SPRITE_MESH_RESOURCE temp;

	Receiver* root = nullptr;
	for (auto& receiver : receiverList_)
	{
		if (receiver && !receiver->GetParent())
		{
			if (root) 
			{
				AddMessage("[error]\"Failed to Save\" because there were more parents");
				return;
			}
			root = receiver;
		}
	}

	root->SaveData(temp);

	LoadSpriteMesh loader;
	loader.Save(directory, temp);

	AddMessage("\"Save\" is complete");
}

void Client::LoadData(void)
{
	// �f�B���N�g�����̐���
	string directory = "Export/";

	std::vector<string> list;
	SearchFile search;
	search.Search(directory, "bin", list);
	if (list.size() <= 0) { return; }

	for (size_t i = list[0].size() - 1; list[0][i] != '/' && i > 0; --i)
	{
		name_.insert(name_.begin(), list[0][i]);
	}

	int num = 0;
	for (num = static_cast<int>(name_.size()) - 1; name_[num] != '.' && num > 0; --num)
	{
		if (name_[num] != 0)
		{
			name_.erase(name_.begin() + num);
		}
	}
	name_.erase(name_.begin() + num);
	name_.resize(256);

	LoadSpriteMesh loader;

	SPRITE_MESH_RESOURCE temp =  loader.Load(list[0]);

	CreateReceiver(&temp);

	AddMessage("\"Load\" is complete");
}

int Client::RemoveSprite(Receiver* receiver)
{
	int size = static_cast<int>(receiverList_.size());
	for (int i = 0; i < size; ++i)
	{
		if (receiverList_[i] == receiver)
		{
			if(size > 1)
			{ 
				if (i == 0)			{ currentReceiver_ = receiverList_[i + 1]; }
				else if (i != 0)	{ currentReceiver_ = receiverList_[i - 1]; }
			}
			else					{ 
				currentReceiver_ = nullptr; }

			receiverList_.erase(receiverList_.begin() + i);

			return i;
		}
	}
	return 0;
}

void Client::AddSprite(Receiver* receiver, int place)
{
	if (place > 0)
	{
		// �ꏊ�w�肪����΂����ɒǉ�
		receiverList_.insert(receiverList_.begin() + place, receiver);
	}
	else
	{
		// �w�肪�Ȃ���Ό��ɒǉ�
		receiverList_.emplace_back(receiver);
	}
}
