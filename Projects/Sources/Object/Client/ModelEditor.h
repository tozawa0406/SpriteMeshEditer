/*
 * @file		ModelEditor.h
 * @brief		モデルエディタ
 * @author		戸澤翔太
 * @data		2019/03/07
 */
#ifndef _MODEL_EDITOR_H_
#define _MODEL_EDITOR_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "SpriteMesh.h"

class Pivot;
class Receiver;
class Editor;
class AnimationEditor;
class ModelEditor : public Object, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	ModelEditor(void);
	/* @brief	デストラクタ		*/
	virtual ~ModelEditor(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)		override;
	/* @brief	後処理				*/
	virtual void Uninit(void)	override;
	/* @brief	更新処理			*/
	virtual void Update(void)	override;

	/* @brief	ロード処理			*/
	void Load(void);

	/* @brief	Gui更新処理			*/
	virtual void GuiUpdate(void) override;

	/* @brief	コントローラの設定
	 * @param	(ctrl)	コントローラのポインタ	*/
	inline void SetCtrl(Controller* ctrl)	{ ctrl_ = ctrl;		}
	/* @brief	ピボットの設定
	 * @param	(pivot)	ピボットのポインタ		*/
	inline void SetPivot(Pivot* pivot)		{ pivot_ = pivot;	}

	/* @brief	ヒエラルキーから消去
	 * @param	(receiver)	消去するスプライト	*/
	void RemoveSprite(Receiver* receiver);
	/* @brief	ヒエラルキーへの追加
	 * @param	(receiver)	新規スプライト		*/
	void AddSprite(Receiver* receiver);

	/* @brief	ヒエラルキーのリストを取得		*/
	const std::vector<Receiver*>& GetReceiverList(void) { return receiverList_; }

	/* @brief	セーブ		*/
	void SaveData(void);

	inline void SetEditor(Editor* editor) { editor_ = editor; }
	inline Editor* GetEditor(void) { return editor_; }

private:
	/* @brief	インスペクタの描画	*/
	void InspectorView(void);
	/* @brief	コンソールの描画	*/
	void ConsoleView(void);
	/* @brief	ヒエラルキーの描画	*/
	void HierarchyView(void);

	/* @brief	ヒエラルキービューに描画
	 * @sa		HierarchyView(), DrawHierarchy()
	 * @param	(draw)		描画するもの
	 * @param	(blank)		空白				*/
	void DrawHierarchy(Receiver* draw, string& blank);

	/* @brief	スプライトの生成
	 * @sa		HierarchyView(), LoadData()
	 * @param	(resource)	元データ(ロード時は処理が少し変わる)	*/
	void CreateReceiver(SPRITE_MESH_RESOURCE* resource = nullptr);

	/* @brief	ロード		*/
	void LoadData(void);

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

	//! フラグ
	uint8					flag_;
	//! アニメーションフラグ
	static constexpr uint8 FLAG_ANIMATION = 0x01;

	//! コントローラのポインタ
	Controller*		ctrl_;
	Editor*			editor_;
	//! アニメーション
	AnimationEditor* animation_;
};

#endif // _MODEL_EDITOR_H_
