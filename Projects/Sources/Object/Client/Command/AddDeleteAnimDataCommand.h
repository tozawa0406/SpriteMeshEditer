#ifndef _ADD_DELETE_ANIM_DATA_COMMAND_H_
#define _ADD_DELETE_ANIM_DATA_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"
#include "../AnimationEditor.h"

class AddDeleteAnimDataCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	AddDeleteAnimDataCommand(void);
	/* @brief	デストラクタ		*/
	~AddDeleteAnimDataCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	/* @brief	アニメーション情報の設定		*/
	inline void SetAnimationEditor(AnimationEditor* animationEditor) { animationEditor_ = animationEditor; }
	/* @brief	フレーム数の設定				*/
	inline void SetFrame(int frame) { frame_	= frame;	}
	/* @brief	追加処理か削除処理か			*/
	inline void SetAdd(bool add)	{ add_		= add;		}

	/* @brief	同じフレームにある前回のアニメーションの取得	*/
	void GetPrevAnim(void);

private:
	/* @brief	レシーバーからアニメーション情報の取得
	 * @sa		Invoke()
	 * @param	(receiver)	アニメーションを取得したいレシーバー
	 * @param	(animData)	取得した情報を格納する		*/
	void GetAnim(Receiver* receiver, SPRITE_MESH_ANIM_DATA& animData);

	//! アニメーション情報
	SPRITE_MESH_ANIMATION	anim_;
	//! 同フレームの前回のアニメーション
	SPRITE_MESH_ANIMATION	prevAnim_;
	//! アニメーションのフレーム数
	int					frame_;
	//! 追加処理か削除処理か
	bool				add_;

	//! エディタ
	AnimationEditor*	animationEditor_;
};

#endif // _ADD_DELETE_ANIM_DATA_COMMAND_H_
