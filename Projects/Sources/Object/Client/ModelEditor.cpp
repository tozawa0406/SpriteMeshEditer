#include "ModelEditor.h"
#include "Editor.h"
#include "Receiver.h"
#include "../Pivot.h"
#include "../Search.h"
#include "Command/CreateSpriteCommand.h"

#include "LoadSpriteMesh.h"

#include <FrameWork/Systems/GameSystems.h>
#include "AnimationEditor.h"

ModelEditor::ModelEditor(void) : Object(ObjectTag::STATIC), GUI(Systems::Instance(), this, "model")
	, currentReceiver_(nullptr)
	, ctrl_(nullptr)
	, pivot_(nullptr)
	, name_("")
	, selectedCnt_(0)
	, workReceiver_(nullptr)
	, editor_(nullptr)
	, animation_(nullptr)
	, flag_(0)
{
}

ModelEditor::~ModelEditor(void)
{
}

void ModelEditor::Init(void)
{
	name_.resize(256);

	animation_ = new AnimationEditor;
	if (animation_) { animation_->Init(); }
}

void ModelEditor::Load(void)
{
	LoadData();
	if (animation_ && receiverList_.size() > 0) 
	{
		animation_->SetEditor(editor_);
		animation_->SetReceiver(receiverList_[0]); 
		animation_->SetCtrl(ctrl_);

		animation_->Load();
	}
}

void ModelEditor::Uninit(void)
{
	UninitDeletePtr(animation_);
	for (auto& c : receiverList_) { UninitDeletePtr(c); }
}

void ModelEditor::Update(void)
{
	if (selectedCnt_ > 0)
	{
		selectedCnt_++;
		if (selectedCnt_ > 20)
		{
			selectedCnt_ = 0; 
			currentReceiver_ = workReceiver_;
			workReceiver_ = nullptr;
		}
	}

	if (animation_) { animation_->Update(); }
}

void ModelEditor::GuiUpdate(void)
{
	float padding = 30;
	ImGui::SetNextWindowPos(ImVec2(Windows::WIDTH - padding - 350, padding), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiSetCond_Once);

	if (ImGui::Begin("Inspector"))
	{
		InspectorView();
	}
	ImGui::End();

	float w = Windows::WIDTH - padding * 2;
	float h = 140;
	ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - Half(w), (Windows::HEIGHT - padding) - h), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiSetCond_Once);

	if (ImGui::Begin("Console"))
	{
		ConsoleView();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(padding, padding), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiSetCond_Once);

	if (ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_MenuBar))
	{
		HierarchyView();

		if (const auto& systems = Systems::Instance())
		{
			if (const auto& graphics = systems->GetGraphics())
			{
				if (const auto& wrapper = graphics->GetWrapper())
				{
					wrapper->GuiUpdate();
				}
			}
		}
	}
	ImGui::End();
}

void ModelEditor::InspectorView(void)
{
	// 現在のスプライトの情報
	if (currentReceiver_)
	{
		currentReceiver_->Update();
	}

	// ピボットの位置調整
	if (pivot_ && currentReceiver_)
	{
		pivot_->SetTransform(currentReceiver_->GetTransform());
	}
}

void ModelEditor::ConsoleView(void)
{
	if (!editor_) { return; }

	auto list = editor_->GetMessageList();
	int size = static_cast<int>(list.size());
	for (int i = size - 1; i >= 0; --i)
	{
		ImGui::Text(list[i].c_str());
	}
}

void ModelEditor::HierarchyView(void)
{
	// ヒエラルキー描画情報をリセット
	for (auto& r : receiverList_) { if (r) { r->SetHierarchy(false); } }

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) { SaveData(); }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Mode"))
		{
			if (ImGui::MenuItem("Model")) { BitSub(flag_, FLAG_ANIMATION); }
			if (ImGui::MenuItem("Animation")) { BitAdd(flag_, FLAG_ANIMATION); }

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	// ファイル名
	ImGui::InputText("fileName", &name_[0], 256);

	// Undo/Redoボタン
	ImGui::Dummy(ImVec2(0, 5));
	if (editor_)
	{
		if (ImGui::Button("Undo")) { editor_->Undo(); }
		ImGui::SameLine();
		if (ImGui::Button("Redo")) { editor_->Redo(); }
	}

	if (BitCheck(flag_, FLAG_ANIMATION))
	{
		if (animation_) { animation_->HierarchyView(); }
	}
	else
	{
		// スプライト追加
		if (ImGui::Button("CreateSprite")) { CreateReceiver(); }

		// ヒエラルキーにスプライトの一覧描画
		ImGui::Dummy(ImVec2(0, 5));
		if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300)))
		{
			for (auto& list : receiverList_)
			{
				// 親がいないものを描画(子は親から再起で描画される)
				if (list && !list->GetTransform().parent)
				{
					string blank = " ";
					DrawHierarchy(list, blank);
				}
			}
		}
		ImGui::EndChild();
	}
}

