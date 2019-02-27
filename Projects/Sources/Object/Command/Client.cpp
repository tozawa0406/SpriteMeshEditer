#include "Client.h"
#include "Receiver.h"
#include "../Pivot.h"
#include "CreateSpriteCommand.h"

#include "../IOFile.h"

Client::Client(void) : Object(ObjectTag::STATIC)
	, currentReceiver_(nullptr)
	, ctrl_(nullptr)
	, pivot_(nullptr)
{
}

Client::~Client(void)
{
}

void Client::Init(void)
{
}

void Client::Load(void)
{
	LoadData();
}

void Client::Uninit(void)
{
	for (auto& c : clientList_) { UninitDeletePtr(c); }

	for (auto& c : prevCommand_) { UninitDeletePtr(c); }
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }
}

void Client::Update(void)
{
	if (!currentReceiver_) { return; }

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
	}
}

void Client::InspectorView(void)
{
	if (currentReceiver_)
	{
		currentReceiver_->Update();

		if (pivot_)
		{
			pivot_->SetTransform(currentReceiver_->GetTransform());
		}
	}
}

void Client::ConsoleView(void)
{
	//	ImGui::Text("prevSize %d, nextSize %d", prevCommand_.size(), nextCommand_.size());
	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}

void Client::HierarchyView(void)
{
	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::Button("Undo")) { Undo(); }
	ImGui::SameLine();
	if (ImGui::Button("Redo")) { Redo(); }

	if (ImGui::Button("CreateSprite"))
	{
		CreateReceiver();
	}

	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::CollapsingHeader("Hierarchy"))
	{
		for (auto& list : clientList_)
		{
			if (list)
			{
				bool select = false;
				ImGui::Text("   "); ImGui::SameLine();
				ImGui::MenuItem(list->GetName().c_str(), nullptr, &select);
				if (select)
				{
					currentReceiver_ = list;
				}
			}
		}
	}

	if (ImGui::Button("Save"))
	{
		SaveData();
	}
}

void Client::AddCommand(ICommand* command)
{
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }

	nextCommand_.clear();
}

void Client::AddMessage(const string& message)
{
	message_.insert(message_.begin(), message);
	int size = static_cast<int>(message_.size());
	if (size > 6)
	{
		message_.erase(message_.end() - 1);
	}
}

void Client::Undo(void)
{
	if (prevCommand_.size() <= 0) { return; }

	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Client::Redo(void)
{
	if (nextCommand_.size() <= 0) { return; }

	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		nextCommand_.erase(nextCommand_.begin());
	}

	AddMessage("performed \"Redo\" process");
}

void Client::CreateReceiver(IOFile* file)
{
	CreateSpriteCommand* command = new CreateSpriteCommand;
	if (command)
	{
		Receiver* client = new Receiver;
		if (client)
		{
			client->SetCtrl(ctrl_);
			client->Init(this);

			if (file)
			{
				client->LoadData(*file);

				DeletePtr(command);
			}
			else
			{
				command->SetReceiver(client);
				command->SetClient(this);
				command->Invoke();

				AddCommand(command);
				AddMessage("\"Create Sprite\"");
			}

			clientList_.emplace_back(client);
			currentReceiver_ = client;
		}
	}
}

void Client::SaveData(void)
{
	IOFile file;
	file.OpenFile("test.bin", std::ios::out);

	size_t size = clientList_.size();
	file.WriteParam(&size, sizeof(size_t));
	for (auto& client : clientList_)
	{
		if (client) { client->SaveData(file); }
	}
	file.CloseFile();

	AddMessage("\"Save\" is complete");
}

void Client::LoadData(void)
{
	IOFile file;
	if (file.OpenFile("test.bin", std::ios::in))
	{
		size_t size = 0;
		file.ReadParam(&size, sizeof(size_t));

		for (size_t i = 0; i < size; ++i)
		{
			CreateReceiver(&file);
		}
		file.CloseFile();

		AddMessage("\"Load\" is complete");
	}
}

int Client::RemoveSprite(Receiver* receiver)
{
	int size = static_cast<int>(clientList_.size());
	for (int i = 0; i < size; ++i)
	{
		if (clientList_[i] == receiver)
		{
			clientList_.erase(clientList_.begin() + i);
			return i;
		}
	}
	return 0;
}

void Client::AddSprite(Receiver* receiver, int place)
{
	clientList_.insert(clientList_.begin() + place, receiver);
}
