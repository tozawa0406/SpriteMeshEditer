#include "Client.h"
#include "Receiver.h"
#include "../Pivot.h"
#include "../Search.h"
#include "Command/CreateSpriteCommand.h"

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
	// キーボード対応
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
	// 現在のスプライトの情報
	if (currentReceiver_)
	{
		currentReceiver_->Update();
	}

	// ピボットの位置調整
	if (pivot_ && currentReceiver_)
	{
		Transform transform = currentReceiver_->GetTransform();
		transform.position.z -= 1;
		pivot_->SetTransform(transform);
	}
}

void Client::ConsoleView(void)
{
	// コンソールでメッセージ描画
	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}

void Client::HierarchyView(void)
{
	// ヒエラルキー描画情報をリセット
	for (auto& r : receiverList_) { if (r) { r->SetHierarchy(false); } }

	// ファイル名
	ImGui::InputText("fileName", &name_[0], 256);

	// Undo/Redoボタン
	ImGui::Dummy(ImVec2(0, 5));
	if (ImGui::Button("Undo")) { Undo(); }
	ImGui::SameLine();
	if (ImGui::Button("Redo")) { Redo(); }

	// スプライト追加
	if (ImGui::Button("CreateSprite")) { CreateReceiver(); }

	// ヒエラルキーにスプライトの一覧描画
	ImGui::Dummy(ImVec2(0, 5));
	if(ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 400)))
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
		ImGui::EndChild();
	}

	// セーブ処理
	if (ImGui::Button("Save", ImVec2(400, 40))) { SaveData(); }
}

void Client::DrawHierarchy(Receiver* draw, string& blank)
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
			currentReceiver_ = draw; 
			draw->SetHierarchyChild(!draw->IsHierarchChild());
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

void Client::AddCommand(ICommand* command)
{
	// 戻る処理の追加
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }

	// 進む処理のリセット
	nextCommand_.clear();
}

void Client::AddMessage(const string& message)
{
	// メッセージの追加
	message_.insert(message_.begin(), message);
	int size = static_cast<int>(message_.size());
	// 一定のメッセージ履歴は消去
	if (size > 30)
	{
		message_.erase(message_.end() - 1);
	}
}

void Client::Undo(void)
{
	// 戻るコマンドがあるか
	if (prevCommand_.size() <= 0) { return; }

	// 先頭を実行
	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		// 進む処理に追加
		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		// 戻る処理からは削除
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Client::Redo(void)
{
	// 進むコマンドはあるか
	if (nextCommand_.size() <= 0) { return; }

	// 先頭を実行
	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		// 戻る処理に追加
		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		// 進む処理からは削除
		nextCommand_.erase(nextCommand_.begin());
	}

	AddMessage("performed \"Redo\" process");
}

void Client::CreateReceiver(IOFile* file)
{
	// コマンド発行
	CreateSpriteCommand* command = new CreateSpriteCommand;
	if (command)
	{
		// スプライト生成
		Receiver* client = new Receiver;
		if (client)
		{
			// スプライト初期化
			client->SetCtrl(ctrl_);
			client->Init(this);

			// リストに追加
			receiverList_.emplace_back(client);

			// ロード時
			if (file)
			{
				// コマンドは積まない
				DeletePtr(command);
				// データのロード
				if (!client->LoadData(*file, false))
				{
					// 失敗した場合、スプライトのメモリ解放
					RemoveVector(receiverList_, client);
					UninitDeletePtr(client);
					return;
				}
			}
			else
			{
				// コマンドの実行
				command->SetReceiver(client);
				command->SetClient(this);
				command->Invoke();

				AddCommand(command);
				AddMessage("\"Create Sprite\"");
			}

			// ワークスペースとして扱う
			currentReceiver_ = client;
		}
	}
}

void Client::SaveData(void)
{
	// ディレクトリ名の生成
	string directory = Define::ResourceDirectoryName + "Export/" + name_;

	for (size_t i = directory.size() - 1; i > 0; --i)
	{
		if (directory[i] == 0) { directory.erase(directory.begin() + i); }
	}

	directory += ".bin";

	// 書き込み
	IOFile file;
	if (file.OpenFile(directory, std::ios::out))
	{
		// 書き込む量を先に指定
		size_t size = receiverList_.size();
		file.WriteParam(&size, sizeof(size_t));
		for (auto& client : receiverList_)
		{
			// それぞれを書き込む
			if (client) { client->SaveData(file, false); }
		}
		// 終了
		file.CloseFile();
	}

	AddMessage("\"Save\" is complete");
}

void Client::LoadData(void)
{
	// ディレクトリ名の生成
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

	// 読み込み
	IOFile file;
	if (file.OpenFile(list[0], std::ios::in))
	{
		// 全体量の取得
		size_t size = 0;
		file.ReadParam(&size, sizeof(size_t));

		for (size_t i = 0; i < size; ++i)
		{
			// 子要素読み込みで全体量に達したら
			if (receiverList_.size() >= size) { break; }

			// それぞれの読込
			CreateReceiver(&file);
		}
		// 終了
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
	if (place > 0)
	{
		// 場所指定があればそこに追加
		receiverList_.insert(receiverList_.begin() + place, receiver);
	}
	else
	{
		// 指定がなければ後ろに追加
		receiverList_.emplace_back(receiver);
	}
}
