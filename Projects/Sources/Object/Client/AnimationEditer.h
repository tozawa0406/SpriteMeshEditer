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
