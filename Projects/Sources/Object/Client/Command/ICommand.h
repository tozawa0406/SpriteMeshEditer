#ifndef _I_COMMAND_H_
#define _I_COMMAND_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include <FrameWork/Systems/Input/Controller.h>

struct RECEIVER_DATA
{
	string*		name;
	Transform*	transform;
	SpriteRenderer* spriteRenderer;

	RECEIVER_DATA(void) : name(nullptr), transform(nullptr), spriteRenderer(nullptr) {}
};

class Receiver;
class ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	ICommand(void) : receiver_(nullptr) {}
	/* @brief	�f�X�g���N�^		*/
	virtual ~ICommand(void) {}

	/* @breif	�㏈�����K�v�Ȃ��	*/
	virtual void Uninit(void) {}

	/* @brief	����		*/
	virtual void Invoke(void)	= 0;
	/* @brief	�i��		*/
	virtual void Undo(void)		= 0;
	/* @brief	�߂�		*/
	virtual void Redo(void)		= 0;

	void SetReceiver(Receiver* receiver) { receiver_ = receiver; }

protected:
	Receiver* receiver_;
};

#endif // _I_COMMAND_H_