void ModelEditor::DrawHierarchy(Receiver* draw, string& blank)
{
	// ヒエラルキービューに描画
	if (draw && !draw->IsHierarchy())
	{
		// 空白描画
		ImGui::TextAlign(blank);
		// 選択カーソルの描画
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
		// 選択されたら現在のワークスペースに
		if (select) 
		{
			workReceiver_ = draw;
			if (selectedCnt_ > 0) 
			{
				draw->SetHierarchyChild(!draw->IsHierarchChild()); 
				selectedCnt_ = 0;
			}
			else { selectedCnt_++; } 			
		}

		// 描画済み状態
		draw->SetHierarchy(true);

		// 子要素の描画
		if (draw->IsHierarchChild())
		{
			const auto& child = draw->GetChild();
			for (auto& c : child)
			{
				// 空白の追加
				string addBlank = blank + "  ";
				DrawHierarchy(c, addBlank);
			}
		}
	}
}

void ModelEditor::CreateReceiver(SPRITE_MESH_RESOURCE* resouce)
{
	// コマンド発行
	CreateSpriteCommand* command = new CreateSpriteCommand;
	if (command)
	{
		// スプライト生成
		Receiver* receiver = new Receiver;
		if (receiver)
		{
			// スプライト初期化
			receiver->SetCtrl(ctrl_);
			receiver->Init(this);

			// リストに追加
			receiverList_.emplace_back(receiver);

			// ロード時
			if (resouce)
			{
				// コマンドは積まない
				DeletePtr(command);
				// データのロード
				if (!receiver->LoadData(*resouce))
				{
					// 失敗した場合、スプライトのメモリ解放
					RemoveVector(receiverList_, receiver);
					UninitDeletePtr(receiver);
					return;
				}
			}
			else
			{
				// コマンドの実行
				command->SetReceiver(receiver);
				command->SetModelEditor(this);
				command->Invoke();

				if (editor_)
				{
					editor_->AddCommand(command);
					editor_->AddMessage("\"Create Sprite\"");
				}
			}

			// ワークスペースとして扱う
			currentReceiver_ = receiver;
		}
	}
}

void ModelEditor::SaveData(void)
{
	// ディレクトリ名の生成
	string directory = "Export/" + name_;

	for (size_t i = directory.size() - 1; i > 0; --i)
	{
		if (directory[i] == 0) { directory.erase(directory.begin() + i); }
	}

	directory += "." + SPRITE_MESH_EXTENSION;

	SPRITE_MESH_RESOURCE temp;

	Receiver* root = nullptr;
	for (auto& receiver : receiverList_)
	{
		if (receiver && !receiver->GetParent())
		{
			if (root) 
			{
				if (editor_)
				{
					editor_->AddMessage("[error]\"Failed to Save\" because there were more parents");
				}
				return;
			}
			root = receiver;
		}
	}

	root->SaveData(temp);

	if (editor_)
	{
		LoadSpriteMesh loader;
		loader.Save(directory, temp);

		editor_->AddMessage("Create \"" + name + "." + SPRITE_MESH_EXTENSION + "\"");
		if (animation_) { animation_->SaveData(); }
		editor_->AddMessage("\"Save\" is complete");
	}
}

void ModelEditor::LoadData(void)
{
	// ディレクトリ名の生成
	string directory = "Export/";

	std::vector<string> list;
	SearchFile search;
	search.Search(directory, SPRITE_MESH_EXTENSION, list);
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

	if (!editor_) { return; }

	LoadSpriteMesh loader;

	string version = "";
	SPRITE_MESH_RESOURCE temp =  loader.Load(list[0], version);

	if (version.find(loader.GetNotSupport()))
	{
		CreateReceiver(&temp);

		editor_->AddMessage("\"" + list[0] + "\" loading is complete " + version);
	}
	else
	{
		editor_->AddMessage(list[0] + " is " + version);
	}
}

void ModelEditor::RemoveSprite(Receiver* receiver)
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
			else { currentReceiver_ = nullptr; }

			receiverList_.erase(receiverList_.begin() + i);
			break;
		}
	}
}

void ModelEditor::AddSprite(Receiver* receiver)
{
	// 後ろに追加
	receiverList_.emplace_back(receiver);
}
