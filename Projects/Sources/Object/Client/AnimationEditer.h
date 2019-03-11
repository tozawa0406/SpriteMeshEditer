/*
 * @file		AnimationEditer.h
 * @brief		アニメーションエディタ
 * @author		戸澤翔太
 * @data		2019/03/07
 */
#ifndef _ANIMATION_EDITER_H_
#define _ANIMATION_EDITER_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "SpriteMesh.h"

struct ANIMATION_EDITER_DATA
{
	int min;
	int max;
};

class Editer;
class Receiver;
class AnimationEditer : public Object, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	AnimationEditer(void);
	/* @brief	デストラクタ		*/
	virtual ~AnimationEditer(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)		override;
	/* @brief	後処理				*/
	virtual void Uninit(void)	override;
	/* @brief	更新処理			*/
	virtual void Update(void)	override;

	virtual void GuiUpdate(void) override;

	const ANIMATION_EDITER_DATA& GetAnimationEditerData(void) { return beforeData_; }
	void SetAnimationEditerData(const ANIMATION_EDITER_DATA& data) { beforeData_ = data; }

	void SetReceiver(Receiver* receiver)	{ receiver_ = receiver; }
	void SetEditer(Editer* editer)			{ editer_	= editer;	}
	void SetCtrl(Controller* ctrl)			{ ctrl_		= ctrl;		}

	inline void SetRange(int range, bool min)	{ (min) ? minFrame_ = range : maxFrame_ = range;	}
	inline int	GetRange(bool min)				{ return (min) ? minFrame_ : maxFrame_;				}

	inline int GetCurrentFrame(void) { return currentFrame_; }

private:
	void ChangeRange(int& range, bool min);

	int				currentFrame_;
	int				minFrame_;
	int				maxFrame_;
	bool			regeneration_;

	ANIMATION_EDITER_DATA beforeData_;

	Receiver*		receiver_;
	Editer*			editer_;

	Controller*		ctrl_;
};

#endif // _ANIMATION_EDITER_H_
