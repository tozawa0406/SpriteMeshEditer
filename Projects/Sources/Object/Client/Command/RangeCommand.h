#ifndef _RANGE_COMMAND_H_
#define _RANGE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"
#include "../AnimationEditor.h"

class RangeCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	RangeCommand(void);
	/* @brief	デストラクタ		*/
	~RangeCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	/* @brief	アニメーション情報の設定		*/
	void SetAnimationEditor(AnimationEditor* animationEditor) { animationEditor_ = animationEditor; }
	/* @brief	最大値or最小値の設定			*/
	void SetMin(bool min) { min_ = min; }

private:
	int		prevRange_;
	int		nextRange_;
	bool	min_;

	AnimationEditor* animationEditor_;
};

#endif // _RANGE_COMMAND_H_
