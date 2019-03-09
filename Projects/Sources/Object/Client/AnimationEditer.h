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

	void SetReceiver(Receiver* receiver)	{ receiver_ = receiver; }
	void SetEditer(Editer* editer)			{ editer_ = editer;		}

private:
	int				currentFrame_;
	int				minFrame_;
	int				maxFrame_;
	bool			regeneration_;

	Receiver*		receiver_;
	Editer*			editer_;
};

#endif // _ANIMATION_EDITER_H_
