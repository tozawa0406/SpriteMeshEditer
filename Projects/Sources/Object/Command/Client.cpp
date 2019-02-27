#include "Client.h"
#include "Receiver.h"
#include "../Pivot.h"
#include "../Search.h"
#include "CreateSpriteCommand.h"

#include "../IOFile.h"

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
	}

	if (pivot_ && currentReceiver_)
	{
		pivot_->SetTransform(currentReceiver_->GetTransform());
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
	ImGui::InputText("fileName", &name_[0], 256);

	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::Button("Undo")) { Undo(); }
	ImGui::SameLine();
	if (ImGui::Button("Redo")) { Redo(); }

	if (ImGui::Button("CreateSprite"))
	{
		CreateReceiver();
	}

	ImGui::Dummy(ImVec2(0, 5));

	if(ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 200)))
	{
		for (auto& list : receiverList_)
		{
			if (list)
			{
				bool select = false;
				ImGui::Text(" "); ImGui::SameLine();
				if (list == currentReceiver_) { ImGui::Text("> "); ImGui::SameLine(); }
				else { ImGui::Text("  "); ImGui::SameLine(); }
				ImGui::MenuItem(list->GetName().c_str(), nullptr, &select);
				if (select)
				{
					currentReceiver_ = list;
				}
			}
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Save", ImVec2(400, 40)))
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

			receiverList_.emplace_back(client);
			currentReceiver_ = client;
		}
	}
}

void Client::SaveData(void)
{
	IOFile file;
	string directory = Define::ResourceDirectoryName + "Export/" + name_;

	for (size_t i = directory.size() - 1; i > 0; --i)
	{
		if (directory[i] == 0) { directory.erase(directory.begin() + i); }
	}

	directory += ".bin";
	file.OpenFile(directory, std::ios::out);

	size_t size = receiverList_.size();
	file.WriteParam(&size, sizeof(size_t));
	for (auto& client : receiverList_)
	{
		if (client) { client->SaveData(file); }
	}
	file.CloseFile();

	AddMessage("\"Save\" is complete");
}

void Client::LoadData(void)
{
	IOFile file;

	string directory = Define::ResourceDirectoryName + "Export/";

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

	if (file.OpenFile(list[0], std::ios::in))
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
	receiverList_.insert(receiverList_.begin() + place, receiver);
}
