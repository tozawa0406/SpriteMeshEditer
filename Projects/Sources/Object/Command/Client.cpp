#include "Client.h"
#include "Receiver.h"
#include "../Pivot.h"

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

void Client::Uninit(void)
{
	for (auto& c : clientList_) { UninitDeletePtr(c); }

	for (auto& c : prevCommand_) { DeletePtr(c); }
	for (auto& c : nextCommand_) { DeletePtr(c); }
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
		if (ImGui::Button("Undo")) { Undo(); }
		ImGui::SameLine();
		if (ImGui::Button("Redo")) { Redo(); }

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
}

void Client::AddCommand(ICommand* command)
{
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { DeletePtr(c); }

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

void Client::CreateReceiver(void)
{
	Receiver* client = new Receiver;
	if (client)
	{
		client->SetCtrl(ctrl_);
		client->Init(this);

		clientList_.emplace_back(client);
		currentReceiver_ = client;
	}
}
