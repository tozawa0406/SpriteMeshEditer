#ifndef _I_COMMAND_H_
#define _I_COMMAND_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include <FrameWork/Systems/Input/Controller.h>

enum class CommandType : int8
{
	UNKOWN = -1,
	SET_POSITION,

	MAX
};

class ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	ICommand(void) : reciver_(nullptr)	{}
	/* @brief	�f�X�g���N�^		*/
	~ICommand(void) {}

	/* @brief	����		*/
	virtual void Invoke(void) = 0;
	/* @brief	�i��		*/
	virtual void Undo(void)		= 0;
	/* @brief	�߂�		*/
	virtual void Redo(void)		= 0;

	void SetReciver(Transform* reciver) { reciver_ = reciver;	}

protected:
	Transform*	reciver_;
};

#endif // _I_COMMAND_H_
