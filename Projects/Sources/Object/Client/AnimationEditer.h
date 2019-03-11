/*
 * @file		AnimationEditer.h
 * @brief		�A�j���[�V�����G�f�B�^
 * @author		���V�đ�
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
	/* @brief	�R���X�g���N�^		*/
	AnimationEditer(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~AnimationEditer(void);

	/* @brief	����������			*/
	virtual void Init(void)		override;
	/* @brief	�㏈��				*/
	virtual void Uninit(void)	override;
	/* @brief	�X�V����			*/
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
