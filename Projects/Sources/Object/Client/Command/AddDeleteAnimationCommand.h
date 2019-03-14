#ifndef _ADD_DELETE_ANIMATION_COMMAND_H_
#define _ADD_DELETE_ANIMATION_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditer.h"
#include "../AnimationEditer.h"

class AddDeleteAnimationCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	AddDeleteAnimationCommand(void);
	/* @brief	デストラクタ		*/
	~AddDeleteAnimationCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	/* @brief	アニメーション情報の設定		*/
	inline void SetAnimationEditer(AnimationEditer* animationEditer) { animationEditer_ = animationEditer; }
	/* @brief	フレーム数の設定				*/
	inline void SetFrame(int frame) { frame_	= frame;	}
	/* @brief	追加処理か削除処理か			*/
	inline void SetAdd(bool add)	{ add_		= add;		}

private:
	/* @brief	レシーバーからアニメーション情報の取得
	 * @sa		Invoke()
	 * @param	(receiver)	アニメーションを取得したいレシーバー
	 * @param	(animData)	取得した情報を格納する		*/
	void GetAnim(Receiver* receiver, STORAGE_ANIMATION& animData);

	//! アニメーション情報
	STORAGE_ANIMATION	anim_;
	//! アニメーションのフレーム数
	int					frame_;
	//! 追加処理か削除処理か
	bool				add_;

	AnimationEditer*	animationEditer_;
};

#endif // _ADD_DELETE_ANIMATION_COMMAND_H_
