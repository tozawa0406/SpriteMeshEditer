#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include "SpriteMesh.h"

class Pivot;
class Receiver;
class Client : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Client(void);
	/* @brief	デストラクタ		*/
	virtual ~Client(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)		override;
	/* @brief	後処理				*/
	virtual void Uninit(void)	override;
	/* @brief	更新処理			*/
	virtual void Update(void)	override;

	/* @brief	ロード処理			*/
	void Load(void);

	/* @brief	インスペクタの描画	*/
	void InspectorView(void);
	/* @brief	コンソールの描画	*/
	void ConsoleView(void);
	/* @brief	ヒエラルキーの描画	*/
	void HierarchyView(void);

	/* @brief	コマンドの追加
	 * @param	(command)	追加するコマンド	*/
	void AddCommand(ICommand* command);
	/* @brief	メッセージの追加
	 * @param	(message)	追加するメッセージ	*/
	void AddMessage(const string& message);

	/* @brief	コントローラの設定
	 * @param	(ctrl)	コントローラのポインタ	*/
	inline void SetCtrl(Controller* ctrl)	{ ctrl_ = ctrl;		}
	/* @brief	ピボットの設定
	 * @param	(pivot)	ピボットのポインタ		*/
	inline void SetPivot(Pivot* pivot)		{ pivot_ = pivot;	}

	/* @brief	ヒエラルキーから消去
	 * @param	(receiver)	消去するスプライト	*/
	int  RemoveSprite(Receiver* receiver);
	/* @brief	ヒエラルキーへの追加
	 * @param	(receiver)	新規スプライト
	 * @param	(place)		配列番号(Undo時)	*/
	void AddSprite(Receiver* receiver, int place);

	/* @brief	ヒエラルキーのリストを取得		*/
	const std::vector<Receiver*>& GetReceiverList(void) { return receiverList_; }

private:
	/* @brief	戻る処理
	 * @sa		Update(), HierarchyView()		*/
	void Undo(void);
	/* @brief	進む処理
	 * @sa		Update(), HierarchyView()		*/
	void Redo(void);

	/* @brief	ヒエラルキービューに描画
	 * @sa		HierarchyView(), DrawHierarchy()
	 * @param	(draw)		描画するもの
	 * @param	(blank)		空白				*/
	void DrawHierarchy(Receiver* draw, string& blank);

	/* @brief	スプライトの生成
	 * @sa		HierarchyView(), LoadData()
	 * @param	(resource)	元データ(ロード時は処理が少し変わる)	*/
	void CreateReceiver(SPRITE_MESH_RESOURCE* resource = nullptr);

	/* @brief	セーブ		*/
	void SaveData(void);
	/* @brief	ロード		*/
	void LoadData(void);

	//! 戻るコマンド
	std::vector<ICommand*>	prevCommand_;
	//! 進むコマンド
	std::vector<ICommand*>	nextCommand_;
	//! メッセージリスト
	std::vector<string>		message_;

	//! ファイル名
	string					name_;
	//! ピボット
	Pivot*					pivot_;
	//! ダブルクリック判定
	int						selectedCnt_;
	//! ワークスペース
	Receiver*				workReceiver_;
	//! ヒエラルキービュー
	std::vector<Receiver*>	receiverList_;
	//! 現在のワークスペース
	Receiver*				currentReceiver_;

	//! コントローラのポインタ
	Controller*		ctrl_;
};

#endif // _CLIENT_H_
